// titan_env_pkg — the titan-axi UVM environment.
//
//   titan_env
//   ├── axi[port]          one axi_agent per fabric port (56): passive, except
//   │                      host.sim_host, which is active (stub-CPU tests)
//   ├── scoreboard         fabric_scoreboard: every host transaction must reach
//   │                      the right device intact, or get DECERR
//   ├── sw                 sw_status_monitor: firmware status + log, decoded from
//   │                      AXI writes on dev.titan_sim_ctrl
//   ├── uart0              uart_monitor on the chip's UART0 TX pad
//   └── gpio               gpio_monitor on GPIO[7:0]

package titan_env_pkg;
  import uvm_pkg::*;
  import axi_agent_pkg::*;
  import uart_agent_pkg::*;
  import gpio_agent_pkg::*;
  import titan_addr_map_pkg::*;
  `include "uvm_macros.svh"

  // Firmware status words (firmware/include/titan.h).
  localparam bit [31:0] SimCtrlBase      = 32'h411F_0080;
  localparam bit [31:0] StatusInTest     = 32'h4354;
  localparam bit [31:0] StatusPassed     = 32'h900D;
  localparam bit [31:0] StatusFailed     = 32'hBAAD;

  // ===========================================================================
  class titan_env_cfg extends uvm_object;
    `uvm_object_utils(titan_env_cfg)
    bit          en_scoreboard = 1;
    int unsigned uart_baud     = 1_000_000;
    function new(string name = "titan_env_cfg");
      super.new(name);
    endfunction
  endclass

  // ===========================================================================
  // Firmware -> testbench channel. crt0 and lib/titan.c write
  //   +0x0 STATUS (IN_TEST / PASSED / FAILED), +0x4 LOG (one character)
  // in titan_sim_ctrl; this subscriber listens to that port's AXI monitor.
  class sw_status_monitor extends uvm_subscriber #(axi_seq_item);
    `uvm_component_utils(sw_status_monitor)

    bit [31:0] status;
    uvm_event  ev_in_test;     // firmware reached main()
    uvm_event  ev_done;        // PASSED or FAILED written
    protected string line;

    function new(string name, uvm_component parent);
      super.new(name, parent);
      ev_in_test = new("ev_in_test");
      ev_done    = new("ev_done");
    endfunction

    function bit passed();
      return status == StatusPassed;
    endfunction

    virtual function void write(axi_seq_item t);
      if (t.kind != AXI_WRITE) return;
      foreach (t.data[i]) begin
        bit [31:0] offset = t.addr + 4 * i - SimCtrlBase;
        case (offset)
          32'h0: begin
            status = t.data[i];
            case (status)
              StatusInTest: begin
                `uvm_info("SW", "firmware status: IN_TEST (main() started)", UVM_LOW)
                ev_in_test.trigger();
              end
              StatusPassed: begin
                `uvm_info("SW", "firmware status: PASSED", UVM_LOW)
                ev_done.trigger();
              end
              StatusFailed: begin
                `uvm_error("SW", "firmware status: FAILED")
                ev_done.trigger();
              end
              default: `uvm_warning("SW", $sformatf("unknown firmware status 0x%08h", status))
            endcase
          end
          32'h4: begin
            byte c = t.data[i][7:0];
            if (c == "\n") begin
              `uvm_info("SW", line, UVM_LOW)
              line = "";
            end else begin
              line = {line, string'(c)};
            end
          end
          default: ;
        endcase
      end
    endfunction
  endclass

  // ===========================================================================
  // Interconnect scoreboard.
  //
  // Device-side transactions are queued per device as they complete. When a
  // host-side transaction completes, its address is decoded:
  //   * unmapped, or not reachable from that host  -> must be DECERR, and no
  //     device may have seen it;
  //   * otherwise the device queue must hold its twin: same host index
  //     (device ID bits [5:4], prepended by xbar_main), same host ID, address,
  //     kind and length, identical write data/strobes, identical read data
  //     and responses.
  // The response always reaches the host after the device saw the request,
  // so the twin is always queued first.
  class fabric_scoreboard extends uvm_component;
    `uvm_component_utils(fabric_scoreboard)

    uvm_analysis_imp #(axi_seq_item, fabric_scoreboard) imp;
    protected axi_seq_item dev_q[string][$];
    int unsigned n_matched, n_decerr, n_errors;

    function new(string name, uvm_component parent);
      super.new(name, parent);
      imp = new("imp", this);
    endfunction

    virtual function void write(axi_seq_item t);
      if (t.port.substr(0, 3) == "dev.") begin
        dev_q[t.port].push_back(t);
      end else if (t.port.substr(0, 4) == "host.") begin
        check_host(t);
      end
      // "link." ports (main -> peri) are internal: watched, not scored.
    endfunction

    protected function int host_index(string host);
      for (int i = 0; i < 4; i++) if (host_name(i) == host) return i;
      return -1;
    endfunction

    protected function void fail(string msg);
      n_errors++;
      `uvm_error("FABRIC_SB", msg)
    endfunction

    protected function void check_host(axi_seq_item h);
      string dev   = decode(h.addr);
      int    hidx  = host_index(h.port);
      if (dev == "" || !reachable(h.port, dev)) begin
        foreach (h.resp[i])
          if (h.resp[i] != AXI_DECERR)
            fail($sformatf("expected DECERR (%s): %s",
                           dev == "" ? "unmapped" : {dev, " not reachable"}, h.convert2string()));
        n_decerr++;
        return;
      end
      foreach (dev_q[dev][i]) begin
        axi_seq_item d = dev_q[dev][i];
        if (d.id[5:4] == hidx[1:0] && d.id[3:0] == h.id[3:0] && d.addr == h.addr &&
            d.kind == h.kind && d.len == h.len) begin
          dev_q[dev].delete(i);
          compare(h, d);
          n_matched++;
          return;
        end
      end
      fail($sformatf("never seen at %s: %s", dev, h.convert2string()));
    endfunction

    protected function void compare(axi_seq_item h, axi_seq_item d);
      string what = $sformatf("%s vs %s", h.convert2string(), d.port);
      if (h.size != d.size || h.burst != d.burst || h.prot != d.prot)
        fail($sformatf("size/burst/prot changed in the fabric: %s", what));
      if (h.kind == AXI_WRITE && (h.data != d.data || h.strb != d.strb))
        fail($sformatf("write data changed in the fabric: %s", what));
      if (h.kind == AXI_READ && h.data != d.data)
        fail($sformatf("read data changed in the fabric: %s (host 0x%08h, device 0x%08h)",
                       what, h.data[0], d.data[0]));
      if (h.resp != d.resp)
        fail($sformatf("response changed in the fabric: %s", what));
    endfunction

    virtual function void check_phase(uvm_phase phase);
      // Anything a device saw well before the end without a host twin is a
      // transaction the fabric invented or misrouted. (The last few may simply
      // still be on their way back to the host when the test stops.)
      foreach (dev_q[dev]) foreach (dev_q[dev][i])
        if (dev_q[dev][i].t_done + 5us < $time)
          fail($sformatf("device transaction with no host twin: %s", dev_q[dev][i].convert2string()));
    endfunction

    virtual function void report_phase(uvm_phase phase);
      `uvm_info("FABRIC_SB", $sformatf("%0d transactions matched host->device, %0d DECERR, %0d errors",
                                       n_matched, n_decerr, n_errors), UVM_LOW)
    endfunction
  endclass

  // ===========================================================================
  class titan_env extends uvm_env;
    `uvm_component_utils(titan_env)

    titan_env_cfg      cfg;
    axi_agent          axi[string];
    fabric_scoreboard  scoreboard;
    sw_status_monitor  sw;
    uart_monitor       uart0;
    gpio_monitor       gpio;

    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction

    protected function string dots_to_underscores(string s);
      foreach (s[i]) if (s[i] == ".") s[i] = "_";
      return s;
    endfunction

    // The UVM test's bus master (stub CPU).
    function axi_sequencer sim_master();
      return axi["host.sim_host"].sequencer;
    endfunction

    virtual function void build_phase(uvm_phase phase);
      if (cfg == null) cfg = titan_env_cfg::type_id::create("cfg");
      foreach (PortNames[i]) begin
        string        port = PortNames[i];
        axi_agent_cfg acfg = axi_agent_cfg::type_id::create({"cfg_", port});
        acfg.port_name = port;
        acfg.is_active = (port == "host.sim_host");
        if (!uvm_config_db #(virtual axi_if)::get(null, "", {"axi_vif:", port}, acfg.vif))
          `uvm_fatal("ENV", {"no axi_if for port ", port, " (is titan_axi_probe compiled?)"})
        // UVM component names may not contain '.': "host.rv_dm.sba" -> "axi_host_rv_dm_sba".
        axi[port] = axi_agent::type_id::create({"axi_", dots_to_underscores(port)}, this);
        axi[port].cfg = acfg;
      end
      if (cfg.en_scoreboard) scoreboard = fabric_scoreboard::type_id::create("scoreboard", this);
      sw    = sw_status_monitor::type_id::create("sw", this);
      uart0 = uart_monitor::type_id::create("uart0", this);
      gpio  = gpio_monitor::type_id::create("gpio", this);
      uart0.baud = cfg.uart_baud;
    endfunction

    virtual function void connect_phase(uvm_phase phase);
      foreach (axi[port]) if (cfg.en_scoreboard) axi[port].monitor.ap.connect(scoreboard.imp);
      axi["dev.titan_sim_ctrl"].monitor.ap.connect(sw.analysis_export);
    endfunction
  endclass

endpackage
