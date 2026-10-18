# Titan-AXI — Earl Grey on an AXI fabric, with a fresh UVM TB

**Written:** 2026-10-18.

## Progress

| Phase | Status | Notes |
|---|---|---|
| 0. Archive & reset | ✅ 2026-10-18 | legacy on `legacy-earlgrey-tlul` / tag `legacy-tlul-v1`; `vendor/opentitan` re-added as a real submodule @ `365c167e` (old copy matched the pin except the two known local additions); OTP RMA image generated (`firmware/tools/gen_otp.sh`); ignored build output parked in `_legacy_local/` |
| 1. Bridges | ✅ 2026-10-18 (SIM-1) | PULP vendored (assert macros namespaced `PULP_*`); `gen_flist.py` maps all 621 chip RTL files to `vendor/` by hash; `uvm/sim/lint.py` (slang) used before every hand-off |
| 2. Fabric + top | 🟡 code done, chip lints clean (slang, sim + non-sim builds) — ✅ SIM-2 passed | `gen_fabric.py` → `titan_axi_fabric.sv` (4 hosts, 24+1 main devices, 27 peri devices, 4 `axi_cdc`) + `fabric_ports.json`; `patch_top.py` changes only the two crossbar instances (117 connections); `rtl.f` = 660 files |
| 3. UVM core | 🟡 code done, chip + TB lint clean — **waiting on SIM-3** | 56 passive AXI agents + fabric scoreboard + sw status monitor; UART/GPIO monitors; generated probe and address map |
| 4. Firmware | 🟡 code done, all 5 tests build — **waiting on SIM-3** | crt0 opens Earl Grey's reset PMP (MMWP: SRAM/PLIC denied at reset) and inits SRAM via sram_ctrl; 36 regtool headers |
| 5. Handover | ⬜ | |

**Simulation hand-offs** (the operator runs them, Claude never does):

| # | Command | Expect | Status |
|---|---|---|---|
| SIM-1 | `source uvm/sim/env.sh && uvm/bridge_tb/run.sh` | `BRIDGE TB PASSED` | ✅ 2026-10-18 (run 2): 324 reads, 338 writes, 32 error responses on host0; 60 AXI bursts on host1; 0 errors. Run 1 failed on a TB driver-alignment bug, since fixed |
| SIM-3 | `uvm/sim/run.sh fw_hello` | `RESULT: PASSED` (TB builds; chip boots from the hex ROM; firmware log in run.log) | ⏳ requested 2026-10-18 |
| SIM-2 | `uvm/sim/elab_rtl.sh` | `RESULT: PASSED` (full chip elaborates on Xcelium) | ✅ 2026-10-18, 42 s, 0 errors; warnings only DEFFIN (PULP deferred `final` assert) and XPWDIS (xprop note, same as upstream) |
**Decisions taken:** full Earl Grey (every IP kept), DUT RTL in a third folder
`rtl/`, PULP `axi` library for crossbar + CDC, old TB archived to a branch then
removed from `main`.

---

## 1. Goal

Every memory-mapped transaction in Earl Grey travels as **AXI4** —
host → crossbar → crossbar → IP. A new engineer only needs two folders:

| Folder | Owner | Contents |
|---|---|---|
| `firmware/` | trainees | C tests, startup code, linker script, drivers, the build that turns C into a hex image |
| `uvm/` | trainees | testbench top, agents, env, scoreboards, sequences, tests, run scripts |
| `rtl/` | DUT, read-only for trainees | the AXI fabric, TL-UL↔AXI bridges, forked top, file lists |
| `vendor/opentitan/` | upstream, never edited | the pinned Earl Grey RTL that `rtl/` builds on |

The deliverable is the infrastructure plus **7 basic tests** as proof. Engineers
then grow it.

---

## 2. Key facts that shape the design

These were checked against the pinned tree before writing this plan.

1. **Only the two crossbars change.** Every IP keeps its own TL-UL port (that
   is how reggen builds their register blocks). `top_earlgrey.sv` connects IPs
   to `u_xbar_main` / `u_xbar_peri` and to nothing else on the bus, so swapping
   those two instances for one AXI fabric touches nothing else in the chip.
