# firmware/ — C tests

Every test is one C file in `tests/`. `make TEST=<name>` compiles it with
the startup code in `lib/` and the linker script in `link/`, and produces
`build/<name>/<name>.hex`, which the UVM testbench loads into the chip's ROM.

| Dir | Contents |
|---|---|
| `include/` | base addresses (`top_earlgrey.h`), test API (`titan.h`), drivers |
| `lib/` | `crt0.S`, trap handling, test status/log, minimal drivers |
| `link/` | `rom.ld` — code in ROM, data and stack in main SRAM |
| `tools/` | `elf2hex.py`, `gen_otp.sh` |
| `otp/` | the OTP image the chip boots with (generated, committed) |
| `tests/` | the tests |

Full guide: written in Phase 5 of [`../AXI_SOC_PLAN.md`](../AXI_SOC_PLAN.md).
