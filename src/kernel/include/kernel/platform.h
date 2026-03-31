/*
 * platform.h - Generic platform lifecycle interface.
 *
 * Responsibilities:
 * - define early platform startup hooks
 * - define platform idle hooks
 * - provide the generic kernel with a hardware-independent entry contract
 */
#ifndef KERNEL_PLATFORM_H
#define KERNEL_PLATFORM_H

void platform_start(void);
void platform_early_init(void);
void platform_idle(void);

#endif
