/* RISC-V Kernel
 *
 * Documentation References:
 * - RISC-V Privileged Spec: https://github.com/riscv/riscv-isa-manual
 *   Chapter 3: Machine-Mode ISA, interrupt handling, CSR registers
 * - RISC-V Calling Convention: https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf
 *   Function calling, register usage, ABI
 * - QEMU VIRT Machine Memory Map: https://www.qemu.org/docs/master/system/riscv/virt.html
 *   Device addresses, interrupt routing
 * - 16550 UART Specification: https://www.ti.com/lit/ds/symlink/pc16550d.pdf
 *   Register map, operation sequences
 * - RISC-V PLIC Specification: https://github.com/riscv/riscv-plic-spec
 *   Interrupt controller operation
 */

/* ============================================================================
 * Hardware Configuration - QEMU RISC-V 'virt' Machine
 * ============================================================================
 * Memory Map (from QEMU hw/riscv/virt.c):
 *   0x10000000 - UART0 (16550A compatible)
 *   0x0C000000 - PLIC (Platform-Level Interrupt Controller)
 *   0x80000000 - RAM start
 */

/* UART 16550A Register Offsets
 * Reference: 16550 UART Datasheet, Register Map section
 *
 * The 16550A has 8 registers accessed via base address + offset:
 *   +0: THR (Transmit) / RHR (Receive) - data register
 *   +1: IER - Interrupt Enable Register
 *   +5: LSR - Line Status Register
 *        Bit 0: Data Ready (DR) - set when data available to read
 *        Bit 5: Transmitter Holding Register Empty (THRE) - ready to transmit
 */
#define UART_BASE 0x10000000
#define UART_THR (UART_BASE + 0) /* Transmit Holding Register */
#define UART_RHR (UART_BASE + 0) /* Receive Holding Register (same address) */
#define UART_IER (UART_BASE + 1) /* Interrupt Enable Register */
#define UART_LSR (UART_BASE + 5) /* Line Status Register */
#define UART_LSR_THRE (1 << 5) /* Bit 5: THR Empty flag */
#define UART_LSR_DR (1 << 0) /* Bit 0: Data Ready flag */

/* PLIC (Platform-Level Interrupt Controller) Configuration
 * Reference: RISC-V PLIC Specification
 * https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc
 *
 * The PLIC routes external interrupts to CPU cores (harts). Key registers:
 * - Priority registers: Set interrupt priority (0 = disabled, 1-7 = enabled)
 * - Enable registers: Enable specific interrupts for each hart
 * - Threshold register: Minimum priority to accept (0 = accept all)
 * - Claim/Complete register: Atomically claim pending interrupt and signal completion
 */
#define PLIC_BASE 0x0C000000
#define PLIC_PRIORITY(id) (PLIC_BASE + (id) * 4) /* Set priority for interrupt ID */
#define PLIC_MENABLE(hart) (PLIC_BASE + 0x2000 + (hart) * 0x80) /* Enable bits for hart */
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x1000) /* Priority threshold */
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000) /* Claim/Complete */

/* UART interrupt ID on QEMU virt machine
 * Reference: QEMU hw/riscv/virt.c - UART0_IRQ = 10
 */
#define UART_IRQ 10

/* Screen buffer for maintaining console state
 * Since we're using UART (not VGA), this is a software-only buffer
 * for applications that need to track what's on screen
 */
#define LINES 25
#define COLUMNS_IN_LINE 80
#define SCREENSIZE (COLUMNS_IN_LINE * LINES)

static char screen_buffer[SCREENSIZE];
static unsigned int current_loc = 0;

/* External assembly functions (defined in kernel.S)
 * These follow RISC-V calling convention:
 * - Arguments in a0-a7 registers
 * - Return value in a0
 * - ra (return address) preserved across calls
 * Reference: RISC-V Calling Convention document
 */
extern unsigned char read_byte(unsigned long addr);
extern void write_byte(unsigned long addr, unsigned char data);
extern unsigned int read_word(unsigned long addr);
extern void write_word(unsigned long addr, unsigned int data);

/* ============================================================================
 * UART Driver Functions
 * ============================================================================ */

/* uart_putc - Transmit a single character via UART
 *
 * Waits for the Transmit Holding Register to become empty (LSR bit 5),
 * then writes the character to the THR.
 *
 * Reference: 16550 UART Datasheet, Transmission Operation section
 */
static void uart_putc(char c) {
        while ((read_byte(UART_LSR) & UART_LSR_THRE) == 0)
                ; /* Wait for THR empty */
        write_byte(UART_THR, c);
}

/* uart_puts - Transmit a null-terminated string
 *
 * Terminal Convention: Converts \n to \r\n (CR+LF) for proper line breaks
 * Most terminal emulators require both carriage return and line feed
 */
static void uart_puts(const char *s) {
        while (*s) {
                if (*s == '\n')
                        uart_putc('\r');
                uart_putc(*s++);
        }
}

/* uart_init - Initialize UART for interrupt-driven operation
 *
 * Enables receive data available interrupt (IER bit 0).
 * QEMU pre-initializes baud rate and line control, so we only
 * need to enable interrupts.
 *
 * Reference: 16550 UART Datasheet, IER register description
 */
static void uart_init(void) {
        write_byte(UART_IER, 0x01); /* Enable Received Data Available Interrupt */
}

/* ============================================================================
 * PLIC (Interrupt Controller) Driver Functions
 * ============================================================================ */

