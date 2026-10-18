// uart_if — the chip's UART0 pins as the testbench sees them.
//   tx: driven by the chip (pad IOC4), monitored
//   rx: driven by the testbench into the chip (pad IOC3); idles high
interface uart_if;
  logic tx;
  logic rx = 1'b1;
endinterface
