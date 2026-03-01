/*
 * console.c - Console routing to the active backend (UART).
 */

#include "console.h"
#include "../drivers/uart/uart.h"

static struct console *active_console;
static char screen_buffer[SCREENSIZE];
static unsigned int current_loc = 0;
static char rx_buffer[128];
static volatile unsigned int rx_head = 0;
static volatile unsigned int rx_tail = 0;

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

void console_rx(char c) {
        unsigned int next;

        if (c == '\r')
                c = '\n';

        /*
         * The ISR is the producer and foreground code is the consumer. On this
         * single-core kernel, fixed-width head/tail updates are sufficient for a
         * simple teaching buffer without locking.
         */
        next = (rx_head + 1U) % sizeof(rx_buffer);
        if (next != rx_tail) {
                rx_buffer[rx_head] = c;
                rx_head = next;
        }

        console_putc(c);
}

int console_getc_nonblock(char *out) {
        if (rx_head == rx_tail)
                return 0;

        *out = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1U) % sizeof(rx_buffer);
        return 1;
}
