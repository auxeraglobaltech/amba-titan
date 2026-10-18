// titan_tlul2axi — host-side bridge: a TL-UL host (Ibex, rv_dm) becomes an
// AXI4 master into the fabric.
//
//   TL-UL A  ──► [holding reg] ──► AW+W   or   AR
//   TL-UL D  ◄── [outstanding FIFO picks the channel] ◄── B   or   R
//
// Three rules make a one-channel, in-order protocol (TL-UL) safe on a
// five-channel protocol (AXI):
//
// 1. One AXI ID per port (always 0).  AXI only orders responses that share an
//    ID; tlul_adapter_host assumes responses come back in issue order and has
//    no way to reorder them.
//
// 2. No read/write mixing in flight.  AR and AW are independent channels, so
//    "store X, then load X" could be reordered.  A request whose direction
//    differs from the ones still outstanding waits until they have all
//    returned (the TL-UL crossbar does the same when switching devices; see
//    tlul_socket_1n's hold_all_requests).
//
// 3. Integrity is carried, not regenerated.  The TL-UL a_user fields ride in
//    AxUSER/WUSER; the device's d_user rides back in RUSER/BUSER.  Only when the
//    response has no device behind it (DECERR from the crossbar's error slave,
//    USER = 0) does this bridge generate the response integrity itself —
//    otherwise Ibex would flag a fatal integrity error instead of an access
//    fault.  Error data matches tlul_err_resp: all-ones for data, zero for
//    instruction fetches.
//
// Bursts: never generated (TL-UL has none).  AxLEN = 0 always.

