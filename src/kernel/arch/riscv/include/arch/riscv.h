#ifndef RISCV_H
#define RISCV_H

#include "../../../../include/kernel/types.h"

/*
* Machine Previous Privilege
* MPP (bits 12:11 of mstatus)
* 00 = U-mode
* 01 = S-mode
* 11 = M-mode
*/
#define MSTATUS_MPP_MASK 0b11UL << 11
#define MSTATUS_MPP_M 0b11L << 11
#define MSTATUS_MPP_S 0b01L << 11
#define MSTATUS_MPP_U 0L

#define SIE_SSIE (1L << 1)
#define SIE_STIE (1L << 5)
#define SIE_SEIE (1L << 9)

// mstatus -> controls privilage transitions, interupts, FP state
static inline uint64 read_mstatus() {
    uint64 mstatus;
    asm volatile("csrr %0, mstatus" : "=r" (mstatus));
    return mstatus;
}

static inline void write_mstatus(uint64 mstatus) {
    asm volatile("csrw mstatus, %0" : : "r" (mstatus));
}

static inline void write_mepc(uint64 mepc) {
    asm volatile("csrw mepc, %0" : : "r" (mepc));
}

static inline void write_satp(uint64 satp) {
    asm volatile("csrw satp, %0" : : "r" (satp));
}

static inline void write_medeleg(uint64 medeleg) {
    asm volatile("csrw medeleg, %0" : : "r" (medeleg));
}

static inline void write_mideleg(uint64 mideleg) {
    asm volatile("csrw mideleg, %0" : : "r" (mideleg));
}

static inline uint64 read_sie() {
    uint64 sie;
    asm volatile("csrr %0, sie" : "=r" (sie));
    return sie;
}

static inline void write_sie(uint64 sie) {
    asm volatile("csrw sie, %0" : : "r" (sie));
}

static inline void write_pmpaddr0(uint64 pmpaddr0) {
    asm volatile("csrw pmpaddr0, %0" : : "r" (pmpaddr0));
}

static inline void write_pmpcfg0(uint64 pmpcfg0) {
    asm volatile("csrw pmpcfg0, %0" : : "r" (pmpcfg0));
}

#endif // RISCV_H