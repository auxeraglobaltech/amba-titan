// titan.h — everything a titan-axi firmware test needs. Include only this.
//
// A test is one C file with
//
//     #include "titan.h"
//     int main(void) {
//       ...
//       return 0;      // 0 = pass, anything else = fail
//     }
//
// crt0.S boots the chip (PMP, SRAM init, .data/.bss), reports "in test",
// calls main(), and reports pass or fail from its return value. The
// testbench sees those reports as AXI writes to titan_sim_ctrl.

#ifndef TITAN_H_
#define TITAN_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mmio.h"
#include "top_earlgrey.h"

// ---------------------------------------------------------------------------
// Test status and log (titan_sim_ctrl, the DV window of rv_core_ibex.cfg)
// ---------------------------------------------------------------------------

#define TITAN_SIM_CTRL_BASE   (TOP_EARLGREY_RV_CORE_IBEX_CFG_BASE_ADDR + 0x80)
#define TITAN_SIM_CTRL_STATUS 0x0   // write a TITAN_STATUS_* value
#define TITAN_SIM_CTRL_LOG    0x4   // write one character

// Same values as OpenTitan's sw_test_status_t, so waves read the same.
#define TITAN_STATUS_IN_TEST 0x4354u
#define TITAN_STATUS_PASSED  0x900Du
#define TITAN_STATUS_FAILED  0xBAADu

void titan_set_status(uint32_t status);
void titan_putc(char c);
void titan_puts(const char *s);
// printf subset: %d %u %x %08x %c %s %p %%. Output goes to the testbench log.
// Every integer argument is taken as 32 bits (int and long are both 32-bit on
// RV32), so uint32_t works with %u/%x as-is. No GCC format checking: this
// toolchain makes uint32_t 'unsigned long', which it would reject for %x.
void titan_printf(const char *fmt, ...);

// End the test now. Never return.
void titan_pass(void) __attribute__((noreturn));
void titan_fail(void) __attribute__((noreturn));

// Fail the test, with file:line, if cond is false.
#define CHECK(cond, ...)                                                   \
  do {                                                                     \
    if (!(cond)) {                                                         \
      titan_printf("CHECK FAILED %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      titan_printf("  " __VA_ARGS__);                                      \
      titan_putc('\n');                                                    \
      titan_fail();                                                        \
    }                                                                      \
  } while (0)

// ---------------------------------------------------------------------------
// Clocks seen by the peripherals (simulation values of the AST oscillators)
// ---------------------------------------------------------------------------

#define TITAN_CLK_MAIN_HZ 100000000u   // CPU, xbar_main
#define TITAN_CLK_PERI_HZ  24000000u   // clk_io_div4: xbar_peri, uart, gpio, rv_timer

// ---------------------------------------------------------------------------
// Pads used by the testbench (see uvm/tb/tb_top.sv)
// ---------------------------------------------------------------------------
//   UART0 TX -> IOC4, UART0 RX <- IOC3          (same pads as OpenTitan DV)
//   GPIO[7:0] <-> IOB0..IOB7

#define TITAN_NUM_TB_GPIOS 8

// ---------------------------------------------------------------------------
// Drivers (firmware/lib/*.c) — minimal and register-level on purpose
// ---------------------------------------------------------------------------

// pinmux.c
void pinmux_connect_uart0(void);
void pinmux_connect_tb_gpios(void);   // GPIO[7:0] on IOB0..IOB7, inputs and outputs

// uart.c (UART0)
void uart_init(uint32_t baud);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_flush(void);                 // wait until the last bit has left the pin

// gpio.c
void gpio_set_oe(uint32_t mask);       // 1 = drive the pin
void gpio_write(uint32_t value);
uint32_t gpio_read(void);

// timer.c (rv_timer hart 0, timer 0; interrupt goes straight to Ibex mtip)
void timer_init_us(void);              // 1 tick = 1 microsecond
uint64_t timer_now_us(void);
void timer_set_deadline_us(uint64_t when);
void timer_irq_enable(bool on);
void timer_irq_ack(void);
void delay_us(uint32_t us);

// irq.c
void irq_global_enable(bool on);       // mstatus.MIE
void irq_timer_enable(bool on);        // mie.MTIE
void irq_external_enable(bool on);     // mie.MEIE (PLIC)
void plic_enable(uint32_t irq_id, uint32_t priority);
uint32_t plic_claim(void);
void plic_complete(uint32_t irq_id);

// Interrupt and exception hooks. Weak defaults in lib/trap.c: the interrupt
// hooks fail the test ("unexpected interrupt"); the exception hook prints
// mcause/mepc/mtval and fails. Define your own in the test to override.
void titan_isr_timer(void);
void titan_isr_external(void);
void titan_isr_software(void);
// Return true to resume after the faulting instruction, false to fail.
bool titan_exception_handler(uint32_t mcause, uint32_t mepc, uint32_t mtval);

// CSR access
#define CSR_READ(csr)                                    \
  ({                                                     \
    uint32_t v_;                                         \
    __asm__ volatile("csrr %0, " #csr : "=r"(v_));       \
    v_;                                                  \
  })
#define CSR_WRITE(csr, v) __asm__ volatile("csrw " #csr ", %0" ::"r"(v))
#define CSR_SET(csr, v)   __asm__ volatile("csrs " #csr ", %0" ::"r"(v))
#define CSR_CLEAR(csr, v) __asm__ volatile("csrc " #csr ", %0" ::"r"(v))

static inline void wait_for_interrupt(void) { __asm__ volatile("wfi"); }

#endif  // TITAN_H_
