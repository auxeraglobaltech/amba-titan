# uvm/sim/env.sh — per-session environment (bash).  Usage:  source uvm/sim/env.sh
#
# Puts the RISC-V toolchain and the project Python on PATH and checks for xrun.
# It does not run any simulator.

if [[ -n "${BASH_SOURCE[0]:-}" ]]; then
    TITAN_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
else
    TITAN_ROOT="$(pwd)"
fi
export TITAN_ROOT

# RISC-V GCC (riscv32-unknown-elf-*), used by firmware/Makefile.
export RISCV_PREFIX="${RISCV_PREFIX:-/home/user1/riscv/bin/riscv32-unknown-elf-}"
export PATH="$(dirname "${RISCV_PREFIX}"):${PATH}"

# Project Python (generators in rtl/, tools in firmware/).  Create it once with:
#   ~/.local/opt/python-3.11.15-titan/bin/python3 -m venv .venv
#   .venv/bin/pip install -r requirements.txt
if [[ -x "${TITAN_ROOT}/.venv/bin/python" ]]; then
    export PATH="${TITAN_ROOT}/.venv/bin:${PATH}"
else
    echo "WARNING: ${TITAN_ROOT}/.venv missing — see the comment in uvm/sim/env.sh" >&2
fi

# Non-UTF-8 locales break some of the vendor Python tools.
export LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8

echo "titan-axi environment:"
echo "  TITAN_ROOT = ${TITAN_ROOT}"
echo "  gcc        = $(command -v "${RISCV_PREFIX}gcc" || echo 'NOT FOUND')"
echo "  python     = $(command -v python) ($(python --version 2>&1))"
echo "  xrun       = $(command -v xrun || echo 'NOT FOUND — load your Cadence setup first')"
