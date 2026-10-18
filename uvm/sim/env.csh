# uvm/sim/env.csh — per-session environment (csh/tcsh).  Usage:  source uvm/sim/env.csh
# Must be sourced from the repo root (csh cannot reliably find its own path).

setenv TITAN_ROOT `pwd`
if (! -d "$TITAN_ROOT/uvm/sim") then
    echo "ERROR: source this from the repo root"
    exit 1
endif
if (! $?RISCV_PREFIX) setenv RISCV_PREFIX /home/user1/riscv/bin/riscv32-unknown-elf-
setenv PATH `dirname $RISCV_PREFIX`:$PATH
if (-x "$TITAN_ROOT/.venv/bin/python") then
    setenv PATH "$TITAN_ROOT/.venv/bin:$PATH"
else
    echo "WARNING: $TITAN_ROOT/.venv missing - see uvm/sim/env.sh"
endif
setenv LANG en_US.UTF-8
setenv LC_ALL en_US.UTF-8
rehash
echo "titan-axi environment:"
echo "  TITAN_ROOT = $TITAN_ROOT"
echo "  gcc        = `which ${RISCV_PREFIX}gcc`"
echo "  xrun       = `which xrun`"
