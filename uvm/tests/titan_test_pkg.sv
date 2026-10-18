// titan_test_pkg — sequences and tests.
//
// Every test boots the real chip: POR -> AST/power-up -> OTP -> life cycle ->
// Ibex runs crt0 from the ROM image -> firmware writes IN_TEST. Then:
//
//   firmware tests  (firmware decides)  wait for PASSED/FAILED, then any
//                                       testbench-side checks
//     fw_test               any C test with nothing to check outside (T4, T7)
//     fw_gpio_out_test      + GPIO pin sequence                       (T5)
//     fw_uart_tx_test       + UART0 TX text                           (T6)
//
//     fw_reset_test         + the chip booted exactly twice (sw_reset, wdog_reset)
//     fw_clk_gate_test      + gated clocks stop and restart
//     fw_plic_gpio_irq_test + drives the GPIO edges the firmware waits for
//
//   stub-CPU tests  (firmware = idle)   the test drives the fabric through
//                                       the sim-only AXI master
//     axi_sram_rw_test       main SRAM: single words and bursts       (T1)
//     axi_csr_reset_test     reset values across both crossbars + CDC (T2)
//     axi_decerr_test        holes in both crossbars -> DECERR        (T3)
//     axi_mem_map_sweep_test every device window answers; gaps DECERR
//     axi_mem_test           SRAM data/address lines, retention SRAM, ROM contents
//
//   testbench-only (firmware = idle)
//     clk_freq_test          every clkmgr output at its nominal frequency
//
// All SoC tests are described in uvm/TESTS.md.
//
// To add a test: extend titan_base_test, override test_body(), add a line to
// uvm/sim/tests.list. See uvm/README.md.

