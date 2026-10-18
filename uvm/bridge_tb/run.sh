#!/usr/bin/env bash
# uvm/bridge_tb/run.sh — compile and run the standalone bridge testbench.
#
#   uvm/bridge_tb/run.sh              run (seed 1)
#   uvm/bridge_tb/run.sh --seed 7     another seed
#   uvm/bridge_tb/run.sh --waves      also dump waves.shm (all signals)
#   uvm/bridge_tb/run.sh --print      print the xrun command, run nothing
#
# Output: uvm/bridge_tb/runs/run.log.  Pass = "BRIDGE TB PASSED".
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export TITAN_ROOT="$(cd "${here}/../.." && pwd)"

seed=1 waves=0 print=0
while [[ $# -gt 0 ]]; do
  case "$1" in
    --seed)  seed="$2"; shift 2 ;;
    --waves) waves=1; shift ;;
    --print) print=1; shift ;;
    *) echo "unknown option: $1" >&2; exit 2 ;;
  esac
done

run_dir="${here}/runs"
cmd=(xrun -64bit -sv -timescale 1ns/1ps
     -f "${here}/files.f"
     -top tb_bridge
     -svseed "${seed}"
     -xmlibdirname "${run_dir}/xcelium.d"
     -l "${run_dir}/run.log"
     -licqueue -nocopyright -errormax 50
     -nowarn CUVIHR -nowarn DSEMEL -nowarn SPDUSD)
if [[ "${waves}" == 1 ]]; then
  cmd+=(-access +r -input "${here}/waves.tcl")
fi

if [[ "${print}" == 1 ]]; then
  printf '%q ' "${cmd[@]}"; echo
  exit 0
fi

command -v xrun >/dev/null || { echo "xrun not on PATH — load the Cadence setup" >&2; exit 1; }
mkdir -p "${run_dir}"
cd "${run_dir}"
"${cmd[@]}" || true
echo
if grep -q "BRIDGE TB PASSED" "${run_dir}/run.log"; then
  echo "RESULT: PASSED  (log: ${run_dir}/run.log)"
else
  echo "RESULT: FAILED  (log: ${run_dir}/run.log)"
  grep -E "^\*E|^xmvlog: \*E|^xmelab: \*E|\*F,|Error|BRIDGE TB FAILED" "${run_dir}/run.log" | head -30 || true
  exit 1
fi
