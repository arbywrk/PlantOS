/*
 * panic.h - Kernel panic interface.
 *
 * Responsibilities:
 * - define fatal error handling entry points
 */

#ifndef PANIC_H
#define PANIC_H

__attribute__((noreturn)) void panic(const char *msg);

#endif // PANIC_H
