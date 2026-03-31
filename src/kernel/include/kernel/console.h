/*
 * console.h - Generic kernel console interface.
 *
 * Responsibilities:
 * - define the console API used by generic kernel code
 *
 * Implemented by:
 * - platform-specific console backends
 */
#ifndef KERNEL_CONSOLE_H
#define KERNEL_CONSOLE_H

#include <stddef.h>

void console_init(void);
void console_putc(char ch);
void console_write(const char *buffer, size_t length);

#endif
