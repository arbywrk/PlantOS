/*
 * timer.h - Generic kernel timer/tick interface.
 *
 * Responsibilities:
 * - define timer initialization and tick query APIs
 *
 * Implemented by:
 * - platform-specific timer backends
 */
#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <stdint.h>

void timer_init(uint32_t hz);
uint64_t timer_ticks(void);

#endif
