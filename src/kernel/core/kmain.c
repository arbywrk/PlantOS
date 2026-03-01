/*
 * kmain.c - Kernel entry point after early boot handoff.
 */

#include <console/console.h>
#include <cpu/interrupt.h>
#include <cpu/timer.h>
#include <drivers/uart/uart.h>
#include <kernel/kernel.h>
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
        uint64 last_reported_tick = 0;

        console_init();
        interrupt_controller_init();
        timer_init(100);
        uart_irq_init();
        interrupt_global_enable();

        kprintf("  /$$$$$$$  /$$                       /$$      /$$$$$$          \n");
        kprintf("| $$__  $$| $$                      | $$     /$$__  $$          \n");
        kprintf("| $$  \\ $$| $$  /$$$$$$  /$$$$$$$  /$$$$$$  | $$  \\ $$  /$$$$$$$\n");
        kprintf("| $$$$$$$/| $$ |____  $$| $$__  $$|_  $$_/  | $$  | $$ /$$_____/\n");
        kprintf("| $$____/ | $$  /$$$$$$$| $$  \\ $$  | $$    | $$  | $$|  $$$$$$ \n");
        kprintf("| $$      | $$ /$$__  $$| $$  | $$  | $$ /$$| $$  | $$ \\____  $$\n");
        kprintf("| $$      | $$|  $$$$$$$| $$  | $$  |  $$$$/|  $$$$$$/ /$$$$$$$/\n");
        kprintf("|__/      |__/ \\_______/|__/  |__/   \\___/   \\______/ |_______/\n");

        kprintf("Kernel booting...\n");
        kprintf("QEMU virt machine-mode trap path enabled.\n");
        kprintf("Timer tick: 100 Hz via CLINT. Type in the QEMU UART to exercise PLIC RX interrupts.\n");

        for (;;) {
                uint64 ticks = timer_ticks();

                if (ticks - last_reported_tick >= 100) {
                        last_reported_tick = ticks;
                        kprintf("ticks=%u\n", (uint32)ticks);
                }

                asm volatile("wfi");
        }
}
