/*
 * timer.c - Board-backed periodic kernel tick.
 */

#include <arch/io.h>
#include <arch/platform/interrupt.h>
#include <arch/platform/timer.h>
#include <cpu/csr.h>
#include <cpu/interrupt.h>
#include <cpu/timer.h>
#include <cpu/trap.h>

static volatile uint64 s_ticks;

static uint64 s_tick_period;

static uint64 clint_mtime_read(void) {
        uint32 hi_before;
        uint32 lo;
        uint32 hi_after;

        /*
         * mtime is a 64-bit register on a 32-bit CPU. The hi/lo/hi retry avoids
         * returning a torn timestamp if the low half wraps between loads.
         */
        do {
                hi_before = mmio_read32(ARCH_TIMER_MTIME_HI_REG);
                lo = mmio_read32(ARCH_TIMER_MTIME_LO_REG);
                hi_after = mmio_read32(ARCH_TIMER_MTIME_HI_REG);
        } while (hi_before != hi_after);

        return ((uint64)hi_after << 32) | lo;
}

static void clint_mtimecmp_write(uint64 value) {
        /*
         * RV32 cannot update mtimecmp atomically. Writing the high half to all
         * ones first prevents the comparator from seeing a partially programmed
         * lower value and firing a spurious early interrupt.
         */
        mmio_write32(ARCH_TIMER_MTIMECMP_HI_REG(0), 0xffffffffUL);
        mmio_write32(ARCH_TIMER_MTIMECMP_LO_REG(0), (uint32)value);
        mmio_write32(ARCH_TIMER_MTIMECMP_HI_REG(0), (uint32)(value >> 32));
}

static void timer_interrupt_handler(struct trap_frame *tf, void *arg) {
        uint64 now;

        (void)tf;
        (void)arg;

        s_ticks++;
        now = clint_mtime_read();
        clint_mtimecmp_write(now + s_tick_period);
}

void timer_init(uint32 tick_hz) {
        uint64 now;

        if (tick_hz == 0)
                tick_hz = 100;

        s_tick_period = ARCH_TIMER_TIMEBASE_HZ / tick_hz;
        if (s_tick_period == 0)
                s_tick_period = 1;

        now = clint_mtime_read();
        clint_mtimecmp_write(now + s_tick_period);

        interrupt_attach(ARCH_INTERRUPT_TIMER_SOURCE,
                         ARCH_INTERRUPT_TIMER_CAUSE,
                         1,
                         INTERRUPT_LEVEL,
                         timer_interrupt_handler,
                         0);
}

uint64 timer_ticks(void) {
        reg_t mstatus = read_mstatus();
        uint64 ticks;

        /*
         * RV32 cannot fetch a shared 64-bit value atomically in one load. Since
         * the timer ISR is the only writer, masking MIE briefly is the simplest
         * correct way to get a stable snapshot.
         */
        disable_machine_interrupts();
        ticks = s_ticks;
        if ((mstatus & MSTATUS_MIE) != 0)
                enable_machine_interrupts();

        return ticks;
}
