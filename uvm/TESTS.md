# titan-axi test plan

Every test boots the real chip (POR → AST/power-up → OTP → life cycle → ROM →
`crt0` → `main()`), and in every test the **fabric scoreboard** checks every
transaction on every AXI port. So each entry below lists only what the test
adds on top of that.

Run one: `uvm/sim/run.sh <name>`. Run all: `uvm/sim/regress.sh`.
Registry: `uvm/sim/tests.list`. Code: `uvm/tests/titan_test_pkg.sv` (UVM side),
`firmware/tests/<firmware>.c` (C side).

| Area | Test | Kind | Status (2026-10-19: 14/14 pass) |
|---|---|---|---|
| Bring-up | [`fw_hello`](#fw_hello) | C | ✅ |
| Interconnect | [`axi_sram_rw`](#axi_sram_rw) | UVM | ✅ |
| Interconnect | [`axi_csr_reset`](#axi_csr_reset) | UVM | ✅ |
| Interconnect | [`axi_decerr`](#axi_decerr) | UVM | ✅ |
| Memory map | [`axi_mem_map_sweep`](#axi_mem_map_sweep) | UVM | ✅ |
| Memory | [`axi_mem_test`](#axi_mem_test) | UVM | ✅ |
| Clocks | [`clk_freq`](#clk_freq) | TB | ✅ |
| Clocks | [`fw_clk_gate`](#fw_clk_gate) | C + UVM | ✅ |
| Reset | [`fw_sw_reset`](#fw_sw_reset) | C + UVM | ✅ |
| Reset | [`fw_wdog_reset`](#fw_wdog_reset) | C + UVM | ✅ |
| Interrupts | [`fw_timer_irq`](#fw_timer_irq) | C | ✅ |
| Interrupts | [`fw_plic_gpio_irq`](#fw_plic_gpio_irq) | C + UVM | ✅ |
| Peripherals | [`fw_gpio_out`](#fw_gpio_out) | C + UVM | ✅ |
| Peripherals | [`fw_uart_tx`](#fw_uart_tx) | C + UVM | ✅ |

Kinds: **C** = the firmware checks and reports; **UVM** = the testbench is the
bus master (firmware `idle`); **TB** = the testbench observes the chip
(firmware `idle`); **C + UVM** = both sides take part, synchronized with
`titan_sync(n)` / `env.sw.wait_sync(n)` or pins.

---

## Bring-up

### fw_hello
| | |
|---|---|
| Goal | The firmware flow works end to end |
| How | `firmware/tests/hello.c` checks `.data` was copied, `.bss` zeroed, SRAM read-back, M-extension results, and prints two log lines |
| Passes when | firmware reports PASSED |
| Proves | hex → ROM backdoor load; boot (PMP, clock calibration, SRAM init); C code; Ibex → xbar_main → `titan_sim_ctrl` |

---

## Interconnect

### axi_sram_rw
| | |
|---|---|
| Goal | Main SRAM through the fabric, including bursts |
| How | UVM AXI master: 32 single words, 1 byte write (one lane), INCR bursts of 2–16 beats, all read back |
| Passes when | every read-back matches |
| Proves | xbar_main routing; `titan_axi2tlul` burst splitting; partial writes (TL-UL PutPartialData) |

### axi_csr_reset
| | |
|---|---|
| Goal | Registers on both crossbars are reachable and hold their reset values |
| How | Read 7 registers: uart0, gpio, rv_timer (xbar_peri, behind the main→peri CDC), spi_host0 (its own CDC), hmac, rv_plic, sram_ctrl (xbar_main) |
| Passes when | every value equals the reset value in the IP's `.hjson` |
| Proves | address decode on both crossbars; both clock-domain crossings |

### axi_decerr
| | |
|---|---|
| Goal | Addresses that decode to nothing are refused correctly |
| How | Read and write 5 holes: gaps in xbar_main, past a device window, above the PLIC, gaps in xbar_peri |
| Passes when | all 10 accesses return DECERR |
| Proves | both crossbars' default error responders; DECERR travels back across the CDC |

---

## Memory map and memories

### axi_mem_map_sweep
| | |
|---|---|
| Goal | **Every** device window in the chip answers, and nothing answers where nothing is mapped |
| How | For each of the 52 address windows (generated from the RTL's own address map, `titan_addr_map_pkg`): read its first word; if the address just past the window belongs to no device, read it too |
| Passes when | every device answers OKAY or SLVERR (a register it refuses — still an answer), never DECERR or a hang; every unowned address gives DECERR |
| Proves | the full memory map — and, through the scoreboard, that each access reached exactly the right port |
| Result | 46 windows answered (45 OKAY; `rv_core_ibex.cfg` above the DV window gives SLVERR — no register there, correct), 42 gaps DECERR |
| Skipped | `flash_ctrl.mem` (flash uninitialized: a read stalls), `flash_ctrl.prim`, `otp_macro.prim` (macro test interfaces), `rv_dm.*` (debug only), `sram_ctrl_ret_aon.ram` (uninitialized; see below). Enabling each is an exercise. |

### axi_mem_test
| | |
|---|---|
| Goal | The memories behave as memories |
| How | (1) main SRAM data bus: walking 1 and walking 0 over 32 bits; (2) main SRAM address lines: a unique value at base + 2^k for k = 2..15, then read all back; (3) retention SRAM: start `sram_ctrl_ret_aon` CTRL.INIT over AXI, wait for INIT_DONE, then the same two tests; (4) ROM: the first 256 words read over AXI equal the firmware hex image |
| Passes when | every read-back matches |
| Proves | data/address-line integrity (stuck or shorted bits, aliasing); SRAM init through software; the ROM read path end to end |

---

## Clocks

### clk_freq
| | |
|---|---|
| Goal | Every clock is at the frequency the spec says |
| How | The testbench counts edges of each clkmgr output (`clk_mon_if`, `clk_agent_pkg`) after the firmware's `crt0` has calibrated the AST oscillators |
| Passes when | main 100 MHz, io 96, io_div2 48, io_div4 24, usb 48 (±2%); aon 200 kHz (±5%) |
| Result | 99.95 / 96.000 / 48.000 / 24.000 / 47.55 MHz / 200 kHz (the USB model has a drift term: −0.9%) |
| Proves | AST calibration, clkmgr dividers |
| Note | Before calibration the models run at random 25–60 MHz (IO) — that is what broke the UART in the first bring-up run |

### fw_clk_gate
| | |
|---|---|
| Goal | Software clock gating works for each gateable clock |
| How | `firmware/tests/clk_gate.c` gates each of `clk_io_div4_peri`, `clk_io_div2_peri`, `clk_io_peri`, `clk_usb_peri` in turn (clkmgr `CLK_ENABLES`), syncs, waits 20 µs, ungates, syncs, waits. The UVM test counts edges 2 µs after each sync |
| Passes when | 0 edges in 5 µs while gated; ≥ 100 edges in 5 µs after ungating; firmware PASSED |
| Proves | clkmgr gating per branch |
| Careful | Never touch an IP while its clock is gated — the access never completes and the bus hangs. The firmware waits on `rv_timer`, which is on the ungated `clk_io_div4_timers` branch |

---

## Reset

### fw_sw_reset
| | |
|---|---|
| Goal | A software-requested reset resets the chip and is recorded as such |
| How | Boot 1: `RESET_INFO` must be POR; clear it; write `rstmgr RESET_REQ`. Boot 2: `RESET_INFO` must be exactly SW_RESET |
| Passes when | firmware PASSED on boot 2 **and** the testbench saw exactly 2 boots (`fw_reset_test`) |
| Proves | rstmgr reset request; reset-cause recording in the always-on domain; the whole boot sequence a second time; the fabric recovering from reset (the scoreboard drops in-flight transactions at reset) |

### fw_wdog_reset
| | |
|---|---|
| Goal | The always-on watchdog resets the chip when not serviced |
| How | Boot 1: enable aon_timer as a pwrmgr reset source (`RESET_EN`, then `CFG_CDC_SYNC` into the AON domain), start the watchdog with a 100 µs bite, never pet it. Boot 2: `RESET_INFO` must be exactly HW_REQ[1] (aon_timer) |
| Passes when | firmware PASSED on boot 2 **and** exactly 2 boots |
| Proves | aon_timer watchdog; pwrmgr reset-request path; slow-domain (200 kHz) register programming |

---

## Interrupts

### fw_timer_irq
| | |
|---|---|
| Goal | rv_timer interrupt reaches the core |
| How | 5 deadlines 20 µs apart; each must cause exactly one machine-timer interrupt, not early |
| Passes when | firmware PASSED |
| Proves | rv_timer → Ibex `mtip`; vector table, trap entry/exit |

### fw_plic_gpio_irq
| | |
|---|---|
| Goal | Peripheral interrupts reach the core through the PLIC |
| How | Firmware: GPIO[3:0] as inputs with rising-edge interrupts, each enabled in the PLIC; `titan_sync(1)` when armed. Testbench: raises pins 0,1,2,3 five µs apart. ISR: claim, clear at the source, complete |
| Passes when | exactly 4 interrupts, from PLIC sources `GpioGpio0..3` in order, none extra |
| Proves | pad → pinmux → GPIO → PLIC → Ibex `meip`; claim/complete |

---

## Peripherals

### fw_gpio_out
| | |
|---|---|
| Goal | GPIO outputs reach the pads |
| How | Firmware drives walking 1, walking 0, 0xA5 on GPIO[7:0] (and reads each back on DATA_IN) |
| Passes when | firmware PASSED **and** the pins showed exactly the 17 values in order |

### fw_uart_tx
| | |
|---|---|
| Goal | UART0 transmits correctly |
| How | Firmware sends one line at 1 Mbaud on pad IOC4 |
| Passes when | firmware PASSED **and** the UART monitor decoded exactly that line |

---

## Next tests to write (suggested)

| Area | Test idea |
|---|---|
| Reset | per-IP software resets (`rstmgr SW_RST_CTRL_N`): reset spi_host0, check its registers return to reset values |
| Reset | POR during traffic: re-assert `POR_N` from the testbench mid-test |
| Clocks | clkmgr frequency measurement (`*_MEAS_CTRL`): program bounds, check no error is flagged; then wrong bounds, check it is |
| Clocks | transactional clock hints (`CLK_HINTS`) for aes/hmac/kmac/otbn |
| Power | `wfi` + pwrmgr low-power entry, woken by aon_timer wake-up |
| Memory map | enable the skipped windows one by one (flash after `flash_ctrl` init, retention SRAM after init) |
| Interrupts | every PLIC source via the IPs' `INTR_TEST` registers — a full interrupt connectivity sweep |
| Alerts | `ALERT_TEST` of each IP → alert_handler; escalation to reset |
| Interconnect | concurrent traffic from the UVM master and the CPU; random addresses; bursts to every memory |
