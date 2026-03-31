# PlantOS

```text
 /$$$$$$$  /$$                       /$$      /$$$$$$
| $$__  $$| $$                      | $$     /$$__  $$
| $$  \ $$| $$  /$$$$$$  /$$$$$$$  /$$$$$$  | $$  \ $$  /$$$$$$$
| $$$$$$$/| $$ |____  $$| $$__  $$|_  $$_/  | $$  | $$ /$$_____/
| $$____/ | $$  /$$$$$$$| $$  \ $$  | $$    | $$  | $$|  $$$$$$
| $$      | $$ /$$__  $$| $$  | $$  | $$ /$$| $$  | $$ \____  $$
| $$      | $$|  $$$$$$$| $$  | $$  |  $$$$/|  $$$$$$/ /$$$$$$$/
|__/      |__/ \_______/|__/  |__/   \___/   \______/ |_______/
```

PlantOS is a small RISC-V operating system in early development.

The project is now focused on **real hardware bring-up** on the **Waveshare ESP32-C6-DEV-KIT-N8** rather than active QEMU `virt` support. The long-term direction is to build a small, understandable kernel with clean abstractions between:

* generic kernel code
* architecture-specific RISC-V code
* SoC-specific code
* board-specific code

The current milestone is to build a strong base for running on the ESP32-C6 board: boot, early startup, console, GPIO, timer, interrupts, and later higher-level kernel services.

## Current target

* **Architecture:** RISC-V 32-bit
* **SoC:** Espressif ESP32-C6
* **Board:** Waveshare ESP32-C6-DEV-KIT-N8
* **Toolchain:** `riscv32-esp-elf-*`

## Current status

Working and in progress:

* custom linker script for ESP32-C6 bare-metal image
* custom `_start` path
* early C handoff
* board bring-up on real hardware
* GPIO test / blink working
* project restructuring into `kernel` / `arch` / `platform`

Still evolving:

* console abstraction
* timer abstraction
* interrupt abstraction
* board/SoC split cleanup
* documentation and interface contracts
* future portability back to another target

## Project goals

PlantOS is intended to become a lean, dependable OS/kernel for embedded control tasks, with emphasis on:

* understandable low-level code
* strong internal abstractions
* predictable behavior
* easy debugging
* direct hardware control

The concrete application focus may still evolve.

## Quick start

### Prerequisites

* CMake 3.20+
* Espressif RISC-V toolchain available in `PATH`
* `esptool`
* optional: `just`
* optional: `picocom`

### Configure

```bash
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DARCH=riscv32 \
  -DSOC=esp32-c6 \
  -DBOARD=waveshare-esp32-c6-dev-kit-n8
```

### Build

```bash
cmake --build build
```

If `just` is installed:

```bash
just build
```

## Flashing the board

```bash
esptool --chip esp32c6 --port /dev/ttyACM1 write-flash 0x0 build/kernel.bin
```

If the board does not boot immediately after flashing, press **RESET** once.

If flashing gets stuck at `Connecting...`, manually enter download mode:

1. hold **BOOT**
2. press **RESET**
3. release **RESET**
4. release **BOOT**

Then try flashing again.

## Serial output

In practice, the useful boot log / runtime console has been observed on:

```bash
picocom -b 115200 /dev/ttyACM0
```

The second ACM device may be used by the board as well, but `ttyACM0` has been the useful boot/debug port during bring-up.

**ttyACM0** is how the board appears on my system this might be different depending on the system used.

## Boot flow

The intended high-level boot path is:

1. ESP32-C6 ROM bootloader loads the image
2. entry goes to `_start` in `src/arch/riscv32/boot/start.S`
3. `_start` sets up stack and clears `.bss`
4. `_start` calls platform-specific early C startup
5. platform startup performs SoC/board early init
6. platform startup calls `kmain()`
7. `kmain()` runs generic kernel initialization

## Source tree overview

```text
src/
  arch/       CPU / ISA specific code
  kernel/     architecture-independent kernel code
  platform/   hardware-specific support
```

### `src/arch/riscv32/`

Owns generic RISC-V 32-bit behavior:

* startup path shared by RV32 targets
* trap entry and trap handling
* context switching
* CSR access
* CPU-local IRQ helpers
* low-level idle / cycle helpers

### `src/kernel/`

Owns generic kernel interfaces and logic:

* `kmain()`
* public kernel-facing headers
* future scheduler / memory management / synchronization

### `src/platform/soc/espressif/esp32-c6/`

Owns ESP32-C6 SoC-specific support:

* linker script
* early platform start
* GPIO / IO MUX
* UART / USB serial backends
* timer backend
* interrupt backend
* watchdog / clock setup

### `src/platform/board/waveshare/esp32-c6-dev-kit-n8/`

Owns board-specific details:

* pin assignments
* board-specific defaults
* board wiring knowledge
* board quirks

## Design rules

The intended dependency direction is:

* `kernel/` may depend on `arch/` abstractions, but not directly on `soc/` or `board/`
* `arch/` must not depend on board-specific code
* `board/` may depend on `soc/`
* `soc/` may depend on `arch/`

The goal is to keep `kmain()` and most of the kernel independent from ESP32-C6-specific details.

## Documentation

See:

* `docs/overview.md`
* `docs/directory-layout.md`
* `docs/boot-flow.md`
* `docs/interfaces.md`
* `docs/build.md`
* `docs/esp32-c6.md`
* `docs/board-waveshare-esp32-c6-dev-kit-n8.md`

## Notes

This project is in active restructuring. Build flags, artifact names, and commands may change as the CMake files are cleaned up around the new board-first layout.

