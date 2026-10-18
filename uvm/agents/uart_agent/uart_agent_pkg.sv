// uart_agent_pkg — UART receiver for the chip's TX pin.
//
// 8N1, LSB first, fixed baud (cfg.baud). Each received byte goes out on `ap`;
// `text` accumulates everything received, for simple string checks.
// Exercise: add a driver for the RX pin, parity, and baud-error checks.

package uart_agent_pkg;
  import uvm_pkg::*;
  `include "uvm_macros.svh"

  class uart_monitor extends uvm_monitor;
    `uvm_component_utils(uart_monitor)

    virtual uart_if             vif;
    int unsigned                baud = 1_000_000;
    uvm_analysis_port #(byte)   ap;
    string                      text;   // all bytes received so far
    protected string            line;   // current line, for the log

    function new(string name, uvm_component parent);
      super.new(name, parent);
      ap = new("ap", this);
    endfunction

    virtual function void build_phase(uvm_phase phase);
      if (!uvm_config_db#(virtual uart_if)::get(this, "", "vif", vif))
        `uvm_fatal(get_name(), "uart_if not found in config_db")
    endfunction

    virtual task run_phase(uvm_phase phase);
      realtime bit_time = 1s / baud;
      wait (vif.tx === 1'b1);                   // pin idle (high) before the first frame
      forever begin
        byte b;
        @(negedge vif.tx);                      // start bit
        #(bit_time / 2);
        if (vif.tx !== 1'b0) continue;          // glitch, not a start bit
        for (int i = 0; i < 8; i++) begin
          #(bit_time);
          b[i] = vif.tx;
        end
        #(bit_time);
        if (vif.tx !== 1'b1)
          `uvm_error(get_name(), $sformatf("framing error: stop bit is %b (byte 0x%02h)", vif.tx, b))
        text = {text, string'(b)};
        ap.write(b);
        if (b == "\n") begin
          `uvm_info(get_name(), $sformatf("UART0 RX: \"%s\"", line), UVM_LOW)
          line = "";
        end else begin
          line = {line, string'(b)};
        end
      end
    endtask
  endclass

endpackage
