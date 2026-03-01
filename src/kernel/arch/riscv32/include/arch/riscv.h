/*
 * riscv.h - RISC-V CSR helpers used during early boot.
 */

#ifndef RISCV_H
#define RISCV_H

#include <kernel/types.h>

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

#define SSTATUS_SIE (1u << 1)
#define SSTATUS_SPIE (1u << 5)
#define SSTATUS_SPP (1u << 8)

#define SIE_SSIE (1u << 1)
#define SIE_STIE (1u << 5)
#define SIE_SEIE (1u << 9)

#define SIP_SSIP (1u << 1)
#define SIP_STIP (1u << 5)
#define SIP_SEIP (1u << 9)

#define MCOUNTEREN_TM (1u << 1)
#define MENVCFGH_STCE (1u << 31) /* RV32 high half of menvcfg bit 63 */

static inline reg_t read_sstatus(void) {
        reg_t x;
        asm volatile("csrr %0, sstatus" : "=r"(x));
        return x;
}

static inline void write_sstatus(reg_t x) {
        asm volatile("csrw sstatus, %0" : : "r"(x));
}

static inline void write_stvec(reg_t x) {
        asm volatile("csrw stvec, %0" : : "r"(x));
}

static inline void write_sscratch(reg_t x) {
        asm volatile("csrw sscratch, %0" : : "r"(x));
}

static inline reg_t read_sip(void) {
        reg_t x;
        asm volatile("csrr %0, sip" : "=r"(x));
        return x;
}

static inline void write_sip(reg_t x) {
        asm volatile("csrw sip, %0" : : "r"(x));
}

static inline reg_t read_mcounteren(void) {
        reg_t x;
        asm volatile("csrr %0, mcounteren" : "=r"(x));
        return x;
}

static inline void write_mcounteren(reg_t x) {
        asm volatile("csrw mcounteren, %0" : : "r"(x));
}

static inline reg_t read_menvcfgh(void) {
        reg_t x;
        asm volatile("csrr %0, menvcfgh" : "=r"(x));
        return x;
}

static inline void write_menvcfgh(reg_t x) {
        asm volatile("csrw menvcfgh, %0" : : "r"(x));
}

static inline void write_mtvec(reg_t x) {
        asm volatile("csrw mtvec, %0" : : "r"(x));
}

// mstatus -> controls privilage transitions, interupts, FP state
static inline reg_t read_mstatus() {
        reg_t mstatus;
        asm volatile("csrr %0, mstatus" : "=r"(mstatus));
        return mstatus;
}

static inline void write_mstatus(reg_t mstatus) {
        asm volatile("csrw mstatus, %0" : : "r"(mstatus));
}

static inline void write_mepc(reg_t mepc) {
        asm volatile("csrw mepc, %0" : : "r"(mepc));
}

static inline void write_satp(reg_t satp) {
        asm volatile("csrw satp, %0" : : "r"(satp));
}

static inline void write_medeleg(reg_t medeleg) {
        asm volatile("csrw medeleg, %0" : : "r"(medeleg));
}

static inline void write_mideleg(reg_t mideleg) {
        asm volatile("csrw mideleg, %0" : : "r"(mideleg));
}

static inline reg_t read_sie() {
        reg_t sie;
        asm volatile("csrr %0, sie" : "=r"(sie));
        return sie;
}

static inline void write_sie(reg_t sie) {
        asm volatile("csrw sie, %0" : : "r"(sie));
}

static inline void write_pmpaddr0(reg_t pmpaddr0) {
        asm volatile("csrw pmpaddr0, %0" : : "r"(pmpaddr0));
}

static inline void write_pmpcfg0(reg_t pmpcfg0) {
        asm volatile("csrw pmpcfg0, %0" : : "r"(pmpcfg0));
}
#endif // RISCV_H