package titan_test_pkg;
  import uvm_pkg::*;
  import axi_agent_pkg::*;
  import titan_env_pkg::*;
  `include "uvm_macros.svh"

  // ===========================================================================
  // Base test
  // ===========================================================================
  class titan_base_test extends uvm_test;
    `uvm_component_utils(titan_base_test)

    titan_env env;
    time      boot_timeout = 10ms;   // POR to firmware IN_TEST
    time      test_timeout = 20ms;   // IN_TEST to the end of test_body()

    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction

    virtual function void build_phase(uvm_phase phase);
      env = titan_env::type_id::create("env", this);
    endfunction

    // What the test does once the firmware is running. Default: wait for the
    // firmware to report PASSED or FAILED.
    virtual task test_body();
      env.sw.ev_done.wait_trigger();
      if (!env.sw.passed()) `uvm_error("TEST", "firmware did not report PASSED")
    endtask

    virtual task run_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
        begin
          wait_with_timeout(env.sw.ev_in_test, boot_timeout, "firmware never reached main()");
          `uvm_info("TEST", "firmware running; starting test body", UVM_LOW)
          fork
            test_body();
            begin
              #(test_timeout);
              `uvm_fatal("TEST", $sformatf("test body did not finish within %0t", test_timeout))
            end
          join_any
          disable fork;
        end
      join
      #1us;   // let the last responses drain to the monitors
      phase.drop_objection(this);
    endtask

    protected task wait_with_timeout(uvm_event ev, time limit, string msg);
      fork
        ev.wait_trigger();
        begin
          #(limit);
          `uvm_fatal("TEST", $sformatf("%s (after %0t)", msg, limit))
        end
      join_any
      disable fork;
    endtask

    virtual function void report_phase(uvm_phase phase);
      uvm_report_server rs = uvm_report_server::get_server();
      int errors = rs.get_severity_count(UVM_ERROR) + rs.get_severity_count(UVM_FATAL);
      `uvm_info("TEST", errors == 0 ? "*** TEST PASSED ***"
                                    : $sformatf("*** TEST FAILED *** (%0d errors)", errors), UVM_NONE)
    endfunction
  endclass

  // ===========================================================================
  // Firmware tests
  // ===========================================================================
  class fw_test extends titan_base_test;
    `uvm_component_utils(fw_test)
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
  endclass

  class fw_gpio_out_test extends titan_base_test;
    `uvm_component_utils(fw_gpio_out_test)
    // Same table as firmware/tests/gpio_out.c.
    bit [7:0] expected[$] = '{8'h01, 8'h02, 8'h04, 8'h08, 8'h10, 8'h20, 8'h40, 8'h80,
                              8'hFE, 8'hFD, 8'hFB, 8'hF7, 8'hEF, 8'hDF, 8'hBF, 8'h7F,
                              8'hA5};
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction

    virtual task test_body();
      super.test_body();
      // The pins start at 0 (outputs enabled with value 0), so 0 may lead the list.
      while (env.gpio.values.size() && env.gpio.values[0] == 8'h00) void'(env.gpio.values.pop_front());
      if (env.gpio.values != expected)
        `uvm_error("TEST", $sformatf("GPIO sequence %p, expected %p", env.gpio.values, expected))
      else
        `uvm_info("TEST", $sformatf("GPIO pins showed all %0d patterns in order", expected.size()), UVM_LOW)
    endtask
  endclass

  class fw_uart_tx_test extends titan_base_test;
    `uvm_component_utils(fw_uart_tx_test)
    string expected = "titan-axi uart0 ok\n";   // same as firmware/tests/uart_tx.c
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction

    virtual task test_body();
      super.test_body();
      if (env.uart0.text != expected)
        `uvm_error("TEST", $sformatf("UART0 sent \"%s\", expected \"%s\"", env.uart0.text, expected))
      else
        `uvm_info("TEST", "UART0 text matches", UVM_LOW)
    endtask
  endclass

  // ===========================================================================
  // Stub-CPU tests: the test is the bus master
  // ===========================================================================
  class axi_test_seq extends axi_base_seq;
    `uvm_object_utils(axi_test_seq)
    int unsigned errors;
    function new(string name = "axi_test_seq");
      super.new(name);
    endfunction
    function void check(bit ok, string msg);
      if (!ok) begin
        errors++;
        `uvm_error("SEQ", msg)
      end
    endfunction
  endclass

  // T1 — main SRAM through xbar_main and the device bridge (which splits bursts).
  // Stays above the firmware's own data/stack (0x1000_0000-0x1000_4fff).
  class sram_rw_seq extends axi_test_seq;
    `uvm_object_utils(sram_rw_seq)
    bit [31:0] base = 32'h1001_0000;
    function new(string name = "sram_rw_seq");
      super.new(name);
    endfunction

    virtual task body();
      bit [31:0] wr[$], rd[$], d;
      bit [1:0]  r, rr[$];
      bit [3:0]  strb[$];
      // Single words
      for (int i = 0; i < 32; i++) begin
        wr.push_back($urandom());
        write32(base + 4 * i, wr[i], r);
        check(r == AXI_OKAY, $sformatf("write 0x%08h: %s", base + 4 * i, resp_name(r)));
      end
      for (int i = 0; i < 32; i++) begin
        read32(base + 4 * i, d, r);
        check(r == AXI_OKAY && d == wr[i],
              $sformatf("read 0x%08h = 0x%08h %s, expected 0x%08h", base + 4 * i, d, resp_name(r), wr[i]));
      end
      begin : byte_write
        // One byte lane of an already-written word: the bridge must issue a
        // TL-UL PutPartialData and leave the other three bytes alone.
        bit [31:0] orig = wr[0];   // ("before" is a SystemVerilog keyword)
        write(base, '{32'h0000_5A00}, '{4'b0010}, r);
        read32(base, d, r);
        check(d == {orig[31:16], 8'h5A, orig[7:0]},
              $sformatf("byte write: 0x%08h, expected 0x%08h", d, {orig[31:16], 8'h5A, orig[7:0]}));
      end
      // INCR bursts of 2..16 beats
      for (int len = 1; len < 16; len++) begin
        bit [31:0] a = base + 32'h1000 + 64 * len;
        wr.delete(); strb.delete();
        for (int b = 0; b <= len; b++) begin wr.push_back($urandom()); strb.push_back(4'hF); end
        write(a, wr, strb, r, len);
        check(r == AXI_OKAY, $sformatf("burst write 0x%08h len %0d: %s", a, len, resp_name(r)));
        read(a, rd, rr, len);
        check(rd == wr, $sformatf("burst read 0x%08h len %0d: data mismatch", a, len));
      end
      `uvm_info("SEQ", $sformatf("SRAM: 32 words, 1 byte write, 15 bursts written and read back, %0d errors", errors), UVM_LOW)
    endtask
  endclass

  // T2 — reset values: proves the address decode, both crossbars, the
  // main->peri and main->spi_host0 CDCs, and the device bridges.
  class csr_reset_seq extends axi_test_seq;
    `uvm_object_utils(csr_reset_seq)
    typedef struct { string name; bit [31:0] addr; bit [31:0] reset; } reg_t;
    // Offsets from firmware/include/regs/*.h, reset values from the IP .hjson files.
    reg_t regs[$] = '{
      '{"uart0.STATUS (peri)",          32'h4000_0014, 32'h0000_003C},
      '{"gpio.DIRECT_OE (peri)",        32'h4004_0020, 32'h0000_0000},
      '{"rv_timer.CFG0 (peri)",         32'h4010_010C, 32'h0001_0000},
      '{"spi_host0.CONTROL (main, CDC)", 32'h4030_0010, 32'h0000_007F},
      '{"hmac.CFG (main)",              32'h4111_0010, 32'h0000_4100},
      '{"rv_plic.PRIO_1 (main)",        32'h4800_0004, 32'h0000_0000},
      '{"sram_ctrl_main.EXEC_REGWEN",   32'h411C_0008, 32'h0000_0001}
    };
    function new(string name = "csr_reset_seq");
      super.new(name);
    endfunction

    virtual task body();
      foreach (regs[i]) begin
        bit [31:0] d;
        bit [1:0]  r;
        read32(regs[i].addr, d, r);
        check(r == AXI_OKAY && d == regs[i].reset,
              $sformatf("%s @0x%08h = 0x%08h %s, reset value 0x%08h",
                        regs[i].name, regs[i].addr, d, resp_name(r), regs[i].reset));
        `uvm_info("SEQ", $sformatf("%-30s = 0x%08h", regs[i].name, d), UVM_LOW)
      end
    endtask
  endclass

  // T3 — addresses that decode to nothing.
  class decerr_seq extends axi_test_seq;
    `uvm_object_utils(decerr_seq)
    typedef struct { string why; bit [31:0] addr; } hole_t;
    hole_t holes[$] = '{
      '{"xbar_main gap (flash_ctrl.prim .. aes)",  32'h4101_0000},
      '{"past the aes window (xbar_main)",         32'h4110_0100},
      '{"above rv_plic (xbar_main)",               32'h5000_0000},
      '{"peri window, no device (xbar_peri)",      32'h4006_0000},
      '{"past the uart1 window (xbar_peri)",       32'h4001_0040}
    };
    function new(string name = "decerr_seq");
      super.new(name);
    endfunction

    virtual task body();
      foreach (holes[i]) begin
        bit [31:0] d;
        bit [1:0]  r;
        read32(holes[i].addr, d, r);
        check(r == AXI_DECERR, $sformatf("read  %s @0x%08h: %s", holes[i].why, holes[i].addr, resp_name(r)));
        write32(holes[i].addr, 32'hDEAD_BEEF, r);
        check(r == AXI_DECERR, $sformatf("write %s @0x%08h: %s", holes[i].why, holes[i].addr, resp_name(r)));
        `uvm_info("SEQ", $sformatf("DECERR as expected: %s", holes[i].why), UVM_LOW)
      end
    endtask
  endclass

  class axi_stub_cpu_test extends titan_base_test;
    `uvm_component_utils(axi_stub_cpu_test)
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
    // Subclasses return the sequence to run on the sim-only master.
    virtual function axi_test_seq make_seq();
      return null;
    endfunction
    virtual task test_body();
      axi_test_seq seq = make_seq();
      if (seq == null) `uvm_fatal("TEST", "make_seq() returned null")
      seq.start(env.sim_master());
      if (seq.errors) `uvm_error("TEST", $sformatf("%s: %0d errors", seq.get_name(), seq.errors))
    endtask
  endclass

  class axi_sram_rw_test extends axi_stub_cpu_test;
    `uvm_component_utils(axi_sram_rw_test)
    function new(string name, uvm_component parent); super.new(name, parent); endfunction
    virtual function axi_test_seq make_seq(); return sram_rw_seq::type_id::create("sram_rw_seq"); endfunction
  endclass

  class axi_csr_reset_test extends axi_stub_cpu_test;
    `uvm_component_utils(axi_csr_reset_test)
    function new(string name, uvm_component parent); super.new(name, parent); endfunction
    virtual function axi_test_seq make_seq(); return csr_reset_seq::type_id::create("csr_reset_seq"); endfunction
  endclass

  class axi_decerr_test extends axi_stub_cpu_test;
    `uvm_component_utils(axi_decerr_test)
    function new(string name, uvm_component parent); super.new(name, parent); endfunction
    virtual function axi_test_seq make_seq(); return decerr_seq::type_id::create("decerr_seq"); endfunction
  endclass

  // ===========================================================================
  // SoC tests: reset, clocks, interrupts (firmware + testbench)
  // ===========================================================================

  // Reset tests: the firmware resets the chip on its first boot and passes on
  // the second. Checks there really were two boots.
  class fw_reset_test extends titan_base_test;
    `uvm_component_utils(fw_reset_test)
    int unsigned expected_boots = 2;
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
    virtual task test_body();
      super.test_body();
      if (env.sw.n_boots != expected_boots)
        `uvm_error("TEST", $sformatf("chip booted %0d times, expected %0d", env.sw.n_boots, expected_boots))
      else
        `uvm_info("TEST", $sformatf("chip booted %0d times, as expected", env.sw.n_boots), UVM_LOW)
    endtask
  endclass

  // clkmgr software clock gating (firmware/tests/clk_gate.c): after each sync
  // step the clock must be stopped (odd steps) or running (even steps).
  class fw_clk_gate_test extends titan_base_test;
    `uvm_component_utils(fw_clk_gate_test)
    string gated[4] = '{"io_div4_peri", "io_div2_peri", "io_peri", "usb_peri"};  // order of clk_gate.c
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
    virtual task test_body();
      foreach (gated[i]) begin
        int unsigned n;
        env.sw.wait_sync(2 * i + 1);
        #2us;                                          // let the gate take effect
        env.clk.count_edges(gated[i], 5us, n);
        if (n != 0) `uvm_error("TEST", $sformatf("clk_%s gated, but %0d edges in 5 us", gated[i], n))
        else        `uvm_info("TEST", $sformatf("clk_%s stopped", gated[i]), UVM_LOW)
        env.sw.wait_sync(2 * i + 2);
        #2us;
        env.clk.count_edges(gated[i], 5us, n);
        if (n < 100) `uvm_error("TEST", $sformatf("clk_%s ungated, but only %0d edges in 5 us", gated[i], n))
        else         `uvm_info("TEST", $sformatf("clk_%s running again (%0d edges in 5 us)", gated[i], n), UVM_LOW)
      end
      super.test_body();
    endtask
  endclass

  // GPIO -> PLIC -> Ibex (firmware/tests/plic_gpio_irq.c): once armed, raise
  // GPIO[0..3] one at a time; the firmware checks one interrupt per edge.
  class fw_plic_gpio_irq_test extends titan_base_test;
    `uvm_component_utils(fw_plic_gpio_irq_test)
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
    virtual task test_body();
      env.gpio.vif.drv_val = '0;
      env.gpio.vif.drv_en  = 8'h0F;                    // pins 0..3 driven low
      env.sw.wait_sync(1);
      for (int i = 0; i < 4; i++) begin
        #5us;
        env.gpio.vif.drv_val[i] = 1'b1;
        `uvm_info("TEST", $sformatf("GPIO[%0d] rising edge", i), UVM_LOW)
      end
      super.test_body();
    endtask
  endclass

  // Every clkmgr output, measured at the testbench. Firmware is idle; by the
  // time it reports IN_TEST, crt0 has calibrated the AST oscillators.
  class clk_freq_test extends titan_base_test;
    `uvm_component_utils(clk_freq_test)
    typedef struct { string name; real hz; time window; real tol; } clk_t;
    clk_t clocks[$] = '{
      '{"main",    100.0e6, 20us, 0.02},
      '{"io",       96.0e6, 20us, 0.02},
      '{"io_div2",  48.0e6, 20us, 0.02},
      '{"io_div4",  24.0e6, 20us, 0.02},
      '{"usb",      48.0e6, 20us, 0.02},
      '{"aon",     200.0e3, 300us, 0.05}
    };
    function new(string name, uvm_component parent);
      super.new(name, parent);
    endfunction
    virtual task test_body();
      foreach (clocks[i]) begin
        real hz, err;
        env.clk.measure_hz(clocks[i].name, clocks[i].window, hz);
        err = (hz - clocks[i].hz) / clocks[i].hz;
        if (err > clocks[i].tol || err < -clocks[i].tol)
          `uvm_error("TEST", $sformatf("clk_%s = %.3f MHz, expected %.3f MHz +/-%0.0f%%",
                                       clocks[i].name, hz / 1e6, clocks[i].hz / 1e6, clocks[i].tol * 100))
        else
          `uvm_info("TEST", $sformatf("clk_%-8s = %9.3f MHz (expected %9.3f, %.2f%%)",
                                      clocks[i].name, hz / 1e6, clocks[i].hz / 1e6, err * 100), UVM_LOW)
      end
    endtask
  endclass

  // ===========================================================================
  // SoC tests: memory map and memories (stub CPU)
  // ===========================================================================

  // Memory-map sweep: read the first word of every device window, and the
  // word just past each window when nothing else owns it. Every device must
  // answer (OKAY, or SLVERR for a register it refuses — never a hang, never
  // DECERR); unowned addresses must be DECERR. The scoreboard checks each
  // access reached the right port.
  class mem_map_sweep_seq extends axi_test_seq;
    `uvm_object_utils(mem_map_sweep_seq)
    // Not swept, and why (each is a good exercise to enable):
    string skip[$] = '{
      "dev.flash_ctrl.mem",        // flash not initialized: the read would stall
      "dev.flash_ctrl.prim",       // flash macro test registers
      "dev.otp_macro.prim",        // OTP macro test registers
      "dev.rv_dm.mem",             // debug module: debug-only
      "dev.rv_dm.regs",
      "dev.sram_ctrl_ret_aon.ram"  // not initialized (X); covered by axi_mem_test
    };
    int unsigned n_dev, n_hole;
    function new(string name = "mem_map_sweep_seq");
      super.new(name);
    endfunction

    virtual task body();
      for (int i = 0; i < titan_addr_map_pkg::NumRanges; i++) begin
        string     dev = titan_addr_map_pkg::RangeDev[i];
        bit [31:0] lo  = titan_addr_map_pkg::RangeLo[i];
        bit [31:0] hi  = titan_addr_map_pkg::RangeHi[i];
        bit [31:0] d;
        bit [1:0]  r;
        if (dev inside {skip}) continue;
        read32(lo, d, r);
        check(r inside {AXI_OKAY, AXI_SLVERR},
              $sformatf("%s @0x%08h: %s (expected the device to answer)", dev, lo, resp_name(r)));
        `uvm_info("SEQ", $sformatf("%-28s 0x%08h..0x%08h  first word 0x%08h %s",
                                   dev, lo, hi - 1, d, resp_name(r)), UVM_LOW)
        n_dev++;
        if (titan_addr_map_pkg::decode(hi) == "") begin
          read32(hi, d, r);
          check(r == AXI_DECERR, $sformatf("past %s @0x%08h: %s, expected DECERR", dev, hi, resp_name(r)));
          n_hole++;
        end
      end
      `uvm_info("SEQ", $sformatf("swept %0d device windows, %0d gaps after them (%0d skipped)",
                                 n_dev, n_hole, skip.size()), UVM_LOW)
    endtask
  endclass

  // Memory tests:
  //  1. main SRAM data bus: walking 1 and walking 0 on one word
  //  2. main SRAM address lines: a unique value at base + 2**k, then read all
  //     back (catches stuck and shorted address bits)
  //  3. retention SRAM: initialize through sram_ctrl_ret_aon over AXI, then
  //     test it the same way
  //  4. ROM: the first 256 words read over AXI equal the firmware hex image
  class mem_test_seq extends axi_test_seq;
    `uvm_object_utils(mem_test_seq)
    localparam bit [31:0] SramBase    = 32'h1001_0000;  // above the firmware's data/stack
    localparam bit [31:0] RetSramBase = 32'h4060_0000;  // 4 KiB
    localparam bit [31:0] RetCtrlBase = 32'h4050_0000;  // sram_ctrl_ret_aon registers
    localparam bit [31:0] RomBase     = 32'h0000_8000;
    function new(string name = "mem_test_seq");
      super.new(name);
    endfunction

    task data_bus(bit [31:0] a, string what);
      bit [31:0] d;
      bit [1:0]  r;
      for (int b = 0; b < 32; b++) begin
        write32(a, 32'h1 << b, r);
        read32(a, d, r);
        check(d == (32'h1 << b), $sformatf("%s data bus: walking 1, bit %0d: read 0x%08h", what, b, d));
        write32(a, ~(32'h1 << b), r);
        read32(a, d, r);
        check(d == ~(32'h1 << b), $sformatf("%s data bus: walking 0, bit %0d: read 0x%08h", what, b, d));
      end
    endtask

    task addr_lines(bit [31:0] base, int unsigned bits, string what);
      bit [31:0] d;
      bit [1:0]  r;
      write32(base, 32'h5A5A_0000, r);
      for (int k = 2; k < bits; k++) write32(base + (1 << k), 32'hA5A5_0000 | k, r);
      read32(base, d, r);
      check(d == 32'h5A5A_0000, $sformatf("%s address lines: base word overwritten (0x%08h)", what, d));
      for (int k = 2; k < bits; k++) begin
        read32(base + (1 << k), d, r);
        check(d == (32'hA5A5_0000 | k),
              $sformatf("%s address line %0d: read 0x%08h, expected 0x%08h", what, k, d, 32'hA5A5_0000 | k));
      end
    endtask

    virtual task body();
      bit [31:0] d, rom[8192];
      bit [1:0]  r;
      string     fw;

      data_bus(SramBase, "main SRAM");
      addr_lines(SramBase, 16, "main SRAM");          // 64 KiB above the base
      `uvm_info("SEQ", "main SRAM: data bus and address lines OK", UVM_LOW)

      // Retention SRAM: CTRL.INIT (bit 1), then wait for STATUS.INIT_DONE (bit 5).
      write32(RetCtrlBase + 32'h14, 32'h2, r);
      for (int i = 0; i < 1000; i++) begin
        read32(RetCtrlBase + 32'h4, d, r);
        if (d[5]) break;
      end
      check(d[5], "retention SRAM: INIT_DONE never set");
      data_bus(RetSramBase, "retention SRAM");
      addr_lines(RetSramBase, 12, "retention SRAM");  // 4 KiB
      `uvm_info("SEQ", "retention SRAM: initialized, data bus and address lines OK", UVM_LOW)

      if (!$value$plusargs("firmware=%s", fw)) `uvm_fatal("SEQ", "no +firmware")
      $readmemh(fw, rom);
      for (int i = 0; i < 256; i++) begin
        read32(RomBase + 4 * i, d, r);
        check(r == AXI_OKAY && d == rom[i],
              $sformatf("ROM word %0d @0x%08h = 0x%08h, image has 0x%08h", i, RomBase + 4 * i, d, rom[i]));
      end
      `uvm_info("SEQ", "ROM: 256 words match the firmware image", UVM_LOW)
    endtask
  endclass

  class axi_mem_map_sweep_test extends axi_stub_cpu_test;
    `uvm_component_utils(axi_mem_map_sweep_test)
    function new(string name, uvm_component parent); super.new(name, parent); endfunction
    virtual function axi_test_seq make_seq(); return mem_map_sweep_seq::type_id::create("mem_map_sweep_seq"); endfunction
  endclass

  class axi_mem_test extends axi_stub_cpu_test;
    `uvm_component_utils(axi_mem_test)
    function new(string name, uvm_component parent); super.new(name, parent); endfunction
    virtual function axi_test_seq make_seq(); return mem_test_seq::type_id::create("mem_test_seq"); endfunction
  endclass

endpackage
