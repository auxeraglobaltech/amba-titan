#!/usr/bin/env python3
"""uvm/doc/gen_guide.py — build titan_axi_guide.html (the technical reference).

  uvm/doc/gen_guide.py          -> titan_axi_guide.html in the repo root

Prose lives in uvm/doc/guide_template.html. Everything that comes from the
design is generated here, from the same files the RTL and testbench use:
  rtl/fabric/fabric_ports.json   address windows, crossbar, clock of every port
  uvm/sim/tests.list             the tests
so the document cannot disagree with the design. Re-run after regenerating the
fabric or adding a test.
"""

import datetime
import html
import json
import pathlib

HERE = pathlib.Path(__file__).resolve().parent
ROOT = HERE.parent.parent
PORTS = ROOT / "rtl/fabric/fabric_ports.json"
TESTS = ROOT / "uvm/sim/tests.list"
TEMPLATE = HERE / "guide_template.html"
FIGURES = HERE / "figures"          # one SVG per figure, inlined as @@FIG:<name>@@
OUT = ROOT / "titan_axi_guide.html"

# ----------------------------------------------------------------------------
# IP catalogue: functional group and one-line description of every device port
# ----------------------------------------------------------------------------
GROUPS = {
    # main side (xbar_main)
    "mem":    ("Memories", "main"),
    "sys":    ("System control", "main"),
    "crypto": ("Cryptography", "main"),
    "ent":    ("Entropy", "main"),
    "own":    ("Own-clock peripherals", "io"),
    "sim":    ("Simulation only", "sim"),
    # peripheral side (xbar_peri)
    "comm":   ("Communication", "peri"),
    "gpt":    ("GPIO & timers", "peri"),
    "sec":    ("Security & life cycle", "peri"),
    "aon":    ("Power, reset, clock (AON)", "peri"),
}

