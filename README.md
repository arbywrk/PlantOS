# PlantOS
PlantOS is a small, bare-metal RISC-V OS in early development. The long-term goal is to run on a greenhouse controller dev board and provide the minimal, reliable services needed to read sensors, drive actuators, and keep a small environment stable.

This project is currently a work in progress and focuses on core bring-up and foundational kernel pieces.

## Goal
Build a lean, dependable OS for a mini greenhouse controller, with predictable timing, simple debugging, and hardware-level control over sensors and actuators.

## Current Status (WIP)
- 32-bit RISC-V boot + basic kernel entry
- UART console + `kprintf` for early logging
- Early trap/interrupt handling code (not fully wired in yet)
- Linker script targeting QEMU virt by default

## Build
### Prerequisites
- CMake 3.20+
- RISC-V GCC toolchain in PATH (`riscv32-unknown-elf-*`)
- (Optional) QEMU for RISC-V if you want to emulate the target

### Configure and build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Build output:
- `build/kernel.elf`

### Optional: run in QEMU
```bash
qemu-system-riscv32 -machine virt -bios none -kernel build/kernel.elf -nographic
```

If you are targeting a specific dev board, you will need to adjust the linker script and board-specific bring-up to match its memory map and peripherals.

## Configuration
You can override the default architecture settings via CMake cache variables:
- `ARCH` (default: `riscv32`)
- `ARCH_ISA` (default: `rv32imac_zicsr`)
- `ARCH_ABI` (default: `ilp32`)

Example:
```bash
cmake -S . -B build -DARCH_ISA=rv32imac -DARCH_ABI=ilp32
```

## Project Layout
- `src/kernel/arch` - architecture-specific startup, traps, and linker scripts
- `src/kernel/core` - kernel entry and core utilities
- `src/kernel/console` - console abstraction
- `src/kernel/drivers` - device drivers (currently UART)
- `src/kernel/lib` - low-level helpers (placeholder)
- `cmake` - CMake tooling

## Roadmap (high level)
- Solidify board bring-up: clocks, memory map, interrupt controller
- Expand hardware drivers: GPIO, timers, I2C/SPI, ADC
- Basic scheduler + timekeeping
- Simple device abstraction layer for sensors/actuators
- Data logging + configuration storage
- Communications interface for telemetry and control
