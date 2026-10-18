// titan.c — test status, log output and a small printf, all through
// titan_sim_ctrl (an AXI slave the testbench watches).

#include <stdarg.h>

#include "titan.h"

void titan_set_status(uint32_t status) {
  reg32_write(TITAN_SIM_CTRL_BASE, TITAN_SIM_CTRL_STATUS, status);
}

void titan_putc(char c) {
  reg32_write(TITAN_SIM_CTRL_BASE, TITAN_SIM_CTRL_LOG, (uint8_t)c);
}

void titan_puts(const char *s) {
  while (*s) titan_putc(*s++);
}

void titan_pass(void) {
  titan_set_status(TITAN_STATUS_PASSED);
  for (;;) wait_for_interrupt();
}

void titan_fail(void) {
  titan_set_status(TITAN_STATUS_FAILED);
  for (;;) wait_for_interrupt();
}

static void put_uint(uint32_t v, unsigned base, int width, char pad) {
  char buf[12];
  int n = 0;
  do {
    unsigned d = v % base;  // rv32imc: hardware divide, no libgcc needed
    buf[n++] = (char)(d < 10 ? '0' + d : 'a' + d - 10);
    v /= base;
  } while (v);
  while (n < width) buf[n++] = pad;
  while (n) titan_putc(buf[--n]);
}

void titan_printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  for (; *fmt; fmt++) {
    if (*fmt != '%') {
      titan_putc(*fmt);
      continue;
    }
    fmt++;
    char pad = ' ';
    int width = 0;
    if (*fmt == '0') {
      pad = '0';
      fmt++;
    }
    while (*fmt >= '0' && *fmt <= '9') width = width * 10 + (*fmt++ - '0');
    switch (*fmt) {
      case 'd': {
        int32_t v = va_arg(ap, int32_t);
        if (v < 0) {
          titan_putc('-');
          v = -v;
        }
        put_uint((uint32_t)v, 10, width, pad);
        break;
      }
      case 'u': put_uint(va_arg(ap, uint32_t), 10, width, pad); break;
      case 'x': put_uint(va_arg(ap, uint32_t), 16, width, pad); break;
      case 'p':
        titan_puts("0x");
        put_uint((uint32_t)(uintptr_t)va_arg(ap, void *), 16, 8, '0');
        break;
      case 'c': titan_putc((char)va_arg(ap, int)); break;
      case 's': {
        const char *s = va_arg(ap, const char *);
        titan_puts(s ? s : "(null)");
        break;
      }
      case '%': titan_putc('%'); break;
      case '\0': fmt--; break;
      default: titan_putc('%'); titan_putc(*fmt); break;
    }
  }
  va_end(ap);
}
