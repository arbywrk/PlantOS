#ifndef IO_H
#define IO_H

#include <stdint.h>

static inline void mmio_write8(uintptr_t addr, uint8_t val) {
        *(volatile uint8_t *) addr = val;
}

static inline uint8_t mmio_read8(uintptr_t addr) {
        return *(volatile uint8_t *) addr;
}

static inline void mmio_write32(uintptr_t addr, uint32_t val) {
        *(volatile uint32_t *) addr = val;
}

static inline uint32_t mmio_read32(uintptr_t addr) {
        return *(volatile uint32_t *) addr;
}

#endif // IO_H
