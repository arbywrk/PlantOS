/*
 * platform.c - ESP32-C6 platform integration layer.
 *
 * Responsibilities:
 * - connect SoC subsystems to generic kernel/platform interfaces
 * - implement functions such as early init and idle behavior
 *
 * Does not:
 * - implement unrelated generic kernel policy
 */
#include <kernel/platform.h>

void platform_early_init(void) {
}

void platform_idle(void) {
        asm volatile("wfi");
}
