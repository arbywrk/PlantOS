# Makefile for RISC-V Bare Metal Kernel
#
# Builds a minimal RISC-V kernel for QEMU virt machine.
#
# Documentation:
# - GNU Make Manual: https://www.gnu.org/software/make/manual/
# - RISC-V Toolchain: https://github.com/riscv/riscv-gnu-toolchain

# Toolchain configuration
PREFIX = riscv32-unknown-elf-
CC = $(PREFIX)gcc
AS = $(PREFIX)as
OBJCOPY = $(PREFIX)objcopy

# Architecture: RV32IMAC + Zicsr
# - RV32I: Base 32-bit integer ISA
# - M: Integer multiply/divide
# - A: Atomic instructions
# - C: Compressed instructions (16-bit)
# - Zicsr: CSR (Control and Status Register) instructions
# Reference: RISC-V ISA Manual
ARCH = rv32imac_zicsr
ABI = ilp32

# Compiler flags
# -march: Target architecture
# -mabi: Application Binary Interface (integer, long, pointer = 32-bit)
# -ffreestanding: No standard library or hosted environment
# -nostdlib: Don't link with standard library
# -O2: Optimize for size and speed
CFLAGS = -march=$(ARCH) -mabi=$(ABI) -ffreestanding -nostdlib -O2 -Wall -Wextra

# Assembler flags
ASFLAGS = -march=$(ARCH) -mabi=$(ABI)

# Paths
SRC_DIR = src/kernel
BUILD_DIR = build
LINKER_SCRIPT = src/qemu_virt.ld

# Source files
ASM_SRC = $(SRC_DIR)/kernel.S
C_SRC = $(SRC_DIR)/kernel.c

# Object files (in build directory)
ASM_OBJ = $(BUILD_DIR)/kernel_asm.o
C_OBJ = $(BUILD_DIR)/kernel.o

# Output
ELF = $(BUILD_DIR)/kernel.elf

# QEMU configuration
QEMU = qemu-system-riscv32
QEMU_MACHINE = virt
QEMU_FLAGS = -machine $(QEMU_MACHINE) -bios none -kernel $(ELF) -nographic -serial mon:stdio

# Default target
all: $(ELF)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Link kernel ELF
# Note: Assembly object must come first so _start is at entry point
$(ELF): $(ASM_OBJ) $(C_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -nostartfiles -T $(LINKER_SCRIPT) $(ASM_OBJ) $(C_OBJ) -o $@
	@echo "Linked $@"

# Compile C source
$(C_OBJ): $(C_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $<"

# Assemble assembly source
$(ASM_OBJ): $(ASM_SRC) | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@
	@echo "Assembled $<"

# Run in QEMU
# Exit: Ctrl-A then X
run: $(ELF)
	@echo "Starting QEMU (exit: Ctrl-A then X)..."
	@$(QEMU) $(QEMU_FLAGS)

# Debug with detailed logging
debug: $(ELF)
	@$(QEMU) $(QEMU_FLAGS) -D qemu.log -d in_asm,int,cpu_reset

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned"

.PHONY: all run debug clean