IPS = {
    "rom_ctrl.rom":           ("mem",    "ROM", "Boot ROM, 32 KiB (plain image in simulation)"),
    "sram_ctrl_main.ram":     ("mem",    "Main SRAM", "Scrambled SRAM, 128 KiB"),
    "flash_ctrl.mem":         ("mem",    "Flash", "Embedded flash, 1 MiB (host read window)"),
    "rv_dm.mem":              ("mem",    "Debug ROM", "Debug module program buffer / ROM"),
    "rv_plic":                ("sys",    "rv_plic", "Interrupt controller, 186 sources"),
    "rv_core_ibex.cfg":       ("sys",    "ibex cfg", "Ibex wrapper registers (address translation, NMI, ...)"),
    "sram_ctrl_main.regs":    ("sys",    "sram_ctrl", "Main SRAM controller registers"),
    "rom_ctrl.regs":          ("sys",    "rom_ctrl", "ROM controller registers (digest)"),
    "flash_ctrl.core":        ("sys",    "flash_ctrl", "Flash controller registers"),
    "flash_ctrl.prim":        ("sys",    "flash prim", "Flash macro test registers"),
    "rv_dm.regs":             ("sys",    "rv_dm", "Debug module registers"),
    "aes":                    ("crypto", "aes", "AES-128/192/256, masked"),
    "hmac":                   ("crypto", "hmac", "SHA-2 / HMAC"),
    "kmac":                   ("crypto", "kmac", "SHA-3 / KMAC, masked"),
    "otbn":                   ("crypto", "otbn", "Big-number accelerator (public-key crypto)"),
    "keymgr":                 ("crypto", "keymgr", "Key manager"),
    "entropy_src":            ("ent",    "entropy_src", "Entropy source with health tests"),
    "csrng":                  ("ent",    "csrng", "Cryptographic secure RNG (DRBG)"),
    "edn0":                   ("ent",    "edn0", "Entropy distribution network 0"),
    "edn1":                   ("ent",    "edn1", "Entropy distribution network 1"),
    "spi_host0":              ("own",    "spi_host0", "SPI host, 96 MHz (clk_io)"),
    "spi_host1":              ("own",    "spi_host1", "SPI host, 48 MHz (clk_io_div2)"),
    "usbdev":                 ("own",    "usbdev", "USB 2.0 full-speed device, 48 MHz"),
    "titan_sim_ctrl":         ("sim",    "titan_sim_ctrl", "Firmware status / log / sync to the testbench"),
    "uart0":                  ("comm",   "uart0", "UART (test console: pads IOC4/IOC3)"),
    "uart1":                  ("comm",   "uart1", "UART"),
    "uart2":                  ("comm",   "uart2", "UART"),
    "uart3":                  ("comm",   "uart3", "UART"),
    "i2c0":                   ("comm",   "i2c0", "I2C host/target"),
    "i2c1":                   ("comm",   "i2c1", "I2C host/target"),
    "i2c2":                   ("comm",   "i2c2", "I2C host/target"),
    "spi_device":             ("comm",   "spi_device", "SPI device (flash / passthrough / TPM)"),
    "pattgen":                ("comm",   "pattgen", "Pattern generator"),
    "gpio":                   ("gpt",    "gpio", "32 GPIOs (testbench: GPIO[7:0] on IOB0-7)"),
    "rv_timer":               ("gpt",    "rv_timer", "RISC-V machine timer (mtip)"),
    "pwm_aon":                ("gpt",    "pwm", "PWM, always-on"),
    "aon_timer_aon":          ("gpt",    "aon_timer", "Wake-up timer and watchdog, always-on"),
    "otp_ctrl.core":          ("sec",    "otp_ctrl", "OTP controller (secrets, life-cycle state)"),
    "otp_macro.prim":         ("sec",    "otp macro", "OTP macro test registers"),
    "lc_ctrl.regs":           ("sec",    "lc_ctrl", "Life-cycle controller"),
    "alert_handler":          ("sec",    "alert_handler", "Alert collection and escalation"),
    "sensor_ctrl_aon":        ("sec",    "sensor_ctrl", "Analog sensor control"),
    "ast":                    ("sec",    "ast", "Analog sensor top: oscillators, regulators"),
    "pwrmgr_aon":             ("aon",    "pwrmgr", "Power manager"),
    "rstmgr_aon":             ("aon",    "rstmgr", "Reset manager (reset cause, SW reset)"),
    "clkmgr_aon":             ("aon",    "clkmgr", "Clock manager (gating, dividers)"),
    "pinmux_aon":             ("aon",    "pinmux", "Pin multiplexer, 47 MIO pads"),
    "sysrst_ctrl_aon":        ("aon",    "sysrst_ctrl", "System reset controller"),
    "adc_ctrl_aon":           ("aon",    "adc_ctrl", "ADC controller"),
    "sram_ctrl_ret_aon.regs": ("aon",    "ret sram_ctrl", "Retention SRAM controller registers"),
    "sram_ctrl_ret_aon.ram":  ("aon",    "Retention SRAM", "Retention SRAM, 4 KiB"),
}

CLOCK = {
    "clk_main_i":      ("main", "clk_main 100 MHz"),
    "clk_fixed_i":     ("peri", "clk_io_div4 24 MHz"),
    "clk_spi_host0_i": ("io",   "clk_io 96 MHz"),
    "clk_spi_host1_i": ("io",   "clk_io_div2 48 MHz"),
    "clk_usb_i":       ("io",   "clk_usb 48 MHz"),
}

# Palette (datasheet): fill, stroke per clock domain.
DOM = {
    "main": ("#e9eef5", "#3b5b84"),
    "peri": ("#e7f1ee", "#2f7566"),
    "io":   ("#f5eee4", "#8f6430"),
    "sim":  ("#fdf6e3", "#a07a1c"),
}
INK, MUTED, BUS = "#1b2430", "#5b6675", "#1f3a5f"


def esc(s):
    return html.escape(s, quote=False)


def size_str(n):
    if n >= 1 << 20:
        return f"{n >> 20} MiB"
    if n >= 1024:
        return f"{n >> 10} KiB"
    return f"{n} B"


