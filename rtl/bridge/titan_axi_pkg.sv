// titan_axi_pkg — AXI4 types used everywhere in the titan-axi fabric.
//
// Two flavours of port exist, differing only in ID width:
//   host_*  : between a bus master (Ibex, rv_dm, the UVM test master) and the
//             fabric.  HostIdWidth bits.
//   dev_*   : between the fabric and an IP.  The PULP crossbar prepends the
//             index of the master port that issued the transaction, so the ID
//             grows by $clog2(number of hosts on xbar_main).
//
// TL-UL integrity travels through the AXI USER signals so that every IP still
// sees, end to end, the ECC its upstream master computed (see AXI_SOC_PLAN.md
// §4.2).  The *_user_t layouts below are that contract between the two
// bridges; nothing else in the fabric looks inside them.

`include "axi/typedef.svh"

package titan_axi_pkg;

  localparam int unsigned AddrWidth = 32;
  localparam int unsigned DataWidth = 32;
  localparam int unsigned StrbWidth = DataWidth / 8;

  // Hosts on xbar_main: corei, cored, rv_dm.sba, and the sim-only UVM master.
  localparam int unsigned NumMainHosts = 4;
  localparam int unsigned HostIdWidth  = 4;
  localparam int unsigned DevIdWidth   = HostIdWidth + $clog2(NumMainHosts);

  typedef logic [AddrWidth-1:0]   addr_t;
  typedef logic [DataWidth-1:0]   data_t;
  typedef logic [StrbWidth-1:0]   strb_t;
  typedef logic [HostIdWidth-1:0] host_id_t;
  typedef logic [DevIdWidth-1:0]  dev_id_t;

  // ---------------------------------------------------------------------------
  // USER signal layouts
  // ---------------------------------------------------------------------------

  // AW and AR.  intg_vld = 1 means cmd_intg (and, for writes, W.user.data_intg)
  // were computed by a TL-UL master and must be passed through unchanged.
  // intg_vld = 0 (e.g. a plain AXI master in the testbench) makes the device
  // bridge generate integrity itself.
  typedef struct packed {
    logic                  intg_vld;
    logic                  put_full;    // AW: TL-UL PutFullData (vs PutPartialData)
    logic [StrbWidth-1:0]  mask;        // TL-UL a_mask; AR has no WSTRB to carry it
    logic [4:0]            rsvd;        // TL-UL a_user.rsvd
    prim_mubi_pkg::mubi4_t instr_type;  // TL-UL a_user.instr_type
    logic [6:0]            cmd_intg;    // TL-UL a_user.cmd_intg
  } ax_user_t;

  typedef struct packed {
    logic [6:0] data_intg;              // TL-UL a_user.data_intg
  } w_user_t;

  // R and B.  intg_vld = 0 is what the crossbar's error slave produces on
  // DECERR (all USER bits zero); the host bridge then generates integrity.
  typedef struct packed {
    logic       intg_vld;
    logic [6:0] rsp_intg;               // TL-UL d_user.rsp_intg
    logic [6:0] data_intg;              // TL-UL d_user.data_intg
  } r_user_t;

  typedef struct packed {
    logic       intg_vld;
    logic [6:0] rsp_intg;
  } b_user_t;

  // ---------------------------------------------------------------------------
  // Channel and request/response structs (PULP axi conventions)
  // ---------------------------------------------------------------------------

  `AXI_TYPEDEF_AW_CHAN_T(host_aw_chan_t, addr_t, host_id_t, ax_user_t)
  `AXI_TYPEDEF_AR_CHAN_T(host_ar_chan_t, addr_t, host_id_t, ax_user_t)
  `AXI_TYPEDEF_B_CHAN_T (host_b_chan_t,  host_id_t, b_user_t)
  `AXI_TYPEDEF_R_CHAN_T (host_r_chan_t,  data_t, host_id_t, r_user_t)

  `AXI_TYPEDEF_AW_CHAN_T(dev_aw_chan_t,  addr_t, dev_id_t, ax_user_t)
  `AXI_TYPEDEF_AR_CHAN_T(dev_ar_chan_t,  addr_t, dev_id_t, ax_user_t)
  `AXI_TYPEDEF_B_CHAN_T (dev_b_chan_t,   dev_id_t, b_user_t)
  `AXI_TYPEDEF_R_CHAN_T (dev_r_chan_t,   data_t, dev_id_t, r_user_t)

  `AXI_TYPEDEF_W_CHAN_T (w_chan_t, data_t, strb_t, w_user_t)

  `AXI_TYPEDEF_REQ_T (host_req_t,  host_aw_chan_t, w_chan_t, host_ar_chan_t)
  `AXI_TYPEDEF_RESP_T(host_resp_t, host_b_chan_t,  host_r_chan_t)
  `AXI_TYPEDEF_REQ_T (dev_req_t,   dev_aw_chan_t,  w_chan_t, dev_ar_chan_t)
  `AXI_TYPEDEF_RESP_T(dev_resp_t,  dev_b_chan_t,   dev_r_chan_t)

  // Idle values for unused ports.
  localparam host_req_t  HOST_REQ_IDLE  = '0;
  localparam dev_resp_t  DEV_RESP_IDLE  = '0;

endpackage
