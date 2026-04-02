/*
 * usb_serial_jtag.h - Public ESP32-C6 USB Serial/JTAG backend definitions.
 */
#ifndef PLATFORM_SOC_USB_SERIAL_JTAG_H
#define PLATFORM_SOC_USB_SERIAL_JTAG_H

void usb_serial_jtag_init(void);

void usb_serial_jtag_putc(char c);

#endif // PLATFORM_SOC_USB_SERIAL_JTAG_H
