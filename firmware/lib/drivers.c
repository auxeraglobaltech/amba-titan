// drivers.c — minimal register-level drivers for the peripherals the basic
// tests use. Read them next to the register headers in include/regs/: they
// are deliberately short, so the register programming is visible.

#include "regs/gpio_regs.h"
#include "regs/pinmux_regs.h"
#include "regs/rv_plic_regs.h"
#include "regs/rv_timer_regs.h"
#include "regs/uart_regs.h"
#include "titan.h"

#define UART0   TOP_EARLGREY_UART0_BASE_ADDR
#define GPIO    TOP_EARLGREY_GPIO_BASE_ADDR
#define PINMUX  TOP_EARLGREY_PINMUX_AON_BASE_ADDR
#define TIMER   TOP_EARLGREY_RV_TIMER_BASE_ADDR
#define PLIC    TOP_EARLGREY_RV_PLIC_BASE_ADDR

// ---------------------------------------------------------------------------
// pinmux: a peripheral input selects a pad (INSEL); a pad output selects a
// peripheral output (OUTSEL). Out of reset nothing is connected.
// ---------------------------------------------------------------------------

static void pinmux_input(uint32_t periph_in, uint32_t insel) {
  reg32_write(PINMUX, PINMUX_MIO_PERIPH_INSEL_0_REG_OFFSET + 4 * periph_in, insel);
}

static void pinmux_output(uint32_t mio_out, uint32_t outsel) {
  reg32_write(PINMUX, PINMUX_MIO_OUTSEL_0_REG_OFFSET + 4 * mio_out, outsel);
}

void pinmux_connect_uart0(void) {
  pinmux_input(kTopEarlgreyPinmuxPeripheralInUart0Rx, kTopEarlgreyPinmuxInselIoc3);
  pinmux_output(kTopEarlgreyPinmuxMioOutIoc4, kTopEarlgreyPinmuxOutselUart0Tx);
}

void pinmux_connect_tb_gpios(void) {
  for (uint32_t i = 0; i < TITAN_NUM_TB_GPIOS; i++) {
    pinmux_input(kTopEarlgreyPinmuxPeripheralInGpioGpio0 + i, kTopEarlgreyPinmuxInselIob0 + i);
    pinmux_output(kTopEarlgreyPinmuxMioOutIob0 + i, kTopEarlgreyPinmuxOutselGpioGpio0 + i);
  }
}

// ---------------------------------------------------------------------------
// UART0. Baud = NCO * f_peri / 2^20.
// ---------------------------------------------------------------------------

void uart_init(uint32_t baud) {
  // 64-bit math avoided (no libgcc): f_peri / 2^4 fits, then scale.
  uint32_t nco = (uint32_t)(((baud << 4) / (TITAN_CLK_PERI_HZ >> 12)) << 4);
  reg32_write(UART0, UART_FIFO_CTRL_REG_OFFSET,
              BIT(UART_FIFO_CTRL_RXRST_BIT) | BIT(UART_FIFO_CTRL_TXRST_BIT));
  reg32_write(UART0, UART_CTRL_REG_OFFSET,
              ((nco & UART_CTRL_NCO_MASK) << UART_CTRL_NCO_OFFSET) |
              BIT(UART_CTRL_TX_BIT) | BIT(UART_CTRL_RX_BIT));
}

void uart_putc(char c) {
  while (reg32_read(UART0, UART_STATUS_REG_OFFSET) & BIT(UART_STATUS_TXFULL_BIT)) {
  }
  reg32_write(UART0, UART_WDATA_REG_OFFSET, (uint8_t)c);
}

void uart_puts(const char *s) {
  while (*s) uart_putc(*s++);
}

void uart_flush(void) {
  while (!(reg32_read(UART0, UART_STATUS_REG_OFFSET) & BIT(UART_STATUS_TXIDLE_BIT))) {
  }
}

// ---------------------------------------------------------------------------
// GPIO
// ---------------------------------------------------------------------------

void gpio_set_oe(uint32_t mask) { reg32_write(GPIO, GPIO_DIRECT_OE_REG_OFFSET, mask); }
void gpio_write(uint32_t value) { reg32_write(GPIO, GPIO_DIRECT_OUT_REG_OFFSET, value); }
uint32_t gpio_read(void) { return reg32_read(GPIO, GPIO_DATA_IN_REG_OFFSET); }

