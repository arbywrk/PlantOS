/*
 * panic.c - Kernel panic handling.
 */

#include <kernel/console.h>
#include <kernel/panic.h>
#include <kernel/platform.h>

void panic(const char *msg) {
        static const char prefix[] = "panic: ";
        static const char suffix[] = "\n";

        console_write(prefix);
        if (msg != 0) {
                while (*msg != '\0') {
                        console_putc(*msg++);
                }
        }
        console_write(suffix);

        for (;;) {
                platform_idle();
        }
}
