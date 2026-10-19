# uvm/ — the testbench

A UVM testbench around the whole OpenTitan Earl Grey chip, whose crossbars are
an **AXI4** fabric. Every bus port of the fabric is monitored, and a scoreboard
checks the interconnect on every transaction of every test.

```
tb_top
├── chip_earlgrey_asic  (Earl Grey + titan_axi_fabric; ROM <- firmware hex, OTP <- OTP image)
│     └── titan_axi_fabric ◄── titan_axi_probe (bound in): one axi_if per port, 56 ports
└── uvm_test_top (a titan_base_test)
      └── env (titan_env)
            ├── axi_<port> ×56   passive axi_agent — active on host.sim_host (stub CPU)
            ├── scoreboard       fabric_scoreboard
            ├── sw               sw_status_monitor (firmware status + log)
            ├── uart0            uart_monitor  (pad IOC4)
            ├── gpio             gpio_monitor  (pads IOB0..7; drive them via env.gpio.vif)
            └── clk              clk_monitor   (clkmgr outputs: frequency, gating)
```

## Run

```bash
source uvm/sim/env.sh                 # once per shell (csh: source uvm/sim/env.csh)
uvm/sim/run.sh --list                 # the tests
uvm/sim/run.sh fw_hello               # build firmware + testbench, run one test
uvm/sim/run.sh fw_hello --seed 7
uvm/sim/run.sh fw_hello --waves       # + waves: simvision uvm/sim/runs/fw_hello/waves.shm
uvm/sim/run.sh fw_hello --verbosity UVM_HIGH   # every AXI transaction on every port
uvm/sim/regress.sh                    # all tests
uvm/sim/lint.py --top tb_top --uvm -D TITAN_SIM -D SIMULATION rtl/flist/rtl.f uvm/sim/uvm.f
                                      # ~1-minute compile check with slang, no simulator
```

Pass = `RESULT: PASSED` (the log says `*** TEST PASSED ***`). Log:
`uvm/sim/runs/<test>/run.log`. Build log: `uvm/sim/runs/build/build.log`.
A test takes about a minute: OpenTitan's power-up (AST, OTP, life cycle)
takes ~2.3 ms of simulated time before the CPU runs.

> One simulation at a time — the server is shared.

## The tests

**Full test plan, one section per test: [`TESTS.md`](TESTS.md).**

| Area | Tests |
|---|---|
| Bring-up | `fw_hello` |
| Interconnect | `axi_sram_rw`, `axi_csr_reset`, `axi_decerr` |
| Memory map / memories | `axi_mem_map_sweep`, `axi_mem_test` |
| Clocks | `clk_freq`, `fw_clk_gate` |
| Reset | `fw_sw_reset`, `fw_wdog_reset` |
| Interrupts | `fw_timer_irq`, `fw_plic_gpio_irq` |
| Peripherals | `fw_gpio_out`, `fw_uart_tx` |

14 tests, all passing (2026-10-19). Each also checks ~20–43 thousand fabric transactions.

## How a test runs

1. `tb_top` loads the firmware hex into ROM and the OTP image, then releases
   POR at 10 µs.
2. The chip powers up by itself; Ibex runs `firmware/lib/crt0.S`, which writes
   **IN_TEST** to `titan_sim_ctrl` → `env.sw.ev_in_test` fires.
3. The test's `test_body()` runs.
   - **Firmware tests**: wait for the firmware to write PASSED/FAILED
     (`env.sw.ev_done`), then check pins (UART text, GPIO values).
   - **Stub-CPU tests**: firmware is `idle` (parks in `wfi`); the test runs a
     sequence on `env.sim_master()`, a UVM AXI master wired into xbar_main as a
     4th host.
4. `report_phase` prints `*** TEST PASSED ***` if there were no UVM errors.

## Add a test

**C only** (firmware checks everything itself):
1. `firmware/tests/my_test.c` (see `firmware/README.md`)
2. `uvm/sim/tests.list`: `my_test   fw_test   my_test`
3. `uvm/sim/run.sh my_test`