/* plic_init - Configure PLIC to route UART interrupts to CPU
 *
 * Three-step initialization:
 * 1. Set interrupt source priority (must be > 0 to enable)
 * 2. Enable the interrupt for this hart (CPU core)
 * 3. Set priority threshold to 0 (accept all enabled interrupts)
 *
 * Reference: RISC-V PLIC Specification, Initialization section
 */
static void plic_init(void) {
        unsigned int hart = 0;

        /* Step 1: Set UART interrupt priority to 1
         * Priorities range from 0 (disabled) to 7 (highest)
         */
        write_word(PLIC_PRIORITY(UART_IRQ), 1);

        /* Step 2: Enable UART interrupt for hart 0
         * Enable bits are packed 32 per register: bit_position = irq_id % 32
         */
        unsigned long enable_addr = PLIC_MENABLE(hart) + (UART_IRQ / 32) * 4;
        unsigned int enable_bit = 1 << (UART_IRQ % 32);
        write_word(enable_addr, enable_bit);

        /* Step 3: Set threshold to 0 to accept all priorities > 0 */
        write_word(PLIC_MTHRESHOLD(hart), 0);
}

/* plic_claim - Atomically claim a pending interrupt
 *
 * Returns the ID of highest-priority pending interrupt (or 0 if none).
 * The act of reading the claim register marks the interrupt as being handled.
 *
 * Reference: RISC-V PLIC Specification, Interrupt Claims section
 */
static unsigned int plic_claim(void) {
        return read_word(PLIC_MCLAIM(0));
}

/* plic_complete - Signal interrupt handling completion
 *
 * Writing the interrupt ID back to the claim/complete register tells
 * the PLIC we're done handling it, allowing it to trigger again.
 *
 * Reference: RISC-V PLIC Specification, Interrupt Completion section
 */
static void plic_complete(unsigned int irq) {
        write_word(PLIC_MCLAIM(0), irq);
}

/* ============================================================================
 * Console Functions
 * ============================================================================ */

/* kprint - Output string to console and update screen buffer */
static void kprint(const char *str) {
        uart_puts(str);
        while (*str && current_loc < SCREENSIZE) {
                screen_buffer[current_loc++] = *str++;
        }
}

/* kprint_newline - Advance to next line in screen buffer */
static void kprint_newline(void) {
        uart_putc('\n');

        /* Round up to next line boundary */
        unsigned int line_offset = current_loc % COLUMNS_IN_LINE;
        if (line_offset != 0) {
                current_loc += COLUMNS_IN_LINE - line_offset;
        }
}

/* clear_screen - Clear console using ANSI escape sequences
 *
 * ESC[2J - Clear entire screen
 * ESC[H  - Move cursor to home (1,1)
 *
 * Reference: ANSI/VT100 Terminal Control Sequences
 */
static void clear_screen(void) {
        uart_puts("\033[2J\033[H");

        for (int i = 0; i < SCREENSIZE; i++) {
                screen_buffer[i] = ' ';
        }
        current_loc = 0;
}

/* ============================================================================
 * Interrupt Handler
 * ============================================================================ */

/* trap_handler - Main interrupt/exception handler
 *
 * Called from trap_vector (assembly) when a trap occurs.
 * Handles UART receive interrupts by echoing input back to terminal.
 *
 * Trap Flow (Reference: RISC-V Privileged Spec, Chapter 3.1.15):
 * 1. Hardware saves PC to mepc CSR
 * 2. Hardware sets mcause CSR with trap cause
 * 3. Hardware jumps to mtvec (trap_vector in our case)
 * 4. trap_vector saves registers and calls this function
 * 5. We claim interrupt from PLIC
 * 6. Process the interrupt
 * 7. Signal completion to PLIC
 * 8. trap_vector restores registers and returns via mret
 */
void trap_handler(void) {
        unsigned int irq = plic_claim();

        if (irq == UART_IRQ) {
                /* Check if data is available (LSR bit 0) */
                if (read_byte(UART_LSR) & UART_LSR_DR) {
                        char c = read_byte(UART_RHR);

                        /* Echo and handle special characters */
                        if (c == '\r' || c == '\n') {
                                kprint_newline();
                        } else if (c == 0x08 || c == 0x7F) { /* Backspace or DEL */
                                if (current_loc > 0) {
                                        current_loc--;
                                        uart_puts("\b \b"); /* Move back, erase, move back */
                                }
                        } else if (c >= 32 && c < 127) { /* Printable ASCII */
                                uart_putc(c);
                                if (current_loc < SCREENSIZE) {
                                        screen_buffer[current_loc++] = c;
                                }
                        }
                }
        }

        /* Signal completion (must be done for every claimed interrupt) */
        if (irq != 0) {
                plic_complete(irq);
        }
}

/* ============================================================================
 * Kernel Entry Point
 * ============================================================================ */

/* kmain - Main kernel function
 *
 * Called from _start (assembly) after CPU initialization.
 * Sets up hardware, displays welcome message, then enters infinite loop
 * waiting for interrupts.
 *
 * Note: This function never returns (infinite loop at end)
 * Reference: RISC-V Calling Convention - caller expects sp/ra preserved
 */
void kmain(void) {
        clear_screen();
        kprint("RamOS RISC-V Kernel\n");
        kprint("Type something...\n\n");

        /* Initialize hardware */
        uart_init();
        plic_init();

        /* Interrupts are enabled by _start before calling kmain
         * (via mstatus.MIE and mie.MEIE CSR bits)
         */

        /* Idle loop - CPU sleeps until interrupt via wfi in halt_loop (assembly) */
        while (1) {
                /* All work done in trap_handler when interrupts occur */
        }
}
