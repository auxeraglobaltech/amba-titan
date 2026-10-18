// axi_agent_pkg — a small, readable AXI4 UVM agent.
//
//   axi_seq_item     one AXI transaction (a whole burst, not a beat)
//   axi_monitor      rebuilds transactions from an axi_if, publishes on `ap`
//   axi_driver       master driver: one transaction at a time
//   axi_agent        monitor always; driver + sequencer when active
//   axi_base_seq     write32/read32/burst helpers for sequences
//
// Deliberately not done here (good exercises): multiple outstanding
// transactions in the driver, write data before address, randomized
// VALID/READY delays, functional coverage.

package axi_agent_pkg;
  import uvm_pkg::*;
  `include "uvm_macros.svh"

  typedef enum bit {AXI_READ, AXI_WRITE} axi_kind_e;

  localparam bit [1:0] AXI_OKAY = 2'b00, AXI_EXOKAY = 2'b01, AXI_SLVERR = 2'b10, AXI_DECERR = 2'b11;
  localparam bit [1:0] AXI_FIXED = 2'b00, AXI_INCR = 2'b01, AXI_WRAP = 2'b10;

  function automatic string resp_name(bit [1:0] r);
    case (r)
      AXI_OKAY:   return "OKAY";
      AXI_EXOKAY: return "EXOKAY";
      AXI_SLVERR: return "SLVERR";
      default:    return "DECERR";
    endcase
  endfunction

  // ===========================================================================
  class axi_seq_item extends uvm_sequence_item;
    rand axi_kind_e kind;
    rand bit [5:0]  id;
    rand bit [31:0] addr;
    rand bit [7:0]  len;      // beats - 1
    rand bit [2:0]  size;     // bytes per beat = 2**size
    rand bit [1:0]  burst;
    bit [2:0]       prot;
    rand bit [31:0] data[$];  // write: to send; read: received
    rand bit [3:0]  strb[$];  // write only
    bit [1:0]       resp[$];  // read: one per beat; write: resp[0] = BRESP

    // Filled in by the monitor.
    string          port;
    time            t_addr, t_done;

    constraint c_shape {
      size <= 2;
      burst inside {AXI_INCR, AXI_WRAP, AXI_FIXED};
      burst == AXI_WRAP -> len inside {1, 3, 7, 15};
      data.size() == len + 1;
      kind == AXI_WRITE -> strb.size() == len + 1;
      kind == AXI_READ  -> strb.size() == 0;
      soft len   == 0;
      soft size  == 2;
      soft burst == AXI_INCR;
      soft id    == 0;
    }

    `uvm_object_utils(axi_seq_item)

    function new(string name = "axi_seq_item");
      super.new(name);
    endfunction

    function bit [1:0] final_resp();
      bit [1:0] worst = AXI_OKAY;
      foreach (resp[i]) if (resp[i] > worst) worst = resp[i];
      return worst;
    endfunction

    virtual function string convert2string();
      string s = $sformatf("%s %s id=0x%0h addr=0x%08h len=%0d size=%0d resp=%s",
                           port, kind == AXI_WRITE ? "WR" : "RD", id, addr, len, size,
                           resp.size() ? resp_name(final_resp()) : "-");
      if (data.size()) s = {s, $sformatf(" data[0]=0x%08h", data[0])};
      return s;
    endfunction

    virtual function void do_copy(uvm_object rhs);
      axi_seq_item r;
      super.do_copy(rhs);
      $cast(r, rhs);
      kind = r.kind; id = r.id; addr = r.addr; len = r.len; size = r.size;
      burst = r.burst; prot = r.prot; data = r.data; strb = r.strb; resp = r.resp;
      port = r.port; t_addr = r.t_addr; t_done = r.t_done;
    endfunction
  endclass

  // ===========================================================================
  class axi_agent_cfg extends uvm_object;
    `uvm_object_utils(axi_agent_cfg)
    virtual axi_if vif;
    string         port_name;
    bit            is_active;
    function new(string name = "axi_agent_cfg");
      super.new(name);
    endfunction
  endclass

  // ===========================================================================
  // Monitor. Write data may arrive before or after its address (both are
  // legal); responses for one ID return in order, different IDs may
  // interleave — so pending transactions are kept per ID.
  class axi_monitor extends uvm_monitor;
    `uvm_component_utils(axi_monitor)

    axi_agent_cfg                     cfg;
    uvm_analysis_port #(axi_seq_item) ap;

    protected axi_seq_item aw_wait_data[$];     // address seen, data not complete
    protected bit [31:0]   w_data_q[$];         // beats of the burst in progress
    protected bit [3:0]    w_strb_q[$];
    protected bit [31:0]   w_done_data[$][$];   // complete bursts, no address yet
    protected bit [3:0]    w_done_strb[$][$];
    protected axi_seq_item wr_wait_b[bit [5:0]][$];
    protected axi_seq_item rd_wait_r[bit [5:0]][$];

    int unsigned n_wr, n_rd;

    function new(string name, uvm_component parent);
      super.new(name, parent);
      ap = new("ap", this);
    endfunction

    virtual task run_phase(uvm_phase phase);
      virtual axi_if vif = cfg.vif;
      forever begin
        @(vif.mon_cb);
        if (vif.aresetn !== 1'b1) begin
          aw_wait_data.delete(); w_data_q.delete(); w_strb_q.delete();
          w_done_data.delete(); w_done_strb.delete();
          wr_wait_b.delete(); rd_wait_r.delete();
          continue;
        end
        sample_aw(vif); sample_w(vif); pair_writes(); sample_b(vif);
        sample_ar(vif); sample_r(vif);
      end
    endtask

    protected function void sample_aw(virtual axi_if vif);
      axi_seq_item t;
      if (!(vif.mon_cb.awvalid && vif.mon_cb.awready)) return;
      t = axi_seq_item::type_id::create("wr");
      t.kind = AXI_WRITE; t.port = cfg.port_name; t.t_addr = $time;
      t.id = vif.mon_cb.awid; t.addr = vif.mon_cb.awaddr; t.len = vif.mon_cb.awlen;
      t.size = vif.mon_cb.awsize; t.burst = vif.mon_cb.awburst; t.prot = vif.mon_cb.awprot;
      aw_wait_data.push_back(t);
    endfunction

    protected function void sample_w(virtual axi_if vif);
      if (!(vif.mon_cb.wvalid && vif.mon_cb.wready)) return;
      w_data_q.push_back(vif.mon_cb.wdata);
      w_strb_q.push_back(vif.mon_cb.wstrb);
      if (vif.mon_cb.wlast) begin
        w_done_data.push_back(w_data_q); w_done_strb.push_back(w_strb_q);
        w_data_q.delete(); w_strb_q.delete();
      end
    endfunction

    protected function void pair_writes();
      while (aw_wait_data.size() && w_done_data.size()) begin
        axi_seq_item t = aw_wait_data.pop_front();
        t.data = w_done_data.pop_front();
        t.strb = w_done_strb.pop_front();
        if (t.data.size() != t.len + 1)
          `uvm_error(cfg.port_name, $sformatf("W burst has %0d beats, AWLEN says %0d: %s",
                                              t.data.size(), t.len + 1, t.convert2string()))
        wr_wait_b[t.id].push_back(t);
      end
    endfunction

    protected function void sample_b(virtual axi_if vif);
      axi_seq_item t;
      bit [5:0] id;
      if (!(vif.mon_cb.bvalid && vif.mon_cb.bready)) return;
      id = vif.mon_cb.bid;
      if (!wr_wait_b.exists(id) || wr_wait_b[id].size() == 0) begin
        `uvm_error(cfg.port_name, $sformatf("B with id 0x%0h but no write outstanding for it", id))
        return;
      end
      t = wr_wait_b[id].pop_front();
      t.resp = '{vif.mon_cb.bresp};
      t.t_done = $time;
      n_wr++;
      `uvm_info(cfg.port_name, t.convert2string(), UVM_HIGH)
      ap.write(t);
    endfunction

    protected function void sample_ar(virtual axi_if vif);
      axi_seq_item t;
      if (!(vif.mon_cb.arvalid && vif.mon_cb.arready)) return;
      t = axi_seq_item::type_id::create("rd");
      t.kind = AXI_READ; t.port = cfg.port_name; t.t_addr = $time;
      t.id = vif.mon_cb.arid; t.addr = vif.mon_cb.araddr; t.len = vif.mon_cb.arlen;
      t.size = vif.mon_cb.arsize; t.burst = vif.mon_cb.arburst; t.prot = vif.mon_cb.arprot;
      rd_wait_r[t.id].push_back(t);
    endfunction

    protected function void sample_r(virtual axi_if vif);
      axi_seq_item t;
      bit [5:0] id;
      if (!(vif.mon_cb.rvalid && vif.mon_cb.rready)) return;
      id = vif.mon_cb.rid;
      if (!rd_wait_r.exists(id) || rd_wait_r[id].size() == 0) begin
        `uvm_error(cfg.port_name, $sformatf("R with id 0x%0h but no read outstanding for it", id))
        return;
      end
      t = rd_wait_r[id][0];
      t.data.push_back(vif.mon_cb.rdata);
      t.resp.push_back(vif.mon_cb.rresp);
      if (vif.mon_cb.rlast) begin
        void'(rd_wait_r[id].pop_front());
        if (t.data.size() != t.len + 1)
          `uvm_error(cfg.port_name, $sformatf("RLAST after %0d beats, ARLEN says %0d: %s",
                                              t.data.size(), t.len + 1, t.convert2string()))
        t.t_done = $time;
        n_rd++;
        `uvm_info(cfg.port_name, t.convert2string(), UVM_HIGH)
        ap.write(t);
      end
    endfunction

    virtual function void report_phase(uvm_phase phase);
      int unsigned pending = aw_wait_data.size();
      foreach (wr_wait_b[i]) pending += wr_wait_b[i].size();
      foreach (rd_wait_r[i]) pending += rd_wait_r[i].size();
      if (n_wr || n_rd || pending)
        `uvm_info(cfg.port_name, $sformatf("%0d writes, %0d reads, %0d unfinished",
                                           n_wr, n_rd, pending), UVM_MEDIUM)
    endfunction
  endclass

  // ===========================================================================
  // Master driver: one transaction at a time; BREADY/RREADY always high.
  // Signals change with nonblocking assignments just after a sampling edge
  // (@mon_cb); READY/VALID from the other side are read through mon_cb, i.e.
  // as they were just before that edge. The item is completed in place.
  class axi_driver extends uvm_driver #(axi_seq_item);
    `uvm_component_utils(axi_driver)
    axi_agent_cfg cfg;

    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction

    virtual task run_phase(uvm_phase phase);
      virtual axi_if vif = cfg.vif;
      idle(vif);
      forever begin
        seq_item_port.get_next_item(req);
        @(vif.mon_cb);
        if (req.kind == AXI_WRITE) do_write(vif, req); else do_read(vif, req);
        seq_item_port.item_done();
      end
    endtask

    protected task idle(virtual axi_if vif);
      vif.awvalid <= 0; vif.wvalid <= 0; vif.arvalid <= 0;
      vif.bready  <= 1; vif.rready <= 1;
      vif.awid <= 0; vif.awaddr <= 0; vif.awlen <= 0; vif.awsize <= 0; vif.awburst <= 0;
      vif.awprot <= 0; vif.wdata <= 0; vif.wstrb <= 0; vif.wlast <= 0;
      vif.arid <= 0; vif.araddr <= 0; vif.arlen <= 0; vif.arsize <= 0; vif.arburst <= 0;
      vif.arprot <= 0;
    endtask

    protected task do_write(virtual axi_if vif, axi_seq_item t);
      vif.awid    <= t.id;
      vif.awaddr  <= t.addr;
      vif.awlen   <= t.len;
      vif.awsize  <= t.size;
      vif.awburst <= t.burst;
      vif.awprot  <= t.prot;
      vif.awvalid <= 1;
      do @(vif.mon_cb); while (!vif.mon_cb.awready);
      vif.awvalid <= 0;
      foreach (t.data[i]) begin
        vif.wdata  <= t.data[i];
        vif.wstrb  <= t.strb[i];
        vif.wlast  <= (i == t.len);
        vif.wvalid <= 1;
        do @(vif.mon_cb); while (!vif.mon_cb.wready);
      end
      vif.wvalid <= 0;
      while (!vif.mon_cb.bvalid) @(vif.mon_cb);
      t.resp = '{vif.mon_cb.bresp};
    endtask

    protected task do_read(virtual axi_if vif, axi_seq_item t);
      vif.arid    <= t.id;
      vif.araddr  <= t.addr;
      vif.arlen   <= t.len;
      vif.arsize  <= t.size;
      vif.arburst <= t.burst;
      vif.arprot  <= t.prot;
      vif.arvalid <= 1;
      do @(vif.mon_cb); while (!vif.mon_cb.arready);
      vif.arvalid <= 0;
      t.data.delete(); t.resp.delete();
      forever begin
        if (vif.mon_cb.rvalid) begin
          t.data.push_back(vif.mon_cb.rdata);
          t.resp.push_back(vif.mon_cb.rresp);
          if (vif.mon_cb.rlast) break;
        end
        @(vif.mon_cb);
      end
    endtask
  endclass

  typedef uvm_sequencer #(axi_seq_item) axi_sequencer;

  // ===========================================================================
  class axi_agent extends uvm_agent;
    `uvm_component_utils(axi_agent)
    axi_agent_cfg cfg;
    axi_monitor   monitor;
    axi_driver    driver;
    axi_sequencer sequencer;

    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction

    virtual function void build_phase(uvm_phase phase);
      if (cfg == null) `uvm_fatal(get_name(), "axi_agent_cfg not set")
      monitor = axi_monitor::type_id::create("monitor", this);
      monitor.cfg = cfg;
      if (cfg.is_active) begin
        driver    = axi_driver::type_id::create("driver", this);
        sequencer = axi_sequencer::type_id::create("sequencer", this);
        driver.cfg = cfg;
      end
    endfunction

    virtual function void connect_phase(uvm_phase phase);
      if (cfg.is_active) driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
  endclass

  // ===========================================================================
  class axi_base_seq extends uvm_sequence #(axi_seq_item);
    `uvm_object_utils(axi_base_seq)
    function new(string name = "axi_base_seq");
      super.new(name);
    endfunction

    task write(bit [31:0] addr, bit [31:0] data[$], bit [3:0] strb[$], output bit [1:0] resp,
               input bit [7:0] len = 0, bit [2:0] size = 2, bit [1:0] burst = AXI_INCR,
               bit [5:0] id = 0);
      axi_seq_item t = axi_seq_item::type_id::create("wr");
      start_item(t);
      t.kind = AXI_WRITE; t.addr = addr; t.len = len; t.size = size; t.burst = burst; t.id = id;
      t.data = data; t.strb = strb;
      finish_item(t);
      resp = t.resp[0];
    endtask

    task read(bit [31:0] addr, output bit [31:0] data[$], output bit [1:0] resp[$],
              input bit [7:0] len = 0, bit [2:0] size = 2, bit [1:0] burst = AXI_INCR,
              bit [5:0] id = 0);
      axi_seq_item t = axi_seq_item::type_id::create("rd");
      start_item(t);
      t.kind = AXI_READ; t.addr = addr; t.len = len; t.size = size; t.burst = burst; t.id = id;
      finish_item(t);
      data = t.data; resp = t.resp;
    endtask

    task write32(bit [31:0] addr, bit [31:0] data, output bit [1:0] resp);
      write(addr, '{data}, '{4'hF}, resp);
    endtask

    task read32(bit [31:0] addr, output bit [31:0] data, output bit [1:0] resp);
      bit [31:0] d[$];
      bit [1:0]  r[$];
      read(addr, d, r);
      data = d[0]; resp = r[0];
    endtask
  endclass

endpackage