2. **The ROM can be loaded as a plain hex file.** `rom_ctrl` with
   `SecDisableScrambling=1` instantiates a plain 32-bit `prim_rom_adv` — no
   PRINCE, no ECC — and skips the KMAC digest checker
   (`hw/ip/rom_ctrl/rtl/rom_ctrl.sv:72,295,426`). The upstream chip TB does
   exactly this (`hw/top_earlgrey/dv/tb/tb.sv:111`). So phase 1 runs firmware
   **straight from ROM**: `$readmemh` of a 32-bit hex, nothing else.
3. **OTP is still mandatory.** In life-cycle state `RAW` the CPU is never
   given `lc_cpu_en`. An OTP image placing the chip in `RMA` (or `DEV`) is
   generated **once** with the vendor tool `util/design/gen-otp-img.py` from
   `hw/top_earlgrey/data/otp/otp_ctrl_img_rma.hjson` and committed as a
   `.vmem`. It is passed in through `OtpMacroMemInitFile`.
4. **Main SRAM works without key setup** if firmware writes before it reads.
   `sram_ctrl` comes out of reset with its default key; reading an
   uninitialised word gives an integrity error. `crt0` zeroes `.bss` and the
   stack before use, so this holds.
5. **Bus integrity must survive the bridges.** Every device checks
   `a_user.cmd_intg` / `data_intg` (SECDED over addr/opcode/mask and data), and
   Ibex checks `d_user.rsp_intg`. Getting this wrong fires a fatal alert on
   every access. The bridges therefore carry the TL-UL user bits through AXI
   `USER` signals untouched (§4.2).
6. **`vendor/opentitan` is not a working submodule.** Its `.git` points at a
   `modules/` directory that does not exist in this repo, so the pinned commit
   (`365c167e`) cannot be checked locally. Phase 0 re-adds it properly.
7. **`IP/` is 1.4 GB**, mostly build output. Archive with a `.gitignore` for
   `xcelium.d/`, `runs/`, `*.shm`, or the archive commit will be huge.

---

## 3. Target architecture

```
                 chip_earlgrey_asic  (pads, AST — unchanged)
                 └── top_earlgrey    (forked copy: two xbar instances → one fabric)
                     │
  rv_core_ibex.corei ─TL─┐                                  ┌─TL─ rom_ctrl.rom, sram_main.ram, flash.mem ...
  rv_core_ibex.cored ─TL─┤                                  │     (24 main-side devices, each via axi2tlul)
  rv_dm.sba          ─TL─┤   ┌──────── titan_axi_fabric ────┴───────────────┐
  [sim] UVM AXI mstr ────┼──►│ tlul2axi ─► AXI xbar_main (clk_main) ─► axi2tlul │
                         │   │                 │ axi_cdc ×3 ─► spi_host0/1, usbdev
                         │   │                 │ axi_cdc ───► AXI xbar_peri (clk_io_div4)
                         │   │                 │                 └─► axi2tlul ×27 ─► uart, gpio, ...
                         │   │                 └─► [sim] titan_sim_ctrl (test status + log)
                         │   └───────────────────────────────────────────────┘
```

- **AXI everywhere between modules.** TL-UL survives only on the
  zero-length wires between an IP's port and its bridge, which sits at the
  edge of the fabric. Crossbar-to-crossbar is an AXI `axi_cdc`.
- **Ibex is untouched.** `rv_core_ibex` keeps its own TL-UL adapter,
  integrity generation and escalation gate; `tlul2axi` sits at the fabric
  input.
