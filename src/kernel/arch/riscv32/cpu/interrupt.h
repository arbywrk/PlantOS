/*
 * interrupt.h - Board-backed machine interrupt controller interface.
 */

#ifndef PLANTOS_INTERRUPT_H
#define PLANTOS_INTERRUPT_H

#include <kernel/types.h>

struct trap_frame;

enum interrupt_type {
        INTERRUPT_LEVEL = 0,
        INTERRUPT_EDGE = 1,
};

typedef void (*interrupt_handler_t)(struct trap_frame *tf, void *arg);

void interrupt_controller_init(void);
int interrupt_attach(unsigned source,
                     unsigned line,
                     unsigned priority,
                     enum interrupt_type type,
                     interrupt_handler_t handler,
                     void *arg);
void interrupt_global_enable(void);
void interrupt_global_disable(void);
void interrupt_handle_line(unsigned line, struct trap_frame *tf);

#endif // PLANTOS_INTERRUPT_H
