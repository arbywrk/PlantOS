/*
 * console.h - Minimal console abstraction for early kernel output.
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#define LINES 25
#define COLUMNS_IN_LINE 80
#define SCREENSIZE (COLUMNS_IN_LINE * LINES)

struct console {
        void (*putc)(char c);
};

void console_init(void);
void console_register(struct console *con);
void console_putc(char c);
void console_puts(const char *s);

#endif // CONSOLE_H
