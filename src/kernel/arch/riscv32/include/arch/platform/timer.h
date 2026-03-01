/*
 * timer.h - Timer hardware contract for the current RISC-V platform.
 *
 * The current target is QEMU virt, which uses CLINT mtime/mtimecmp for the
 * machine timer interrupt. A later board can provide a different header with
 * the same intent while keeping timer.c structurally similar.
 */

#ifndef PLANTOS_ARCH_PLATFORM_TIMER_H
#define PLANTOS_ARCH_PLATFORM_TIMER_H

#define ARCH_TIMER_TIMEBASE_HZ             10000000ULL

#define ARCH_TIMER_CLINT_BASE              0x02000000UL

#define ARCH_TIMER_MTIMECMP_LO_REG(hart)   (ARCH_TIMER_CLINT_BASE + 0x4000UL + ((hart) * 8UL))
#define ARCH_TIMER_MTIMECMP_HI_REG(hart)   (ARCH_TIMER_CLINT_BASE + 0x4004UL + ((hart) * 8UL))
#define ARCH_TIMER_MTIME_LO_REG            (ARCH_TIMER_CLINT_BASE + 0xbff8UL)
#define ARCH_TIMER_MTIME_HI_REG            (ARCH_TIMER_CLINT_BASE + 0xbffcUL)

#endif // PLANTOS_ARCH_PLATFORM_TIMER_H
