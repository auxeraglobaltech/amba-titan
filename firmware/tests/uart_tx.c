// uart_tx — UART0 TX reaches the pad (plan test T6).
//
// Firmware sends one line at 1 Mbaud on UART0 (pad IOC4). The UVM test
// (fw_uart_tx_test) decodes the pin and checks the exact text.
//
// Keep kMessage and uvm/tests/fw_uart_tx_test.sv in step.

#include "titan.h"

#define BAUD 1000000u
static const char kMessage[] = "titan-axi uart0 ok\n";

int main(void) {
  pinmux_connect_uart0();
  uart_init(BAUD);
  uart_puts(kMessage);
  uart_flush();       // do not report pass before the last stop bit
  titan_printf("uart_tx: sent %u bytes\n", (unsigned)(sizeof(kMessage) - 1));
  return 0;
}
