#!/usr/bin/env python3
"""uvm/sim/lint.py — parse + elaborate a file list with slang (no simulation).

Catches syntax, type, port and parameter errors in seconds, before a
multi-minute xrun build.  It does not replace xrun: slang and Xcelium differ
in places, and xrun has the final word.

  uvm/sim/lint.py --top tb_bridge uvm/bridge_tb/files.f
  uvm/sim/lint.py --top tb_top --uvm -D TITAN_SIM rtl/flist/rtl.f uvm/sim/uvm.f

Exit status 0 = no errors.
"""

import argparse
import os
import pathlib
import re
import sys

import pyslang

ROOT = pathlib.Path(__file__).resolve().parents[2]


def expand(flist, env, out_files, out_incs, out_defs):
    """Flatten an xrun-style .f file: -f nesting, +incdir+, +define+, ${VARS}."""
    for raw in pathlib.Path(flist).read_text().splitlines():
        line = re.sub(r"//.*", "", raw).strip()
        if not line:
            continue
        line = re.sub(r"\$\{(\w+)\}|\$(\w+)", lambda m: env.get(m.group(1) or m.group(2), ""), line)
        if line.startswith("-f ") or line.startswith("-F "):
            expand(line[3:].strip(), env, out_files, out_incs, out_defs)
        elif line.startswith("+incdir+"):
            out_incs.extend(p for p in line[len("+incdir+"):].split("+") if p)
        elif line.startswith("+define+"):
            out_defs.extend(p for p in line[len("+define+"):].split("+") if p)
        elif line.startswith(("-", "+")):
            continue  # simulator options: not for slang
        else:
            out_files.append(line)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("flists", nargs="+")
    ap.add_argument("--top", required=True)
    ap.add_argument("-D", dest="defines", action="append", default=[])
    ap.add_argument("--max-errors", type=int, default=50)
    ap.add_argument("--uvm", action="store_true", help="compile the UVM library first")
    # Open-source UVM (Cadence's CDNS-1.2 has encrypted parts slang cannot read):
    #   git clone --depth 1 https://github.com/accellera-official/uvm-core .tools/uvm-core
    ap.add_argument("--uvm-src", default=str(ROOT / ".tools/uvm-core/src"),
                    help="UVM 1.2 source directory (contains uvm_pkg.sv)")
    args = ap.parse_args()

    env = dict(os.environ, TITAN_ROOT=str(ROOT))
    files, incs, defs = [], [], list(args.defines)
    if args.uvm:
        incs.append(args.uvm_src)
        files.append(f"{args.uvm_src}/uvm_pkg.sv")
        defs += ["UVM_NO_DPI", "UVM_REGEX_NO_DPI"]
    for f in args.flists:
        expand(f, env, files, incs, defs)

    cmd = ["slang", "--single-unit", "--top", args.top,
           "--timescale", "1ns/1ps",
           "--error-limit", str(args.max_errors),
           # Xcelium accepts these; slang is stricter by default.
           "--allow-use-before-declare", "--relax-enum-conversions",
           "-Wno-unconnected-port", "-Wno-width-trunc", "-Wno-width-expand",
           "-Wno-unused", "-Wno-implicit-net-port", "-Wno-missing-top"]
    for i in incs:
        cmd += ["-I", i]
    for d in defs:
        cmd += ["-D", d]
    cmd += files

    driver = pyslang.driver.Driver()
    driver.addStandardArgs()
    if not driver.parseCommandLine(" ".join(cmd), pyslang.driver.CommandLineOptions()):
        return 2
    if not driver.processOptions():
        return 2
    if not driver.parseAllSources():
        print("lint: parse errors"); return 1
    ok = driver.runFullCompilation(quiet=False)
    print(f"lint: {len(files)} files, top {args.top}: {'OK' if ok else 'ERRORS'}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
