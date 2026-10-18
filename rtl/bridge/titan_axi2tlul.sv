// titan_axi2tlul — device-side bridge: the fabric's AXI4 master port becomes a
// TL-UL host driving one IP's TL-UL device port.
//
//   AR ─┐                                      ┌─► R
//       ├─► [arbiter] ─► [cmd: beat generator] ─► TL-UL A ...TL-UL D ─► [pend FIFO] ─┤
//   AW ─┘          W ─────────────┘                                                 └─► B
//
// Every IP keeps its TL-UL register/memory port; this bridge sits right in
// front of it, so between modules the traffic is AXI.
//
// Behaviour
// - AR and AW are served one command at a time, round-robin.
// - Bursts are split: one TL-UL access per AXI beat (INCR, FIXED and WRAP).
//   TL-UL requires each beat's address to be aligned to its size, so an
//   unaligned INCR start gets a TL-UL error on that beat -> SLVERR.
// - Integrity: for a single-beat transaction whose AxUSER.intg_vld is set (it
//   came from a TL-UL host through titan_tlul2axi), the original cmd_intg /
//   data_intg are forwarded untouched, so the IP checks the ECC its real
//   master computed.  Otherwise (bursts, or a plain AXI master) integrity is
//   generated here.  Read data and response integrity always travel back in
//   RUSER/BUSER.
// - Not supported, answered with SLVERR without touching the IP: ATOPs, beat
//   size > 32 bits, reserved burst type, illegal WRAP length.  Exclusive
//   accesses are executed as normal ones and answered OKAY (never EXOKAY),
//   which per the AXI spec tells the master the exclusive access failed.