- **Sim-only additions** sit behind `` `ifdef TITAN_SIM ``:
  - a 4th AXI master on `xbar_main`, driven by the UVM AXI agent. This is the
    "stub CPU" path for pure-UVM tests, used while firmware is an idle loop.
  - `titan_sim_ctrl`, an AXI slave at `0x411F_0080–0x411F_00FF`. That is the
    same DV window OpenTitan uses, carved out of `rv_core_ibex.cfg`.
    Firmware writes test status to `+0x0` and log characters to `+0x4`. The
    UVM AXI monitor decodes both.

---

## 4. `rtl/` — the DUT

```
rtl/
├── README.md
├── vendor/
│   ├── pulp_axi/          pulp-platform/axi        @ v0.39.11 (pinned)
│   └── common_cells/      pulp-platform/common_cells @ version axi's Bender.yml requires
├── bridge/
│   ├── titan_tlul2axi.sv  host-side bridge   (TL-UL host  → AXI master)
│   └── titan_axi2tlul.sv  device-side bridge (AXI slave   → TL-UL device)
├── fabric/
│   ├── fabric_cfg.hjson         derived from vendor xbar_main/xbar_peri.hjson
│   ├── gen_fabric.py            generates the fabric + the UVM address map
│   ├── titan_axi_pkg.sv         widths, typedefs, address rules (generated)
│   ├── titan_axi_fabric.sv      the fabric (generated)
│   └── titan_sim_ctrl.sv        sim-only status/log slave
├── top/
│   ├── patch_top.py             replaces the two xbar instances; re-runnable
│   └── top_earlgrey.sv          output of patch_top.py (committed)
└── flist/
    ├── gen_flist.py             builds rtl.f from the FuseSoC-resolved source list
    └── rtl.f                    Earl Grey RTL − xbar_main/peri + fabric + PULP
