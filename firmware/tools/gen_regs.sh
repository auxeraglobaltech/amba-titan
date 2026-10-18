#!/usr/bin/env bash
# firmware/tools/gen_regs.sh — regenerate the register headers firmware uses.
#
# Writes firmware/include/regs/<ip>_regs.h for every IP type in Earl Grey
# (OpenTitan's regtool, the same tool its own software build uses), and copies
# the top-level header with base addresses, interrupt IDs and pinmux indices.
# Outputs are committed; re-run only when the OpenTitan pin moves.
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
ot="${root}/vendor/opentitan"
py="${root}/.venv/bin/python"
out="${root}/firmware/include/regs"
mkdir -p "${out}"

cd "${ot}"
"${py}" - "${out}" <<'PY'
import hjson, pathlib, subprocess, sys, os
out = pathlib.Path(sys.argv[1])
top = hjson.loads(pathlib.Path("hw/top_earlgrey/data/autogen/top_earlgrey.gen.hjson").read_text())
env = dict(os.environ, PYTHONPATH="util")
for t in sorted({m["type"] for m in top["module"]}):
    cands = [f"hw/top_earlgrey/ip_autogen/{t}/data/{t}.hjson",
             f"hw/top_earlgrey/ip/{t}/data/{t}.hjson",
             f"hw/ip/{t}/data/{t}.hjson"]
    src = next(c for c in cands if pathlib.Path(c).exists())
    subprocess.run([sys.executable, "util/regtool.py", "-D", "-o", str(out / f"{t}_regs.h"), src],
                   check=True, env=env)
    print(f"  {t}_regs.h  <- {src}")
PY
cp hw/top_earlgrey/sw/autogen/top_earlgrey.h "${root}/firmware/include/top_earlgrey.h"
cp hw/top_earlgrey/sw/autogen/top_earlgrey_memory.h "${root}/firmware/include/top_earlgrey_memory.h"   # assembler-safe subset
echo "  top_earlgrey.h <- hw/top_earlgrey/sw/autogen/top_earlgrey.h"
