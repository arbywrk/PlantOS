/*
 * kmain.c - Generic kernel main entry after platform startup.
 *
 * Responsibilities:
 * - run generic kernel initialization
 * - use only documented kernel/arch/platform interfaces
 * - remain mostly independent of concrete SoC/board details
 *
 * Called by:
 * - platform-specific early startup after essential hardware bring-up
 */
#include <kernel/console.h>
#include <kernel/irq.h>
#include <kernel/platform.h>
#include <kernel/printk.h>
#include <kernel/timer.h>
#include <kernel/types.h>

/* kernel_main - Main kernel function
 *
 * Called from _start (in start.S) after CPU initialization.
 * Sets up hardware, displays welcome message, then enters infinite loop
 * waiting for interrupts.
 *
 * Note: This function never returns (infinite loop at end)
 * Reference: RISC-V Calling Convention - caller expects sp/ra preserved
 */
void kmain(void) {
        uint64_t last_reported_tick = 0;

        console_init();
        irq_init();
        timer_init(100);
        irq_enable_global();

        kprintf("  /$$$$$$$  /$$                       /$$      /$$$$$$          \n");
        kprintf("| $$__  $$| $$                      | $$     /$$__  $$          \n");
        kprintf("| $$  \\ $$| $$  /$$$$$$  /$$$$$$$  /$$$$$$  | $$  \\ $$  /$$$$$$$\n");
        kprintf("| $$$$$$$/| $$ |____  $$| $$__  $$|_  $$_/  | $$  | $$ /$$_____/\n");
        kprintf("| $$____/ | $$  /$$$$$$$| $$  \\ $$  | $$    | $$  | $$|  $$$$$$ \n");
        kprintf("| $$      | $$ /$$__  $$| $$  | $$  | $$ /$$| $$  | $$ \\____  $$\n");
        kprintf("| $$      | $$|  $$$$$$$| $$  | $$  |  $$$$/|  $$$$$$/ /$$$$$$$/\n");
        kprintf("|__/      |__/ \\_______/|__/  |__/   \\___/   \\______/ |_______/\n");

        kprintf("Kernel booting...\n");

        for (;;) {
                uint64_t ticks = timer_ticks();

                if (ticks - last_reported_tick >= 100) {
                        last_reported_tick = ticks;
                        kprintf("ticks=%u\n", (uint32_t) ticks);
                }

                platform_idle();
        }
}