`include "prim_assert.sv"

module titan_tlul2axi
  import tlul_pkg::*;
  import titan_axi_pkg::*;
#(
  // Must be >= the host's own limit (rv_core_ibex: 8 with the icache, else 2).
  parameter int unsigned MaxOutstanding = 8
) (
  input  logic       clk_i,
  input  logic       rst_ni,

  input  tl_h2d_t    tl_i,
  output tl_d2h_t    tl_o,

  output host_req_t  axi_req_o,
  input  host_resp_t axi_rsp_i
);

  // ---------------------------------------------------------------------------
  // Request side
  // ---------------------------------------------------------------------------

  typedef struct packed {
    logic                     is_wr;
    logic                     instr;
    logic [top_pkg::TL_AIW-1:0] source;
    logic [top_pkg::TL_SZW-1:0] size;
  } pend_t;

  logic    a_is_wr, a_instr;
  assign a_is_wr = (tl_i.a_opcode == PutFullData) || (tl_i.a_opcode == PutPartialData);
  assign a_instr = prim_mubi_pkg::mubi4_test_true_strict(tl_i.a_user.instr_type);

  // Holding register: one accepted TL-UL request, waiting for its AXI handshakes.
  logic    hold_q, hold_is_wr_q, aw_done_q, w_done_q;
  tl_h2d_t a_q;

  logic aw_hs, w_hs, ar_hs, hold_done;
  assign aw_hs = axi_req_o.aw_valid & axi_rsp_i.aw_ready;
  assign w_hs  = axi_req_o.w_valid  & axi_rsp_i.w_ready;
  assign ar_hs = axi_req_o.ar_valid & axi_rsp_i.ar_ready;
  assign hold_done = hold_q & (hold_is_wr_q ? ((aw_done_q | aw_hs) & (w_done_q | w_hs)) : ar_hs);

  // Outstanding FIFO, also the source of truth for "which direction is in flight".
  logic  pend_wready, pend_rvalid, pend_pop;
  pend_t pend_wdata, pend_head;
  logic  dir_is_wr_q;          // direction of everything currently outstanding

  logic  dir_ok, a_ready, a_ack;
  assign dir_ok  = ~pend_rvalid | (dir_is_wr_q == a_is_wr);
  assign a_ready = (~hold_q | hold_done) & pend_wready & dir_ok;
  assign a_ack   = tl_i.a_valid & a_ready;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      hold_q       <= 1'b0;
      hold_is_wr_q <= 1'b0;
      aw_done_q    <= 1'b0;
      w_done_q     <= 1'b0;
      a_q          <= TL_H2D_DEFAULT;
      dir_is_wr_q  <= 1'b0;
    end else begin
      if (a_ack) begin
        hold_q       <= 1'b1;
        hold_is_wr_q <= a_is_wr;
        aw_done_q    <= 1'b0;
        w_done_q     <= 1'b0;
        a_q          <= tl_i;
        dir_is_wr_q  <= a_is_wr;
      end else if (hold_done) begin
        hold_q       <= 1'b0;
      end else begin
        aw_done_q    <= aw_done_q | aw_hs;
        w_done_q     <= w_done_q  | w_hs;
      end
    end
  end

  assign pend_wdata = '{
    is_wr:  a_is_wr,
    instr:  a_instr,
    source: tl_i.a_source,
    size:   tl_i.a_size
  };

  prim_fifo_sync #(
    .Width ($bits(pend_t)),
    .Pass  (1'b0),
    .Depth (MaxOutstanding)
  ) u_pend_fifo (
    .clk_i,
    .rst_ni,
    .clr_i    (1'b0),
    .wvalid_i (a_ack),
    .wready_o (pend_wready),
    .wdata_i  (pend_wdata),
    .rvalid_o (pend_rvalid),
    .rready_i (pend_pop),
    .rdata_o  (pend_head),
    .full_o   (),
    .depth_o  (),
    .err_o    ()
  );

  // TL-UL -> AXI field mapping.  Both AW and AR carry the full a_user.
  ax_user_t ax_user;
  assign ax_user = '{
    intg_vld:   1'b1,
    put_full:   (a_q.a_opcode == PutFullData),
    mask:       a_q.a_mask,
    rsvd:       a_q.a_user.rsvd,
    instr_type: a_q.a_user.instr_type,
    cmd_intg:   a_q.a_user.cmd_intg
  };

  axi_pkg::prot_t prot;
  // AxPROT[2] = instruction access, [1] = non-secure (0: secure), [0] = privileged.
  assign prot = {prim_mubi_pkg::mubi4_test_true_strict(a_q.a_user.instr_type), 1'b0, 1'b0};

  always_comb begin
    axi_req_o = '0;

    axi_req_o.aw = '{
      id:     '0,
      addr:   a_q.a_address,
      len:    '0,
      size:   axi_pkg::size_t'(a_q.a_size),
      burst:  axi_pkg::BURST_INCR,
      lock:   1'b0,
      cache:  '0,      // device, non-bufferable
      prot:   prot,
      qos:    '0,
      region: '0,
      atop:   '0,
      user:   ax_user
    };
    axi_req_o.aw_valid = hold_q & hold_is_wr_q & ~aw_done_q;

    axi_req_o.w = '{
      data: a_q.a_data,
      strb: a_q.a_mask,
      last: 1'b1,
      user: '{data_intg: a_q.a_user.data_intg}
    };
    axi_req_o.w_valid = hold_q & hold_is_wr_q & ~w_done_q;

    axi_req_o.ar = '{
      id:     '0,
      addr:   a_q.a_address,
      len:    '0,
      size:   axi_pkg::size_t'(a_q.a_size),
      burst:  axi_pkg::BURST_INCR,
      lock:   1'b0,
      cache:  '0,
      prot:   prot,
      qos:    '0,
      region: '0,
      user:   ax_user
    };
    axi_req_o.ar_valid = hold_q & ~hold_is_wr_q;

    axi_req_o.b_ready = pend_rvalid &  pend_head.is_wr & tl_i.d_ready;
    axi_req_o.r_ready = pend_rvalid & ~pend_head.is_wr & tl_i.d_ready;
  end

  // ---------------------------------------------------------------------------
  // Response side
  // ---------------------------------------------------------------------------

  logic           rsp_valid, rsp_err, rsp_intg_vld;
  axi_pkg::resp_t rsp_resp;
  data_t          rsp_data;

  assign rsp_valid    = pend_rvalid & (pend_head.is_wr ? axi_rsp_i.b_valid : axi_rsp_i.r_valid);
  assign rsp_resp     = pend_head.is_wr ? axi_rsp_i.b.resp : axi_rsp_i.r.resp;
  assign rsp_intg_vld = pend_head.is_wr ? axi_rsp_i.b.user.intg_vld : axi_rsp_i.r.user.intg_vld;
  assign rsp_err      = rsp_resp[1];   // SLVERR (2'b10) or DECERR (2'b11)
  assign pend_pop     = rsp_valid & tl_i.d_ready;

  always_comb begin
    if (pend_head.is_wr) begin
      rsp_data = '0;
    end else if (!rsp_intg_vld && rsp_err) begin
      rsp_data = pend_head.instr ? DataWhenInstrError : DataWhenError;
    end else begin
      rsp_data = axi_rsp_i.r.data;
    end
  end

  tl_d2h_t d2h_raw, d2h_gen;
  assign d2h_raw = '{
    d_valid:  rsp_valid,
    d_opcode: pend_head.is_wr ? AccessAck : AccessAckData,
    d_param:  '0,
    d_size:   pend_head.size,
    d_source: pend_head.source,
    d_sink:   '0,
    d_data:   rsp_data,
    d_user:   '0,
    d_error:  rsp_err,
    a_ready:  a_ready
  };

  // Locally generated integrity, used only where the fabric carried none.
  tlul_rsp_intg_gen #(
    .EnableRspIntgGen  (1'b1),
    .EnableDataIntgGen (1'b1)
  ) u_rsp_intg_gen (
    .tl_i (d2h_raw),
    .tl_o (d2h_gen)
  );

  always_comb begin
    tl_o = d2h_raw;
    tl_o.d_user.rsp_intg  = rsp_intg_vld ? (pend_head.is_wr ? axi_rsp_i.b.user.rsp_intg
                                                            : axi_rsp_i.r.user.rsp_intg)
                                         : d2h_gen.d_user.rsp_intg;
    // Write responses carry no data on AXI, so their data integrity is always local.
    tl_o.d_user.data_intg = (rsp_intg_vld && !pend_head.is_wr) ? axi_rsp_i.r.user.data_intg
                                                               : d2h_gen.d_user.data_intg;
  end

  // ---------------------------------------------------------------------------
  // Assertions
  // ---------------------------------------------------------------------------

  // Rule 2 holds: the channel that answers is always the one the head expects.
  `ASSERT(NoUnexpectedB_A, axi_rsp_i.b_valid |-> pend_rvalid &&  pend_head.is_wr)
  `ASSERT(NoUnexpectedR_A, axi_rsp_i.r_valid |-> pend_rvalid && !pend_head.is_wr)
  // Single-beat only.
  `ASSERT(RLastAlways_A, axi_rsp_i.r_valid |-> axi_rsp_i.r.last)
  // Outstanding requests all go the same direction.
  `ASSERT(DirConsistent_A, a_ack && pend_rvalid |-> a_is_wr == dir_is_wr_q)

endmodule
