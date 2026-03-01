/*
 * panic.h - Fatal kernel stop helper.
 */

#ifndef PANIC_H
#define PANIC_H

__attribute__((noreturn)) void panic(const char *msg);

#endif // PANIC_H
