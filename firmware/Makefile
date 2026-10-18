# firmware/Makefile — build C tests into ROM images for the testbench.
#
#   make TEST=hello       build/hello/hello.{elf,dis,map,hex}
#   make all              every tests/*.c
#   make list             list the tests
#   make clean
#
# The testbench loads build/<test>/<test>.hex into the chip's ROM.

RISCV_PREFIX ?= /home/user1/riscv/bin/riscv32-unknown-elf-
CC      := $(RISCV_PREFIX)gcc
OBJCOPY := $(RISCV_PREFIX)objcopy
OBJDUMP := $(RISCV_PREFIX)objdump
SIZE    := $(RISCV_PREFIX)size
PYTHON  ?= python3

ARCH    := -march=rv32imc_zicsr_zifencei -mabi=ilp32
CFLAGS  := $(ARCH) -Os -g -std=gnu11 -ffreestanding -fno-builtin -nostdlib \
           -ffunction-sections -fdata-sections -mcmodel=medlow \
           -Wall -Wextra -Werror -Wno-unused-parameter -Iinclude
LDFLAGS := $(ARCH) -nostdlib -nostartfiles -T link/rom.ld -Wl,--gc-sections

LIB     := lib/crt0.S lib/titan.c lib/trap.c lib/drivers.c lib/string.c
HDRS    := $(wildcard include/*.h)
TESTS   := $(basename $(notdir $(wildcard tests/*.c)))
TEST    ?= hello

.PHONY: default all list clean
default: build/$(TEST)/$(TEST).hex
all: $(foreach t,$(TESTS),build/$(t)/$(t).hex)
list:
	@echo $(TESTS)

.SECONDEXPANSION:
build/%.elf: tests/$$(notdir $$*).c $(LIB) $(HDRS) link/rom.ld
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-Map=$(@:.elf=.map) -o $@ $(LIB) tests/$(notdir $*).c
	$(SIZE) $@

build/%.dis: build/%.elf
	$(OBJDUMP) -d -S $< > $@

build/%.bin: build/%.elf
	$(OBJCOPY) -O binary $< $@

build/%.hex: build/%.bin build/%.dis tools/elf2hex.py
	$(PYTHON) tools/elf2hex.py $< $@

.PRECIOUS: build/%.elf build/%.bin build/%.dis
clean:
	rm -rf build
