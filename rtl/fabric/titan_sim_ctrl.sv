// titan_sim_ctrl — simulation-only AXI slave in the DV window of rv_core_ibex.cfg.
//
// Firmware reports to the testbench by writing here (see firmware/include/titan.h):
//   +0x00 STATUS  test status word (0x4354 in test, 0x900D passed, 0xBAAD failed)
//   +0x04 LOG     one character of log text per write
//   +0x08..+0x1C  scratch
//
// This module only stores the words and answers OKAY. The UVM testbench
// watches this port with a passive AXI monitor and decodes the writes, so the
// pass/fail mechanism is itself an AXI transaction you can see in the waves.
//
// Only instantiated with +define+TITAN_SIM (see titan_axi_fabric).

module titan_sim_ctrl
  import titan_axi_pkg::*;
#(
  parameter int unsigned NumWords = 8
) (
  input  logic      clk_i,
  input  logic      rst_ni,
  input  dev_req_t  axi_req_i,
  output dev_resp_t axi_rsp_o
);

  localparam int unsigned IdxW = $clog2(NumWords);

  logic [31:0] regs_q [NumWords];

  // ---- write path: AW, then W beats, then B -----------------------------------
  typedef enum logic [1:0] {WIdle, WData, WResp} wstate_e;
  wstate_e         wstate_q;
  dev_id_t         wid_q;
  logic [IdxW-1:0] widx_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      wstate_q <= WIdle;
      wid_q    <= '0;
      widx_q   <= '0;
      for (int i = 0; i < NumWords; i++) regs_q[i] <= '0;
    end else begin
      unique case (wstate_q)
        WIdle: if (axi_req_i.aw_valid) begin
          wid_q    <= axi_req_i.aw.id;
          widx_q   <= axi_req_i.aw.addr[2 +: IdxW];
          wstate_q <= WData;
        end
        WData: if (axi_req_i.w_valid) begin
          for (int b = 0; b < 4; b++) begin
            if (axi_req_i.w.strb[b]) regs_q[widx_q][8*b +: 8] <= axi_req_i.w.data[8*b +: 8];
          end
          widx_q <= widx_q + 1'b1;
          if (axi_req_i.w.last) wstate_q <= WResp;
        end
        WResp: if (axi_req_i.b_ready) wstate_q <= WIdle;
        default: wstate_q <= WIdle;
      endcase
    end
  end

  // ---- read path: AR, then len+1 R beats ---------------------------------------
  logic            rbusy_q;
  dev_id_t         rid_q;
  logic [IdxW-1:0] ridx_q;
  axi_pkg::len_t   rleft_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      rbusy_q <= 1'b0;
      rid_q   <= '0;
      ridx_q  <= '0;
      rleft_q <= '0;
    end else if (!rbusy_q) begin
      if (axi_req_i.ar_valid) begin
        rbusy_q <= 1'b1;
        rid_q   <= axi_req_i.ar.id;
        ridx_q  <= axi_req_i.ar.addr[2 +: IdxW];
        rleft_q <= axi_req_i.ar.len;
      end
    end else if (axi_req_i.r_ready) begin
      ridx_q  <= ridx_q + 1'b1;
      rleft_q <= rleft_q - 1'b1;
      if (rleft_q == '0) rbusy_q <= 1'b0;
    end
  end

  always_comb begin
    axi_rsp_o          = '0;
    axi_rsp_o.aw_ready = (wstate_q == WIdle);
    axi_rsp_o.w_ready  = (wstate_q == WData);
    axi_rsp_o.b_valid  = (wstate_q == WResp);
    axi_rsp_o.b.id     = wid_q;
    axi_rsp_o.b.resp   = axi_pkg::RESP_OKAY;
    axi_rsp_o.ar_ready = ~rbusy_q;
    axi_rsp_o.r_valid  = rbusy_q;
    axi_rsp_o.r.id     = rid_q;
    axi_rsp_o.r.data   = regs_q[ridx_q];
    axi_rsp_o.r.resp   = axi_pkg::RESP_OKAY;
    axi_rsp_o.r.last   = (rleft_q == '0);
    // USER stays zero: no TL-UL device behind this slave, so the host bridge
    // generates the response integrity (as it does for DECERR).
  end

endmodule
