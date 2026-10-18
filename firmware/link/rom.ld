/* rom.ld — titan-axi firmware memory layout.
 *
 * Code and constants live in ROM (the testbench loads the hex image there);
 * data, bss and stack live in main SRAM.
 *
 *   ROM      0x0000_8000  32 KiB   vectors @ +0x00, reset @ +0x80
 *   RAM_MAIN 0x1000_0000 128 KiB   .data .bss ... stack (top 16 KiB)
 */
OUTPUT_ARCH(riscv)
ENTRY(_reset_start)

MEMORY {
  rom (rx)  : ORIGIN = 0x00008000, LENGTH = 32K
  ram (rwx) : ORIGIN = 0x10000000, LENGTH = 128K
}

_stack_size = 16K;

SECTIONS {
  .vectors ORIGIN(rom) : { KEEP(*(.vectors)) } > rom
  .crt : { *(.crt) } > rom
  .text : { *(.text .text.*) } > rom
  .rodata : {
    . = ALIGN(4);
    *(.rodata .rodata.* .srodata .srodata.*)
    . = ALIGN(4);
  } > rom

  .data : {
    . = ALIGN(4);
    _data_start = .;
    __global_pointer$ = . + 0x800;
    *(.sdata .sdata.* .data .data.*)
    . = ALIGN(4);
    _data_end = .;
  } > ram AT > rom
  _data_init_start = LOADADDR(.data);

  .bss (NOLOAD) : {
    . = ALIGN(4);
    _bss_start = .;
    *(.sbss .sbss.* .bss .bss.* COMMON)
    . = ALIGN(4);
    _bss_end = .;
  } > ram

  .stack (NOLOAD) : {
    . = ALIGN(16);
    _stack_start = .;
    . += _stack_size;
    _stack_end = .;
  } > ram

  /DISCARD/ : { *(.comment) *(.note .note.*) *(.eh_frame*) }
}

ASSERT(_vectors == 0x8000, "vector table must be at the start of ROM")
ASSERT(_crt_end <= 0x8800, "crt0 must fit in the first 2 KiB of ROM (reset PMP rule)")
