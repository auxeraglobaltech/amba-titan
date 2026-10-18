#!/usr/bin/env python3
"""firmware/tools/elf2hex.py — ROM image for the testbench.

Turns the raw ROM contents (objcopy -O binary, which starts at the ROM base
0x8000 and includes .data's load image) into a $readmemh file: one 32-bit
little-endian word per line, padded with zeros to the full ROM so that
prefetches past the end of the code read 0, not X.

  elf2hex.py build/hello/hello.bin build/hello/hello.hex
"""
import sys

ROM_BYTES = 32 * 1024

def main():
    src, dst = sys.argv[1], sys.argv[2]
    data = open(src, "rb").read()
    if len(data) > ROM_BYTES:
        sys.exit(f"{src}: {len(data)} bytes does not fit in the {ROM_BYTES}-byte ROM")
    data = data + bytes(ROM_BYTES - len(data))
    with open(dst, "w") as f:
        f.write(f"// {src}: {ROM_BYTES // 4} words for rom_ctrl's ROM at 0x00008000\n")
        for i in range(0, ROM_BYTES, 4):
            f.write(f"{int.from_bytes(data[i:i + 4], 'little'):08x}\n")

if __name__ == "__main__":
    main()
