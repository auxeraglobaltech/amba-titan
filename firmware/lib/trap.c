// trap.c — dispatch for every trap; crt0.S's _trap_entry calls titan_trap().
//
// Ibex runs mtvec in vectored mode and crt0 points every vector at the same
// entry, so interrupts and exceptions are told apart here by mcause.

#include "titan.h"

#define MCAUSE_IRQ_BIT     0x80000000u
#define IRQ_M_SOFTWARE     3
#define IRQ_M_TIMER        7
#define IRQ_M_EXTERNAL     11

__attribute__((weak)) void titan_isr_timer(void) {
  titan_puts("unexpected timer interrupt\n");
  titan_fail();
}

__attribute__((weak)) void titan_isr_external(void) {
  titan_printf("unexpected external interrupt, PLIC id %u\n", plic_claim());
  titan_fail();
}

__attribute__((weak)) void titan_isr_software(void) {
  titan_puts("unexpected software interrupt\n");
  titan_fail();
}

__attribute__((weak)) bool titan_exception_handler(uint32_t mcause, uint32_t mepc,
                                                   uint32_t mtval) {
  titan_printf("EXCEPTION mcause=%u mepc=0x%08x mtval=0x%08x\n", mcause, mepc, mtval);
  return false;
}

uint32_t titan_trap(uint32_t mcause, uint32_t mepc, uint32_t mtval) {
  if (mcause & MCAUSE_IRQ_BIT) {
    switch (mcause & 0x1f) {
      case IRQ_M_TIMER:    titan_isr_timer(); break;
      case IRQ_M_EXTERNAL: titan_isr_external(); break;
      case IRQ_M_SOFTWARE: titan_isr_software(); break;
      default:
        titan_printf("unexpected interrupt, mcause=0x%08x\n", mcause);
        titan_fail();
    }
    return mepc;  // interrupts resume where they hit
  }
  if (!titan_exception_handler(mcause, mepc, mtval)) titan_fail();
  // Resume after the faulting instruction (16-bit if its low bits are not 11).
  uint16_t insn = *(volatile uint16_t *)mepc;
  return mepc + (((insn & 0x3) == 0x3) ? 4 : 2);
}
