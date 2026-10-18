# rtl/ — the DUT

Read-only for trainees. Earl Grey from `vendor/opentitan`, with
`xbar_main` + `xbar_peri` replaced by one AXI fabric.

| Dir | Contents |
|---|---|
| `vendor/` | PULP `axi` + `common_cells`, pinned |
| `bridge/` | `titan_tlul2axi` (host side), `titan_axi2tlul` (device side) |
| `fabric/` | fabric config, generator, generated fabric, sim-only status slave |
| `top/` | patch script + patched `top_earlgrey.sv` |
| `flist/` | file-list generator + the frozen `rtl.f` |

Design rules: [`../AXI_SOC_PLAN.md`](../AXI_SOC_PLAN.md) §4.
