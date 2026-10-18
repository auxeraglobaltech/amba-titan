// axi_if — one AXI4 port, as plain signals.
//
// Every port of the fabric gets one of these (uvm/tb/titan_axi_probe.sv binds
// them in and connects them). Monitors only read it; on the sim-only master
// port the driver writes the master-side signals.
//
// ID is 6 bits everywhere: device-side ports really are 6 bits ({host index,
// host ID}); host-side ports use the low 4.
//
// The assertions are the AXI handshake rules a trainee most often breaks.

interface axi_if #(
  parameter int unsigned ID_W   = 6,
  parameter int unsigned ADDR_W = 32,
  parameter int unsigned DATA_W = 32
) (
  input logic aclk,
  input logic aresetn
);
  localparam int unsigned STRB_W = DATA_W / 8;

  // AW
  logic [ID_W-1:0]   awid;
  logic [ADDR_W-1:0] awaddr;
  logic [7:0]        awlen;
  logic [2:0]        awsize;
  logic [1:0]        awburst;
  logic [2:0]        awprot;
  logic              awvalid;
  logic              awready;
  // W
  logic [DATA_W-1:0] wdata;
  logic [STRB_W-1:0] wstrb;
  logic              wlast;
  logic              wvalid;
  logic              wready;
  // B
  logic [ID_W-1:0]   bid;
  logic [1:0]        bresp;
  logic              bvalid;
  logic              bready;
  // AR
  logic [ID_W-1:0]   arid;
  logic [ADDR_W-1:0] araddr;
  logic [7:0]        arlen;
  logic [2:0]        arsize;
  logic [1:0]        arburst;
  logic [2:0]        arprot;
  logic              arvalid;
  logic              arready;
  // R
  logic [ID_W-1:0]   rid;
  logic [DATA_W-1:0] rdata;
  logic [1:0]        rresp;
  logic              rlast;
  logic              rvalid;
  logic              rready;

  // Sampling for monitors: values just before the clock edge.
  clocking mon_cb @(posedge aclk);
    default input #1step;
    input awid, awaddr, awlen, awsize, awburst, awprot, awvalid, awready;
    input wdata, wstrb, wlast, wvalid, wready;
    input bid, bresp, bvalid, bready;
    input arid, araddr, arlen, arsize, arburst, arprot, arvalid, arready;
    input rid, rdata, rresp, rlast, rvalid, rready;
  endclocking

  // No output clocking block on purpose: on monitored ports the probe drives
  // every signal with continuous assignments, which a clocking output would
  // forbid. The master driver (sim-only port) writes signals with nonblocking
  // assignments right after a mon_cb edge and samples READY through mon_cb.

  // --------------------------------------------------------------------------
  // Protocol checks (AXI4 spec A3.2.1: once VALID is high it stays high, with
  // stable payload, until the handshake)
  // --------------------------------------------------------------------------
`ifndef AXI_IF_NO_ASSERTIONS
  `define AXI_STABLE(name, valid, ready, payload)                                    \
    name: assert property (@(posedge aclk) disable iff (!aresetn)                    \
      (valid && !ready) |=> (valid && $stable(payload)))                             \
      else $error("%m: AXI rule broken: %s dropped or changed before handshake", `"valid`");

  `AXI_STABLE(aw_stable_a, awvalid, awready, {awid, awaddr, awlen, awsize, awburst, awprot})
  `AXI_STABLE(w_stable_a,  wvalid,  wready,  {wdata, wstrb, wlast})
  `AXI_STABLE(b_stable_a,  bvalid,  bready,  {bid, bresp})
  `AXI_STABLE(ar_stable_a, arvalid, arready, {arid, araddr, arlen, arsize, arburst, arprot})
  `AXI_STABLE(r_stable_a,  rvalid,  rready,  {rid, rdata, rresp, rlast})
  `undef AXI_STABLE

  valid_known_a: assert property (@(posedge aclk) disable iff (!aresetn)
    !$isunknown({awvalid, wvalid, bvalid, arvalid, rvalid}))
    else $error("%m: X on a VALID signal");
`endif

endinterface
