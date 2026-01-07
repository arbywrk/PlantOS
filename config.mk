PREFIX  := riscv32-unknown-elf-
CC      := $(PREFIX)gcc
AS      := $(PREFIX)gcc
LD      := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy

ARCH_ISA := rv32imac_zicsr
ARCH_ABI := ilp32

COMMON_CFLAGS := \
	-march=$(ARCH_ISA) \
	-mabi=$(ARCH_ABI) \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-pic \
	-nostdlib \
	-Wall -Wextra \
	-Og \
	-g

COMMON_ASFLAGS := \
	-march=$(ARCH_ISA) \
	-mabi=$(ARCH_ABI)
