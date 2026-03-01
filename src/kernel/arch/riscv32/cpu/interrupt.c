/*
 * interrupt.c - Machine interrupt backend for the current platform.
 *
 * The kernel uses one machine-mode trap vector and lets this file translate
 * the raw mcause value into an interrupt source dispatch. On QEMU virt that
 * means:
 * - mcause=7  -> machine timer interrupt from CLINT
 * - mcause=11 -> machine external interrupt, then PLIC claim/complete
 */

#include <arch/io.h>
#include <arch/platform/interrupt.h>
#include <cpu/csr.h>
#include <cpu/interrupt.h>
#include <cpu/trap.h>
#include <kernel/panic.h>

struct interrupt_source {
        interrupt_handler_t handler;
        void *arg;
        enum interrupt_type type;
};

static struct interrupt_source s_plic_sources[ARCH_INTERRUPT_PLIC_MAX_SOURCES + 1U];
static struct interrupt_source s_timer_source;

static reg_t save_and_disable_machine_interrupts(void) {
        reg_t mstatus = read_mstatus();

        disable_machine_interrupts();
        return mstatus;
}

static void restore_machine_interrupts(reg_t mstatus) {
        if ((mstatus & MSTATUS_MIE) != 0)
                enable_machine_interrupts();
}

static void interrupt_enable_local_source(unsigned line) {
        reg_t mie = read_mie();

        if (line == ARCH_INTERRUPT_TIMER_CAUSE)
                mie |= MIE_MTIE;
        else if (line == ARCH_INTERRUPT_EXTERNAL_CAUSE)
                mie |= MIE_MEIE;

        write_mie(mie);
}

static void plic_set_priority(unsigned source, unsigned priority) {
        mmio_write32(ARCH_INTERRUPT_PLIC_PRIORITY_REG(source), priority);
}

static void plic_enable_source(unsigned source) {
        unsigned word = source / 32U;
        unsigned bit = source % 32U;
        uint32 value = mmio_read32(ARCH_INTERRUPT_PLIC_ENABLE_REG(word));

        value |= (1UL << bit);
        mmio_write32(ARCH_INTERRUPT_PLIC_ENABLE_REG(word), value);
}

static unsigned plic_claim(void) {
        return mmio_read32(ARCH_INTERRUPT_PLIC_CLAIM_REG);
}

static void plic_complete(unsigned source) {
        mmio_write32(ARCH_INTERRUPT_PLIC_CLAIM_REG, source);
}

void interrupt_controller_init(void) {
        unsigned i;

        interrupt_global_disable();
        write_mie(0);

        for (i = 0; i < ARCH_INTERRUPT_PLIC_ENABLE_WORDS; ++i)
                mmio_write32(ARCH_INTERRUPT_PLIC_ENABLE_REG(i), 0);

        mmio_write32(ARCH_INTERRUPT_PLIC_THRESHOLD_REG, 0);

        for (i = 0; i <= ARCH_INTERRUPT_PLIC_MAX_SOURCES; ++i) {
                s_plic_sources[i].handler = 0;
                s_plic_sources[i].arg = 0;
                s_plic_sources[i].type = INTERRUPT_LEVEL;
                if (i != 0)
                        plic_set_priority(i, 0);
        }

        s_timer_source.handler = 0;
        s_timer_source.arg = 0;
        s_timer_source.type = INTERRUPT_LEVEL;
}

int interrupt_attach(unsigned source,
                     unsigned line,
                     unsigned priority,
                     enum interrupt_type type,
                     interrupt_handler_t handler,
                     void *arg) {
        reg_t mstatus;

        if (priority == 0 || handler == 0)
                return -1;
        if (line != ARCH_INTERRUPT_TIMER_CAUSE && line != ARCH_INTERRUPT_EXTERNAL_CAUSE)
                return -1;

        mstatus = save_and_disable_machine_interrupts();

        if (line == ARCH_INTERRUPT_TIMER_CAUSE) {
                if (source != ARCH_INTERRUPT_TIMER_SOURCE) {
                        restore_machine_interrupts(mstatus);
                        return -1;
                }

                s_timer_source.handler = handler;
                s_timer_source.arg = arg;
                s_timer_source.type = type;
                interrupt_enable_local_source(line);
                restore_machine_interrupts(mstatus);
                return 0;
        }

        if (source == ARCH_INTERRUPT_INVALID_SOURCE || source > ARCH_INTERRUPT_PLIC_MAX_SOURCES) {
                restore_machine_interrupts(mstatus);
                return -1;
        }

        s_plic_sources[source].handler = handler;
        s_plic_sources[source].arg = arg;
        s_plic_sources[source].type = type;

        /*
         * On QEMU virt, machine external interrupts enter through mcause=11.
         * The PLIC then needs a claim/complete transaction to identify and
         * acknowledge the concrete source, such as UART0 on source 10.
         */
        plic_set_priority(source, priority);
        plic_enable_source(source);
        interrupt_enable_local_source(line);

        restore_machine_interrupts(mstatus);
        return 0;
}

void interrupt_global_enable(void) {
        enable_machine_interrupts();
}

void interrupt_global_disable(void) {
        disable_machine_interrupts();
}

void interrupt_handle_line(unsigned line, struct trap_frame *tf) {
        struct interrupt_source *desc;
        unsigned source;

        if (line == ARCH_INTERRUPT_TIMER_CAUSE) {
                if (s_timer_source.handler == 0) {
                        trap_dump(tf);
                        panic("unexpected machine timer interrupt");
                }

                s_timer_source.handler(tf, s_timer_source.arg);
                return;
        }

        if (line != ARCH_INTERRUPT_EXTERNAL_CAUSE) {
                trap_dump(tf);
                panic("unexpected machine interrupt cause");
        }

        source = plic_claim();
        if (source == 0)
                return;
        if (source > ARCH_INTERRUPT_PLIC_MAX_SOURCES) {
                trap_dump(tf);
                panic("invalid PLIC claim");
        }

        desc = &s_plic_sources[source];
        if (desc->handler == 0) {
                plic_complete(source);
                trap_dump(tf);
                panic("unexpected PLIC source");
        }

        desc->handler(tf, desc->arg);
        plic_complete(source);
}