```

### 4.1 AXI parameters

| Parameter | Value | Why |
|---|---|---|
| Data / addr width | 32 / 32 | TL-UL is 32/32 |
| Master ID width | 8 | carries TL-UL `a_source` |
| Slave ID width | 8 + ⌈log2(4)⌉ = 10 | PULP xbar prepends the master index |
| `AwUser`/`ArUser` | 23 bits = TL `a_user` + 3-bit opcode | `cmd_intg` covers the opcode, so it must be reproduced exactly |
| `WUser` | 7 bits = `data_intg` | |
| `RUser`/`BUser` | 14 bits = TL `d_user` | `rsp_intg` + `data_intg` from the device |
| Bursts | single beat (`AxLEN=0`) on every port in phase 1 | TL-UL has no bursts; noted as a phase-5 extension |

### 4.2 Bridge rules (where the real design work is)

**TL-UL → AXI field mapping**

| TL-UL | AXI |
|---|---|
| `Get` | AR, `ARLEN=0`, `ARSIZE=a_size` |
| `PutFullData` / `PutPartialData` | AW + W, `WSTRB=a_mask`, opcode in `AWUSER` |
| `a_source` | `AxID` |
| `a_user.instr_type == True` | `ARPROT[2]=1` (instruction), and also carried in `ARUSER` |
| `d_error=1` from device | `RRESP`/`BRESP = SLVERR` |
| no device at address | `DECERR` from the PULP xbar's default slave |

**Rules `titan_tlul2axi` must enforce**

- **Ordering.** TL-UL is one channel; AXI reads and writes are independent.
  A store followed by a load to the same address could reorder. The bridge
  therefore stalls on a read↔write direction change until all outstanding
  transactions have drained. This mirrors `tlul_socket_1n`'s
  `hold_all_requests`.
- **In-order responses.** Issue a single AXI ID per host port and keep
  `a_source`/`a_size` in a FIFO, so responses come back in issue order the way
  `tlul_adapter_host` expects.
- **`DECERR` carries no device user bits.** On `DECERR` the bridge builds a
  correct `rsp_intg` itself (`tlul_rsp_intg_gen`) and returns data
  `0xFFFF_FFFF` (`0` for instruction fetches), matching `tlul_err_resp`. If it
  didn't, an unmapped access would raise a fatal Ibex integrity alert instead
  of an access fault.

**Rules for `titan_axi2tlul`**

- Rebuild `tl_h2d_t` from AW/W (or AR) and the user bits.
- If an AXI master provides no integrity (a USER "intg-valid" bit is clear —
  e.g. a simple UVM sequence), generate it with `tlul_cmd_intg_gen`. Trainees
  can then write plain AXI sequences, and integrity-error tests can still send
  deliberately bad bits.
- Reject bursts (`AxLEN≠0`) with `SLVERR`.

### 4.3 Generation, not hand-editing

`gen_fabric.py` reads `fabric_cfg.hjson`, which is derived from the vendor
crossbar files, and emits:
- `titan_axi_fabric.sv`: PULP `axi_xbar` instances, address rules, connectivity, CDCs, bridges
- `uvm/env/titan_addr_map.sv`: the **same** address map, for the scoreboard

One source keeps the DUT and the checker in agreement. The address map follows
`tl_main_pkg.sv` / `tl_peri_pkg.sv` exactly, so `top_earlgrey.h` stays valid
for firmware.

`patch_top.py` takes the pinned `top_earlgrey.sv`, replaces the
`u_xbar_main` + `u_xbar_peri` instances with `u_axi_fabric`, and leaves the
`main_tl_peri_*` nets unused. It is idempotent and re-run when the OpenTitan
pin moves.

`gen_flist.py` starts from the source list FuseSoC already resolved
(`sim/scratch/.../lowrisc_dv_top_earlgrey_chip_sim_0.1.scr`, 866 entries,
including defines and incdirs). It drops DV files and the two generated
crossbars, and adds the fabric and PULP. Output: one frozen `rtl/flist/rtl.f`.
**From here on, FuseSoC, Bazel and dvsim are no longer needed to simulate.**

---

## 5. `firmware/` — C tests to hex image

```
firmware/
├── README.md               how to write and build a test
├── Makefile                make TEST=hello  |  make all  |  make clean
├── include/
│   ├── top_earlgrey.h      copied from vendor autogen (base addresses, IRQ ids)
│   ├── titan.h             test API: titan_pass(), titan_fail(), titan_puts()
│   ├── mmio.h  uart.h  gpio.h  timer.h  plic.h  irq.h
├── lib/
│   ├── crt0.S              vectors @0x8000, reset @0x8080, stack in main SRAM,
│   │                       zero .bss, copy .data, call main(), report result
│   ├── trap.c              weak exception/IRQ handlers, mcause/mtval capture
│   ├── titan.c             status + log writes to titan_sim_ctrl
│   └── uart.c gpio.c timer.c plic.c   minimal register-level drivers
├── link/rom.ld             .text/.rodata in ROM (32 KiB @ 0x8000),
│                           .data/.bss/stack in main SRAM (128 KiB @ 0x1000_0000)
├── tools/
│   ├── elf2hex.py          ELF → 32-bit-word $readmemh hex (+ .dis listing)
│   └── gen_otp.sh          one-time: vendor gen-otp-img.py → otp/otp_rma.vmem
├── otp/otp_rma.vmem        committed, generated
├── tests/                  one .c per test
└── build/<test>/           <test>.elf .dis .map .hex   (gitignored)
```

**Flow:** `riscv32-unknown-elf-gcc` (`/home/user1/riscv/bin`,
`-march=rv32imc -mabi=ilp32 -nostdlib`) → ELF → `elf2hex.py` → `<test>.hex`.
The UVM run script passes `+firmware=firmware/build/<test>/<test>.hex`. The TB
`$readmemh`s that into the ROM array at time 0, before POR releases, and the
OTP `.vmem` the same way. **Pass/fail:** `main()` returns 0 → `crt0` writes
PASS to `0x411F_0080`. The UVM `sw_status_monitor` sees that AXI write and ends
the test. A timeout means fail.

No OTTF, no DIFs, no Bazel: every line trainees see is in `firmware/`.

---

## 6. `uvm/` — the fresh testbench

```
uvm/
├── README.md
├── tb/
│   ├── tb_top.sv            chip_earlgrey_asic #(SecRomCtrlDisableScrambling=1,
│   │                        OTP init file), POR driver, pad pull-ups/downs (no
│   │                        floating pads → no X on TL-UL), firmware/OTP loader
│   └── titan_axi_probe.sv   bound into titan_axi_fabric: one axi_if per port,
│                            published to uvm_config_db by port name
├── agents/
│   ├── axi_agent/           axi_if (+ protocol SVA), item, master driver,
│   │                        monitor, sequencer, agent cfg (active/passive)
│   ├── uart_agent/          monitor + simple driver (RX/TX bytes)
│   └── gpio_agent/          pin driver/monitor on the 32 GPIO pads
├── env/
│   ├── titan_env_pkg.sv     cfg, env, virtual sequencer
│   ├── titan_addr_map.sv    generated by rtl/fabric/gen_fabric.py
│   ├── fabric_scoreboard.sv every master-side transaction must appear on the
│   │                        right slave port (or as DECERR), same data and resp
│   └── sw_status_monitor.sv decodes titan_sim_ctrl writes: PASS/FAIL + log text
├── seq/                     AXI sequences + virtual sequences
├── tests/                   titan_base_test + tests (§7)
└── sim/
    ├── run.sh               TEST=<uvm_test> [FW=<c test>] [SEED=] [WAVES=1]
    ├── uvm.f                TB files; pulls in ../../rtl/flist/rtl.f
    ├── regress.list
    └── runs/<test>/         run.log, waves (gitignored)
