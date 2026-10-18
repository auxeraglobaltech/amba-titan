#!/usr/bin/env bash
# uvm/sim/elab_rtl.sh — compile + elaborate the DUT alone (no testbench) on Xcelium.
#
# Quick check after any change under rtl/: does chip_earlgrey_asic, with the
# AXI fabric, build on the real simulator?  Runs nothing.
#
#   uvm/sim/elab_rtl.sh            simulation build (+define+TITAN_SIM)
#   uvm/sim/elab_rtl.sh --no-sim   without the sim-only fabric additions
#   uvm/sim/elab_rtl.sh --print    print the xrun command only
#
# Output: uvm/sim/runs/elab_rtl/elab.log.  Pass = "RESULT: PASSED".
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export TITAN_ROOT="$(cd "${here}/../.." && pwd)"

sim_def=(+define+TITAN_SIM) print=0
for a in "$@"; do
  case "$a" in
    --no-sim) sim_def=() ;;
    --print)  print=1 ;;
    *) echo "unknown option: $a" >&2; exit 2 ;;
  esac
done

out="${here}/runs/elab_rtl"
cmd=(xrun -64bit -elaborate -sv -timescale 1ns/1ps -xprop F -ALLOWREDEFINITION
     +define+SIMULATION +define+XCELIUM "${sim_def[@]}"
     -f "${TITAN_ROOT}/rtl/flist/rtl.f"
     -top chip_earlgrey_asic
     -xmlibdirname "${out}/xcelium.d"
     -l "${out}/elab.log"
     -licqueue -nocopyright -errormax 50
     -nowarn CUVIHR -nowarn DSEMEL -nowarn REVRNG -nowarn SPDUSD -nowarn TSNSPK)

if [[ "${print}" == 1 ]]; then printf '%q ' "${cmd[@]}"; echo; exit 0; fi
command -v xrun >/dev/null || { echo "xrun not on PATH — load the Cadence setup" >&2; exit 1; }
mkdir -p "${out}"; cd "${out}"
if "${cmd[@]}"; then
  echo "RESULT: PASSED  (log: ${out}/elab.log)"
else
  echo "RESULT: FAILED  (log: ${out}/elab.log)"
  grep -E "\*E,|\*F," "${out}/elab.log" | head -30 || true
  exit 1
fi
