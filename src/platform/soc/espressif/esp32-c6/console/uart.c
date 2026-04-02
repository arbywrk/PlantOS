/*
 * uart.c - ESP32-C6 UART console backend.
 * Implements the soc_console functions used in 'kernel/console/console.c'.
 * This file is chosen at build time using cmake build flags.
 *
 * Responsibilities:
 * - implement console output/input using UART on the ESP32-C6
 * - provide a backend for the generic kernel console interface
 */
#include <soc/uart.h>

#include <stddef.h>

void soc_console_init(void) {
        uart_init();
}

void soc_console_putc(char c) {
        uart_putc(c);
}
