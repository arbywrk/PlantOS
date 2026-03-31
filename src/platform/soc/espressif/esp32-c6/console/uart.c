/*
 * uart.c - ESP32-C6 UART console backend.
 *
 * Responsibilities:
 * - implement console output/input using UART on the ESP32-C6
 * - provide a backend for the generic kernel console interface
 */
#include <kernel/console.h>

void console_init(void) {
        // Does nothing, no screen support planned
}

void console_putc(char ch) {
        (void) ch;
        // Does nothing, no screen support planned
}

void console_write(const char *buffer, size_t length) {
        for (size_t index = 0; index < length; ++index) {
                console_putc(buffer[index]);
        }
}
