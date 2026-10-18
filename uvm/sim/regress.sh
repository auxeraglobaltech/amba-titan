#!/usr/bin/env bash
# uvm/sim/regress.sh — run every test in uvm/sim/tests.list (or the ones named).
#   uvm/sim/regress.sh                 all tests
#   uvm/sim/regress.sh fw_hello axi_decerr
# One build, then each test in turn (one simulation at a time: shared server).
set -uo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
tests=("$@")
[[ ${#tests[@]} -eq 0 ]] && mapfile -t tests < <(grep -vE '^\s*(#|$)' "${here}/tests.list" | awk '{print $1}')

"${here}/run.sh" --build-only || exit 1
pass=0; fail=0; failed=()
for t in "${tests[@]}"; do
  if "${here}/run.sh" "${t}"; then pass=$((pass + 1)); else fail=$((fail + 1)); failed+=("${t}"); fi
done
echo "=============================================="
echo "regression: ${pass} passed, ${fail} failed (of ${#tests[@]})"
for t in "${failed[@]}"; do echo "  FAILED: ${t}  (uvm/sim/runs/${t}/run.log)"; done
[[ ${fail} -eq 0 ]]
