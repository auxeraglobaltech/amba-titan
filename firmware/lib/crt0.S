// crt0.S — reset to main() on titan-axi Earl Grey.
//
// Layout (link/rom.ld): this file's .vectors section sits at the start of ROM
// (0x8000). Ibex resets to boot_addr + 0x80 with mtvec = boot_addr in
// vectored mode, so 0x8000 holds 32 interrupt vectors and 0x8080 the reset
// jump.
//
// Order matters:
//  1. PMP. Earl Grey's reset PMP (hw/top_earlgrey/rtl/ibex_pmp_reset_pkg.sv)
//     uses MMWP: M-mode may only touch what a rule allows, and the rules allow
//     just the first 2 KiB of ROM, MMIO 0x4000_0000-0x4201_0000 and the debug
//     ROM. Main SRAM and the PLIC are NOT allowed. So, from inside those
//     first 2 KiB and before any stack use, open everything with one locked
//     all-access rule (entry 15) and clear the reset rules — as OpenTitan's
//     test ROM does. (mseccfg.RLB=1 at reset permits editing locked rules.)
//  2. SRAM. Main SRAM holds random data with bad ECC until initialized; the
//     first read of such a word is an integrity error. sram_ctrl CTRL.INIT
//     fills it with valid data in hardware (~32k cycles).
//  3. .bss zero, .data copy, then main().

#include "regs/sram_ctrl_regs.h"
#include "top_earlgrey_memory.h"   // top_earlgrey.h has C enums; this one is assembler-safe

#define SIM_CTRL_BASE (TOP_EARLGREY_RV_CORE_IBEX_CFG_BASE_ADDR + 0x80)
#define STATUS_IN_TEST 0x4354
#define STATUS_PASSED  0x900D
#define STATUS_FAILED  0xBAAD

  .option push
  .option norvc
  .option norelax

// ---------------------------------------------------------------------------
// Vector table (mtvec base, vectored mode). Entry n = mcause n for interrupts;
// every exception goes to entry 0.
// ---------------------------------------------------------------------------
  .section .vectors, "ax"
  .balign 256
  .global _vectors
_vectors:
  .rept 32
  j _trap_entry
  .endr
  // 0x80: reset vector
  j _reset_start

// ---------------------------------------------------------------------------
// Reset. Everything up to the call to main must stay in the first 2 KiB of
// ROM (checked by an ASSERT in rom.ld).
// ---------------------------------------------------------------------------
  .section .crt, "ax"
  .balign 4
  .global _reset_start
_reset_start:
  // 1. PMP: entry 15 = locked NAPOT RWX over the whole 34-bit space.
  li   t0, (0x9f << 24)
  csrw pmpcfg3, t0
  li   t0, 0x7fffffff
  csrw pmpaddr15, t0
  csrw pmpcfg0, zero
  csrw pmpcfg1, zero
  csrw pmpcfg2, zero

  // Registers to a known state (helps X-free waves).
  li x1, 0;  li x2, 0;  li x3, 0;  li x4, 0;  li x5, 0;  li x6, 0;  li x7, 0
  li x8, 0;  li x9, 0;  li x10, 0; li x11, 0; li x12, 0; li x13, 0; li x14, 0
  li x15, 0; li x16, 0; li x17, 0; li x18, 0; li x19, 0; li x20, 0; li x21, 0
  li x22, 0; li x23, 0; li x24, 0; li x25, 0; li x26, 0; li x27, 0; li x28, 0
  li x29, 0; li x30, 0; li x31, 0

  // 2. Initialize main SRAM and wait for STATUS.INIT_DONE.
  li   a0, TOP_EARLGREY_SRAM_CTRL_MAIN_REGS_BASE_ADDR
  li   t0, (1 << SRAM_CTRL_CTRL_INIT_BIT)
  sw   t0, SRAM_CTRL_CTRL_REG_OFFSET(a0)
1:
  lw   t0, SRAM_CTRL_STATUS_REG_OFFSET(a0)
  andi t0, t0, (1 << SRAM_CTRL_STATUS_INIT_DONE_BIT)
  beqz t0, 1b

  // Stack and global pointer (SRAM is usable from here on).
  la   sp, _stack_end
  la   gp, __global_pointer$

  // 3. Zero .bss, copy .data from ROM.
  la   a0, _bss_start
  la   a1, _bss_end
2:
  bgeu a0, a1, 3f
  sw   zero, 0(a0)
  addi a0, a0, 4
  j    2b
3:
  la   a0, _data_start
  la   a1, _data_end
  la   a2, _data_init_start
4:
  bgeu a0, a1, 5f
  lw   t0, 0(a2)
  sw   t0, 0(a0)
  addi a0, a0, 4
  addi a2, a2, 4
  j    4b
5:
  // Tell the testbench the test has started, run it, report the result.
  li   s0, SIM_CTRL_BASE
  li   t0, STATUS_IN_TEST
  sw   t0, 0(s0)
  call main
  li   t0, STATUS_PASSED
  beqz a0, 6f
  li   t0, STATUS_FAILED
6:
  sw   t0, 0(s0)
7:
  wfi
  j    7b
  .global _crt_end
_crt_end:

// ---------------------------------------------------------------------------
// Trap entry: save caller-saved registers, call titan_trap() in trap.c,
// which returns the address to resume at.
// ---------------------------------------------------------------------------
  .section .text
  .balign 4
  .global _trap_entry
_trap_entry:
  addi sp, sp, -64
  sw   ra,  0(sp)
  sw   t0,  4(sp)
  sw   t1,  8(sp)
  sw   t2, 12(sp)
  sw   a0, 16(sp)
  sw   a1, 20(sp)
  sw   a2, 24(sp)
  sw   a3, 28(sp)
  sw   a4, 32(sp)
  sw   a5, 36(sp)
  sw   a6, 40(sp)
  sw   a7, 44(sp)
  sw   t3, 48(sp)
  sw   t4, 52(sp)
  sw   t5, 56(sp)
  sw   t6, 60(sp)
  csrr a0, mcause
  csrr a1, mepc
  csrr a2, mtval
  call titan_trap
  csrw mepc, a0
  lw   ra,  0(sp)
  lw   t0,  4(sp)
  lw   t1,  8(sp)
  lw   t2, 12(sp)
  lw   a0, 16(sp)
  lw   a1, 20(sp)
  lw   a2, 24(sp)
  lw   a3, 28(sp)
  lw   a4, 32(sp)
  lw   a5, 36(sp)
  lw   a6, 40(sp)
  lw   a7, 44(sp)
  lw   t3, 48(sp)
  lw   t4, 52(sp)
  lw   t5, 56(sp)
  lw   t6, 60(sp)
  addi sp, sp, 64
  mret

  .option pop
