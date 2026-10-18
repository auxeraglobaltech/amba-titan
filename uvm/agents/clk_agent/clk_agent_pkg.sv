// clk_agent_pkg — measures the chip's clocks from the testbench.
//
//   count_edges("io_div4", 10us, n)   rising edges in a window
//   measure_hz("io_div4", 10us)       frequency over a window
//
// Names: main io io_div2 io_div4 usb aon io_div4_peri io_div2_peri io_peri usb_peri

package clk_agent_pkg;
  import uvm_pkg::*;
  `include "uvm_macros.svh"

  class clk_monitor extends uvm_component;
    `uvm_component_utils(clk_monitor)
    virtual clk_mon_if vif;

    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction

    virtual function void build_phase(uvm_phase phase);
      if (!uvm_config_db#(virtual clk_mon_if)::get(this, "", "vif", vif))
        `uvm_fatal(get_name(), "clk_mon_if not found in config_db")
    endfunction

    task wait_posedge(string clk);
      case (clk)
        "main":         @(posedge vif.clk_main);
        "io":           @(posedge vif.clk_io);
        "io_div2":      @(posedge vif.clk_io_div2);
        "io_div4":      @(posedge vif.clk_io_div4);
        "usb":          @(posedge vif.clk_usb);
        "aon":          @(posedge vif.clk_aon);
        "io_div4_peri": @(posedge vif.clk_io_div4_peri);
        "io_div2_peri": @(posedge vif.clk_io_div2_peri);
        "io_peri":      @(posedge vif.clk_io_peri);
        "usb_peri":     @(posedge vif.clk_usb_peri);
        default: `uvm_fatal(get_name(), {"unknown clock ", clk})
      endcase
    endtask

    task count_edges(string clk, time window, output int unsigned n);
      int unsigned count = 0;
      fork
        forever begin wait_posedge(clk); count++; end
        #(window);
      join_any
      disable fork;
      n = count;
    endtask

    task measure_hz(string clk, time window, output real hz);
      int unsigned n;
      count_edges(clk, window, n);
      hz = n / (window / 1.0s);
    endtask
  endclass

endpackage
