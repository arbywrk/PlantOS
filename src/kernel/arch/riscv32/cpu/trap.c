/*
 * trap.c - Machine-mode trap dispatcher for board-specific interrupt backends.
 */

#include <cpu/csr.h>
#include <cpu/interrupt.h>
#include <cpu/trap.h>
#include <kernel/kernel.h>
#include <kernel/panic.h>

#define MCAUSE_INTERRUPT_BIT ((reg_t)1UL << 31)
#define MCAUSE_REASON_MASK   (~MCAUSE_INTERRUPT_BIT)

extern void trap_vector(void);

static int mcause_is_interrupt(reg_t mcause) {
        return (mcause & MCAUSE_INTERRUPT_BIT) != 0;
}

static reg_t mcause_reason(reg_t mcause) {
        return mcause & MCAUSE_REASON_MASK;
}

void trap_init(void) {
        /*
         * Direct mode is deliberate here. A single C-visible trap entry keeps
         * bring-up simpler while the kernel is still learning basic machine
         * timer and external interrupt flow.
         */
        disable_machine_interrupts();
        write_mscratch(0);
        write_mtvec((reg_t)trap_vector);
}

void trap_dump(const struct trap_frame *tf) {
        kprintf("trap mcause=%x mepc=%x mtval=%x mstatus=%x\n",
                (uint32)tf->mcause,
                (uint32)tf->mepc,
                (uint32)tf->mtval,
                (uint32)tf->mstatus);
        kprintf(" ra=%x sp=%x gp=%x tp=%x\n",
                (uint32)tf->ra,
                (uint32)tf->sp,
                (uint32)tf->gp,
                (uint32)tf->tp);
        kprintf(" a0=%x a1=%x a2=%x a3=%x\n",
                (uint32)tf->a0,
                (uint32)tf->a1,
                (uint32)tf->a2,
                (uint32)tf->a3);
        kprintf(" a4=%x a5=%x a6=%x a7=%x\n",
                (uint32)tf->a4,
                (uint32)tf->a5,
                (uint32)tf->a6,
                (uint32)tf->a7);
}

void trap_dispatch(struct trap_frame *tf) {
        reg_t reason = mcause_reason(tf->mcause);

        if (mcause_is_interrupt(tf->mcause)) {
            interrupt_handle_line((unsigned)reason, tf);
            return;
        }

        switch (reason) {
        case 3:
                tf->mepc += 4;
                return;
        case 11:
                tf->mepc += 4;
                return;
        default:
                trap_dump(tf);
                panic("unhandled machine exception");
        }
}
