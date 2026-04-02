/*
 * irq.c - RISC-V CPU-local interrupt control helpers.
 *
 * Responsibilities:
 * - enable/disable CPU interrupt state at the architectural level
 * - provide generic RV32 interrupt state primitives used by higher layers
 *
 * (not implemented)
 */
#include <arch/csr.h>

void irq_enable(void) {
        write_mstatus(read_mstatus() | MSTATUS_MIE);
}

void irq_disable(void) {
        write_mstatus(read_mstatus() & ~MSTATUS_MIE);
}

reg_t irq_save(void) {
        unsigned long mstatus = read_mstatus();
        irq_disable();
        return mstatus;
}

void irq_restore(reg_t state) {
        if (state & MSTATUS_MIE) {
                irq_enable();
        }
}

void irq_enable_timer(void) {
        // enable Machine Timer Interrupt Extension bit on MIE
        write_mie(read_mie() | MIE_MTIE);
}

void irq_enable_external(void) {
        // TODO: implement
}
