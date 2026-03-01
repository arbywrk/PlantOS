/*
 * panic.c - Kernel panic handling.
 */

#include <console/console.h>
#include <kernel/panic.h>

void panic(const char *msg) {
        console_puts("panic: ");
        console_puts(msg);
        console_puts("\n");

        for (;;)
                asm volatile("wfi");
}
