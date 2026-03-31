/*
 * systimer.c - ESP32-C6 system timer backend.
 *
 * Responsibilities:
 * - implement the timer/tick backend for PlantOS on ESP32-C6
 */
#include <kernel/timer.h>

static volatile uint64_t s_ticks;

void timer_init(uint32_t hz) {
        (void) hz;
        s_ticks = 0;
}

uint64_t timer_ticks(void) {
        return s_ticks;
}
