/*
 * gpio.h - Generic kernel-facing GPIO/debug GPIO interface.
 *
 * Responsibilities:
 * - define simple GPIO operations needed by generic code
 *
 */
#ifndef KERNEL_GPIO_H
#define KERNEL_GPIO_H

void gpio_debug_init(void);
void gpio_debug_set(int value);
void gpio_debug_toggle(void);

#endif
