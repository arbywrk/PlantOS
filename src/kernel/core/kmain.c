/*
 * kmain.c - Kernel entry point after early boot handoff.
 */

#include "../console/console.h"
#include "../include/kernel/kernel.h"
#include "../include/kernel/types.h"

/* kernel_main - Main kernel function
 *
 * Called from _start (assembly) after CPU initialization.
 * Sets up hardware, displays welcome message, then enters infinite loop
 * waiting for interrupts.
 *
 * Note: This function never returns (infinite loop at end)
 * Reference: RISC-V Calling Convention - caller expects sp/ra preserved
 */
void kmain(void) {
        console_init();
        kprintf("  /$$$$$$$  /$$                       /$$      /$$$$$$          \n");
        kprintf("| $$__  $$| $$                      | $$     /$$__  $$          \n");
        kprintf("| $$  \\ $$| $$  /$$$$$$  /$$$$$$$  /$$$$$$  | $$  \\ $$  /$$$$$$$\n");
        kprintf("| $$$$$$$/| $$ |____  $$| $$__  $$|_  $$_/  | $$  | $$ /$$_____/\n");
        kprintf("| $$____/ | $$  /$$$$$$$| $$  \\ $$  | $$    | $$  | $$|  $$$$$$ \n");
        kprintf("| $$      | $$ /$$__  $$| $$  | $$  | $$ /$$| $$  | $$ \\____  $$\n");
        kprintf("| $$      | $$|  $$$$$$$| $$  | $$  |  $$$$/|  $$$$$$/ /$$$$$$$/\n");
        kprintf("|__/      |__/ \\_______/|__/  |__/   \\___/   \\______/ |_______/\n");

        kprintf("Kernel booting...\n");
        // mm_init();
        // trap_enable();
        // timer_init();

        // sched_init();
        // sched_start(); // hands control to schedular (interrupts + schedular run the system)

        /* Idle loop - CPU sleeps until interrupt via wfi in halt_loop (assembly) */
        while (1)
                ;
}
