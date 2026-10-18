// tb_bridge — standalone proof of the TL-UL <-> AXI bridges (plan phase 1).
//
// The same chain the chip uses, minus the chip:
//
//   host0: Ibex-like req/gnt ─► tlul_adapter_host ─► titan_tlul2axi ─┐
//   host1: plain AXI master (bursts, no integrity) ─► axi_atop_filter ┤
//   host2/3: idle                                                    ├─► axi_xbar ─┬─► titan_axi2tlul ─► tlul_adapter_sram ─► mem0  (clk)
//                                                                    ┘             └─► axi_cdc ─► titan_axi2tlul ─► tlul_adapter_sram ─► mem1 (clk_slow, 30% stalls)
//
// tlul_adapter_sram has command-integrity checking on, exactly like the real
// IPs, so any mistake in carrying cmd_intg/data_intg through AXI shows up as
// intg_error_o.  Every read response on host0 is checked for data integrity
// the way SecureIbex checks it.
//
// Checks
//   C1  host0 random reads/writes to both memories match a reference model
//   C2  host0 store-then-load to the same address returns the new data
//   C3  unmapped addresses: host0 sees err_o + all-ones data, host1 sees DECERR
//   C4  host1 INCR / FIXED / WRAP bursts and narrow (8/16-bit) beats
//   C5  host1 atomic operation answered SLVERR
//   C6  no integrity error anywhere, ever (host adapter, both device adapters)
//   C7  host0 response data integrity is valid on every read, including errors
//
// Pass: prints "BRIDGE TB PASSED".  Run: uvm/bridge_tb/run.sh

