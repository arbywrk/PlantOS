/*
 * console.c - kernel console implementation.
 * Uses sos console implementation.
 *
 * Responsabilities:
 * - Use the low level soc drivers to implement kernel level console interaction.
 */
#include <kernel/console.h>

void soc_console_init(void);
void soc_console_putc(char c);

void console_init(void) {
        soc_console_init();
}

void console_putc(char c) {
        soc_console_putc(c);
}

void console_write(const char *buff) {
        for (size_t i = 0; buff[i] != '\0'; ++i) {
                console_putc(buff[i]);
        }
}

void console_writen(const char *buff, size_t n) {
        for (size_t i = 0; i < n; ++i) {
                console_putc(buff[i]);
        }
}
