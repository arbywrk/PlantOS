/*
 * kstdarg.h - Freestanding variadic argument helpers for kernel code.
 *
 * Responsibilities:
 * - provide variadic argument support where needed without relying on hosted libc assumptions
 */
#ifndef KSTDARG_H
#define KSTDARG_H

typedef __builtin_va_list va_list;

#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

#endif // KSTDARG_H
