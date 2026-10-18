// gpio_agent_pkg — records the values the GPIO pins settle to.
//
// A value is recorded once it has been stable for `settle` (so the few-ns
// skew between pins changing does not show up as extra values).
// Exercise: add a driver so the testbench can stimulate GPIO inputs.

package gpio_agent_pkg;
  import uvm_pkg::*;
  `include "uvm_macros.svh"

  class gpio_monitor extends uvm_monitor;
    `uvm_component_utils(gpio_monitor)

    virtual gpio_if  vif;
    realtime         settle = 200ns;
    bit [7:0]        values[$];      // settled values, in order

    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction

    virtual function void build_phase(uvm_phase phase);
      if (!uvm_config_db#(virtual gpio_if)::get(this, "", "vif", vif))
        `uvm_fatal(get_name(), "gpio_if not found in config_db")
    endfunction

    virtual task run_phase(uvm_phase phase);
      logic [7:0] last = 'x;
      forever begin
        @(vif.pins);
        // Restart the settle window on every change; leave when it passes quietly.
        forever begin
          bit changed = 0;
          fork
            #(settle);
            begin @(vif.pins); changed = 1; end
          join_any
          disable fork;
          if (!changed) break;
        end
        if ($isunknown(vif.pins) || vif.pins === last) continue;
        last = vif.pins;
        values.push_back(last);
        `uvm_info(get_name(), $sformatf("GPIO[7:0] = 0x%02h", last), UVM_MEDIUM)
      end
    endtask
  endclass

endpackage
