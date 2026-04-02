/*
 * usb_serial_jtag.c - ESP32-C6 USB Serial/JTAG console backend.
 * Implements the soc_console functions used in 'kernel/console/console.c'.
 * This file is chosen at build time using cmake build flags.
 *
 * Responsibilities:
 * - provide an alternative console backend using the ESP32-C6 USB Serial/JTAG path
 */
#include <soc/usb_serial_jtag.h>

#include <stddef.h>

void soc_console_init(void) {
        usb_serial_jtag_init();
}

void soc_console_putc(char c) {
        usb_serial_jtag_putc(c);
}
