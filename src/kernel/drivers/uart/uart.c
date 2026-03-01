/*
 * uart.c - UART driver used by the console on the current platform.
 */

#include "uart.h"

#include <arch/io.h>
#include <arch/platform/interrupt.h>
#include <arch/platform/uart.h>
#include <console/console.h>
#include <cpu/interrupt.h>

static uint8 uart_lsr(void) {
        return (uint8) mmio_read8(ARCH_UART_LSR_REG);
}

static uint8 uart_iir(void) {
        return (uint8) mmio_read8(ARCH_UART_IIR_REG);
}

void uart_putc(char c) {
        while ((uart_lsr() & ARCH_UART_LSR_THR_EMPTY) == 0)
                ;
        mmio_write8(ARCH_UART_THR_REG, (uint8) c);
}

static void uart_console_putc(char c) {
        if (c == '\n')
                uart_putc('\r');
        uart_putc(c);
}

struct console uart_console = {
        .putc = uart_console_putc,
};

void uart_init(void) {
        uint16 divisor = arch_uart_divisor();

        /*
         * The 16550 exposes its baud-rate divisor through the DLL/DLM registers
         * only while DLAB is set in LCR. QEMU models the same programming
         * sequence as real hardware, so we do the full initialization here.
         */
        mmio_write8(ARCH_UART_IER_REG, 0);
        mmio_write8(ARCH_UART_LCR_REG, ARCH_UART_LCR_DLAB);
        mmio_write8(ARCH_UART_DLL_REG, (uint8) (divisor & 0xffU));
        mmio_write8(ARCH_UART_DLM_REG, (uint8) (divisor >> 8));
        mmio_write8(ARCH_UART_LCR_REG, ARCH_UART_LCR_WORD_LEN_8);
        mmio_write8(ARCH_UART_FCR_REG,
                ARCH_UART_FCR_FIFO_ENABLE | ARCH_UART_FCR_RX_FIFO_RESET |
                        ARCH_UART_FCR_TX_FIFO_RESET);
        mmio_write8(ARCH_UART_MCR_REG, ARCH_UART_MCR_DTR | ARCH_UART_MCR_RTS);
}

static int uart_getc_nonblock(char *out) {
        if ((uart_lsr() & ARCH_UART_LSR_DATA_READY) == 0)
                return 0;

        *out = (char) mmio_read8(ARCH_UART_RBR_REG);
        return 1;
}

static void uart_irq_handler(struct trap_frame *tf, void *arg) {
        char c;
        uint8 iir;

        (void) tf;
        (void) arg;

        /*
         * The 16550 keeps the interrupt asserted while receive data remains in
         * the FIFO. Draining the FIFO is therefore the "acknowledge" step for
         * the device, after which the PLIC completion drops the external line.
         */
        for (;;) {
                iir = uart_iir();
                if ((iir & ARCH_UART_IIR_NO_INTERRUPT) != 0)
                        break;
                if ((iir & ARCH_UART_IIR_ID_MASK) != ARCH_UART_IIR_RX_READY &&
                        (iir & ARCH_UART_IIR_ID_MASK) != ARCH_UART_IIR_RX_TIMEOUT)
                        break;

                while (uart_getc_nonblock(&c))
                        console_rx(c);
        }
}

void uart_irq_init(void) {
        mmio_write8(ARCH_UART_IER_REG, ARCH_UART_IER_RX_READY);
        interrupt_attach(ARCH_INTERRUPT_UART0_SOURCE,
                ARCH_INTERRUPT_EXTERNAL_CAUSE,
                1,
                INTERRUPT_LEVEL,
                uart_irq_handler,
                0);
}
