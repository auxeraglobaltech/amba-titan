#!/usr/bin/env bash
# firmware/tools/gen_otp.sh — regenerate the OTP image the chip boots with.
#
# Run once; the output (firmware/otp/otp_rma.24.vmem) is committed. Re-run only
# when the OpenTitan pin moves or you want a different life-cycle state.
#
# Why OTP matters: in life-cycle state RAW the CPU is never enabled. This image
# puts the chip in RMA, where lc_ctrl grants lc_cpu_en and the core can fetch.
#
# Usage: firmware/tools/gen_otp.sh [rma|dev|prod|test_unlocked0 ...]
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
ot="${root}/vendor/opentitan"
py="${root}/.venv/bin/python"
state="${1:-rma}"
data="${ot}/hw/top_earlgrey/data"

cd "${ot}"
PYTHONPATH=util:util/design "${py}" util/design/gen-otp-img.py --quiet \
  --lc-state-def  hw/ip/lc_ctrl/data/lc_ctrl_state.hjson \
  --mmap-def      "${data}/otp/otp_ctrl_mmap.hjson" \
  --img-cfg       "${data}/otp/otp_ctrl_img_${state}.hjson" \
  --top-secret-cfg "${data}/autogen/top_earlgrey.secrets.testing.gen.hjson" \
  --add-cfg       "${data}/otp/otp_ctrl_img_hw_cfg.hjson" \
  --add-cfg       "${data}/otp/otp_ctrl_img_creator_sw_cfg.hjson" \
  --add-cfg       "${data}/otp/otp_ctrl_img_owner_sw_cfg.hjson" \
  --out           "${root}/firmware/otp/otp_${state}.BITWIDTH.vmem"

echo "wrote firmware/otp/otp_${state}.24.vmem"
