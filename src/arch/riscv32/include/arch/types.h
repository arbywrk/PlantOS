/*
 * types.h - Arch-wide semantic type definitions.
 *
 * Responsibilities:
 * - define shared types used across the arch layer
 * - provide semantic aliases such as address/register-sized types where needed
 */
#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef uintptr_t paddr_t;
typedef uintptr_t vaddr_t;
typedef uintptr_t reg_t;

#endif
