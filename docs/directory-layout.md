# Directory Layout

## Root

- `CMakeLists.txt`: top-level build entry
- `cmake/`: toolchain and CMake helper files
- `justfile`: optional shorthand commands
- `README.md`: project entry point
- `docs/`: detailed project documentation

## `src/arch/riscv32/`

RISC-V 32-bit architecture support.

### `boot/`
Generic early bootstrap and trap entry assembly.

### `cpu/`
CPU-local low-level helpers such as IRQ control, CSR helpers, context switching support.

### `include/arch/`
Public architecture-facing headers.

### `timer/`
Very low-level cycle/time helpers that are architectural rather than platform-specific.

### `trap/`
Trap handling logic and trap frame definitions.

## `src/kernel/`

Generic kernel layer.

### `include/kernel/`
Public kernel-facing interfaces used across the project.

### `init/`
Kernel entry logic such as `kmain()`.

### `lib/`
Small freestanding helpers.

### `mm/`
Memory management.

### `sched/`
Scheduling.

### `sync/`
Synchronization primitives.

## `src/platform/`

Hardware-specific support.

## `src/platform/soc/espressif/esp32-c6/`

ESP32-C6 SoC support.

### `boot/`
Platform-specific early C handoff.

### `clock/`
Clock-related setup.

### `console/`
Console backends such as UART or USB Serial/JTAG.

### `gpio/`
GPIO and IO MUX handling.

### `irq/`
Interrupt backend.

### `timer/`
Timer backend.

### `watchdog/`
Watchdog setup / disabling.

### `include/soc/`
Public SoC-facing headers.

## `src/platform/board/waveshare/esp32-c6-dev-kit-n8/`

Board-specific support for the Waveshare ESP32-C6-DEV-KIT-N8.

This is where pin assignments and board wiring knowledge belong.