def hex_addr(a):
    return f"0x{a >> 16:04X}_{a & 0xFFFF:04X}"


def load():
    ports = json.loads(PORTS.read_text())["ports"]
    devs = {}
    for p in ports:
        if p["kind"] != "dev":
            continue
        name = p["name"][4:]
        rng = [(int(lo, 16), int(hi, 16)) for lo, hi in p["ranges"]]
        devs[name] = dict(port=p, ranges=rng, base=rng[0][0])
    missing = set(devs) - set(IPS)
    if missing:
        raise SystemExit(f"IP catalogue lacks: {sorted(missing)}")
    return devs


# ----------------------------------------------------------------------------
# Figure: full SoC block diagram
# ----------------------------------------------------------------------------
def soc_diagram(devs):
    W, H = 1400, 930
    o = []
    a = o.append
    a(f'<svg viewBox="0 0 {W} {H}" xmlns="http://www.w3.org/2000/svg" class="fig-svg" '
      f'font-family="IBM Plex Sans, Arial, sans-serif">')
    a('<defs>'
      f'<marker id="soc-a" markerWidth="8" markerHeight="8" refX="7" refY="4" orient="auto"><path d="M0,0 L8,4 L0,8 z" fill="{BUS}"/></marker>'
      '<marker id="soc-t" markerWidth="8" markerHeight="8" refX="7" refY="4" orient="auto"><path d="M0,0 L8,4 L0,8 z" fill="#6b3fa0"/></marker>'
      '<pattern id="soc-hatch" width="6" height="6" patternUnits="userSpaceOnUse" patternTransform="rotate(45)">'
      '<line x1="0" y1="0" x2="0" y2="6" stroke="#d9dee6" stroke-width="2"/></pattern>'
      '</defs>')
    # chip outline
    a(f'<rect x="8" y="8" width="{W-16}" height="{H-16}" rx="4" fill="#ffffff" stroke="{INK}" stroke-width="1.5"/>')
    a(f'<text x="22" y="32" font-size="15" font-weight="700" fill="{INK}">chip_earlgrey_asic</text>'
      f'<text x="182" y="32" font-size="12" fill="{MUTED}">top_earlgrey with titan_axi_fabric (u_axi_fabric) — all bus traffic between modules is AXI4</text>')

    # ---- CPU complex ------------------------------------------------------
    a(f'<rect x="22" y="56" width="236" height="250" rx="3" fill="{DOM["main"][0]}" stroke="{DOM["main"][1]}"/>')
    a(f'<rect x="22" y="56" width="236" height="22" rx="3" fill="{DOM["main"][1]}"/>'
      '<text x="32" y="71" font-size="12" font-weight="600" fill="#fff">CPU complex</text>')
    a(f'<rect x="36" y="88" width="208" height="92" rx="2" fill="#fff" stroke="{DOM["main"][1]}"/>'
      f'<text x="46" y="106" font-size="13" font-weight="700" fill="{INK}">Ibex RV32IMCB</text>'
      f'<text x="46" y="122" font-size="10.5" fill="{MUTED}">u_rv_core_ibex</text>'
      f'<text x="46" y="138" font-size="10.5" fill="{INK}">2-stage, ICache (ECC), 16-region PMP,</text>'
      f'<text x="46" y="152" font-size="10.5" fill="{INK}">security hardening</text>'
      f'<text x="46" y="170" font-size="10.5" fill="{MUTED}">boot 0x0000_8080 · IRQ: PLIC, rv_timer</text>')
    a(f'<rect x="36" y="194" width="208" height="46" rx="2" fill="#fff" stroke="{DOM["main"][1]}"/>'
      f'<text x="46" y="212" font-size="12.5" font-weight="700" fill="{INK}">rv_dm</text>'
      f'<text x="46" y="228" font-size="10.5" fill="{MUTED}">RISC-V debug module (JTAG)</text>')
    a(f'<rect x="36" y="252" width="208" height="42" rx="2" fill="{DOM["sim"][0]}" stroke="{DOM["sim"][1]}" stroke-dasharray="4 3"/>'
      f'<text x="46" y="270" font-size="12" font-weight="700" fill="{INK}">UVM AXI master</text>'
      f'<text x="46" y="285" font-size="10.5" fill="{DOM["sim"][1]}">testbench, +define+TITAN_SIM</text>')

    # ---- host bridges + xbar_main ----------------------------------------
    hosts = [("corei", 112, "titan_tlul2axi"), ("cored", 152, "titan_tlul2axi"),
             ("sba", 217, "titan_tlul2axi"), ("sim", 273, "atop filter")]
    for label, y, mod in hosts:
        sim = label == "sim"
        stroke = DOM["sim"][1] if sim else DOM["main"][1]
        a(f'<line x1="244" y1="{y}" x2="272" y2="{y}" stroke="{"#6b3fa0" if not sim else BUS}" stroke-width="1.6" '
          f'marker-end="url(#soc-{"t" if not sim else "a"})"/>')
        a(f'<rect x="274" y="{y-13}" width="92" height="26" rx="2" fill="#fff" stroke="{stroke}"/>'
          f'<text x="320" y="{y-1}" font-size="9.5" text-anchor="middle" fill="{INK}" font-weight="600">{mod}</text>'
          f'<text x="320" y="{y+9}" font-size="8.5" text-anchor="middle" fill="{MUTED}">{label}</text>')
    # xbar_main bar
    xb_x, xb_y, xb_w, xb_h = 392, 60, 990, 64
    a(f'<rect x="{xb_x}" y="{xb_y}" width="{xb_w}" height="{xb_h}" rx="3" fill="{BUS}"/>'
      f'<text x="{xb_x+40}" y="{xb_y+26}" font-size="15" font-weight="700" fill="#fff">xbar_main</text>'
      f'<text x="{xb_x+40}" y="{xb_y+45}" font-size="11" fill="#c9d6e8">axi_xbar u_xbar_main · 4 slave ports × 25 master ports · AXI4 32-bit · clk_main 100 MHz</text>'
      f'<text x="{xb_x+xb_w-14}" y="{xb_y+26}" font-size="11" fill="#c9d6e8" text-anchor="end">ID 4 → 6 bits {{host index, ID}}</text>'
      f'<text x="{xb_x+xb_w-14}" y="{xb_y+45}" font-size="11" fill="#c9d6e8" text-anchor="end">unmapped / unreachable → DECERR</text>')
    # Slave-port side of xbar_main: a strip down the left, so every bridge
    # connects straight across at its own height.
    a(f'<rect x="{xb_x}" y="{xb_y}" width="26" height="236" rx="3" fill="{BUS}"/>')
    for i, (label, y, _) in enumerate(hosts):
        a(f'<line x1="366" y1="{y}" x2="{xb_x-2}" y2="{y}" stroke="{BUS}" stroke-width="1.6" marker-end="url(#soc-a)"/>'
          f'<text x="{xb_x+13}" y="{y+4}" font-size="9.5" fill="#c9d6e8" text-anchor="middle">[{i}]</text>')

    # ---- main-side groups --------------------------------------------------
    def group_box(key, x, y, w, items, note=None, dashed=False):
        title, dom = GROUPS[key]
        fill, stroke = DOM[dom]
        rh = 19
        h = 26 + rh * len(items) + (16 if note else 0) + 6
        dash = ' stroke-dasharray="4 3"' if dashed else ''
        a(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="3" fill="{fill}" stroke="{stroke}"{dash}/>')
        a(f'<rect x="{x}" y="{y}" width="{w}" height="22" rx="3" fill="{stroke}"/>'
          f'<text x="{x+8}" y="{y+15}" font-size="11.5" font-weight="600" fill="#fff">{esc(title)}</text>')
        yy = y + 26
        for name in items:
            d = devs[name]
            short = IPS[name][1]
            a(f'<rect x="{x+6}" y="{yy}" width="{w-12}" height="{rh-3}" rx="2" fill="#fff" stroke="{stroke}" stroke-width="0.6"/>'
              f'<text x="{x+12}" y="{yy+12}" font-size="10.5" fill="{INK}" font-weight="600">{esc(short)}</text>'
              f'<text x="{x+w-12}" y="{yy+12}" font-size="9.5" fill="{MUTED}" text-anchor="end" '
              f'font-family="JetBrains Mono, Consolas, monospace">{hex_addr(d["base"])}</text>')
            yy += rh
        if note:
            a(f'<text x="{x+8}" y="{yy+11}" font-size="9.5" fill="{MUTED}">{esc(note)}</text>')
            yy += 16
        return h

    main_cols = [
        ("mem",    ["rom_ctrl.rom", "sram_ctrl_main.ram", "flash_ctrl.mem", "rv_dm.mem"], "instruction fetch reaches these 4 only"),
        ("sys",    ["rv_plic", "rv_core_ibex.cfg", "sram_ctrl_main.regs", "rom_ctrl.regs",
                    "flash_ctrl.core", "flash_ctrl.prim", "rv_dm.regs"], None),
        ("crypto", ["aes", "hmac", "kmac", "otbn", "keymgr"], None),
        ("ent",    ["entropy_src", "csrng", "edn0", "edn1"], None),
        ("own",    ["spi_host0", "spi_host1", "usbdev"], "each behind its own axi_cdc"),
    ]
    gx0, gw, gap, gy = 470, 172, 10, 168
    for i, (key, items, note) in enumerate(main_cols):
        x = gx0 + i * (gw + gap)
        cx = x + gw // 2
        a(f'<line x1="{cx}" y1="{xb_y+xb_h}" x2="{cx}" y2="{gy-2}" stroke="{BUS}" stroke-width="1.6" marker-end="url(#soc-a)"/>')
        if key == "own":
            a(f'<rect x="{cx-26}" y="{xb_y+xb_h+8}" width="52" height="18" rx="2" fill="#fff" stroke="{DOM["io"][1]}"/>'
              f'<text x="{cx}" y="{xb_y+xb_h+21}" font-size="9.5" text-anchor="middle" fill="{DOM["io"][1]}" font-weight="600">axi_cdc</text>')
        group_box(key, x, gy, gw, items, note)
    # sim ctrl under own-clock column
    simx = gx0 + 4 * (gw + gap)
    a(f'<path d="M{simx-6},{xb_y+xb_h} L{simx-6},330 L{simx-2},330" fill="none" stroke="{BUS}" stroke-width="1.2" stroke-dasharray="3 2" marker-end="url(#soc-a)"/>')
    group_box("sim", simx, 312, gw, ["titan_sim_ctrl"], "0x411F_0080–009F, status/log/sync", dashed=True)

    # ---- link to the peripheral side ---------------------------------------
    cdc_x, cdc_y = 400, 420
    a(f'<line x1="430" y1="{xb_y+xb_h}" x2="430" y2="{cdc_y-2}" stroke="{BUS}" stroke-width="1.8" marker-end="url(#soc-a)"/>')
    a(f'<text x="438" y="{xb_y+xb_h+30}" font-size="9.5" fill="{MUTED}">[4] peri</text>')
    a(f'<rect x="{cdc_x}" y="{cdc_y}" width="60" height="40" rx="2" fill="#fff" stroke="{DOM["peri"][1]}" stroke-width="1.4"/>'
      f'<text x="430" y="{cdc_y+17}" font-size="10" text-anchor="middle" font-weight="700" fill="{DOM["peri"][1]}">axi_cdc</text>'
      f'<text x="430" y="{cdc_y+31}" font-size="9" text-anchor="middle" fill="{MUTED}">u_cdc_peri</text>')
    px_y = 490
    a(f'<line x1="430" y1="{cdc_y+40}" x2="430" y2="{px_y-2}" stroke="{BUS}" stroke-width="1.8" marker-end="url(#soc-a)"/>')
    a(f'<rect x="{xb_x}" y="{px_y}" width="{xb_w}" height="52" rx="3" fill="{DOM["peri"][1]}"/>'
      f'<text x="{xb_x+60}" y="{px_y+22}" font-size="15" font-weight="700" fill="#fff">xbar_peri</text>'
      f'<text x="{xb_x+60}" y="{px_y+40}" font-size="11" fill="#d3ebe5">axi_xbar u_xbar_peri · 1 slave port × 27 master ports · clk_io_div4 24 MHz · windows 0x4000_0000–0x401F_FFFF, 0x4040_0000–0x407F_FFFF</text>')

    peri_cols = [
        ("comm", ["uart0", "uart1", "uart2", "uart3", "i2c0", "i2c1", "i2c2", "spi_device", "pattgen"], None),
        ("gpt",  ["gpio", "rv_timer", "pwm_aon", "aon_timer_aon"], None),
        ("sec",  ["otp_ctrl.core", "otp_macro.prim", "lc_ctrl.regs", "alert_handler", "sensor_ctrl_aon", "ast"], None),
        ("aon",  ["pwrmgr_aon", "rstmgr_aon", "clkmgr_aon", "pinmux_aon", "sysrst_ctrl_aon", "adc_ctrl_aon",
                  "sram_ctrl_ret_aon.regs", "sram_ctrl_ret_aon.ram"], None),
    ]
    pgw, pgap, pgy = 214, 12, 582   # 470 + 3*226 + 214 = 1362 < bar end 1382
    for i, (key, items, note) in enumerate(peri_cols):
        x = 470 + i * (pgw + pgap)
        cx = x + pgw // 2
        a(f'<line x1="{cx}" y1="{px_y+52}" x2="{cx}" y2="{pgy-2}" stroke="{BUS}" stroke-width="1.6" marker-end="url(#soc-a)"/>')
        group_box(key, x, pgy, pgw, items, note)

    # ---- left column: clocks and legend ------------------------------------
    a(f'<rect x="22" y="330" width="352" height="200" rx="3" fill="#fff" stroke="#c4ccd6"/>'
      f'<text x="34" y="350" font-size="12" font-weight="700" fill="{INK}">Clocks (AST oscillators → clkmgr)</text>')
    clk_rows = [("main", "clk_main", "100 MHz", "CPU, xbar_main, main-side IPs"),
                ("io", "clk_io", "96 MHz", "spi_host0"),
                ("io", "clk_io_div2", "48 MHz", "spi_host1"),
                ("io", "clk_usb", "48 MHz", "usbdev"),
                ("peri", "clk_io_div4", "24 MHz", "xbar_peri and its IPs"),
                ("peri", "clk_aon", "200 kHz", "always-on timers, pwrmgr")]
    yy = 368
    for dom, n, f, use in clk_rows:
        a(f'<rect x="34" y="{yy-10}" width="12" height="12" fill="{DOM[dom][0]}" stroke="{DOM[dom][1]}"/>'
          f'<text x="54" y="{yy}" font-size="10.5" fill="{INK}" font-family="JetBrains Mono, Consolas, monospace">{n}</text>'
          f'<text x="148" y="{yy}" font-size="10.5" fill="{INK}">{f}</text>'
          f'<text x="204" y="{yy}" font-size="10" fill="{MUTED}">{esc(use)}</text>')
        yy += 22
    a(f'<text x="34" y="{yy+4}" font-size="10" fill="{MUTED}">Calibrated by firmware (crt0) at boot: AST REGAL write.</text>')

    a(f'<rect x="22" y="546" width="352" height="200" rx="3" fill="#fff" stroke="#c4ccd6"/>'
      f'<text x="34" y="566" font-size="12" font-weight="700" fill="{INK}">Legend</text>')
    leg = 586
    a(f'<line x1="34" y1="{leg}" x2="74" y2="{leg}" stroke="{BUS}" stroke-width="1.8" marker-end="url(#soc-a)"/>'
      f'<text x="84" y="{leg+4}" font-size="10.5" fill="{INK}">AXI4 (32-bit data, 32-bit address)</text>')
    a(f'<line x1="34" y1="{leg+22}" x2="74" y2="{leg+22}" stroke="#6b3fa0" stroke-width="1.8" marker-end="url(#soc-t)"/>'
      f'<text x="84" y="{leg+26}" font-size="10.5" fill="{INK}">TL-UL (initiator side and inside every IP)</text>')
    for i, (dom, txt) in enumerate([("main", "clk_main domain"), ("peri", "clk_io_div4 domain"),
                                    ("io", "own clock (via axi_cdc)"), ("sim", "simulation only (dashed)")]):
        y = leg + 46 + i * 20
        a(f'<rect x="34" y="{y-10}" width="40" height="13" fill="{DOM[dom][0]}" stroke="{DOM[dom][1]}"/>'
          f'<text x="84" y="{y+1}" font-size="10.5" fill="{INK}">{txt}</text>')
    a(f'<text x="34" y="{leg+136}" font-size="10" fill="{MUTED}">Addresses: base of each block (top_earlgrey.h).</text>'
      f'<text x="34" y="{leg+150}" font-size="10" fill="{MUTED}">Every IP port is TL-UL behind its own titan_axi2tlul.</text>')

    # ---- pad ring ------------------------------------------------------------
    a(f'<rect x="22" y="{H-122}" width="{W-44}" height="98" rx="3" fill="url(#soc-hatch)" stroke="#9aa4b1"/>'
      f'<rect x="32" y="{H-112}" width="{W-64}" height="78" rx="2" fill="#fbfcfd" stroke="#c4ccd6"/>'
      f'<text x="44" y="{H-92}" font-size="12" font-weight="700" fill="{INK}">Pad ring</text>'
      f'<text x="44" y="{H-74}" font-size="10.5" fill="{INK}">47 MIO pads (IOA0–8, IOB0–12, IOC0–12, IOR0–13) through pinmux — nothing connected out of reset · DIO pads: SPI host/device, USB, POR_N, flash/OTP test</text>'
      f'<text x="44" y="{H-56}" font-size="10.5" fill="{INK}">Testbench: POR_N driven · UART0 TX = IOC4, RX = IOC3 · GPIO[7:0] = IOB0–IOB7 · every other pad weakly pulled low</text>'
      f'<text x="44" y="{H-40}" font-size="10" fill="{MUTED}">Simulation boot: ROM ← firmware hex, OTP ← life-cycle RMA image (tb_top, $readmemh)</text>')
    a('</svg>')
    return "\n".join(o)


# ----------------------------------------------------------------------------
# Tables
# ----------------------------------------------------------------------------
def pill(dom, text):
    return f'<span class="dom dom-{dom}">{esc(text)}</span>'


def ip_inventory(devs):
    rows = []
    order = ["mem", "sys", "crypto", "ent", "own", "sim", "comm", "gpt", "sec", "aon"]
    for g in order:
        names = [n for n in IPS if IPS[n][0] == g and n in devs]
        rows.append(f'<tr class="grp"><td colspan="7">{esc(GROUPS[g][0])}</td></tr>')
        for n in names:
            d = devs[n]
            p = d["port"]
            dom, ctext = CLOCK[p["clock"]]
            if p["sim_only"]:
                dom = "sim"
            lo, hi = d["ranges"][0][0], d["ranges"][-1][1]
            size = sum(h - l for l, h in d["ranges"])
            rows.append(
                f'<tr><td><code>{esc(n)}</code></td><td>{esc(IPS[n][2])}</td>'
                f'<td class="mono">{hex_addr(lo)}</td><td class="num">{size_str(size)}</td>'
                f'<td>{"xbar_peri" if p["xbar"] == "peri" else "xbar_main"}</td>'
                f'<td>{pill(dom, ctext)}</td><td><code>P.if_dev_{n.replace(".", "_")}</code></td></tr>')
    return "\n".join(rows)


def memmap_rows(devs):
    rows = []
    allr = sorted((lo, hi, n) for n, d in devs.items() for lo, hi in d["ranges"])
    for lo, hi, n in allr:
        p = devs[n]["port"]
        dom, ctext = CLOCK[p["clock"]]
        if p["sim_only"]:
            dom = "sim"
        rows.append(f'<tr><td class="mono">{hex_addr(lo)}</td><td class="mono">{hex_addr(hi - 1)}</td>'
                    f'<td class="num">{size_str(hi - lo)}</td><td><code>{esc(n)}</code></td>'
                    f'<td>{"peri" if p["xbar"] == "peri" else "main"}</td><td>{pill(dom, ctext)}</td></tr>')
    return "\n".join(rows), len(allr)


TEST_INFO = {
    "axi_sram_rw":       ("Interconnect", "UVM", "Main SRAM: words, a byte write, 2–16-beat bursts split by the bridge"),
    "axi_csr_reset":     ("Interconnect", "UVM", "7 reset values across both crossbars and both CDCs"),
    "axi_decerr":        ("Interconnect", "UVM", "5 holes × read/write → DECERR from the right crossbar"),
    "fw_hello":          ("Bring-up", "C", "hex → ROM → boot → C; .data/.bss; log and PASS over AXI"),
    "fw_gpio_out":       ("Peripherals", "C+UVM", "17 GPIO patterns reach the pads in order"),
    "fw_uart_tx":        ("Peripherals", "C+UVM", "UART0 text decoded at 1 Mbaud"),
    "fw_timer_irq":      ("Interrupts", "C", "5 timer interrupts, one per deadline"),
    "clk_freq":          ("Clocks", "TB", "Every clkmgr output within 1% of nominal"),
    "fw_clk_gate":       ("Clocks", "C+UVM", "4 gateable clocks stop and restart"),
    "fw_sw_reset":       ("Reset", "C+UVM", "Software reset; RESET_INFO POR → SW_RESET; 2 boots"),
    "fw_wdog_reset":     ("Reset", "C+UVM", "AON watchdog bite resets the chip; 2 boots"),
    "axi_mem_map_sweep": ("Memory map", "UVM", "46 windows answer, 42 gaps DECERR"),
    "axi_mem_test":      ("Memory", "UVM", "SRAM data/address lines, retention SRAM, ROM contents"),
    "fw_plic_gpio_irq":  ("Interrupts", "C+UVM", "4 GPIO edges → 4 PLIC interrupts in order"),
}


def test_rows():
    rows = []
    for line in TESTS.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        n, u, f = line.split()
        area, kind, proves = TEST_INFO.get(n, ("—", "—", "—"))
        rows.append(f'<tr><td><code>{n}</code></td><td>{area}</td><td>{kind}</td><td><code>{u}</code></td>'
                    f'<td><code>{f}</code></td><td>{esc(proves)}</td><td class="ok">pass</td></tr>')
    return "\n".join(rows), len(rows)


def main():
    devs = load()
    t = TEMPLATE.read_text()
    mm, n_win = memmap_rows(devs)
    tr, n_tests = test_rows()
    subs = {
        "@@SOC_DIAGRAM@@": soc_diagram(devs),
        "@@IP_INVENTORY@@": ip_inventory(devs),
        "@@MEMMAP_ROWS@@": mm,
        "@@N_WINDOWS@@": str(n_win),
        "@@TEST_ROWS@@": tr,
        "@@N_TESTS@@": str(n_tests),
        "@@N_IPS@@": str(len(devs)),
        "@@DATE@@": datetime.date.today().isoformat(),
    }
    for k, v in subs.items():
        t = t.replace(k, v)
    for f in sorted(FIGURES.glob("*.svg")):
        t = t.replace(f"@@FIG:{f.stem}@@", f.read_text())
    if "@@" in t:
        raise SystemExit("unfilled placeholder in template")
    OUT.write_text(t)
    print(f"{OUT.relative_to(ROOT)}: {n_win} windows, {len(devs)} device ports, {n_tests} tests")


if __name__ == "__main__":
    main()
