// gpio_out — GPIO outputs reach the pads (plan test T5).
//
// Firmware drives a pattern sequence on GPIO[7:0] (pads IOB0..IOB7), checks
// each value also reads back on DATA_IN, and holds each one for HOLD_US.
// The UVM test (fw_gpio_out_test) watches the pins and checks it sees exactly
// this sequence: walking 1 over 8 bits, walking 0, then 0xA5.
//
// Keep this table and uvm/tests/fw_gpio_out_test.sv in step.

#include "titan.h"

#define HOLD_US 5

static const uint8_t kPattern[] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,   // walking 1
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F,   // walking 0
    0xA5,
};

int main(void) {
  timer_init_us();
  pinmux_connect_tb_gpios();
  gpio_write(0);
  gpio_set_oe(0xFF);

  for (unsigned i = 0; i < sizeof(kPattern); i++) {
    gpio_write(kPattern[i]);
    delay_us(HOLD_US);
    uint32_t in = gpio_read() & 0xFF;
    CHECK(in == kPattern[i], "GPIO wrote 0x%02x, DATA_IN reads 0x%02x", kPattern[i], in);
  }
  titan_printf("gpio_out: %u patterns driven and read back\n", (unsigned)sizeof(kPattern));
  return 0;
}
