# Overview

PlantOS is a small RISC-V kernel/OS project currently focused on real hardware bring-up on the ESP32-C6.

## Main idea

The codebase is split into three major layers:

- `kernel/`: generic kernel logic and public interfaces
- `arch/riscv32/`: ISA / CPU specific code
- `platform/`: hardware-specific code

Within `platform/`, the split is:

- `soc/`: chip-specific support
- `board/`: board-specific wiring and assignments

## Why this structure exists

The structure is intended to keep the kernel logic mostly independent from:
- exact SoC register layout
- exact board wiring
- boot quirks of a particular target

The current active implementation target is ESP32-C6, but the abstractions are being designed so that another target can be added later.

## Current boot chain

The intended boot chain is:

1. hardware / ROM bootloader loads the image
2. `_start` in `arch/riscv32/boot/start.S`
3. early platform handoff in platform boot code
4. generic `kmain()` in `kernel/init/kmain.c`

## Current focus

Near-term priorities:

- clean architecture boundaries
- documented interfaces
- console backend
- timer backend
- interrupt backend
