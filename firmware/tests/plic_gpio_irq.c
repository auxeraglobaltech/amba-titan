// plic_gpio_irq — GPIO edge -> PLIC -> Ibex external interrupt (SoC
// interrupt-connectivity test).
//
// GPIO[3:0] are inputs with rising-edge interrupts, each routed through the
// PLIC. Once armed, the firmware says so (titan_sync(1)); the UVM test
// (fw_plic_gpio_irq_test) then raises pins 0,1,2,3 in turn. Each edge must
// produce exactly one interrupt, from the right PLIC source, in order.

#include "regs/gpio_regs.h"
#include "titan.h"

#define NUM_PINS 4
#define GPIO     TOP_EARLGREY_GPIO_BASE_ADDR

static volatile uint32_t irq_count;
static volatile uint32_t irq_pin[NUM_PINS + 4];

void titan_isr_external(void) {           // overrides the weak default
  uint32_t id = plic_claim();
  uint32_t pin = id - kTopEarlgreyPlicIrqIdGpioGpio0;
  CHECK(pin < NUM_PINS, "interrupt from unexpected PLIC source %u", id);
  reg32_write(GPIO, GPIO_INTR_STATE_REG_OFFSET, BIT(pin));   // W1C at the source first
  if (irq_count < NUM_PINS + 4) irq_pin[irq_count] = pin;
  irq_count++;
  plic_complete(id);
}

int main(void) {
  timer_init_us();
  pinmux_connect_tb_gpios();
  gpio_set_oe(0);                                             // all inputs
  reg32_write(GPIO, GPIO_INTR_STATE_REG_OFFSET, 0xFFFFFFFFu);
  reg32_write(GPIO, GPIO_INTR_CTRL_EN_RISING_REG_OFFSET, (1u << NUM_PINS) - 1);
  reg32_write(GPIO, GPIO_INTR_ENABLE_REG_OFFSET, (1u << NUM_PINS) - 1);
  for (uint32_t i = 0; i < NUM_PINS; i++) plic_enable(kTopEarlgreyPlicIrqIdGpioGpio0 + i, 1);
  irq_external_enable(true);
  irq_global_enable(true);

  titan_sync(1);                                              // armed: testbench, go
  uint64_t give_up = timer_now_us() + 500;
  while (irq_count < NUM_PINS && timer_now_us() < give_up) wait_for_interrupt();
  delay_us(20);                                               // catch any extra interrupt
  irq_global_enable(false);

  CHECK(irq_count == NUM_PINS, "got %u interrupts, expected %u", irq_count, NUM_PINS);
  for (uint32_t i = 0; i < NUM_PINS; i++)
    CHECK(irq_pin[i] == i, "interrupt %u came from pin %u", i, irq_pin[i]);
  titan_printf("plic_gpio_irq: %u edges, %u interrupts, in order\n", NUM_PINS, irq_count);
  return 0;
}
