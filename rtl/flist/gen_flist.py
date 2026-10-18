#!/usr/bin/env python3
"""rtl/flist/gen_flist.py — freeze an Earl Grey file list without FuseSoC.

Input: fusesoc_chip_sim.scr, the source list FuseSoC resolved for the
upstream chip simulation (every file it names is a copy of a file in
vendor/opentitan; checked by content hash).

Output: a .f file for xrun that
  * keeps only the files reachable from the requested ROOT modules, in the
    order FuseSoC put them (packages before users),
  * points every path back into vendor/opentitan (no copies),
  * drops the modules listed in --drop (e.g. the TL-UL crossbars),
  * leaves out the files of --omit modules but keeps what they use (e.g.
    top_earlgrey, replaced by rtl/top/top_earlgrey.sv),
  * adds the include directories those files need.

Paths are written as ${TITAN_ROOT}/...; uvm/sim/env.sh sets TITAN_ROOT.

Examples
  gen_flist.py --root chip_earlgrey_asic --drop xbar_main,xbar_peri --omit top_earlgrey \
               --out rtl/flist/earlgrey.f
  gen_flist.py --root tlul_adapter_host,tlul_adapter_sram --out uvm/bridge_tb/ot.f
"""

import argparse
import collections
import hashlib
import pathlib
import re
import sys

HERE = pathlib.Path(__file__).resolve().parent
ROOT = HERE.parent.parent                    # repo root
VENDOR = ROOT / "vendor" / "opentitan"
DEFAULT_SCR = HERE / "fusesoc_chip_sim.scr"
# Where the FuseSoC work tree of the last upstream build was parked (only
# needed to map .scr entries to vendor files; the output never refers to it).
DEFAULT_WORK = (ROOT / "_legacy_local/sim/scratch/HEAD/chip_earlgrey_asic-sim-xcelium"
                "/default/fusesoc-work")

DECL_RE = re.compile(r"^\s*(?:module|package|interface)\s+(?:automatic\s+|static\s+)?(\w+)", re.M)
INCL_RE = re.compile(r'`include\s+"([^"]+)"')
WORD_RE = re.compile(r"\b(\w+)\b")


def strip_comments(text):
    text = re.sub(r"//[^\n]*", "", text)
    return re.sub(r"/\*.*?\*/", "", text, flags=re.S)


def sha1(path):
    return hashlib.sha1(path.read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--root", required=True, help="comma-separated root modules")
    ap.add_argument("--drop", default="", help="comma-separated modules to leave out, with "
                    "everything only they need")
    ap.add_argument("--omit", default="", help="comma-separated modules whose file is replaced "
                    "by one of ours: their dependencies are kept, their own file is not")
    ap.add_argument("--out", required=True)
    ap.add_argument("--scr", default=str(DEFAULT_SCR))
    ap.add_argument("--work", default=str(DEFAULT_WORK), help="FuseSoC work dir the .scr is relative to")
    args = ap.parse_args()

    work = pathlib.Path(args.work)
    if not work.is_dir():
        sys.exit(f"FuseSoC work dir not found: {work}")

    # --- read the .scr ------------------------------------------------------
    incdirs, sources = [], []
    for line in pathlib.Path(args.scr).read_text().splitlines():
        line = line.strip()
        if line.startswith("+incdir+"):
            incdirs.append(work / line[len("+incdir+"):])
        elif line.endswith((".sv", ".v")) and not line.startswith(("+", "-")):
            sources.append(work / line)

    # --- map every work file to its vendor twin by content ------------------
    by_hash = collections.defaultdict(list)
    for p in VENDOR.rglob("*"):
        if p.suffix in (".sv", ".svh", ".v", ".vh") and p.is_file():
            by_hash[sha1(p)].append(p)

    def to_vendor(work_file):
        cands = by_hash.get(sha1(work_file), [])
        if not cands:
            return None
        # Several identical copies can exist; prefer the one with the same file name
        # and the longest matching path tail.
        tail = work_file.parts
        def score(c):
            n = 0
            for a, b in zip(reversed(c.parts), reversed(tail)):
                if a != b:
                    break
                n += 1
            return n
        # Identical files also exist under the other tops (darjeeling, englishbreakfast);
        # on a tie, take the earlgrey one.
        def other_top(c):
            return any(part.startswith("top_") and part != "top_earlgrey" for part in c.parts)
        return max(cands, key=lambda c: (score(c), not other_top(c), "top_earlgrey" in c.parts))

    # --- module closure from the roots --------------------------------------
    defs = {}             # name -> work file (first definition in .scr order wins)
    for f in sources:
        for name in DECL_RE.findall(f.read_text(errors="ignore")):
            defs.setdefault(name, f)

    def resolve_include(name, from_file):
        for d in incdirs + [from_file.parent]:
            if (d / name).is_file():
                return d / name
        return None

    # Words a file can reach include the macros it pulls in: `PRIM_FLOP_SPARSE_FSM
    # instantiates prim_sparse_fsm_flop without the name appearing in the source.
    word_cache = {}
    def words_of(path, seen=None):
        if path in word_cache:
            return word_cache[path]
        seen = seen or set()
        seen.add(path)
        text = path.read_text(errors="ignore")
        words = set(WORD_RE.findall(strip_comments(text)))
        for inc in INCL_RE.findall(text):
            hit = resolve_include(inc, path)
            if hit and hit not in seen:
                words |= words_of(hit, seen)
        word_cache[path] = words
        return words

    drop = {d for d in args.drop.split(",") if d}
    need, todo = set(), [r for r in args.root.split(",") if r]
    for r in todo:
        if r not in defs:
            sys.exit(f"root module not found in .scr: {r}")
    while todo:
        name = todo.pop()
        if name in need or name in drop:
            continue
        need.add(name)
        words = words_of(defs[name])
        todo.extend(w for w in words if w in defs and w not in need)

    omit = {o for o in args.omit.split(",") if o}
    keep = {defs[n] for n in need if n not in omit}
    ordered = [f for f in dict.fromkeys(sources) if f in keep]

    # --- include files those sources use ------------------------------------
    inc_out = []
    for f in ordered:
        for inc in INCL_RE.findall(f.read_text(errors="ignore")):
            for d in incdirs + [f.parent]:
                hit = d / inc
                if hit.is_file():
                    v = to_vendor(hit)
                    if v is None:
                        sys.exit(f"include {hit} has no vendor twin")
                    vdir = pathlib.Path(str(v)[: -len(inc)])
                    if vdir not in inc_out:
                        inc_out.append(vdir)
                    break

    # --- write --------------------------------------------------------------
    def rel(p):
        return "${TITAN_ROOT}/" + str(p.relative_to(ROOT))

    lines = [f"// Generated by rtl/flist/gen_flist.py — do not edit.",
             f"// roots: {args.root}",
             f"// dropped: {args.drop or '-'}",
             f"// omitted (replaced elsewhere): {args.omit or '-'}",
             f"// {len(ordered)} source files"]
    lines += ["+incdir+" + rel(d) for d in inc_out]
    for f in ordered:
        v = to_vendor(f)
        if v is None:
            sys.exit(f"source {f} has no vendor twin")
        lines.append(rel(v))
    pathlib.Path(args.out).write_text("\n".join(lines) + "\n")
    print(f"{args.out}: {len(ordered)} sources, {len(inc_out)} include dirs")


if __name__ == "__main__":
    main()
