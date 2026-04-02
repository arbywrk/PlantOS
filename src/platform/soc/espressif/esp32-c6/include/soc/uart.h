/*
 * uart.h - Public ESP32-C6 UART backend definitions.
 */
#ifndef PLATFORM_SOC_UART_H
#define PLATFORM_SOC_UART_H

void uart_init(void);

void uart_putc(char c);

#endif // PLATFORM_SOC_UART_H
