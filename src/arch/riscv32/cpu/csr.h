/*
 * csr.h - Low-level RISC-V CSR access helpers.
 *
 * Responsibilities:
 * - provide helpers/macros for reading and writing control/status registers
 * - hide inline assembly details from higher layers
 */

#ifndef CSR_H
#define CSR_H

#include <kernel/types.h>

#define MSTATUS_MIE (1UL << 3)
#define MIE_MSIE (1UL << 3)
#define MIE_MTIE (1UL << 7)
#define MIE_MEIE (1UL << 11)

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

static inline void enable_machine_interrupts(void) {
        write_mstatus(read_mstatus() | MSTATUS_MIE);
}

static inline void disable_machine_interrupts(void) {
        write_mstatus(read_mstatus() & ~MSTATUS_MIE);
}

#endif // CSR_H
