#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_putc(char c);

struct console;
extern struct console uart_console;

#endif // UART_H
