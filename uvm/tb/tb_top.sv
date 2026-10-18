// tb_top — the titan-axi testbench top.
//
//   chip_earlgrey_asic (with the AXI fabric)
//     pads: POR_N driven here; UART0 on IOC4/IOC3 and GPIO[7:0] on IOB0-7 to
//           the agents; every other pad weakly pulled low, so nothing floats
//           (a floating pad read by software returns X, and X on the bus trips
//           assertions far from the cause).
//     clocks: generated inside the chip by the AST oscillator models.
//   images: firmware hex -> ROM, OTP vmem -> OTP, by $readmemh at time 0.
//   AXI:   titan_axi_probe (bound into the fabric) publishes every port.
//
// Plusargs
//   +firmware=<hex>   ROM image (firmware/build/<t>/<t>.hex) — required
//   +otp=<vmem>       OTP image (default firmware/otp/otp_rma.24.vmem, via run.sh)
//   +UVM_TESTNAME=<t> the UVM test

`timescale 1ns/1ps

module tb_top;
  import uvm_pkg::*;
  import titan_test_pkg::*;
  `include "uvm_macros.svh"

  // ---------------------------------------------------------------------------
  // Pads (same names as chip_earlgrey_asic's ports, connected with .*)
  // ---------------------------------------------------------------------------
  wire POR_N, USB_P, USB_N, CC1, CC2, FLASH_TEST_VOLT, FLASH_TEST_MODE0,
       FLASH_TEST_MODE1, OTP_EXT_VOLT, SPI_HOST_D0, SPI_HOST_D1, SPI_HOST_D2,
       SPI_HOST_D3, SPI_HOST_CLK, SPI_HOST_CS_L, SPI_DEV_D0, SPI_DEV_D1, SPI_DEV_D2,
       SPI_DEV_D3, SPI_DEV_CLK, SPI_DEV_CS_L, IOR8, IOR9, IOA0, IOA1, IOA2, IOA3,
       IOA4, IOA5, IOA6, IOA7, IOA8, IOB0, IOB1, IOB2, IOB3, IOB4, IOB5, IOB6, IOB7,
       IOB8, IOB9, IOB10, IOB11, IOB12, IOC0, IOC1, IOC2, IOC3, IOC4, IOC5, IOC6,
       IOC7, IOC8, IOC9, IOC10, IOC11, IOC12, IOR0, IOR1, IOR2, IOR3, IOR4, IOR5,
       IOR6, IOR7, IOR10, IOR11, IOR12, IOR13;

  // Every pad gets a weak pull-down; the drivers below override it.
  // (chip pads are inout, so a pull here is what a board resistor would be.)
  pulldown (weak0) pd_USB_P (USB_P);
  pulldown (weak0) pd_USB_N (USB_N);
  pulldown (weak0) pd_CC1 (CC1);
  pulldown (weak0) pd_CC2 (CC2);
  pulldown (weak0) pd_FLASH_TEST_VOLT (FLASH_TEST_VOLT);
  pulldown (weak0) pd_FLASH_TEST_MODE0 (FLASH_TEST_MODE0);
  pulldown (weak0) pd_FLASH_TEST_MODE1 (FLASH_TEST_MODE1);
  pulldown (weak0) pd_OTP_EXT_VOLT (OTP_EXT_VOLT);
  pulldown (weak0) pd_SPI_HOST_D0 (SPI_HOST_D0);
  pulldown (weak0) pd_SPI_HOST_D1 (SPI_HOST_D1);
  pulldown (weak0) pd_SPI_HOST_D2 (SPI_HOST_D2);
  pulldown (weak0) pd_SPI_HOST_D3 (SPI_HOST_D3);
  pulldown (weak0) pd_SPI_HOST_CLK (SPI_HOST_CLK);
  pulldown (weak0) pd_SPI_HOST_CS_L (SPI_HOST_CS_L);
  pulldown (weak0) pd_SPI_DEV_D0 (SPI_DEV_D0);
  pulldown (weak0) pd_SPI_DEV_D1 (SPI_DEV_D1);
  pulldown (weak0) pd_SPI_DEV_D2 (SPI_DEV_D2);
  pulldown (weak0) pd_SPI_DEV_D3 (SPI_DEV_D3);
  pulldown (weak0) pd_SPI_DEV_CLK (SPI_DEV_CLK);
  pulldown (weak0) pd_SPI_DEV_CS_L (SPI_DEV_CS_L);
  pulldown (weak0) pd_IOR8 (IOR8);
  pulldown (weak0) pd_IOR9 (IOR9);
  pulldown (weak0) pd_IOA0 (IOA0);
  pulldown (weak0) pd_IOA1 (IOA1);
  pulldown (weak0) pd_IOA2 (IOA2);
  pulldown (weak0) pd_IOA3 (IOA3);
  pulldown (weak0) pd_IOA4 (IOA4);
  pulldown (weak0) pd_IOA5 (IOA5);
  pulldown (weak0) pd_IOA6 (IOA6);
  pulldown (weak0) pd_IOA7 (IOA7);
  pulldown (weak0) pd_IOA8 (IOA8);
  pulldown (weak0) pd_IOB0 (IOB0);
  pulldown (weak0) pd_IOB1 (IOB1);
  pulldown (weak0) pd_IOB2 (IOB2);
  pulldown (weak0) pd_IOB3 (IOB3);
  pulldown (weak0) pd_IOB4 (IOB4);
  pulldown (weak0) pd_IOB5 (IOB5);
  pulldown (weak0) pd_IOB6 (IOB6);
  pulldown (weak0) pd_IOB7 (IOB7);
  pulldown (weak0) pd_IOB8 (IOB8);
  pulldown (weak0) pd_IOB9 (IOB9);
  pulldown (weak0) pd_IOB10 (IOB10);
  pulldown (weak0) pd_IOB11 (IOB11);
  pulldown (weak0) pd_IOB12 (IOB12);
  pulldown (weak0) pd_IOC0 (IOC0);
  pulldown (weak0) pd_IOC1 (IOC1);
  pulldown (weak0) pd_IOC2 (IOC2);
  pulldown (weak0) pd_IOC4 (IOC4);
  pulldown (weak0) pd_IOC5 (IOC5);
  pulldown (weak0) pd_IOC6 (IOC6);
  pulldown (weak0) pd_IOC7 (IOC7);
  pulldown (weak0) pd_IOC8 (IOC8);
  pulldown (weak0) pd_IOC9 (IOC9);
  pulldown (weak0) pd_IOC10 (IOC10);
  pulldown (weak0) pd_IOC11 (IOC11);
  pulldown (weak0) pd_IOC12 (IOC12);
  pulldown (weak0) pd_IOR0 (IOR0);
  pulldown (weak0) pd_IOR1 (IOR1);
  pulldown (weak0) pd_IOR2 (IOR2);
  pulldown (weak0) pd_IOR3 (IOR3);
  pulldown (weak0) pd_IOR4 (IOR4);
  pulldown (weak0) pd_IOR5 (IOR5);
  pulldown (weak0) pd_IOR6 (IOR6);
  pulldown (weak0) pd_IOR7 (IOR7);
  pulldown (weak0) pd_IOR10 (IOR10);
  pulldown (weak0) pd_IOR11 (IOR11);
  pulldown (weak0) pd_IOR12 (IOR12);
  pulldown (weak0) pd_IOR13 (IOR13);

  // ---------------------------------------------------------------------------
  // Power-on reset: low for 10 us, then released.
  // ---------------------------------------------------------------------------
  logic por_n = 1'b0;
  assign POR_N = por_n;
  initial #10us por_n = 1'b1;

  // ---------------------------------------------------------------------------
  // UART0 (TX = IOC4 from the chip, RX = IOC3 into the chip) and GPIO[7:0]
  // ---------------------------------------------------------------------------
  uart_if uart0_if ();
  assign uart0_if.tx = IOC4;
  assign IOC3 = uart0_if.rx;

  gpio_if gpio_if_i ();
  assign gpio_if_i.pins = {IOB7, IOB6, IOB5, IOB4, IOB3, IOB2, IOB1, IOB0};
  // Testbench drive of the GPIO pads (inputs to the chip), when drv_en is set.
  assign IOB0 = gpio_if_i.drv_en[0] ? gpio_if_i.drv_val[0] : 1'bz;
  assign IOB1 = gpio_if_i.drv_en[1] ? gpio_if_i.drv_val[1] : 1'bz;
  assign IOB2 = gpio_if_i.drv_en[2] ? gpio_if_i.drv_val[2] : 1'bz;
  assign IOB3 = gpio_if_i.drv_en[3] ? gpio_if_i.drv_val[3] : 1'bz;
  assign IOB4 = gpio_if_i.drv_en[4] ? gpio_if_i.drv_val[4] : 1'bz;
  assign IOB5 = gpio_if_i.drv_en[5] ? gpio_if_i.drv_val[5] : 1'bz;
  assign IOB6 = gpio_if_i.drv_en[6] ? gpio_if_i.drv_val[6] : 1'bz;
  assign IOB7 = gpio_if_i.drv_en[7] ? gpio_if_i.drv_val[7] : 1'bz;

  // ---------------------------------------------------------------------------
  // Clock monitor: clkmgr outputs, for the clock tests
  // ---------------------------------------------------------------------------
  clk_mon_if clk_mon_if_i ();
  assign clk_mon_if_i.clk_main         = dut.top_earlgrey.clkmgr_aon_clocks.clk_main_infra;
  assign clk_mon_if_i.clk_io           = dut.top_earlgrey.clkmgr_aon_clocks.clk_io_infra;
  assign clk_mon_if_i.clk_io_div2      = dut.top_earlgrey.clkmgr_aon_clocks.clk_io_div2_infra;
  assign clk_mon_if_i.clk_io_div4      = dut.top_earlgrey.clkmgr_aon_clocks.clk_io_div4_infra;
  assign clk_mon_if_i.clk_usb          = dut.top_earlgrey.clkmgr_aon_clocks.clk_usb_infra;
  assign clk_mon_if_i.clk_aon          = dut.top_earlgrey.clkmgr_aon_clocks.clk_aon_timers;
  assign clk_mon_if_i.clk_io_div4_peri = dut.top_earlgrey.clkmgr_aon_clocks.clk_io_div4_peri;
  assign clk_mon_if_i.clk_io_div2_peri = dut.top_earlgrey.clkmgr_aon_clocks.clk_io_div2_peri;
  assign clk_mon_if_i.clk_io_peri      = dut.top_earlgrey.clkmgr_aon_clocks.clk_io_peri;
  assign clk_mon_if_i.clk_usb_peri     = dut.top_earlgrey.clkmgr_aon_clocks.clk_usb_peri;

  // ---------------------------------------------------------------------------
  // The chip
  // ---------------------------------------------------------------------------
  chip_earlgrey_asic #(
    // Plain ROM: no PRINCE scrambling, no KMAC digest check — so a hex file
    // straight from the compiler boots. (Same as OpenTitan's own DV.)
    .SecRomCtrlDisableScrambling(1'b1)
  ) dut (.*);

  // ---------------------------------------------------------------------------
  // Images, loaded at #1 (after any time-0 memory initialization in the RTL,
  // long before reset releases).
  // ---------------------------------------------------------------------------
  initial begin
    string fw, otp;
    if (!$value$plusargs("firmware=%s", fw))
      `uvm_fatal("TB", "missing +firmware=<hex> (firmware/build/<test>/<test>.hex)")
    if (!$value$plusargs("otp=%s", otp))
      `uvm_fatal("TB", "missing +otp=<vmem> (firmware/otp/otp_rma.24.vmem)")
    #1;
    $readmemh(fw, dut.top_earlgrey.u_rom_ctrl.gen_rom_scramble_disabled.u_rom.u_prim_rom.mem);
    $readmemh(otp, dut.top_earlgrey.u_otp_macro.u_prim_ram_1p_adv.gen_ram_inst[0].u_mem.mem);
    `uvm_info("TB", $sformatf("ROM <- %s", fw), UVM_LOW)
    `uvm_info("TB", $sformatf("OTP <- %s", otp), UVM_LOW)
  end

  // ---------------------------------------------------------------------------
  // UVM
  // ---------------------------------------------------------------------------
  initial begin
    uvm_config_db #(virtual uart_if)::set(null, "*", "vif", uart0_if);
    uvm_config_db #(virtual gpio_if)::set(null, "*", "vif", gpio_if_i);
    uvm_config_db #(virtual clk_mon_if)::set(null, "*", "vif", clk_mon_if_i);
    run_test();
  end

endmodule
