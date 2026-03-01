/*
 * interrupt.h - Interrupt controller contract for the current RISC-V platform.
 *
 * The rest of the kernel should only care about:
 * - which mcause values represent timer vs external delivery
 * - which external source number belongs to UART0
 * - how many PLIC enable registers exist
 *
 * A future board port can replace this header and the matching backend logic
 * without rewriting generic kernel code.
 */

#ifndef PLANTOS_ARCH_PLATFORM_INTERRUPT_H
#define PLANTOS_ARCH_PLATFORM_INTERRUPT_H

#define ARCH_INTERRUPT_TIMER_CAUSE         7U
#define ARCH_INTERRUPT_EXTERNAL_CAUSE      11U

#define ARCH_INTERRUPT_INVALID_SOURCE      0U
#define ARCH_INTERRUPT_TIMER_SOURCE        0U
#define ARCH_INTERRUPT_UART0_SOURCE        10U

#define ARCH_INTERRUPT_PLIC_MAX_SOURCES    95U
#define ARCH_INTERRUPT_PLIC_ENABLE_WORDS   3U

#define ARCH_INTERRUPT_PLIC_BASE           0x0c000000UL

#define ARCH_INTERRUPT_PLIC_PRIORITY_REG(source) \
        (ARCH_INTERRUPT_PLIC_BASE + ((source) * 4UL))
#define ARCH_INTERRUPT_PLIC_ENABLE_REG(word) \
        (ARCH_INTERRUPT_PLIC_BASE + 0x2000UL + ((word) * 4UL))
#define ARCH_INTERRUPT_PLIC_THRESHOLD_REG  (ARCH_INTERRUPT_PLIC_BASE + 0x200000UL)
#define ARCH_INTERRUPT_PLIC_CLAIM_REG      (ARCH_INTERRUPT_PLIC_BASE + 0x200004UL)

#endif // PLANTOS_ARCH_PLATFORM_INTERRUPT_H
