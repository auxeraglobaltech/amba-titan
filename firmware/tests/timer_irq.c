// timer_irq — rv_timer interrupt reaches Ibex (plan test T7).
//
// Five deadlines, PERIOD_US apart. Each must raise exactly one machine-timer
// interrupt, no earlier than its deadline. Exercises crt0's vector table,
// _trap_entry, trap.c dispatch and mret.

#include "titan.h"

#define NUM_IRQS  5
#define PERIOD_US 20

static volatile uint32_t irq_count;
static volatile uint64_t deadline;

void titan_isr_timer(void) {             // overrides the weak default in trap.c
  uint64_t now = timer_now_us();
  CHECK(now >= deadline, "interrupt early: now=%u deadline=%u", (uint32_t)now, (uint32_t)deadline);
  irq_count++;
  timer_irq_ack();
  deadline += PERIOD_US;
  if (irq_count < NUM_IRQS) timer_set_deadline_us(deadline);
  else timer_irq_enable(false);
}

int main(void) {
  timer_init_us();
  deadline = timer_now_us() + PERIOD_US;
  timer_set_deadline_us(deadline);
  timer_irq_enable(true);
  irq_timer_enable(true);
  irq_global_enable(true);

  uint64_t give_up = timer_now_us() + (NUM_IRQS + 5) * PERIOD_US;
  while (irq_count < NUM_IRQS && timer_now_us() < give_up) wait_for_interrupt();

  irq_global_enable(false);
  CHECK(irq_count == NUM_IRQS, "got %u timer interrupts, expected %u", irq_count, NUM_IRQS);
  titan_printf("timer_irq: %u interrupts, one per deadline\n", irq_count);
  return 0;
}
