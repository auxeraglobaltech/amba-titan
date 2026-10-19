# Following a transaction from initiator to IP

Every memory-mapped access in the chip crosses the same kinds of boundary.
This guide names each one, where to find it in the hierarchy, and which test
shows it best.

Two ways to watch:

```bash
# 1. Text: list every AXI port one address passed through, in hop order
uvm/sim/run.sh axi_csr_reset --trace 0x40000014
grep TRACE uvm/sim/runs/axi_csr_reset/run.log

# 2. Waves: every hop, signal by signal
uvm/sim/run.sh axi_csr_reset --waves
simvision uvm/sim/runs/axi_csr_reset/waves.shm &
```

`--trace` only sees the AXI ports (the probe interfaces). For the two TL-UL
ends of a path — the CPU side of the host bridge and the IP side of the device
bridge — use waves.

In the tables below:

```
F = tb_top.dut.top_earlgrey.u_axi_fabric             (the fabric)
P = tb_top.dut.top_earlgrey.u_axi_fabric.u_titan_axi_probe   (one axi_if per AXI port)
```

Each `P.if_*` is an `axi_if` with plain AXI names (`awvalid awready awaddr
awid … rdata rresp rlast`): open it in SimVision and add its signals by channel.
Each `F.*_req / *_rsp` is the same port as SystemVerilog structs.

---

## The boundaries, in order

```
 ┌──────────────┐   TL-UL   ┌──────────────┐  AXI   ┌───────────┐  AXI   ┌─────────┐  AXI   ┌───────────┐  AXI   ┌──────────────┐  TL-UL  ┌──────┐
 │ initiator    │──────────►│ host bridge  │───────►│ xbar_main │───────►│ axi_cdc │───────►│ xbar_peri │───────►│ device bridge│────────►│  IP  │
 │ Ibex / UVM   │  (1)      │ tlul2axi     │  (2)   │           │  (3)   │         │  (4)   │           │  (5)   │ axi2tlul     │  (6)    │      │
 └──────────────┘           └──────────────┘        └───────────┘        └─────────┘        └───────────┘        └──────────────┘         └──────┘
                                                         │ main-side IPs: (3) is the IP's AXI port, then device bridge (6)
                                                         └ spi_host0/1, usbdev: (3) → axi_cdc → (5') IP's AXI port → (6)
```

| # | Boundary | Protocol / clock | What changes here |
|---|---|---|---|
| 1 | initiator → host bridge | TL-UL, clk_main | — (TL-UL `a_source`, `a_user` integrity computed by `rv_core_ibex`) |
| 2 | host bridge → xbar_main slave port | AXI, clk_main | TL-UL becomes AXI: `Get` → AR, `Put*` → AW+W; ID = 0 always; TL-UL integrity rides in `AxUSER`/`WUSER` (`intg_vld=1`); `ARPROT[2]=1` for instruction fetches |
| 3 | xbar_main master port | AXI, clk_main | **ID grows from 4 to 6 bits**: `{host index, host ID}` — corei=0, cored=1, rv_dm.sba=2, UVM master=3 (so a CPU data access shows ID `0x10`, the UVM master `0x30`). Unmapped / unreachable addresses stop here: DECERR from the crossbar's error slave |
| 4 | after the main→peri `axi_cdc` | AXI, **clk_io_div4** (24 MHz) | same transaction, slower clock, a few cycles later |
| 5 | xbar_peri master port = the peripheral's AXI port | AXI, clk_io_div4 | — (single host: no ID bits added). Unmapped peri addresses stop here: DECERR, which then travels back across the CDC |
| 6 | device bridge → IP | TL-UL | AXI becomes TL-UL; **a burst becomes one TL-UL request per beat**; integrity passed through (CPU) or generated (UVM master, bursts) |

---

## Path A — UVM master → peripheral (uart0)

**Test:** `axi_csr_reset` (only 7 reads, all known). **Trace:** `--trace 0x40000014` (uart0 STATUS).

| # | Hierarchy (waves) | `--trace` port name |
|---|---|---|
| 1 | `P.if_host_sim_host` (= `F.sim_host_req/rsp`; driven by the UVM `axi_driver`) | `host.sim_host` |
| 2 | `F.u_sim_host_atop_filter` → `F.main_host_req[3]` / `F.main_host_rsp[3]` | (same transaction) |
| 3 | `F.u_xbar_main` → `F.main_dev_req[4]` / `F.main_dev_rsp[4]` (index 4 = `MainDevPeri`), ID `0x30` | — |
| 4 | `F.u_cdc_peri` → `F.peri_host_req/rsp` = `P.if_link_main_to_peri` | `link.main_to_peri` |
| 5 | `F.u_xbar_peri` → `F.peri_dev_req[0]` / `F.peri_dev_rsp[0]` (`PeriDevUart0`) = `P.if_dev_uart0` | `dev.uart0` |
| 6 | `F.u_dev_uart0` → `F.tl_uart0_o` / `F.tl_uart0_i` | — (TL-UL) |
| IP | `tb_top.dut.top_earlgrey.u_uart0` (`tl_i`, `tl_o`) | — |

The read returns `0x3C` at every hop.

## Path B — CPU data access → peripheral (uart0 writes)

**Test:** `fw_uart_tx` (the firmware writes each character to UART0 WDATA).
**Trace:** `--trace 0x4000001c`.

