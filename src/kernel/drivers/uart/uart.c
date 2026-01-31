/*
 * uart.c - Minimal 16550 UART driver for QEMU virt.
 */

#include "uart.h"
#include <arch/io.h>
#include "../../console/console.h"

#define UART_BASE 0x10000000UL
#define UART_THR (UART_BASE + 0x00)
#define UART_LSR (UART_BASE + 0x05)
#define UART_LSR_THRE (1 << 5)

void uart_putc(char c) {
        while ((mmio_read8(UART_LSR) & UART_LSR_THRE) == 0)
                ;
        mmio_write8(UART_THR, c);
}


static void uart_console_putc(char c) {
        if (c == '\n')
                uart_putc('\r');
        uart_putc(c);
}

struct console uart_console = {
        .putc = uart_console_putc,
};

void uart_init(void) {
        /* polling */
}
