# Minimal ESP32-C6 bare-metal build
# Usage:
#   make
#   make flash PORT=/dev/ttyACM1
#   make clean

TARGET      := app
BUILD_DIR   := build
PORT        ?= /dev/ttyACM1
IDF_PATH    ?= $(HOME)/Repos/esp-idf

CROSS       := riscv32-esp-elf
CC          := $(CROSS)-gcc
ESPTOOL     ?= esptool

CFLAGS      := -march=rv32imac_zicsr_zifencei -mabi=ilp32 \
               -ffreestanding -fno-builtin -nostdlib \
               -Wall -Wextra -O2 \
               -I$(IDF_PATH)/components/esp_rom/include \
               -I$(IDF_PATH)/components/esp_rom/esp32c6/include \
               -I$(IDF_PATH)/components/soc/include \
               -I$(IDF_PATH)/components/soc/esp32c6/include

ASFLAGS     := -march=rv32imac_zicsr_zifencei -mabi=ilp32

LDFLAGS     := -march=rv32imac_zicsr_zifencei -mabi=ilp32 \
               -nostdlib \
               -T linker.ld \
               -T $(IDF_PATH)/components/esp_rom/esp32c6/ld/esp32c6.rom.ld \
               -T $(IDF_PATH)/components/esp_rom/esp32c6/ld/esp32c6.rom.api.ld

ELF2IMAGE_FLAGS := --flash-mode dio \
                   --flash-freq 80m \
                   --flash-size 8MB

SRCS_C      := main.c
SRCS_S      := start.S

OBJS_C      := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS_C))
OBJS_S      := $(patsubst %.S,$(BUILD_DIR)/%.o,$(SRCS_S))
OBJS        := $(OBJS_C) $(OBJS_S)

ELF         := $(BUILD_DIR)/$(TARGET).elf
BIN         := $(BUILD_DIR)/$(TARGET).bin

.PHONY: all flash clean dirs

all: $(BIN)

dirs:
	mkdir -p $(BUILD_DIR)

$(ELF): $(OBJS) linker.ld | dirs
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BIN): $(ELF) | dirs
	$(ESPTOOL) --chip esp32c6 elf2image $(ELF2IMAGE_FLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S | dirs
	$(CC) $(ASFLAGS) -c $< -o $@

flash: $(BIN)
	$(ESPTOOL) --chip esp32c6 --port $(PORT) write-flash 0x0 $(BIN)

clean:
	rm -rf $(BUILD_DIR)
