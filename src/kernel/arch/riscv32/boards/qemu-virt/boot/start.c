/*
 * start.c - Early machine-mode startup for QEMU virt.
 *
 * The kernel stays in M-mode, clears BSS itself, installs mtvec, and reaches
 * kmain() without any privilege transition. That keeps the control flow short
 * and easy to debug before adding more platform complexity.
 */

#include <arch/param.h>
#include <cpu/trap.h>

void kmain(void);

extern char _bss_start[];
extern char _bss_end[];

__attribute__((section(".noinit.stack"), aligned(16))) char stack0[4096 * HARTS_COUNT];

static void clear_bss(void) {
        char *p = _bss_start;

        while (p < _bss_end)
                *p++ = 0;
}

void start(void) {
        clear_bss();
        trap_init();
        kmain();

        for (;;)
                asm volatile("wfi");
}