**C + testbench check** (the testbench must observe or drive pins, or act at
a particular point of the firmware — the firmware calls `titan_sync(n)`, the
test waits with `env.sw.wait_sync(n)`):
1. the C file, as above
2. in `uvm/tests/titan_test_pkg.sv`, a class extending `titan_base_test` whose
   `test_body()` calls `super.test_body()` (wait for the firmware) and then
   checks `env.uart0.text`, `env.gpio.values`, ... — model it on
   `fw_uart_tx_test`
3. a line in `tests.list` naming the new class

**UVM only** (the testbench is the bus master):
1. a sequence extending `axi_test_seq` (use `write32`, `read32`, `write`, `read`
   and `check`) — model it on `csr_reset_seq`
2. a test extending `axi_stub_cpu_test` returning it from `make_seq()`
3. a line in `tests.list` with firmware `idle`

## What checks what

| Component | Checks |
|---|---|
| `axi_if` assertions | VALID stays high and payload stable until READY, on every port; no X on VALIDs |
| `axi_monitor` | WLAST/RLAST agree with AxLEN; every B/R has an outstanding request with its ID |
| `fabric_scoreboard` | (drops in-flight transactions when the chip resets) every host-side transaction is decoded (`titan_addr_map_pkg::decode`): unmapped or unreachable from that host → must be DECERR; otherwise the device port must have seen it with the same host index (ID bits [5:4]), ID, address, length, size, burst, prot, write data/strobes, and returned the same read data and responses. Device transactions without a host twin are errors. |
| `sw_status_monitor` | firmware FAILED → UVM error; prints the firmware's log as `[SW]`; counts boots (`n_boots`); `wait_sync(n)` |
| the tests | their own expectations (sequences, pins) |

## Files

| Path | What |
|---|---|
| `agents/axi_agent/` | `axi_if.sv` (signals + SVA), `axi_agent_pkg.sv` (item, monitor, driver, agent, base sequence) |
| `agents/uart_agent/`, `agents/gpio_agent/` | pin-level monitors (GPIO can also drive) |
| `agents/clk_agent/` | `clk_mon_if` + `clk_monitor`: count edges / measure frequency of the clkmgr outputs |
| `env/titan_env_pkg.sv` | env, scoreboard, firmware status monitor |
| `env/titan_addr_map_pkg.sv` | **generated**: address map + connectivity |
| `tests/titan_test_pkg.sv` | base test, tests, sequences |
| `TESTS.md` | the test plan |
| `tb/tb_top.sv` | chip, pads, POR, image loading |
| `tb/titan_axi_probe.sv` | **generated**: one `axi_if` per fabric port, bound into the fabric |
| `tb/gen_tb.py` | generates the two files above from `rtl/fabric/fabric_ports.json` |
| `sim/run.sh`, `sim/regress.sh`, `sim/tests.list` | running |
| `sim/uvm.f` | testbench file list (the DUT's is `rtl/flist/rtl.f`) |
| `sim/elab_rtl.sh` | elaborate the DUT alone (after an `rtl/` change) |
| `sim/lint.py` | slang compile check (needs `.tools/uvm-core`: `git clone --depth 1 https://github.com/accellera-official/uvm-core .tools/uvm-core`) |
| `bridge_tb/` | standalone testbench for the TL-UL↔AXI bridges (seconds per run) |

## Exercises (the testbench is intentionally minimal)

1. **AXI driver**: several outstanding transactions, random VALID delays, W
   before AW. Watch the scoreboard keep up.
2. **Functional coverage** on the AXI monitor: burst type × length × size ×
   response, per port. Which ports never see a burst?
3. **Random stub-CPU test**: random addresses across the whole map, reads and
   writes, with the scoreboard as the only checker.
4. **Concurrency**: sim master traffic *while* firmware runs — real arbitration
   between hosts in xbar_main.
5. **Integrity**: send a write with `AWUSER.intg_vld = 1` and wrong `cmd_intg`
   (extend the probe's driving block): the IP must flag a fatal alert.
6. **UART RX driver** and a firmware echo test.
