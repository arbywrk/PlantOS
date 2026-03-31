# Boot Flow

## Purpose

This document describes the intended runtime path from reset to `kmain()`.

## High-level sequence

1. ESP32-C6 ROM bootloader loads the image from flash.
2. Control transfers to `_start`.
3. `_start` performs minimal generic runtime setup.
4. `_start` calls platform-specific early startup.
5. Platform early startup performs SoC/board initialization.
6. Platform code calls `kmain()`.
7. `kmain()` runs generic kernel logic.

## Stage 1: ROM bootloader

The chip's ROM bootloader is responsible for loading the bootable image and transferring control to the linked entry point.

PlantOS does not replace the immutable first-stage ROM bootloader.

## Stage 2: `_start`

File:
- `src/arch/riscv32/boot/start.S`

Responsibilities:
- establish initial stack pointer
- clear `.bss`
- hand off to early platform startup

Non-responsibilities:
- no board-specific initialization
- no GPIO/UART/timer logic
- no kernel policy

## Stage 3: platform early startup

File:
- `src/platform/soc/espressif/esp32-c6/boot/start.c`

Responsibilities:
- perform early platform setup
- disable/handle watchdogs if needed
- prepare enough hardware state to safely continue
- call `kmain()`

## Stage 4: generic kernel entry

File:
- `src/kernel/init/kmain.c`

Responsibilities:
- generic kernel bring-up
- initialization sequencing for generic subsystems
- runtime loop / scheduler handoff later

## Notes

The exact ordering of console, timer, IRQ, and watchdog setup may evolve, but the intended separation should remain:
- generic startup in `arch`
- SoC/board startup in `platform`
- generic kernel logic in `kernel`
