// clk_gate — software clock gating in clkmgr (SoC clock test).
//
// For each of the four software-gateable peripheral clocks: gate it, tell the
// testbench (titan_sync odd step), wait; ungate it, tell the testbench (even
// step), wait. The UVM test (fw_clk_gate_test) checks the clock really stops
// and restarts at the clkmgr output.
//
// While a clock is gated nothing on it may be touched: a register access to a
// gated IP would never complete. Waiting uses rv_timer, which runs on
// clk_io_div4_timers — a different, ungated branch.
//
// Keep the order in step with uvm/tests/titan_test_pkg.sv (fw_clk_gate_test).

#include "regs/clkmgr_regs.h"
#include "titan.h"

#define HOLD_US 20

static const uint32_t kGates[] = {
    CLKMGR_CLK_ENABLES_CLK_IO_DIV4_PERI_EN_BIT,   // uart, gpio, i2c, spi_device, ...
    CLKMGR_CLK_ENABLES_CLK_IO_DIV2_PERI_EN_BIT,   // spi_host1
    CLKMGR_CLK_ENABLES_CLK_IO_PERI_EN_BIT,        // spi_host0
    CLKMGR_CLK_ENABLES_CLK_USB_PERI_EN_BIT,       // usbdev
};

int main(void) {
  timer_init_us();
  for (uint32_t i = 0; i < sizeof(kGates) / sizeof(kGates[0]); i++) {
    clkmgr_peri_clock_enable(kGates[i], false);
    titan_sync(2 * i + 1);
    delay_us(HOLD_US);
    clkmgr_peri_clock_enable(kGates[i], true);
    titan_sync(2 * i + 2);
    delay_us(HOLD_US);
  }
  titan_puts("clk_gate: 4 clocks gated and ungated\n");
  return 0;
}
