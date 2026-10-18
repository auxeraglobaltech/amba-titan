# uvm/ — the testbench

| Dir | Contents |
|---|---|
| `tb/` | `tb_top.sv`, chip harness (POR, pads), AXI probe bind, image loaders |
| `agents/` | `axi_agent`, `uart_agent`, `gpio_agent` |
| `env/` | env, config, address map, fabric scoreboard, SW status monitor |
| `seq/` | sequences and virtual sequences |
| `tests/` | `titan_base_test` and the tests |
| `sim/` | `env.sh`, `run.sh`, file lists, regression list |

Full guide: written in Phase 5 of [`../AXI_SOC_PLAN.md`](../AXI_SOC_PLAN.md).
