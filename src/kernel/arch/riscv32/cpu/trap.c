/*
 * trap.c - Supervisor-mode trap handler for RV32.
 * Not wired yet: stvec is not configured during boot.
 */

#include <kernel/types.h>

static inline reg_t read_scause(void) {
        reg_t value;
        asm volatile("csrr %0, scause" : "=r"(value));
        return value;
}

static inline reg_t read_sepc(void) {
        reg_t value;
        asm volatile("csrr %0, sepc" : "=r"(value));
        return value;
}

static inline reg_t read_stval(void) {
        reg_t value;
        asm volatile("csrr %0, stval" : "=r"(value));
        return value;
}

static inline void write_sepc(reg_t value) {
        asm volatile("csrw sepc, %0" : : "r"(value));
}

void trap_handler(void) {
        reg_t scause = read_scause();
        reg_t sepc = read_sepc();
        reg_t stval = read_stval();

        (void)scause;
        (void)stval;

        /* TODO: decode scause and dispatch to interrupt/exception handlers. */
        write_sepc(sepc);
}
