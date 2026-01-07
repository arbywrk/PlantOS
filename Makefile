include config.mk

KERNEL_SRC_ROOT   := src/kernel
BUILD_DIR  := build
ARCH       := riscv

ELF        := $(BUILD_DIR)/kernel.elf
LINKER     := $(KERNEL_SRC_ROOT)/arch/$(ARCH)/qemu_virt.ld

INCLUDES += \
	-I$(KERNEL_SRC_ROOT)/include \
	-I$(KERNEL_SRC_ROOT)/arch/$(ARCH)/include

ARCH_ASM_SRCS := \
	$(KERNEL_SRC_ROOT)/arch/$(ARCH)/boot/entry.S

ARCH_C_SRCS := \
	$(KERNEL_SRC_ROOT)/arch/$(ARCH)/boot/start.c

CONSOLE_C_SRCS := \
	$(KERNEL_SRC_ROOT)/console/console.c

CORE_C_SRCS := \
	$(KERNEL_SRC_ROOT)/core/kmain.c \
	$(KERNEL_SRC_ROOT)/core/kprintf.c \
	$(KERNEL_SRC_ROOT)/core/panic.c

UART_C_SRCS := \
	$(KERNEL_SRC_ROOT)/drivers/uart/sifive.c

LIB_ASM_SRCS := \
	$(KERNEL_SRC_ROOT)/lib/memset.S

LIB_C_SRCS := \
	$(KERNEL_SRC_ROOT)/lib/string.c

C_SRCS := \
	$(ARCH_C_SRCS) \
	$(CONSOLE_C_SRCS) \
	$(CORE_C_SRCS) \
	$(UART_C_SRCS) \
	$(LIB_C_SRCS)

ASM_SRCS := \
	$(ARCH_ASM_SRCS) \
	$(LIB_ASM_SRCS)

OBJS := \
	$(C_SRCS:$(KERNEL_SRC_ROOT)/%.c=$(BUILD_DIR)/%.o) \
	$(ASM_SRCS:$(KERNEL_SRC_ROOT)/%.S=$(BUILD_DIR)/%.o)

# --------------------------
# Build type
# --------------------------
# Usage: make MODE=debug
MODE ?= debug

ifeq ($(MODE),debug)
	CFLAGS_MODE := -g -O0
else
	CFLAGS_MODE := -O2
endif

# Combine common flags with mode flags
CFLAGS := $(COMMON_CFLAGS) $(CFLAGS_MODE)
ASFLAGS := $(COMMON_ASFLAGS) $(CFLAGS_MODE)

# --------------------------
# Targets
# --------------------------
.PHONY: all
all: $(ELF)

# Link kernel
$(ELF): $(OBJS)
	@echo "  LD  $@"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -nostartfiles -T $(LINKER) $^ -o $@

# Compile C sources
$(BUILD_DIR)/%.o: $(KERNEL_SRC_ROOT)/%.c
	@echo "  CC  $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile assembly sources (.S) with GCC + debug info if debug
$(BUILD_DIR)/%.o: $(KERNEL_SRC_ROOT)/%.S
	@echo "  AS  $<"
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) $(INCLUDES) -c $< -o $@

# --------------------------
# Run / Debug
# --------------------------
.PHONY: run
run: $(ELF)
	@qemu-system-riscv32 -machine virt -bios none -kernel $(ELF) -nographic

.PHONY: debug
debug: $(ELF)
	qemu-system-riscv32 -machine virt -bios none -kernel $(ELF) -nographic -S -gdb tcp::1234

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
