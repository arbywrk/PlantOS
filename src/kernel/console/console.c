#include "console.h"
#include "../drivers/uart/uart.h"

static struct console *active_console;
static char screen_buffer[SCREENSIZE];
static unsigned int current_loc = 0;

void console_init(void) {
        uart_init();
        console_register(&uart_console);
}

void console_register(struct console *con) {
        active_console = con;
}

void console_putc(char c) {
        if (active_console)
                active_console->putc(c);

        /* Update software screen buffer */
        if (current_loc < SCREENSIZE)
                screen_buffer[current_loc++] = c;
}

/* console_puts - Transmit a null-terminated string
 *
 * Terminal Convention: Converts \n to \r\n (CR+LF) for proper line breaks
 * Most terminal emulators require both carriage return and line feed
 */
void console_puts(const char *s) {
        while (*s) {
                if (*s == '\n')
                        uart_putc('\r');
                uart_putc(*s++);
        }
}
