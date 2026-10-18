// gpio_if — the GPIO pads the testbench is connected to (IOB0..IOB7 = GPIO[7:0]).
interface gpio_if;
  wire  [7:0] pins;        // pad values (resolved: chip drive or testbench pull)
endinterface
