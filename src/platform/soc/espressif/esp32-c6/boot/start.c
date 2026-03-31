/*
 * start.c - ESP32-C6 early platform C startup.
 *
 * Responsibilities:
 * - receive control from generic `_start`
 * - perform essential early SoC/platform initialization
 * - hand off to generic `kmain()`
 */
#include <kernel/platform.h>

extern void kmain(void);

void platform_start(void) {
        platform_early_init();
        kmain();

        for (;;) {
                platform_idle();
        }
}
