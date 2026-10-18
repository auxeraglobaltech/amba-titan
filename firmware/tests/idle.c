// idle — firmware for pure-UVM tests (plan tests T1-T3).
//
// The core boots, reports "in test", then sleeps forever. The UVM test drives
// the fabric through the sim-only AXI master and decides pass/fail itself.

#include "titan.h"

int main(void) {
  titan_puts("idle: core parked, the bus belongs to the testbench\n");
  for (;;) wait_for_interrupt();
}
