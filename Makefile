C=gcc
ASM=nasm
LINK=ld

SRC_DIR=src
BUILD_DIR=build

.PHONY: all kernel clean always

# 
# Kernel
#
kernel: $(BUILD_DIR)/kasm.o $(BUILD_DIR)/kc.o
$(BUILD_DIR)/kasm.o: always
	$(ASM) $(SRC_DIR)/kernel.asm -f elf32 -o $(BUILD_DIR)/kasm.o
	$(C) -m32 -ffreestanding -fno-stack-protector -nostdlib -c $(SRC_DIR)/kmain.c -o $(BUILD_DIR)/kc.o
	$(LINK) -m elf_i386 -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel-0 $(BUILD_DIR)/kasm.o $(BUILD_DIR)/kc.o

# 
# Always
#
always:
	mkdir -p $(BUILD_DIR)

# 
# Clean
#
clean:
	rm -rf $(BUILD_DIR)/*