| # | Hierarchy | `--trace` port name |
|---|---|---|
| 0 | `tb_top.dut.top_earlgrey.u_rv_core_ibex.main_core_data_req / _addr / _we / _wdata / _gnt / _rvalid` (Ibex's own request interface) | — |
| 1 | `F.tl_rv_core_ibex__cored_i` / `F.tl_rv_core_ibex__cored_o` (TL-UL, `a_opcode` PutFullData) | — |
| 2 | `F.u_host_rv_core_ibex__cored` → `F.main_host_req[1]/rsp[1]` = `P.if_host_rv_core_ibex_cored`, ID 0 | `host.rv_core_ibex.cored` |
| 3 | `F.main_dev_req[4]/rsp[4]`, ID `0x10` | — |
| 4 | `P.if_link_main_to_peri` | `link.main_to_peri` |
| 5 | `P.if_dev_uart0` | `dev.uart0` |
| 6 | `F.tl_uart0_o/i` | — |
| IP | `u_uart0` → and on the pin: `tb_top.IOC4`, decoded by `env.uart0` | — |

## Path C — CPU instruction fetch → ROM

**Test:** `fw_hello`. **Trace:** `--trace 0x8100` (`_reset_start`, the first instructions after the reset jump).

| # | Hierarchy | `--trace` port name |
|---|---|---|
| 0 | `u_rv_core_ibex.main_core_instr_req / _addr / _rvalid` | — |
| 1 | `F.tl_rv_core_ibex__corei_i/o` (`a_user.instr_type` = MuBi4True) | — |
| 2 | `F.u_host_rv_core_ibex__corei` → `F.main_host_req[0]` = `P.if_host_rv_core_ibex_corei`, **`ARPROT[2]=1`** | `host.rv_core_ibex.corei` |
| 3 | `F.main_dev_req[2]` (`MainDevRomCtrlRom`) = `P.if_dev_rom_ctrl_rom`, ID `0x00` | `dev.rom_ctrl.rom` |
| 6 | `F.u_dev_rom_ctrl__rom` → `F.tl_rom_ctrl__rom_o/i` | — |
| IP | `u_rom_ctrl` (the ROM array is `u_rom_ctrl.gen_rom_scramble_disabled.u_rom.u_prim_rom.mem`) | — |

The fetch port only reaches 4 devices (ROM, main SRAM, flash, debug ROM); a fetch from anywhere else is DECERR at hop 3.

## Path D — main-side memory, with a burst

**Test:** `axi_sram_rw`. **Trace:** `--trace 0x10011040` (a 2-beat INCR burst).

| # | Hierarchy | `--trace` port name |
|---|---|---|
| 1–2 | `P.if_host_sim_host` → `F.main_host_req[3]` (`awlen`=1) | `host.sim_host` |
| 3 | `F.main_dev_req[23]` (`MainDevSramCtrlMainRam`) = `P.if_dev_sram_ctrl_main_ram`, still **one** AXI burst | `dev.sram_ctrl_main.ram` |
| 6 | `F.u_dev_sram_ctrl_main__ram` → `F.tl_sram_ctrl_main__ram_o/i`: **two** TL-UL requests, one per beat | — |
| IP | `u_sram_ctrl_main` | — |

In the waves, compare `awlen`/`wlast` at hop 3 with the `a_valid` pulses at hop 6.

## Path E — a main-side IP in its own clock domain (spi_host0)

**Test:** `axi_csr_reset`. **Trace:** `--trace 0x40300010` (spi_host0 CONTROL).

| # | Hierarchy | `--trace` port name |
|---|---|---|
| 3 | `F.main_dev_req[5]` (`MainDevSpiHost0`), clk_main | — |
| 5′ | `F.u_cdc_spi_host0` → `F.spi_host0_req/rsp` = `P.if_dev_spi_host0`, **clk_io (96 MHz)** | `dev.spi_host0` |
| 6 | `F.u_dev_spi_host0` → `F.tl_spi_host0_o/i` | — |
| IP | `u_spi_host0` | — |

## Path F — where DECERR is born

**Test:** `axi_decerr`.

| Address | Trace | Born at | What you see |
|---|---|---|---|
| `0x41010000` (gap in xbar_main) | `--trace 0x41010000` | `F.u_xbar_main` (its internal error slave) | `host.sim_host` only, `DECERR`; no other port sees it |
| `0x40060000` (gap in xbar_peri) | `--trace 0x40060000` | `F.u_xbar_peri` | `host.sim_host` **and** `link.main_to_peri`, both `DECERR`; no `dev.*` port — the request crossed the CDC, the error came back across it |

## Path G — firmware to testbench (the test status itself)

**Test:** `fw_hello`. **Trace:** `--trace 0x411f0080` (status), `0x411f0084` (log characters).

`P.if_host_rv_core_ibex_cored` → `F.main_dev_req[24]` (`MainDevSimCtrl`) =
`P.if_dev_titan_sim_ctrl` → `F.u_sim_ctrl`. This is how PASSED reaches the
testbench: `env.sw` decodes these writes.

## Path H — CPU → PLIC (interrupt claim/complete)

**Test:** `fw_plic_gpio_irq`. **Trace:** `--trace 0x48200004` (PLIC CC0: claim = read, complete = write).

`P.if_host_rv_core_ibex_cored` → `F.main_dev_req[18]` (`MainDevRvPlic`) =
`P.if_dev_rv_plic` → `F.u_dev_rv_plic` → `F.tl_rv_plic_o/i` → `u_rv_plic`. Four
reads return the GPIO interrupt IDs 37, 38, 39, 40.

---

## Finding any other port

- Port list: `rtl/fabric/fabric_ports.json` (`name`, `req`/`rsp` signal, clock).
- Probe interface: `P.if_` + the port name with `.` → `_`
  (`dev.sram_ctrl_ret_aon.ram` → `P.if_dev_sram_ctrl_ret_aon_ram`).
- Index constants: `F.MainHost*`, `F.MainDev*`, `F.PeriDev*` (top of `titan_axi_fabric.sv`).
- Every transaction on every port, as text: `uvm/sim/run.sh <test> --verbosity UVM_HIGH`
  (large log; grep for the port name).
