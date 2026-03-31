/*
 * irq.h - Generic kernel interrupt interface.
 *
 * Responsibilities:
 * - define the interrupt-control API used by generic kernel code
 *
 * Implemented by:
 * - arch and platform layers together
 */
#ifndef KERNEL_IRQ_H
#define KERNEL_IRQ_H

void irq_init(void);
void irq_enable_global(void);
void irq_disable_global(void);

#endif
