# uvm/bridge_tb — standalone bridge testbench

Proves `rtl/bridge/titan_tlul2axi` and `titan_axi2tlul` on their own, in
seconds, before they are trusted inside the chip. It's plain SystemVerilog,
not UVM: a reference memory model plus direct checks.

```
host0: Ibex-like req/gnt ─► tlul_adapter_host ─► titan_tlul2axi ─┐
host1: AXI master (bursts) ─► axi_atop_filter ───────────────────┼─► axi_xbar ─┬─► axi2tlul ─► tlul_adapter_sram ─► mem0
                                                                 ┘             └─► axi_cdc ─► axi2tlul ─► tlul_adapter_sram ─► mem1 (slow clock, stalls)
```

The SRAM adapters check command integrity just like the real IPs, so any
mistake in carrying TL-UL integrity through AXI fails the test.

| Check | What |
|---|---|
| C1 | host0 random reads/writes to both memories match the model |
| C2 | store then load to the same address returns the new data |
| C3 | unmapped address: host0 gets an error + all-ones, host1 gets DECERR |
| C4 | INCR / FIXED / WRAP bursts, 8- and 16-bit beats |
| C5 | atomic operation answered SLVERR, memory untouched |
| C6 | no integrity error anywhere |
| C7 | read-data integrity valid on every host0 response |

```bash
uvm/bridge_tb/run.sh            # pass = "BRIDGE TB PASSED"
uvm/bridge_tb/run.sh --seed 7
uvm/bridge_tb/run.sh --waves    # waves in runs/waves.shm
uvm/sim/lint.py --top tb_bridge uvm/bridge_tb/files.f   # 5-second compile check, no simulator
```
