// tb_sram — behavioural single-port SRAM for the bridge testbench.
//
// Speaks the req/gnt/rvalid interface of tlul_adapter_sram.  With
// StallPct > 0 it randomly withholds the grant, which back-pressures the
// TL-UL adapter, the bridge and finally the AXI crossbar.

module tb_sram #(
  parameter int unsigned AddrW    = 10,
  parameter int unsigned StallPct = 0
) (
  input  logic             clk_i,
  input  logic             rst_ni,
  input  logic             req_i,
  output logic             gnt_o,
  input  logic             we_i,
  input  logic [AddrW-1:0] addr_i,
  input  logic [31:0]      wdata_i,
  input  logic [31:0]      wmask_i,   // bit-level
  output logic [31:0]      rdata_o,
  output logic             rvalid_o
);

  logic [31:0] mem [2**AddrW];

  initial begin
    // Known contents so a read of an unwritten word is deterministic.
    for (int i = 0; i < 2**AddrW; i++) mem[i] = 32'hDEAD_0000 | i;
  end

  logic stall;
  always_ff @(posedge clk_i) stall <= ($urandom_range(99) < StallPct);
  assign gnt_o = req_i & ~stall;

  always @(posedge clk_i or negedge rst_ni) begin   // not always_ff: mem is also preloaded by the initial block
    if (!rst_ni) begin
      rvalid_o <= 1'b0;
      rdata_o  <= '0;
    end else begin
      rvalid_o <= gnt_o & ~we_i;
      if (gnt_o) begin
        if (we_i) mem[addr_i] <= (mem[addr_i] & ~wmask_i) | (wdata_i & wmask_i);
        else      rdata_o     <= mem[addr_i];
      end
    end
  end

endmodule
