/*
 * timer.h - Board-backed periodic kernel tick driver.
 */

#ifndef PLANTOS_TIMER_H
#define PLANTOS_TIMER_H

#include <kernel/types.h>

void timer_init(uint32 tick_hz);
uint64 timer_ticks(void);

#endif // PLANTOS_TIMER_H
