# titan-axi — OpenTitan Earl Grey on an AXI fabric

A SoC verification training environment. The DUT is the full OpenTitan
**Earl Grey** chip with its two TL-UL crossbars replaced by an **AXI4** fabric,
so every bus transaction between modules — CPU to crossbar, crossbar to
crossbar, crossbar to IP — is AXI. The testbench is a fresh UVM environment
targeting Cadence Xcelium; tests are UVM sequences, C firmware, or both.

> 🚧 **Under construction.** See [`AXI_SOC_PLAN.md`](AXI_SOC_PLAN.md) for the
> plan, phases and current status.

## Where to work

| Folder | You touch it? | What lives there |
|---|---|---|
| [`firmware/`](firmware/) | **yes** | C tests, startup code, drivers, and the build that turns C into a hex image |
| [`uvm/`](uvm/) | **yes** | testbench, agents, env, scoreboards, sequences, tests, run scripts |
| [`rtl/`](rtl/) | no — the DUT | AXI fabric, TL-UL↔AXI bridges, patched top, file list |
| `vendor/opentitan/` | never | upstream Earl Grey RTL, pinned submodule |

## Setup

```bash
git clone --recurse-submodules <this repo>
~/.local/opt/python-3.11.15-titan/bin/python3 -m venv .venv
.venv/bin/pip install -r requirements.txt
source uvm/sim/env.sh            # csh: source uvm/sim/env.csh
```

## History

The previous environment — Earl Grey on its native TL-UL bus, driven through
the upstream OpenTitan dvsim/UVM flow, plus the IP block-level track — is
preserved on branch `legacy-earlgrey-tlul` (tag `legacy-tlul-v1`).
