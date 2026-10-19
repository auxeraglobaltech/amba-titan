#!/usr/bin/env bash
# uvm/sim/run.sh — build and run one titan-axi test.
#
#   uvm/sim/run.sh fw_hello              a test from uvm/sim/tests.list
#   uvm/sim/run.sh fw_hello --seed 7
#   uvm/sim/run.sh fw_hello --waves      dump the fabric + testbench to waves.shm
#   uvm/sim/run.sh axi_csr_reset --trace 40000014   follow one address through every AXI port
#   uvm/sim/run.sh fw_hello --plusarg +foo=1        pass any plusarg to the simulation
#   uvm/sim/run.sh --build-only          compile/elaborate only
#   uvm/sim/run.sh fw_hello --print      print the commands, run nothing
#   uvm/sim/run.sh --list
#
# Steps: (1) make the firmware image, (2) compile + elaborate the chip and the
# testbench into one snapshot (incremental: unchanged files are not
# recompiled), (3) run the snapshot with the test's plusargs.
#
# Output: uvm/sim/runs/<test>/run.log; pass = "*** TEST PASSED ***".
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export TITAN_ROOT="$(cd "${here}/../.." && pwd)"
list="${here}/tests.list"

test="" seed=1 waves=0 print=0 build_only=0 verbosity=UVM_LOW extra=()
while [[ $# -gt 0 ]]; do
  case "$1" in
    --seed)       seed="$2"; shift 2 ;;
    --waves)      waves=1; shift ;;
    --print)      print=1; shift ;;
    --build-only) build_only=1; shift ;;
    --verbosity)  verbosity="$2"; shift 2 ;;
    --trace)      extra+=("+trace_addr=${2#0x}"); shift 2 ;;
    --plusarg)    extra+=("$2"); shift 2 ;;
    --list)       grep -vE '^\s*(#|$)' "${list}"; exit 0 ;;
    -*)           echo "unknown option: $1" >&2; exit 2 ;;
    *)            test="$1"; shift ;;
  esac
done

if [[ "${build_only}" == 0 ]]; then
  [[ -n "${test}" ]] || { echo "usage: run.sh <test> [options]   (run.sh --list)" >&2; exit 2; }
  read -r _ uvm_test fw < <(awk -v t="${test}" '$1 == t {print; exit}' "${list}") || true
  [[ -n "${uvm_test:-}" ]] || { echo "test '${test}' not in ${list}" >&2; exit 2; }
fi

build="${here}/runs/build"
otp="${TITAN_ROOT}/firmware/otp/otp_rma.24.vmem"

build_cmd=(xrun -64bit -elaborate -sv -timescale 1ns/1ps -xprop F -ALLOWREDEFINITION
           -uvm -uvmhome CDNS-1.2 +define+UVM_NO_DEPRECATED
           +define+SIMULATION +define+XCELIUM +define+TITAN_SIM
           -f "${TITAN_ROOT}/rtl/flist/rtl.f"
           -f "${here}/uvm.f"
           -top tb_top -snapshot tb -access +r
           -xmlibdirname "${build}/xcelium.d" -l "${build}/build.log"
           -licqueue -nocopyright -errormax 50
           -nowarn CUVIHR -nowarn DSEMEL -nowarn REVRNG -nowarn SPDUSD -nowarn TSNSPK)

if [[ "${build_only}" == 0 ]]; then
  out="${here}/runs/${test}"
  run_cmd=(xrun -64bit -R -snapshot tb -xmlibdirname "${build}/xcelium.d"
           -uvmhome CDNS-1.2
           +UVM_TESTNAME="${uvm_test}" +UVM_VERBOSITY="${verbosity}"
           +firmware="${TITAN_ROOT}/firmware/build/${fw}/${fw}.hex" +otp="${otp}"
           -svseed "${seed}" -l "${out}/run.log" -licqueue -nocopyright "${extra[@]}")
  [[ "${waves}" == 1 ]] && run_cmd+=(-input "${here}/waves.tcl")
fi

if [[ "${print}" == 1 ]]; then
  [[ "${build_only}" == 0 ]] && echo "make -C firmware TEST=${fw}"
  printf '%q ' "${build_cmd[@]}"; echo
  [[ "${build_only}" == 0 ]] && { printf '%q ' "${run_cmd[@]}"; echo; }
  exit 0
fi

command -v xrun >/dev/null || { echo "xrun not on PATH — load the Cadence setup" >&2; exit 1; }

# 1. firmware
if [[ "${build_only}" == 0 ]]; then
  make -s -C "${TITAN_ROOT}/firmware" TEST="${fw}" RISCV_PREFIX="${RISCV_PREFIX:-/home/user1/riscv/bin/riscv32-unknown-elf-}"
fi

# 2. build
mkdir -p "${build}"
( cd "${build}" && "${build_cmd[@]}" ) > /dev/null || {
  echo "BUILD FAILED (log: ${build}/build.log)"
  grep -E "\*E,|\*F," "${build}/build.log" | head -30 || true
  exit 1
}
echo "build ok (log: ${build}/build.log)"
[[ "${build_only}" == 1 ]] && exit 0

# 3. run
mkdir -p "${out}"
( cd "${out}" && "${run_cmd[@]}" ) > /dev/null || true
if grep -q "\*\*\* TEST PASSED \*\*\*" "${out}/run.log"; then
  echo "RESULT: PASSED  ${test}  (log: ${out}/run.log)"
else
  echo "RESULT: FAILED  ${test}  (log: ${out}/run.log)"
  grep -E "UVM_(ERROR|FATAL) .*@|\*E,|\*F,|TEST FAILED" "${out}/run.log" | head -20 || true
  exit 1
fi
