/*
 * io.h - Minimal MMIO helpers for RISC-V targets.
 */

#ifndef IO_H
#define IO_H

#include <kernel/types.h>

static inline void mmio_write8(uintptr addr, uint8 val) {
        *(volatile uint8 *) addr = val;
}

static inline uint8 mmio_read8(uintptr addr) {
        return *(volatile uint8 *) addr;
}

static inline void mmio_write32(uintptr addr, uint32 val) {
        *(volatile uint32 *) addr = val;
}

static inline uint32 mmio_read32(uintptr addr) {
        return *(volatile uint32 *) addr;
}

#endif // IO_H
