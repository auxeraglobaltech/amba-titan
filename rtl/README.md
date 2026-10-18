# rtl/ — the DUT: Earl Grey on an AXI4 fabric

**Read-only for trainees.** OpenTitan Earl Grey (from `vendor/opentitan`,
unmodified) with its two TL-UL crossbars, `xbar_main` and `xbar_peri`,
replaced by one AXI4 fabric. Every IP keeps its own TL-UL port; a bridge sits
directly in front of each one, so **between modules every transaction is AXI**:
host → crossbar → crossbar → IP.

```
 Ibex corei/cored, rv_dm.sba (TL-UL)                     [sim] UVM AXI master
        │ titan_tlul2axi ×3                                  │ axi_atop_filter
        └───────────────► axi_xbar main (4 hosts, clk_main) ◄┘
                            │
       ┌────────────────────┼─────────────────────┬─────────────────┐
  titan_axi2tlul ×20    axi_cdc ×3             axi_cdc        [sim] titan_sim_ctrl
  (SRAM, ROM, flash,    → titan_axi2tlul     → axi_xbar peri        (status/log)
   crypto, PLIC, ...)    spi_host0/1, usbdev    (clk_io_div4)
                                                  └► titan_axi2tlul ×27 (uart, gpio, timers, AON, OTP, LC, ...)
```

## Pieces

| File | What |
|---|---|
| `bridge/titan_axi_pkg.sv` | AXI types; the USER-signal layouts that carry TL-UL integrity |
| `bridge/titan_tlul2axi.sv` | host side: TL-UL host → AXI master |
| `bridge/titan_axi2tlul.sv` | device side: AXI → TL-UL device |
| `fabric/titan_axi_fabric.sv` | the fabric — **generated** by `fabric/gen_fabric.py` |
| `fabric/fabric_ports.json` | every port, its clock, address ranges, connectivity — **generated**; feeds `uvm/tb/gen_tb.py` |
| `fabric/titan_sim_ctrl.sv` | sim-only slave in the DV window (`0x411F_0080`): firmware status + log |
| `top/top_earlgrey.sv` | the vendor top with only the two crossbar instances replaced — **generated** by `top/patch_top.py` |
| `flist/rtl.f` | the whole DUT (660 files); `flist/earlgrey.f` is **generated** by `flist/gen_flist.py` |
| `vendor/pulp/` | PULP `axi` v0.39.11 + `common_cells` v1.39.0 (31 files), via `vendor/update_pulp.sh` |

## Bridge rules (the interesting design work)

**titan_tlul2axi** (TL-UL is one in-order channel; AXI is five channels)
- One AXI ID per port, always 0: AXI only orders responses within an ID, and
  `tlul_adapter_host` cannot reorder.
- No read/write mixing in flight: AR and AW are independent, so a store
  then load to the same address could swap. A request in the other direction
  waits until the outstanding ones have returned.
- Integrity is carried end to end: TL-UL `a_user` (`cmd_intg`, `data_intg`,
  `instr_type`) rides in AxUSER/WUSER, the device's `d_user` comes back in
  RUSER/BUSER. Only for DECERR (no device; USER = 0) does the bridge generate
  response integrity itself, so Ibex sees an access fault rather than a fatal
  integrity alert.
- `instr_type` → `AxPROT[2]`; `d_error` ↔ `SLVERR`.

**titan_axi2tlul**
- AR/AW round-robin, one command at a time.
- Bursts (INCR, FIXED, WRAP) split into one TL-UL access per beat.
- Integrity passed through for single-beat transactions that came from a
  TL-UL host (`AxUSER.intg_vld`); generated otherwise (bursts, plain AXI masters).
- ATOPs, beats > 32 bits, reserved bursts, illegal WRAP lengths → SLVERR
  without touching the IP. Exclusives are answered OKAY (never EXOKAY).

## Built with `+define+TITAN_SIM` (simulation only)

- `sim_host_req/rsp`: a 4th xbar_main host, idle unless the testbench forces it.
- `titan_sim_ctrl` at `rv_core_ibex.cfg + 0x80..0x9F`.

Without the define the fabric is the plain drop-in replacement.

## After changing anything here

```bash
rtl/fabric/gen_fabric.py && uvm/tb/gen_tb.py   # if the topology changed
uvm/sim/lint.py --top chip_earlgrey_asic -D TITAN_SIM -D SIMULATION rtl/flist/rtl.f
uvm/sim/elab_rtl.sh                            # Xcelium elaboration
uvm/bridge_tb/run.sh                           # if a bridge changed
uvm/sim/regress.sh
```

After moving the OpenTitan pin: `rtl/top/patch_top.py`, `rtl/fabric/gen_fabric.py`,
`uvm/tb/gen_tb.py`, `firmware/tools/gen_regs.sh`, `firmware/tools/gen_otp.sh`, and
regenerate `flist/earlgrey.f`. That last step needs FuseSoC's resolved source
list for the new pin (`fusesoc ... --setup lowrisc:dv:top_earlgrey_chip_sim`,
or any upstream chip build): copy its `.scr` over `flist/fusesoc_chip_sim.scr`
and pass its work directory to `gen_flist.py --work`. Nothing else in this
repo needs FuseSoC, Bazel or dvsim — the committed `earlgrey.f` points straight
into `vendor/opentitan`.
