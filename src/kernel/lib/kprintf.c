/*
 * kprintf.c - Tiny freestanding formatter for early kernel logging.
 */

#include <kernel/console.h>
#include <kernel/printk.h>

#include "kstdarg.h"

static const char digits[] = "0123456789abcdef";

static void console_put_unsigned(unsigned int value, unsigned int base) {
        char buffer[32];
        unsigned int index = 0;

        do {
                buffer[index++] = digits[value % base];
                value /= base;
        } while (value != 0U);

        while (index > 0U) {
                console_putc(buffer[--index]);
        }
}

static void console_put_signed(int value) {
        unsigned int magnitude = (unsigned int) value;

        if (value < 0) {
                console_putc('-');
                magnitude = (unsigned int) (-value);
        }

        console_put_unsigned(magnitude, 10U);
}

int kprintf(const char *format, ...) {
        va_list arguments;

        va_start(arguments, format);

        for (; *format != '\0'; ++format) {
                if (*format != '%') {
                        console_putc(*format);
                        continue;
                }

                ++format;
                if (*format == '\0') {
                        break;
                }

                switch (*format) {
                case '%':
                        console_putc('%');
                        break;
                case 'c':
                        console_putc((char) va_arg(arguments, int));
                        break;
                case 'd':
                case 'i':
                        console_put_signed(va_arg(arguments, int));
                        break;
                case 's': {
                        const char *string = va_arg(arguments, const char *);

                        if (string == 0) {
                                string = "(null)";
                        }

                        while (*string != '\0') {
                                console_putc(*string++);
                        }
                        break;
                }
                case 'u':
                        console_put_unsigned(va_arg(arguments, unsigned int), 10U);
                        break;
                case 'x':
                case 'p':
                        console_put_unsigned(va_arg(arguments, unsigned int), 16U);
                        break;
                default:
                        console_putc('%');
                        console_putc(*format);
                        break;
                }
        }

        va_end(arguments);
        return 0;
}
