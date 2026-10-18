# firmware/ — C tests that run on the chip

Every test is one C file in `tests/`. It is compiled with the startup code
and drivers in `lib/`, linked into the chip's **ROM**, and turned into a hex
file that the UVM testbench loads into ROM before reset. The CPU (Ibex) boots
from it, runs your `main()`, and reports pass/fail to the testbench — through
the AXI fabric, like every other access.

```
tests/hello.c ─► gcc ─► build/hello/hello.elf ─► objcopy + elf2hex ─► build/hello/hello.hex ─► ROM
```

## Build

```bash
source uvm/sim/env.sh          # once per shell (csh: source uvm/sim/env.csh)
cd firmware
make TEST=hello                # build/hello/{hello.elf,.dis,.map,.hex}
make all                       # every tests/*.c
make list
```

You never need to run make yourself to simulate: `uvm/sim/run.sh <test>` builds
the right image first.

## Write a test

```c
// tests/my_test.c
#include "titan.h"

int main(void) {
  titan_printf("my_test: starting\n");            // appears in run.log as [SW]
  uint32_t v = reg32_read(TOP_EARLGREY_GPIO_BASE_ADDR, GPIO_DATA_IN_REG_OFFSET);
  CHECK(v == 0, "DATA_IN = 0x%x", v);             // fails the test with file:line
  return 0;                                        // 0 = PASS, anything else = FAIL
}
```

Then register it in `uvm/sim/tests.list` (one line, see `uvm/README.md`) and
run `uvm/sim/run.sh my_test`.

Include `regs/<ip>_regs.h` for an IP's register offsets and fields.

## What happens before `main()` (lib/crt0.S)

Earl Grey is a security chip, so even "jump to C" takes four steps. Each one
was learned by a failing simulation:

| # | Step | Why |
|---|---|---|
| 1 | **Open the PMP**: one locked all-access rule (entry 15), reset rules cleared | Earl Grey's reset PMP uses MMWP: M-mode may only touch what a rule allows, and the reset rules allow only the first 2 KiB of ROM, MMIO 0x4000_0000–0x4201_0000 and the debug ROM. Main SRAM (the stack) and the PLIC are **denied**. So this must run from the first 2 KiB of ROM — `rom.ld` asserts it does. |
| 2 | **Calibrate the clocks**: rewrite AST `REGAL`, wait for `AST_INIT_DONE` | The oscillators start uncalibrated (IO clock ~26 MHz instead of 96). Without this, every baud rate and delay is ~4× off. |
| 3 | **Initialize main SRAM**: `sram_ctrl` `CTRL.INIT` | Until written, SRAM words have bad ECC; reading one is an integrity error. |
| 4 | Zero `.bss`, copy `.data`, report `IN_TEST`, call `main()`, report `PASSED`/`FAILED` | The usual C runtime start. |

## Memory map (link/rom.ld)

| Region | Address | Size | Holds |
|---|---|---|---|
| ROM | `0x0000_8000` | 32 KiB | vectors (+0x00), reset (+0x80), code, constants, `.data` image |
| main SRAM | `0x1000_0000` | 128 KiB | `.data`, `.bss`, 16 KiB stack. Stub-CPU UVM tests use `0x1001_0000`+ |
| titan_sim_ctrl | `0x411F_0080` | 32 B | status (+0x0) and log (+0x4) to the testbench — sim only |

Base addresses of every IP: `include/top_earlgrey.h` (`TOP_EARLGREY_*_BASE_ADDR`).

## The test API (include/titan.h)

| Call | Does |
|---|---|
| `return 0` from `main()` | pass (non-zero: fail) |
| `titan_pass()`, `titan_fail()` | end the test now |
| `CHECK(cond, fmt, ...)` | fail with file:line and a message if `cond` is false |
| `titan_printf/puts/putc` | log text to the testbench (`%d %u %x %08x %c %s %p`) |
| `reg32_read/write(base, offset[, value])` | register access |
| `pinmux_connect_uart0()`, `pinmux_connect_tb_gpios()` | connect peripherals to the pads the testbench watches |
| `uart_init/putc/puts/flush` | UART0 |
| `gpio_set_oe/write/read` | GPIO |
| `timer_init_us`, `timer_now_us`, `delay_us`, `timer_set_deadline_us`, `timer_irq_enable/ack` | rv_timer, 1 tick = 1 µs |
| `irq_global_enable`, `irq_timer_enable`, `irq_external_enable`, `plic_enable/claim/complete` | interrupts |
| define `titan_isr_timer()` / `titan_isr_external()` | your interrupt handler (default: fail "unexpected interrupt") |
| define `titan_exception_handler()` | your exception handler; return true to skip the faulting instruction |

The drivers in `lib/drivers.c` are deliberately minimal: read them next to the
register header to see the programming sequence.

## Pads the testbench is connected to

Out of reset the pinmux connects **nothing** — a test that uses a pad must
connect it first.

| Signal | Pad | Testbench side |
|---|---|---|
| UART0 TX / RX | IOC4 / IOC3 | `uart_monitor` (1 Mbaud, 8N1) / held idle-high |
| GPIO[7:0] | IOB0..IOB7 | `gpio_monitor` |
| every other pad | — | weak pull-down (no floating inputs, so no X on reads) |

## Files

| Path | What |
|---|---|
| `tests/*.c` | the tests (`hello`, `idle`, `gpio_out`, `uart_tx`, `timer_irq`) |
| `include/titan.h` | the API above — include only this |
| `include/regs/*_regs.h` | register offsets/fields for all 36 IPs — **generated**, `tools/gen_regs.sh` |
| `include/top_earlgrey.h`, `top_earlgrey_memory.h` | base addresses, IRQ IDs, pinmux indices — copied from OpenTitan |
| `lib/crt0.S`, `lib/trap.c` | boot, vector table, trap dispatch |
| `lib/titan.c`, `lib/drivers.c`, `lib/string.c` | status/log/printf, drivers, `memset`/`memcpy` |
| `link/rom.ld` | memory layout |
| `tools/elf2hex.py` | ELF → 8192-word `$readmemh` ROM image (zero padded: prefetches past the code read 0, not X) |
| `tools/gen_otp.sh`, `otp/otp_rma.24.vmem` | the OTP image (life cycle RMA, so the CPU is allowed to run) |

## When it goes wrong

| Symptom in `uvm/sim/runs/<test>/run.log` | Look at |
|---|---|
| `firmware never reached main()` | boot: `build/<t>/<t>.dis` around `_reset_start`; waves of `u_rv_core_ibex` |
| `EXCEPTION mcause=... mepc=... mtval=...` | `mepc` in the `.dis` file; mcause 1/5/7 = fetch/load/store access fault (wrong address, or PMP) |
| `CHECK FAILED file:line` | your test |
| wrong baud / timing off by ~4× | clocks not calibrated (crt0 step 2) |
| test passes in C, testbench check fails | the C file and its UVM test must agree on data (e.g. the GPIO pattern table) |

## Exercises

1. A UART **RX** test: add a driver to `uvm/agents/uart_agent`, receive bytes in C.
2. GPIO **input** interrupts through the PLIC (`plic_enable`, `titan_isr_external`).
3. `rv_timer` + `aon_timer` together; a watchdog bark.
4. Entropy → CSRNG → AES: port the entropy init from OpenTitan's test ROM into `lib/`.
5. Execute from main SRAM: copy a function there and call it (`corei` can reach SRAM — and nothing on `xbar_peri`; try it and watch the DECERR).
