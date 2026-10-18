// Bridge testbench file list.  Paths use ${TITAN_ROOT} (set by run.sh).
// OpenTitan primitives first: prim_assert.sv defines the `ASSERT macros the
// bridges use.
-f ${TITAN_ROOT}/uvm/bridge_tb/ot.f
-f ${TITAN_ROOT}/rtl/vendor/pulp/pulp.f
${TITAN_ROOT}/rtl/bridge/titan_axi_pkg.sv
${TITAN_ROOT}/rtl/bridge/titan_tlul2axi.sv
${TITAN_ROOT}/rtl/bridge/titan_axi2tlul.sv
${TITAN_ROOT}/uvm/bridge_tb/tb_sram.sv
${TITAN_ROOT}/uvm/bridge_tb/tb_bridge.sv
