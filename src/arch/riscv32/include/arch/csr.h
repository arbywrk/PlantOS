/*
 * csr.h - Public RISC-V CSR interface.
 *
 * Responsibilities:
 * - expose architecture-safe CSR helpers to the rest of the project
 */
#ifndef ARCH_CSR_H
#define ARCH_CSR_H

#include <arch/types.h>

// Global Machine Interrupts Enable.
#define MSTATUS_MIE (1UL << 3)

/* MIE is used for Per-Interrupt enable */

// Enable Machine Software Interrupts (IPI, schedular, etc.).
#define MIE_MSIE (1UL << 3)
// Enable Machine Timer Interrupts (CLINT, system timer, etc.).
#define MIE_MTIE (1UL << 7)
// #define MIE_MEIE (1UL << 11) // TODO: replace this with esp32-c6 compatible mie.MXIE and consider
// moving to soc

static inline reg_t read_mstatus(void) {
        reg_t value;
        asm volatile("csrr %0, mstatus" : "=r"(value));
        return value;
}

static inline void write_mstatus(reg_t value) {
        asm volatile("csrw mstatus, %0" : : "r"(value));
}

static inline reg_t read_mie(void) {
        reg_t value;
        asm volatile("csrr %0, mie" : "=r"(value));
        return value;
}

static inline void write_mie(reg_t value) {
        asm volatile("csrw mie, %0" : : "r"(value));
}

static inline reg_t read_mepc(void) {
        reg_t value;
        asm volatile("csrr %0, mepc" : "=r"(value));
        return value;
}

static inline void write_mepc(reg_t value) {
        asm volatile("csrw mepc, %0" : : "r"(value));
}

static inline reg_t read_mcause(void) {
        reg_t value;
        asm volatile("csrr %0, mcause" : "=r"(value));
        return value;
}

static inline reg_t read_mtval(void) {
        reg_t value;
        asm volatile("csrr %0, mtval" : "=r"(value));
        return value;
}

static inline void write_mtvec(reg_t value) {
        asm volatile("csrw mtvec, %0" : : "r"(value));
}

static inline void write_mscratch(reg_t value) {
        asm volatile("csrw mscratch, %0" : : "r"(value));
}

#endif // ARCH_CSR_H
