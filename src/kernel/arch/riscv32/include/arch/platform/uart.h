/*
 * uart.h - UART hardware contract for the current RISC-V platform.
 *
 * The active target is the NS16550A UART on QEMU virt. Keeping the addresses
 * and bit definitions in one arch header makes the driver easy to retarget
 * later without teaching the console layer about a specific board.
 */

#ifndef PLANTOS_ARCH_PLATFORM_UART_H
#define PLANTOS_ARCH_PLATFORM_UART_H

#include <kernel/types.h>

#define ARCH_UART_CLOCK_HZ                 3686400U
#define ARCH_UART_BAUD                     115200U

#define ARCH_UART0_BASE                    0x10000000UL

#define ARCH_UART_RBR_REG                  (ARCH_UART0_BASE + 0x00)
#define ARCH_UART_THR_REG                  (ARCH_UART0_BASE + 0x00)
#define ARCH_UART_DLL_REG                  (ARCH_UART0_BASE + 0x00)
#define ARCH_UART_IER_REG                  (ARCH_UART0_BASE + 0x01)
#define ARCH_UART_DLM_REG                  (ARCH_UART0_BASE + 0x01)
#define ARCH_UART_IIR_REG                  (ARCH_UART0_BASE + 0x02)
#define ARCH_UART_FCR_REG                  (ARCH_UART0_BASE + 0x02)
#define ARCH_UART_LCR_REG                  (ARCH_UART0_BASE + 0x03)
#define ARCH_UART_MCR_REG                  (ARCH_UART0_BASE + 0x04)
#define ARCH_UART_LSR_REG                  (ARCH_UART0_BASE + 0x05)

#define ARCH_UART_IER_RX_READY             (1U << 0)

#define ARCH_UART_IIR_NO_INTERRUPT         (1U << 0)
#define ARCH_UART_IIR_ID_MASK              0x0eU
#define ARCH_UART_IIR_RX_READY             0x04U
#define ARCH_UART_IIR_RX_TIMEOUT           0x0cU

#define ARCH_UART_FCR_FIFO_ENABLE          (1U << 0)
#define ARCH_UART_FCR_RX_FIFO_RESET        (1U << 1)
#define ARCH_UART_FCR_TX_FIFO_RESET        (1U << 2)

#define ARCH_UART_LCR_WORD_LEN_8           0x03U
#define ARCH_UART_LCR_DLAB                 (1U << 7)

#define ARCH_UART_MCR_DTR                  (1U << 0)
#define ARCH_UART_MCR_RTS                  (1U << 1)

#define ARCH_UART_LSR_DATA_READY           (1U << 0)
#define ARCH_UART_LSR_THR_EMPTY            (1U << 5)

static inline uint16 arch_uart_divisor(void) {
        return (uint16)(ARCH_UART_CLOCK_HZ / (16U * ARCH_UART_BAUD));
}

#endif // PLANTOS_ARCH_PLATFORM_UART_H
