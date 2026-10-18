// clk_mon_if — clkmgr output clocks, for frequency and gating checks.
// tb_top connects each wire to dut.top_earlgrey.clkmgr_aon_clocks.<clock>.
interface clk_mon_if;
  // always-on branches (cannot be gated by software)
  wire clk_main;            // clk_main_infra      100 MHz
  wire clk_io;              // clk_io_infra         96 MHz
  wire clk_io_div2;         // clk_io_div2_infra    48 MHz
  wire clk_io_div4;         // clk_io_div4_infra    24 MHz
  wire clk_usb;             // clk_usb_infra        48 MHz
  wire clk_aon;             // clk_aon_timers      200 kHz
  // software-gateable branches (clkmgr CLK_ENABLES)
  wire clk_io_div4_peri;
  wire clk_io_div2_peri;
  wire clk_io_peri;
  wire clk_usb_peri;
endinterface
