// gpio_if — the GPIO pads the testbench is connected to (IOB0..IOB7 = GPIO[7:0]).
//   pins:    what is on the pads (chip output, testbench drive, or the pull-down)
//   drv_en:  1 = the testbench drives that pad with drv_val (strong);
//            0 = it lets go (the chip, or the weak pull-down, decides)
interface gpio_if;
  wire  [7:0] pins;
  logic [7:0] drv_en  = '0;
  logic [7:0] drv_val = '0;
endinterface