`timescale 1ns/1ps

module tb_bridge;
  import tlul_pkg::*;
  import titan_axi_pkg::*;

  localparam int unsigned NumHosts = NumMainHosts;     // 4, as on xbar_main
  localparam addr_t       Mem0Base = 32'h1000_0000;
  localparam addr_t       Mem1Base = 32'h2000_0000;
  localparam addr_t       MemSize  = 32'h0000_1000;     // 4 KiB = 1024 words
  localparam addr_t       Unmapped = 32'h3000_0000;
  localparam int unsigned H0Ops    = 600;

  int errors = 0;
  `define CHECK(cond, msg) \
    if (!(cond)) begin errors++; $error("[%0t] %s", $time, msg); end

  // ---------------------------------------------------------------------------
  // Clocks and reset
  // ---------------------------------------------------------------------------
  logic clk = 1'b0, clk_slow = 1'b0, rst_n = 1'b0;
  always #5  clk      = ~clk;        // 100 MHz, like clk_main
  always #17 clk_slow = ~clk_slow;   // ~29 MHz, an unrelated clock for the CDC
  initial begin
    repeat (10) @(posedge clk);
    rst_n <= 1'b1;
  end

  // ---------------------------------------------------------------------------
  // Host 0: Ibex-style request interface -> TL-UL -> AXI
  // ---------------------------------------------------------------------------
  logic        h0_req = 1'b0, h0_we = 1'b0;
  logic [31:0] h0_addr = '0, h0_wdata = '0;
  logic [3:0]  h0_be = '0;
  logic        h0_gnt, h0_valid, h0_err, h0_intg_err;
  logic [31:0] h0_rdata;
  logic [6:0]  h0_rdata_intg;
  tl_h2d_t     h0_tl_h2d;
  tl_d2h_t     h0_tl_d2h;

  host_req_t  [NumHosts-1:0] host_req;
  host_resp_t [NumHosts-1:0] host_rsp;

  tlul_adapter_host #(
    .MAX_REQS          (2),
    .EnableDataIntgGen (1)
  ) u_h0_adapter (
    .clk_i        (clk),
    .rst_ni       (rst_n),
    .req_i        (h0_req),
    .instr_type_i (prim_mubi_pkg::MuBi4False),
    .gnt_o        (h0_gnt),
    .addr_i       (h0_addr),
    .we_i         (h0_we),
    .wdata_i      (h0_wdata),
    .wdata_intg_i ('0),
    .be_i         (h0_be),
    .user_rsvd_i  ('0),
    .valid_o      (h0_valid),
    .rdata_o      (h0_rdata),
    .rdata_intg_o (h0_rdata_intg),
    .err_o        (h0_err),
    .intg_err_o   (h0_intg_err),
    .tl_o         (h0_tl_h2d),
    .tl_i         (h0_tl_d2h)
  );

  titan_tlul2axi #(.MaxOutstanding(2)) u_h0_bridge (
    .clk_i     (clk),
    .rst_ni    (rst_n),
    .tl_i      (h0_tl_h2d),
    .tl_o      (h0_tl_d2h),
    .axi_req_o (host_req[0]),
    .axi_rsp_i (host_rsp[0])
  );

  // ---------------------------------------------------------------------------
  // Host 1: plain AXI master driven by tasks below
  // ---------------------------------------------------------------------------
  host_req_t  h1_req = '0;
  host_resp_t h1_rsp;

  axi_atop_filter #(
    .AxiIdWidth      (HostIdWidth),
    .AxiMaxWriteTxns (4),
    .axi_req_t       (host_req_t),
    .axi_resp_t      (host_resp_t)
  ) u_h1_atop_filter (
    .clk_i      (clk),
    .rst_ni     (rst_n),
    .slv_req_i  (h1_req),
    .slv_resp_o (h1_rsp),
    .mst_req_o  (host_req[1]),
    .mst_resp_i (host_rsp[1])
  );

  assign host_req[2] = HOST_REQ_IDLE;
  assign host_req[3] = HOST_REQ_IDLE;

  // ---------------------------------------------------------------------------
  // Crossbar
  // ---------------------------------------------------------------------------
  localparam axi_pkg::xbar_cfg_t XbarCfg = '{
    NoSlvPorts:         NumHosts,
    NoMstPorts:         2,
    MaxMstTrans:        8,
    MaxSlvTrans:        8,
    FallThrough:        1'b0,
    LatencyMode:        axi_pkg::CUT_ALL_AX,
    PipelineStages:     0,
    AxiIdWidthSlvPorts: HostIdWidth,
    AxiIdUsedSlvPorts:  HostIdWidth,
    UniqueIds:          1'b0,
    AxiAddrWidth:       AddrWidth,
    AxiDataWidth:       DataWidth,
    NoAddrRules:        2
  };

  axi_pkg::xbar_rule_32_t [1:0] addr_map;
  assign addr_map[0] = '{idx: 32'd0, start_addr: Mem0Base, end_addr: Mem0Base + MemSize};
  assign addr_map[1] = '{idx: 32'd1, start_addr: Mem1Base, end_addr: Mem1Base + MemSize};

  dev_req_t  [1:0] dev_req;
  dev_resp_t [1:0] dev_rsp;

  axi_xbar #(
    .Cfg           (XbarCfg),
    .ATOPs         (1'b0),
    .slv_aw_chan_t (host_aw_chan_t),
    .mst_aw_chan_t (dev_aw_chan_t),
    .w_chan_t      (w_chan_t),
    .slv_b_chan_t  (host_b_chan_t),
    .mst_b_chan_t  (dev_b_chan_t),
    .slv_ar_chan_t (host_ar_chan_t),
    .mst_ar_chan_t (dev_ar_chan_t),
    .slv_r_chan_t  (host_r_chan_t),
    .mst_r_chan_t  (dev_r_chan_t),
    .slv_req_t     (host_req_t),
    .slv_resp_t    (host_resp_t),
    .mst_req_t     (dev_req_t),
    .mst_resp_t    (dev_resp_t),
    .rule_t        (axi_pkg::xbar_rule_32_t)
  ) u_xbar (
    .clk_i                 (clk),
    .rst_ni                (rst_n),
    .test_i                (1'b0),
    .slv_ports_req_i       (host_req),
    .slv_ports_resp_o      (host_rsp),
    .mst_ports_req_o       (dev_req),
    .mst_ports_resp_i      (dev_rsp),
    .addr_map_i            (addr_map),
    .en_default_mst_port_i ('0),
    .default_mst_port_i    ('0)
  );

  // ---------------------------------------------------------------------------
  // Devices
  // ---------------------------------------------------------------------------
  logic [1:0] dev_intg_err;

  tb_mem_dev #(.StallPct(0)) u_mem0 (
    .clk_i        (clk),
    .rst_ni       (rst_n),
    .axi_req_i    (dev_req[0]),
    .axi_rsp_o    (dev_rsp[0]),
    .intg_error_o (dev_intg_err[0])
  );

  dev_req_t  mem1_req;
  dev_resp_t mem1_rsp;

  axi_cdc #(
    .aw_chan_t  (dev_aw_chan_t),
    .w_chan_t   (w_chan_t),
    .b_chan_t   (dev_b_chan_t),
    .ar_chan_t  (dev_ar_chan_t),
    .r_chan_t   (dev_r_chan_t),
    .axi_req_t  (dev_req_t),
    .axi_resp_t (dev_resp_t),
    .LogDepth   (2)
  ) u_cdc (
    .src_clk_i  (clk),
    .src_rst_ni (rst_n),
    .src_req_i  (dev_req[1]),
    .src_resp_o (dev_rsp[1]),
    .dst_clk_i  (clk_slow),
    .dst_rst_ni (rst_n),
    .dst_req_o  (mem1_req),
    .dst_resp_i (mem1_rsp)
  );

  tb_mem_dev #(.StallPct(30)) u_mem1 (
    .clk_i        (clk_slow),
    .rst_ni       (rst_n),
    .axi_req_i    (mem1_req),
    .axi_rsp_o    (mem1_rsp),
    .intg_error_o (dev_intg_err[1])
  );

  // ---------------------------------------------------------------------------
  // Reference model (same initial contents as tb_sram)
  // ---------------------------------------------------------------------------
  logic [31:0] model [2][1024];
  initial for (int m = 0; m < 2; m++) for (int i = 0; i < 1024; i++) model[m][i] = 32'hDEAD_0000 | i;

  function automatic int region(addr_t a);
    if (a >= Mem0Base && a < Mem0Base + MemSize) return 0;
    if (a >= Mem1Base && a < Mem1Base + MemSize) return 1;
    return -1;
  endfunction

  function automatic void model_write(addr_t a, logic [31:0] data, logic [3:0] strb);
    int m = region(a);
    for (int b = 0; b < 4; b++)
      if (strb[b]) model[m][a[11:2]][8*b +: 8] = data[8*b +: 8];
  endfunction

  function automatic logic [31:0] model_read(addr_t a);
    return model[region(a)][a[11:2]];
  endfunction

  // C6: integrity errors anywhere.
  always @(posedge clk) if (rst_n) begin
    `CHECK(!h0_intg_err, "C6: host0 adapter flagged a response integrity error")
    `CHECK(!dev_intg_err[0], "C6: mem0 adapter flagged a command integrity error")
  end
  always @(posedge clk_slow) if (rst_n) begin
    `CHECK(!dev_intg_err[1], "C6: mem1 adapter flagged a command integrity error")
  end

  // ---------------------------------------------------------------------------
  // Host 0 driver + checker
  // ---------------------------------------------------------------------------
  clocking h0_cb @(posedge clk);
    default input #1step output #1;
    output h0_req, h0_we, h0_addr, h0_wdata, h0_be;
    input  h0_gnt, h0_valid, h0_rdata, h0_rdata_intg, h0_err;
  endclocking

  typedef struct {
    logic        we;
    logic        err;
    logic [31:0] data;
    addr_t       addr;
  } h0_exp_t;

  h0_exp_t h0_exp_q[$];
  int      h0_outstanding = 0;
  int      h0_reads = 0, h0_writes = 0, h0_errs = 0;

  task automatic h0_access(logic we, addr_t addr, logic [31:0] wdata, logic [3:0] be);
    h0_exp_t e;
    while (h0_outstanding >= 2) @(h0_cb);
    h0_cb.h0_req   <= 1'b1;
    h0_cb.h0_we    <= we;
    h0_cb.h0_addr  <= addr;
    h0_cb.h0_wdata <= wdata;
    h0_cb.h0_be    <= we ? be : 4'hF;
    do @(h0_cb); while (!h0_cb.h0_gnt);
    h0_cb.h0_req   <= 1'b0;
    // Granted: requests are ordered, so the model is updated in grant order.
    e.we = we; e.addr = addr; e.err = (region(addr) < 0);
    if (e.err) begin
      e.data = DataWhenError;
    end else if (we) begin
      model_write(addr, wdata, be);
    end else begin
      e.data = model_read(addr);
    end
    h0_exp_q.push_back(e);
    h0_outstanding++;
  endtask

  always @(h0_cb) if (h0_cb.h0_valid) begin
    h0_exp_t e;
    if (h0_exp_q.size() == 0) begin
      `CHECK(0, "host0: response with nothing outstanding")
    end else begin
      e = h0_exp_q.pop_front();
      h0_outstanding--;
      `CHECK(h0_cb.h0_err == e.err,
             $sformatf("C1/C3: host0 %s 0x%08x err=%0b, expected %0b",
                       e.we ? "write" : "read", e.addr, h0_cb.h0_err, e.err))
      if (!e.we) begin
        `CHECK(h0_cb.h0_rdata == e.data,
               $sformatf("C1/C2/C3: host0 read 0x%08x = 0x%08x, expected 0x%08x",
                         e.addr, h0_cb.h0_rdata, e.data))
        `CHECK(h0_cb.h0_rdata_intg == get_data_intg(h0_cb.h0_rdata),
               $sformatf("C7: host0 read 0x%08x: bad data integrity", e.addr))
      end
      if (e.err) h0_errs++; else if (e.we) h0_writes++; else h0_reads++;
    end
  end

  // Host 0 owns the lower half of each memory, host 1 the upper half.
  function automatic addr_t h0_rand_addr();
    int r = $urandom_range(99);
    if (r < 5) return Unmapped + ($urandom_range(255) << 2);
    return (r < 52 ? Mem0Base : Mem1Base) + ($urandom_range(511) << 2);
  endfunction

  task automatic host0_random();
    for (int i = 0; i < H0Ops; i++) begin
      logic [3:0] be;
      do be = $urandom(); while (be == '0);
      h0_access($urandom_range(1), h0_rand_addr(), $urandom(), be);
    end
  endtask

  task automatic host0_store_load();   // C2
    for (int i = 0; i < 50; i++) begin
      addr_t a = h0_rand_addr();
      if (region(a) < 0) continue;
      h0_access(1'b1, a, $urandom(), 4'hF);
      h0_access(1'b0, a, '0, 4'hF);    // issued while the store may still be in flight
    end
  endtask

  // ---------------------------------------------------------------------------
  // Host 1 AXI master tasks (one transaction at a time; B and R always ready)
  // ---------------------------------------------------------------------------
  clocking h1_cb @(posedge clk);
    default input #1step output #1;
    output h1_req;
    input  h1_rsp;
  endclocking

  host_req_t h1_drv = '0;
  int        h1_txns = 0;

  function automatic addr_t beat_addr(addr_t start, int beat, int size, axi_pkg::burst_t burst, int len);
    // Straight from the AXI spec (A3.4.2), independent of the bridge's implementation.
    int    nbytes  = 1 << size;
    addr_t aligned = (start / nbytes) * nbytes;
    if (burst == axi_pkg::BURST_FIXED) return start;
    if (beat == 0) return start;
    if (burst == axi_pkg::BURST_INCR) return aligned + beat * nbytes;
    begin  // WRAP
      int    total    = nbytes * (len + 1);
      addr_t boundary = (start / total) * total;
      addr_t a        = aligned + beat * nbytes;
      if (a >= boundary + total) a -= total;
      return a;
    end
  endfunction

  task automatic h1_write(addr_t addr, int len, int size, axi_pkg::burst_t burst,
                          logic [31:0] data[$], logic [3:0] strb[$],
                          axi_pkg::atop_t atop, output axi_pkg::resp_t resp);
    host_id_t id = $urandom();
    h1_drv.b_ready  = 1'b1;
    h1_drv.r_ready  = 1'b1;
    h1_drv.aw       = '0;
    h1_drv.aw.id    = id;
    h1_drv.aw.addr  = addr;
    h1_drv.aw.len   = len;
    h1_drv.aw.size  = size;
    h1_drv.aw.burst = burst;
    h1_drv.aw.atop  = atop;
    h1_drv.aw_valid = 1'b1;
    h1_cb.h1_req <= h1_drv;
    do @(h1_cb); while (!h1_cb.h1_rsp.aw_ready);
    h1_drv.aw_valid = 1'b0;
    for (int b = 0; b <= len; b++) begin
      h1_drv.w       = '0;
      h1_drv.w.data  = data[b];
      h1_drv.w.strb  = strb[b];
      h1_drv.w.last  = (b == len);
      h1_drv.w_valid = 1'b1;
      h1_cb.h1_req <= h1_drv;
      do @(h1_cb); while (!h1_cb.h1_rsp.w_ready);
    end
    h1_drv.w_valid = 1'b0;
    h1_cb.h1_req <= h1_drv;
    while (!h1_cb.h1_rsp.b_valid) @(h1_cb);
    resp = h1_cb.h1_rsp.b.resp;
    `CHECK(h1_cb.h1_rsp.b.id == id, "host1: B id mismatch")
    h1_txns++;
  endtask

  task automatic h1_read(addr_t addr, int len, int size, axi_pkg::burst_t burst,
                         output logic [31:0] data[$], output axi_pkg::resp_t resp[$]);
    host_id_t id = $urandom();
    data = {}; resp = {};
    h1_drv.b_ready  = 1'b1;
    h1_drv.r_ready  = 1'b1;
    h1_drv.ar       = '0;
    h1_drv.ar.id    = id;
    h1_drv.ar.addr  = addr;
    h1_drv.ar.len   = len;
    h1_drv.ar.size  = size;
    h1_drv.ar.burst = burst;
    h1_drv.ar_valid = 1'b1;
    h1_cb.h1_req <= h1_drv;
    do @(h1_cb); while (!h1_cb.h1_rsp.ar_ready);
    h1_drv.ar_valid = 1'b0;
    h1_cb.h1_req <= h1_drv;
    // The R beat can be sampled on the same edge as the AR handshake at the earliest.
    forever begin
      if (h1_cb.h1_rsp.r_valid) begin
        data.push_back(h1_cb.h1_rsp.r.data);
        resp.push_back(h1_cb.h1_rsp.r.resp);
        `CHECK(h1_cb.h1_rsp.r.id == id, "host1: R id mismatch")
        `CHECK(h1_cb.h1_rsp.r.last == (data.size() == len + 1),
               $sformatf("C4: host1 RLAST wrong on beat %0d of %0d", data.size() - 1, len + 1))
        if (h1_cb.h1_rsp.r.last) break;
      end
      @(h1_cb);
    end
    h1_txns++;
  endtask

  // Write a burst, read it back the same way, compare against the model.
  task automatic h1_burst_check(addr_t addr, int len, int size, axi_pkg::burst_t burst);
    logic [31:0]    wdata[$], rdata[$];
    logic [3:0]     strb[$];
    axi_pkg::resp_t bresp, rresp[$];
    for (int b = 0; b <= len; b++) begin
      addr_t a = beat_addr(addr, b, size, burst, len);
      logic [3:0] lanes = ((1 << (1 << size)) - 1) << a[1:0];
      wdata.push_back($urandom());
      strb.push_back(lanes);
    end
    h1_write(addr, len, size, burst, wdata, strb, '0, bresp);
    `CHECK(bresp == axi_pkg::RESP_OKAY,
           $sformatf("C4: host1 write burst @0x%08x len %0d size %0d burst %0d: resp %0d",
                     addr, len, size, burst, bresp))
    for (int b = 0; b <= len; b++) begin
      model_write(beat_addr(addr, b, size, burst, len), wdata[b], strb[b]);
    end
    h1_read(addr, len, size, burst, rdata, rresp);
    for (int b = 0; b <= len; b++) begin
      addr_t       a     = beat_addr(addr, b, size, burst, len);
      logic [31:0] lmask = '0;
      for (int l = 0; l < 4; l++) if (strb[b][l]) lmask[8*l +: 8] = 8'hFF;
      `CHECK(rresp[b] == axi_pkg::RESP_OKAY,
             $sformatf("C4: host1 read beat %0d @0x%08x resp %0d", b, a, rresp[b]))
      `CHECK((rdata[b] & lmask) == (model_read(a) & lmask),
             $sformatf("C4: host1 read beat %0d @0x%08x = 0x%08x, expected 0x%08x (lanes %b)",
                       b, a, rdata[b], model_read(a), strb[b]))
    end
  endtask

  task automatic host1_sequence();
    logic [31:0]    rdata[$];
    axi_pkg::resp_t bresp, rresp[$];
    // C4: INCR bursts of every length 1..16 beats, alternating memories (upper halves).
    for (int len = 0; len < 16; len++) begin
      addr_t base = ((len % 2) ? Mem1Base : Mem0Base) + 32'h800 + (len << 6);
      h1_burst_check(base, len, 2, axi_pkg::BURST_INCR);
    end
    // C4: WRAP bursts of each legal length, starting mid-block.
    h1_burst_check(Mem0Base + 32'hC04, 1,  2, axi_pkg::BURST_WRAP);
    h1_burst_check(Mem1Base + 32'hC18, 3,  2, axi_pkg::BURST_WRAP);
    h1_burst_check(Mem0Base + 32'hD14, 7,  2, axi_pkg::BURST_WRAP);
    h1_burst_check(Mem1Base + 32'hE3C, 15, 2, axi_pkg::BURST_WRAP);
    // C4: FIXED burst (all beats to one address; the last write wins).
    h1_burst_check(Mem0Base + 32'hF00, 3,  2, axi_pkg::BURST_FIXED);
    // C4: narrow beats — bytes and halfwords at every legal lane.
    for (int off = 0; off < 4; off++) h1_burst_check(Mem1Base + 32'hF80 + off, 0, 0, axi_pkg::BURST_INCR);
    for (int off = 0; off < 4; off += 2) h1_burst_check(Mem0Base + 32'hF90 + off, 0, 1, axi_pkg::BURST_INCR);
    // C4: narrow INCR burst crossing words.
    h1_burst_check(Mem0Base + 32'hFA0, 7, 0, axi_pkg::BURST_INCR);
    // C3: unmapped, read burst and write.
    h1_read(Unmapped, 3, 2, axi_pkg::BURST_INCR, rdata, rresp);
    foreach (rresp[b]) `CHECK(rresp[b] == axi_pkg::RESP_DECERR, "C3: host1 unmapped read not DECERR")
    h1_write(Unmapped, 0, 2, axi_pkg::BURST_INCR, '{32'h0}, '{4'hF}, '0, bresp);
    `CHECK(bresp == axi_pkg::RESP_DECERR, "C3: host1 unmapped write not DECERR")
    // C5: an atomic store is filtered with SLVERR and must not reach memory.
    h1_write(Mem0Base + 32'hFF0, 0, 2, axi_pkg::BURST_INCR, '{32'h1234_5678}, '{4'hF},
             {axi_pkg::ATOP_ATOMICSTORE, axi_pkg::ATOP_LITTLE_END, axi_pkg::ATOP_ADD}, bresp);
    `CHECK(bresp == axi_pkg::RESP_SLVERR, "C5: host1 atomic store not SLVERR")
    h1_read(Mem0Base + 32'hFF0, 0, 2, axi_pkg::BURST_INCR, rdata, rresp);
    `CHECK(rdata[0] == model_read(Mem0Base + 32'hFF0), "C5: atomic store modified memory")
  endtask

  // ---------------------------------------------------------------------------
  // Test sequence
  // ---------------------------------------------------------------------------
  initial begin
    wait (rst_n);
    repeat (5) @(posedge clk);
    fork
      host0_random();
      host1_sequence();
    join
    host0_store_load();
    wait (h0_outstanding == 0);
    repeat (20) @(posedge clk);

    `CHECK(h0_exp_q.size() == 0, "host0: responses missing at end of test")
    $display("--------------------------------------------------------------");
    $display("host0: %0d reads, %0d writes, %0d error responses", h0_reads, h0_writes, h0_errs);
    $display("host1: %0d AXI transactions", h1_txns);
    if (errors == 0) $display("BRIDGE TB PASSED");
    else             $display("BRIDGE TB FAILED: %0d errors", errors);
    $display("--------------------------------------------------------------");
    $finish;
  end

  initial begin
    #2ms;
    $display("BRIDGE TB FAILED: timeout (outstanding host0 = %0d)", h0_outstanding);
    $finish;
  end

endmodule


// One memory device: AXI -> titan_axi2tlul -> tlul_adapter_sram -> tb_sram.
module tb_mem_dev
  import tlul_pkg::*;
  import titan_axi_pkg::*;
#(
  parameter int unsigned StallPct = 0
) (
  input  logic      clk_i,
  input  logic      rst_ni,
  input  dev_req_t  axi_req_i,
  output dev_resp_t axi_rsp_o,
  output logic      intg_error_o
);
  tl_h2d_t     tl_h2d;
  tl_d2h_t     tl_d2h;
  logic        req, gnt, we, rvalid;
  logic [9:0]  addr;
  logic [31:0] wdata, wmask, rdata;

  titan_axi2tlul #(.MaxOutstanding(2)) u_bridge (
    .clk_i,
    .rst_ni,
    .axi_req_i,
    .axi_rsp_o,
    .tl_o (tl_h2d),
    .tl_i (tl_d2h)
  );

  tlul_adapter_sram #(
    .SramAw           (10),
    .SramDw           (32),
    .Outstanding      (2),
    .ByteAccess       (1),
    .CmdIntgCheck     (1),
    .EnableRspIntgGen (1),
    .EnableDataIntgGen(1)
  ) u_adapter (
    .clk_i,
    .rst_ni,
    .tl_i                       (tl_h2d),
    .tl_o                       (tl_d2h),
    .en_ifetch_i                (prim_mubi_pkg::MuBi4True),
    .req_o                      (req),
    .req_type_o                 (),
    .gnt_i                      (gnt),
    .we_o                       (we),
    .addr_o                     (addr),
    .wdata_o                    (wdata),
    .wmask_o                    (wmask),
    .intg_error_o,
    .user_rsvd_o                (),
    .rdata_i                    (rdata),
    .rvalid_i                   (rvalid),
    .rerror_i                   ('0),
    .compound_txn_in_progress_o (),
    .readback_en_i              (prim_mubi_pkg::MuBi4False),
    .readback_error_o           (),
    .wr_collision_i             (1'b0),
    .write_pending_i            (1'b0)
  );

  tb_sram #(.AddrW(10), .StallPct(StallPct)) u_sram (
    .clk_i,
    .rst_ni,
    .req_i    (req),
    .gnt_o    (gnt),
    .we_i     (we),
    .addr_i   (addr),
    .wdata_i  (wdata),
    .wmask_i  (wmask),
    .rdata_o  (rdata),
    .rvalid_o (rvalid)
  );
endmodule
