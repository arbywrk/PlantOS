# PlantOS
PlantOS is a small, bare-metal RISC-V OS in early development. The long-term goal is to run on a greenhouse controller dev board and provide the minimal, reliable services needed to read sensors, drive actuators, and keep a small environment stable.

This project is currently a work in progress and focuses on core bring-up and foundational kernel pieces.

## Goal
Build a lean, dependable OS for a mini greenhouse controller, with predictable timing, simple debugging, and hardware-level control over sensors and actuators.

## Current Status (WIP)
- 32-bit RISC-V boot + basic kernel entry
- UART console + `kprintf` for early logging
- Machine-mode trap/interrupt path for `qemu-virt`
- `qemu-virt` is the only active target for now

## Build
### Prerequisites
- CMake 3.20+
- RISC-V GCC toolchain in PATH (`riscv32-unknown-elf-*`)
- (Optional) QEMU for RISC-V if you want to emulate the target
- (Optional) Just

### Configure and build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBOARD=qemu-virt
cmake --build build
# or if you have just installed as well
just build
```

Build output:
- `build/kernel.elf`

### Optional: run in QEMU
```bash
qemu-system-riscv32 -machine virt -bios none -kernel build/kernel.elf -nographic
# or if you have just installed as well
just run
# to exit qemu: Ctr+A Z 
```

The current code keeps device details behind `arch/platform/*.h` so the UART,
timer, and interrupt code can be retargeted later without rewriting the kernel
core. A real board port will still need its own linker script and early startup.

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

## Roadmap
- Solidify board bring-up: clocks, memory map, interrupt controller
- Expand hardware drivers: GPIO, timers, I2C/SPI, ADC
- Basic scheduler + timekeeping
- Simple device abstraction layer for sensors/actuators
- Data logging + configuration storage
- Communications interface for telemetry and control