```

**Monitoring strategy:** a *passive* AXI agent on every fabric port (31
devices plus 4 hosts), plus one *active* master agent on the sim-only master
port. The fabric scoreboard turns all of that into an interconnect checker
from day one. That is the AXI-learning core of the project.

**Base test** does POR, waits for `pwrmgr` fetch-enable, loads images, and
sets a global timeout. A C test only needs `+firmware=`. A pure-UVM test loads
`firmware/tests/idle.c` (a `wfi` loop) and drives the AXI master.

---

## 7. Basic tests (proof of infrastructure)

| # | Test | Kind | Proves |
|---|---|---|---|
| T1 | `axi_sram_rw_test` | UVM, stub CPU | AXI master → `xbar_main` → `axi2tlul` → main SRAM; write/read-back |
| T2 | `axi_csr_reset_read_test` | UVM, stub CPU | reset values from IPs on both crossbars, through `axi_cdc` |
| T3 | `axi_decerr_test` | UVM, stub CPU | holes in both crossbars return `DECERR`; scoreboard agrees |
| T4 | `fw_hello_test` | C | firmware flow end to end: hex → ROM → boot → log → PASS |
| T5 | `fw_gpio_out_test` | C + UVM | firmware drives patterns; GPIO agent checks pins |
| T6 | `fw_uart_tx_test` | C + UVM | firmware sends a string; UART agent captures it |
| T7 | `fw_timer_irq_test` | C | `rv_timer` IRQ → Ibex → trap handler (validates `crt0`/`trap.c`) |

Everything beyond these is for the engineers: SPI/I²C agents, PLIC tests,
integrity-error injection, CDC stress, flash boot, coverage.

---

## 8. Phases and effort

One engineer, full time. Each milestone ends with a **human-run** simulation:
repo rule — automation never invokes `xrun`. I prepare and print the command,
and you run it.

| Phase | Work | Milestone (exit criteria) | Effort |
|---|---|---|---|
| **0. Archive & reset** | `.gitignore` build output; commit the current tree to branch `legacy-earlgrey-tlul` + tag; re-add `vendor/opentitan` as a real submodule @ `365c167e`; remove old folders from `main`; create the empty `rtl/ firmware/ uvm/` skeleton | `main` holds only the new skeleton + vendor; legacy branch has everything | 1–2 days |
| **1. Bridges** | `titan_tlul2axi`, `titan_axi2tlul`; vendor PULP `axi` + `common_cells`; a small standalone bridge TB under `uvm/` (TL host model ↔ bridge pair ↔ TL device model) | ordering, `DECERR` integrity, user-bit passthrough all pass in a seconds-long sim | 2 wks |
| **2. Fabric + top** | `fabric_cfg.hjson`, `gen_fabric.py`, `patch_top.py`, `gen_flist.py`, `titan_sim_ctrl` | full chip **elaborates** on Xcelium with the AXI fabric | 1.5–2 wks |
| **3. UVM core** | `axi_agent` (+SVA), probe bind, env, fabric scoreboard, `tb_top`, POR, pads, OTP/ROM loader, `run.sh` | **T1–T3 pass** (CPU idle) | 2.5–3 wks |
| **4. Firmware** | `crt0`, `rom.ld`, `trap.c`, `titan.c`, drivers, `Makefile`, `elf2hex.py`, `gen_otp.sh`, `sw_status_monitor`, UART/GPIO agents | **T4–T7 pass** | 2 wks |
| **5. Handover** | `regress.list`, three READMEs, a short "write your first test" guide in each of `firmware/` and `uvm/` | full regression green; a new engineer adds a test using only the READMEs | 0.5–1 wk |

**Total: about 9–11 weeks** to "7 basic tests green".

### Later phases (for the engineers, not this plan)

- Flash boot: second-stage image in eflash via `gen-flash-img.py`
- Scrambled ROM + digest check (`SecRomCtrlDisableScrambling=0`, vendor `scramble_image.py`)
- AXI bursts on memory ports (needs a burst-aware `axi2tlul` or an AXI-native SRAM)
- JTAG/`rv_dm` agent; alert/escalation tests; functional coverage

---

## 9. Risks

| Risk | Impact | Mitigation |
|---|---|---|
| Bridge ordering or integrity bug | every access fails or alerts; slow to debug at chip level | phase 1 proves the bridges standalone before any chip sim |
| `rv_core_ibex` alerts on response integrity | Ibex locks up on first `DECERR` | explicit rule in §4.2; covered by T3 and the bridge TB |
| Frozen file list misses a define or incdir | elaboration errors | start from FuseSoC's resolved `.scr`, not by hand |
| PULP + OpenTitan package/macro clashes (`assertions.svh` vs `prim_assert.sv`) | compile errors | compile PULP first, using its own include dirs; rename if needed |
| Boot sequencing (AST → pwrmgr → rom_ctrl → lc_cpu_en) | CPU never fetches | OTP RMA image; check fetch-enable in the base test with a clear error |
| Floating pads → X on reads | `dKnown` assertion failures (old quirk #14) | TB pulls every pad |
| Sim speed: full chip, AST power-up | minutes per test | ROM boot skips the TEST ROM → flash hop; bridge TB for fast iteration |
| Pinned vendor tree cannot be verified (§2.6) | wrong RTL baseline | phase 0 re-clones at `365c167e` and diffs against the current copy |

---

## 10. Phase 0 — exact steps, for approval

1. Add `.gitignore`: `**/xcelium.d/`, `**/runs/`, `**/scratch/`, `*.shm`,
   `*.trn`, `*.dsn`, `**/build/`, `.venv/`, `__pycache__/`, `*.log`.
2. `git checkout -b legacy-earlgrey-tlul`; commit `docs/ IP/ overlay/ scripts/
   sim/ sw/ testplan/ tests/ joules_work/ RESUME_TITAN.md README.md`, minus the
   ignored build output. Tag `legacy-tlul-v1`. Push only if you say so.
3. Back on `main`: re-add `vendor/opentitan` as a submodule at `365c167e` and
   diff it against the current copy, to see what (if anything) was hand-edited.
4. On `main`, remove the legacy folders. Create `rtl/ firmware/ uvm/`, each
   with a README stub. Rewrite `README.md` to point at the three folders.
   Keep `scripts/activate_env.*` (moved to `uvm/sim/env.*`) so Xcelium setup
   still works.
5. Commit. Nothing is deleted that is not on the legacy branch.

**Open point:** the `IP/` block-level track (23 IPs) goes to the legacy branch
with everything else. Say if you want it kept on `main` instead.
