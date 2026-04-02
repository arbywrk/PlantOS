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

/// Initializes the console.
void console_init(void);

/// Write the character to the console.
/// The console must be initialized with console_init(void).
void console_putc(char ch);

/// Write the buffer to the console.
/// The console must be initialized with console_init(void).
void console_write(const char *buff);

/// Write 'n' characters of the buffer to the console.
/// The console must be initialized with console_init(void).
void console_writen(const char *buff, const size_t n);

#endif