`include "prim_assert.sv"

module titan_axi2tlul
  import tlul_pkg::*;
  import titan_axi_pkg::*;
#(
  // TL-UL beats in flight towards the IP (plus beats being error-answered).
  parameter int unsigned MaxOutstanding = 2
) (
  input  logic      clk_i,
  input  logic      rst_ni,

  input  dev_req_t  axi_req_i,
  output dev_resp_t axi_rsp_o,

  output tl_h2d_t   tl_o,
  input  tl_d2h_t   tl_i
);

  // ---------------------------------------------------------------------------
  // Command register: the AXI transaction currently being split into beats
  // ---------------------------------------------------------------------------

  logic             cmd_q, cmd_is_wr_q, cmd_err_q, cmd_passthru_q, cmd_single_q;
  dev_id_t          cmd_id_q;
  addr_t            cmd_addr_q, cmd_wrap_mask_q;
  axi_pkg::len_t    cmd_left_q;      // beats remaining after the current one
  axi_pkg::size_t   cmd_size_q;
  axi_pkg::burst_t  cmd_burst_q;
  ax_user_t         cmd_user_q;
  logic             cmd_instr_q;

  logic issue, cmd_last_beat;
  assign cmd_last_beat = (cmd_left_q == '0);

  // Arbitration between AR and AW when the command register is free.
  logic prefer_wr_q, grant_wr, grant_rd;
  always_comb begin
    grant_wr = 1'b0;
    grant_rd = 1'b0;
    if (!cmd_q) begin
      if (axi_req_i.aw_valid && axi_req_i.ar_valid) begin
        grant_wr =  prefer_wr_q;
        grant_rd = ~prefer_wr_q;
      end else begin
        grant_wr = axi_req_i.aw_valid;
        grant_rd = axi_req_i.ar_valid;
      end
    end
  end

  // Fields of the granted command, and whether we can execute it at all.
  logic             new_is_wr, new_err;
  dev_id_t          new_id;
  addr_t            new_addr;
  axi_pkg::len_t    new_len;
  axi_pkg::size_t   new_size;
  axi_pkg::burst_t  new_burst;
  ax_user_t         new_user;
  axi_pkg::prot_t   new_prot;
  logic             new_wrap_len_ok;

  always_comb begin
    new_is_wr = grant_wr;
    if (grant_wr) begin
      new_id    = axi_req_i.aw.id;
      new_addr  = axi_req_i.aw.addr;
      new_len   = axi_req_i.aw.len;
      new_size  = axi_req_i.aw.size;
      new_burst = axi_req_i.aw.burst;
      new_user  = axi_req_i.aw.user;
      new_prot  = axi_req_i.aw.prot;
    end else begin
      new_id    = axi_req_i.ar.id;
      new_addr  = axi_req_i.ar.addr;
      new_len   = axi_req_i.ar.len;
      new_size  = axi_req_i.ar.size;
      new_burst = axi_req_i.ar.burst;
      new_user  = axi_req_i.ar.user;
      new_prot  = axi_req_i.ar.prot;
    end
    new_wrap_len_ok = (new_len == 8'd1) || (new_len == 8'd3) ||
                      (new_len == 8'd7) || (new_len == 8'd15);
    new_err = (new_size > axi_pkg::size_t'(2))                              ||
              (new_burst == 2'b11)                                          ||
              (new_burst == axi_pkg::BURST_WRAP && !new_wrap_len_ok)        ||
              (grant_wr && (axi_req_i.aw.atop != '0));
  end

  assign axi_rsp_o.aw_ready = grant_wr;
  assign axi_rsp_o.ar_ready = grant_rd;

  // Next beat address.
  addr_t beat_bytes, incr_addr, next_addr;
  assign beat_bytes = addr_t'(1) << cmd_size_q;
  assign incr_addr  = (cmd_addr_q & ~(beat_bytes - 1)) + beat_bytes;
  always_comb begin
    unique case (cmd_burst_q)
      axi_pkg::BURST_FIXED: next_addr = cmd_addr_q;
      axi_pkg::BURST_WRAP:  next_addr = (cmd_addr_q & ~cmd_wrap_mask_q) |
                                        (incr_addr  &  cmd_wrap_mask_q);
      default:              next_addr = incr_addr;
    endcase
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      cmd_q           <= 1'b0;
      cmd_is_wr_q     <= 1'b0;
      cmd_err_q       <= 1'b0;
      cmd_passthru_q  <= 1'b0;
      cmd_single_q    <= 1'b0;
      cmd_id_q        <= '0;
      cmd_addr_q      <= '0;
      cmd_wrap_mask_q <= '0;
      cmd_left_q      <= '0;
      cmd_size_q      <= '0;
      cmd_burst_q     <= '0;
      cmd_user_q      <= '0;
      cmd_instr_q     <= 1'b0;
      prefer_wr_q     <= 1'b0;
    end else if (grant_wr || grant_rd) begin
      cmd_q           <= 1'b1;
      cmd_is_wr_q     <= new_is_wr;
      cmd_err_q       <= new_err;
      cmd_passthru_q  <= new_user.intg_vld && (new_len == '0);
      cmd_single_q    <= (new_len == '0);
      cmd_id_q        <= new_id;
      cmd_addr_q      <= new_addr;
      cmd_wrap_mask_q <= ((addr_t'(new_len) + 1) << new_size) - 1;
      cmd_left_q      <= new_len;
      cmd_size_q      <= new_size;
      cmd_burst_q     <= new_burst;
      cmd_user_q      <= new_user;
      cmd_instr_q     <= new_prot[2];
      prefer_wr_q     <= ~new_is_wr;
    end else if (issue) begin
      cmd_addr_q      <= next_addr;
      cmd_left_q      <= cmd_left_q - 1'b1;
      if (cmd_last_beat) cmd_q <= 1'b0;
    end
  end

  // ---------------------------------------------------------------------------
  // Beat issue: one TL-UL A request per AXI beat
  // ---------------------------------------------------------------------------

  typedef struct packed {
    logic    is_wr;
    logic    last;      // last beat of its AXI transaction
    logic    fake;      // no TL-UL access was made; answer SLVERR
    logic    single;    // single-beat transaction
    dev_id_t id;
  } pend_t;

  logic  pend_wready, pend_rvalid, pend_pop;
  pend_t pend_wdata, pend_head;

  logic beat_ready;     // the W data a write beat needs is present
  assign beat_ready = cmd_q & pend_wready & (~cmd_is_wr_q | axi_req_i.w_valid);

  // Lanes covered by a beat of the current size at the current address.
  strb_t size_lanes;
  always_comb begin
    unique case (cmd_size_q)
      3'd0:    size_lanes = strb_t'(1) << cmd_addr_q[1:0];
      3'd1:    size_lanes = cmd_addr_q[1] ? 4'b1100 : 4'b0011;
      default: size_lanes = 4'b1111;
    endcase
  end

  tl_h2d_t tl_req;
  always_comb begin
    tl_req           = TL_H2D_DEFAULT;
    tl_req.a_valid   = beat_ready & ~cmd_err_q;
    tl_req.a_address = cmd_addr_q;
    tl_req.a_size    = top_pkg::TL_SZW'(cmd_size_q);
    tl_req.a_source  = '0;   // responses are matched in order, not by source
    tl_req.d_ready   = 1'b0; // driven below by the response side

    if (cmd_is_wr_q) begin
      tl_req.a_mask   = axi_req_i.w.strb;
      tl_req.a_data   = axi_req_i.w.data;
      if (cmd_passthru_q) begin
        tl_req.a_opcode = cmd_user_q.put_full ? PutFullData : PutPartialData;
      end else begin
        tl_req.a_opcode = (axi_req_i.w.strb == size_lanes) ? PutFullData : PutPartialData;
      end
    end else begin
      tl_req.a_opcode = Get;
      tl_req.a_mask   = cmd_passthru_q ? cmd_user_q.mask : size_lanes;
      tl_req.a_data   = BlankedAData;
    end

    if (cmd_passthru_q) begin
      tl_req.a_user.rsvd       = cmd_user_q.rsvd;
      tl_req.a_user.instr_type = cmd_user_q.instr_type;
      tl_req.a_user.cmd_intg   = cmd_user_q.cmd_intg;
      tl_req.a_user.data_intg  = cmd_is_wr_q ? axi_req_i.w.user.data_intg
                                             : get_data_intg(tl_req.a_data);
    end else begin
      tl_req.a_user.rsvd       = '0;
      tl_req.a_user.instr_type = cmd_instr_q ? prim_mubi_pkg::MuBi4True
                                             : prim_mubi_pkg::MuBi4False;
      tl_req.a_user.cmd_intg   = get_cmd_intg(tl_req);
      tl_req.a_user.data_intg  = get_data_intg(tl_req.a_data);
    end
  end

  assign issue = cmd_err_q ? beat_ready : (tl_req.a_valid & tl_i.a_ready);
  assign axi_rsp_o.w_ready = cmd_is_wr_q & issue;

  assign pend_wdata = '{
    is_wr:  cmd_is_wr_q,
    last:   cmd_last_beat,
    fake:   cmd_err_q,
    single: cmd_single_q,
    id:     cmd_id_q
  };

  prim_fifo_sync #(
    .Width ($bits(pend_t)),
    .Pass  (1'b0),
    .Depth (MaxOutstanding)
  ) u_pend_fifo (
    .clk_i,
    .rst_ni,
    .clr_i    (1'b0),
    .wvalid_i (issue),
    .wready_o (pend_wready),
    .wdata_i  (pend_wdata),
    .rvalid_o (pend_rvalid),
    .rready_i (pend_pop),
    .rdata_o  (pend_head),
    .full_o   (),
    .depth_o  (),
    .err_o    ()
  );

  // ---------------------------------------------------------------------------
  // Responses: in order, one TL-UL D per beat; B only on the last write beat
  // ---------------------------------------------------------------------------

  logic beat_valid, beat_err, wr_err_acc_q;
  assign beat_valid = pend_rvalid & (pend_head.fake | tl_i.d_valid);
  assign beat_err   = pend_head.fake | tl_i.d_error;

  logic r_hs, b_hs, wr_absorb;
  assign r_hs      = axi_rsp_o.r_valid & axi_req_i.r_ready;
  assign b_hs      = axi_rsp_o.b_valid & axi_req_i.b_ready;
  assign wr_absorb = beat_valid & pend_head.is_wr & ~pend_head.last;  // no B for this beat
  assign pend_pop  = r_hs | b_hs | wr_absorb;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      wr_err_acc_q <= 1'b0;
    end else if (b_hs) begin
      wr_err_acc_q <= 1'b0;
    end else if (wr_absorb) begin
      wr_err_acc_q <= wr_err_acc_q | beat_err;
    end
  end

  always_comb begin
    tl_o         = tl_req;
    tl_o.d_ready = pend_rvalid & ~pend_head.fake &
                   (pend_head.is_wr ? (~pend_head.last | axi_req_i.b_ready)
                                    : axi_req_i.r_ready);

    axi_rsp_o.r_valid = beat_valid & ~pend_head.is_wr;
    axi_rsp_o.r = '{
      id:   pend_head.id,
      data: pend_head.fake ? DataWhenError : tl_i.d_data,
      resp: beat_err ? axi_pkg::RESP_SLVERR : axi_pkg::RESP_OKAY,
      last: pend_head.last,
      user: '{intg_vld:  ~pend_head.fake,
              rsp_intg:  tl_i.d_user.rsp_intg,
              data_intg: tl_i.d_user.data_intg}
    };

    axi_rsp_o.b_valid = beat_valid & pend_head.is_wr & pend_head.last;
    axi_rsp_o.b = '{
      id:   pend_head.id,
      resp: (beat_err | wr_err_acc_q) ? axi_pkg::RESP_SLVERR : axi_pkg::RESP_OKAY,
      // A merged multi-beat response has no single TL-UL integrity to carry.
      user: '{intg_vld: ~pend_head.fake & pend_head.single,
              rsp_intg: tl_i.d_user.rsp_intg}
    };
  end

  // ---------------------------------------------------------------------------
  // Assertions
  // ---------------------------------------------------------------------------

  `ASSERT(WLastMatchesLen_A,
          axi_rsp_o.w_ready && axi_req_i.w_valid |-> axi_req_i.w.last == cmd_last_beat)
  `ASSERT(NoTlRspWithoutPending_A, tl_i.d_valid |-> pend_rvalid)

endmodule
