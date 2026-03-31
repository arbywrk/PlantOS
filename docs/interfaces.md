# Interfaces

## Purpose

This document defines the intended contracts between the generic kernel and the platform-specific implementation.

## `kernel/console.h`

### Purpose
Generic console output interface.

### Expected functions
- `console_init()`
- `console_putc(char c)`
- `console_write(const char *buf, size_t len)`

### Contract
- Must be callable after platform early init.
- Intended for early debug output.
- Backend is platform-specific.

## `kernel/gpio.h`

### Purpose
Generic debug/simple GPIO interface.

### Expected functions
- `gpio_debug_init()`
- `gpio_debug_set(int value)`
- `gpio_debug_toggle()`

### Contract
- Intended for bring-up and debug first.
- Must not expose raw SoC register details to generic kernel code.

## `kernel/irq.h`

### Purpose
Generic interrupt control interface.

### Expected functions
- `irq_init()`
- `irq_enable_global()`
- `irq_disable_global()`

### Contract
- Platform backend owns interrupt controller details.
- Arch layer owns CPU interrupt enable/disable mechanics where appropriate.

## `kernel/timer.h`

### Purpose
Generic tick/time interface.

### Expected functions
- `timer_init(uint32_t hz)`
- `timer_ticks()`

### Contract
- Platform backend owns timer peripheral configuration.
- Generic kernel consumes ticks, not hardware details.

## `kernel/platform.h`

### Purpose
Platform lifecycle hooks.

### Expected functions
- `platform_start()`
- `platform_early_init()`
- `platform_idle()`

### Contract
- `platform_start()` is the first platform-specific C handoff from `_start`.
- `platform_early_init()` performs essential early platform bring-up.
- `platform_idle()` may use `wfi` or another backend-specific idle method.

## Ownership rules

- generic kernel code must not include `soc/...`
- board code may include `soc/...`
- arch code must not depend on board code