// ---------------------------------------------------------------------------
// rv_timer: 64-bit counter, tick = (prescale + 1) peripheral clocks.
// ---------------------------------------------------------------------------

void timer_init_us(void) {
  reg32_write(TIMER, RV_TIMER_CTRL_REG_OFFSET, 0);
  reg32_write(TIMER, RV_TIMER_CFG0_REG_OFFSET,
              ((TITAN_CLK_PERI_HZ / 1000000u - 1) << RV_TIMER_CFG0_PRESCALE_OFFSET) |
              (1u << RV_TIMER_CFG0_STEP_OFFSET));
  reg32_write(TIMER, RV_TIMER_TIMER_V_UPPER0_REG_OFFSET, 0);
  reg32_write(TIMER, RV_TIMER_TIMER_V_LOWER0_REG_OFFSET, 0);
  timer_set_deadline_us(UINT64_MAX);
  reg32_write(TIMER, RV_TIMER_CTRL_REG_OFFSET, BIT(RV_TIMER_CTRL_ACTIVE_0_BIT));
}

uint64_t timer_now_us(void) {
  uint32_t hi, lo;
  do {
    hi = reg32_read(TIMER, RV_TIMER_TIMER_V_UPPER0_REG_OFFSET);
    lo = reg32_read(TIMER, RV_TIMER_TIMER_V_LOWER0_REG_OFFSET);
  } while (hi != reg32_read(TIMER, RV_TIMER_TIMER_V_UPPER0_REG_OFFSET));
  return ((uint64_t)hi << 32) | lo;
}

void timer_set_deadline_us(uint64_t when) {
  // Upper first, via an "impossible" value, so no spurious match in between.
  reg32_write(TIMER, RV_TIMER_COMPARE_UPPER0_0_REG_OFFSET, UINT32_MAX);
  reg32_write(TIMER, RV_TIMER_COMPARE_LOWER0_0_REG_OFFSET, (uint32_t)when);
  reg32_write(TIMER, RV_TIMER_COMPARE_UPPER0_0_REG_OFFSET, (uint32_t)(when >> 32));
}

void timer_irq_enable(bool on) {
  reg32_write(TIMER, RV_TIMER_INTR_ENABLE0_REG_OFFSET, on ? BIT(RV_TIMER_INTR_ENABLE0_IE_0_BIT) : 0);
}

void timer_irq_ack(void) {
  reg32_write(TIMER, RV_TIMER_INTR_STATE0_REG_OFFSET, BIT(RV_TIMER_INTR_STATE0_IS_0_BIT));
}

void delay_us(uint32_t us) {
  uint64_t end = timer_now_us() + us;
  while (timer_now_us() < end) {
  }
}

// ---------------------------------------------------------------------------
// Interrupt enables and the PLIC (target 0 = Ibex)
// ---------------------------------------------------------------------------

#define MSTATUS_MIE BIT(3)
#define MIE_MSIE    BIT(3)
#define MIE_MTIE    BIT(7)
#define MIE_MEIE    BIT(11)

void irq_global_enable(bool on) {
  if (on) CSR_SET(mstatus, MSTATUS_MIE); else CSR_CLEAR(mstatus, MSTATUS_MIE);
}
void irq_timer_enable(bool on) {
  if (on) CSR_SET(mie, MIE_MTIE); else CSR_CLEAR(mie, MIE_MTIE);
}
void irq_external_enable(bool on) {
  if (on) CSR_SET(mie, MIE_MEIE); else CSR_CLEAR(mie, MIE_MEIE);
}

void plic_enable(uint32_t irq_id, uint32_t priority) {
  reg32_write(PLIC, RV_PLIC_PRIO_0_REG_OFFSET + 4 * irq_id, priority);
  uint32_t off = RV_PLIC_IE0_0_REG_OFFSET + 4 * (irq_id / 32);
  reg32_write(PLIC, off, reg32_read(PLIC, off) | BIT(irq_id % 32));
  reg32_write(PLIC, RV_PLIC_THRESHOLD0_REG_OFFSET, 0);
}

uint32_t plic_claim(void) { return reg32_read(PLIC, RV_PLIC_CC0_REG_OFFSET); }
void plic_complete(uint32_t irq_id) { reg32_write(PLIC, RV_PLIC_CC0_REG_OFFSET, irq_id); }
