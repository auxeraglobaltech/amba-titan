// hello — the firmware flow end to end (plan test T4).
//
// Proves: the hex image boots from ROM through crt0 (PMP opened, SRAM
// initialized, .data copied, .bss zeroed), C code runs from ROM with data in
// SRAM, and the testbench receives log text and the pass status — all as AXI
// writes from Ibex through xbar_main to titan_sim_ctrl.
//
// C only: the default UVM test (fw_test) needs nothing more.

#include "titan.h"

static uint32_t counter = 41;       // .data: copied from ROM by crt0
static volatile uint32_t zeroed[16];  // .bss: zeroed by crt0 (volatile: GCC would fold the checks away)

int main(void) {
  titan_printf("hello from titan-axi: Ibex -> AXI fabric -> titan_sim_ctrl\n");

  CHECK(counter == 41, ".data not copied: counter=%u", counter);
  for (int i = 0; i < 16; i++) CHECK(zeroed[i] == 0, ".bss[%d] = 0x%x", i, zeroed[i]);

  counter++;
  CHECK(counter == 42, "SRAM read-back");

  // A few multiply/divide results, so a broken M extension fails loudly.
  volatile uint32_t a = 123456789, b = 1000;
  CHECK(a / b == 123456 && a % b == 789 && b * b == 1000000u, "rv32m");

  titan_printf("hello: all checks passed (counter=%u)\n", counter);
  return 0;
}
