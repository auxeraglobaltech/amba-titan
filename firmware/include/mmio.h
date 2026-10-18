// mmio.h — 32-bit register access, and the one type the generated
// regs/<ip>_regs.h headers need.

#ifndef MMIO_H_
#define MMIO_H_

#include <stdint.h>

// Used by the *_FIELD macros in regs/*_regs.h.
typedef struct bitfield_field32 {
  uint32_t mask;
  uint32_t index;
} bitfield_field32_t;

static inline uint32_t reg32_read(uintptr_t base, uint32_t offset) {
  return *(volatile uint32_t *)(base + offset);
}

static inline void reg32_write(uintptr_t base, uint32_t offset, uint32_t value) {
  *(volatile uint32_t *)(base + offset) = value;
}

// Read-modify-write of a field: value is the unshifted field value.
static inline void reg32_write_field(uintptr_t base, uint32_t offset, uint32_t mask,
                                     uint32_t index, uint32_t value) {
  uint32_t r = reg32_read(base, offset);
  r = (r & ~(mask << index)) | ((value & mask) << index);
  reg32_write(base, offset, r);
}

#define BIT(n) (1u << (n))

#endif  // MMIO_H_
