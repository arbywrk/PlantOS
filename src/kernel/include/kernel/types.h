/*
 * types.h - Kernel fixed-width and pointer-sized types.
 */

#ifndef TYPES_H
#define TYPES_H

// Fixed-width integer types
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef signed char        int8;
typedef signed short       int16;
typedef signed int         int32;
typedef signed long long   int64;

// Pointer-sized types
typedef unsigned long uintptr;
typedef unsigned long size_t;
typedef long ssize_t;

// Architecture-dependent types
typedef uintptr paddr_t;
typedef uintptr vaddr_t;
typedef uintptr reg_t;


#endif
