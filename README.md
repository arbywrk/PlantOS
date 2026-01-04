# RISC-V Kernel with UART Support

A simple RISC-V kernel that demonstrates:
- Bare metal RISC-V programming in M-mode (machine mode)
- UART (serial) console I/O
- PLIC (Platform-Level Interrupt Controller) configuration
- Interrupt handling with context saving/restoration
- Memory-mapped I/O

## Documentation References

### RISC-V Architecture
- **RISC-V ISA Manual**: https://riscv.org/technical/specifications/
  - Core instruction set reference
  - Chapters 2 (RV32I Base Integer Instruction Set)
  - Chapter 7 (RISC-V Assembly Programmer's Handbook)

- **RISC-V Privileged Specification**: https://github.com/riscv/riscv-isa-manual
  - Machine-mode (M-mode) programming
  - CSR (Control and Status Register) reference
  - Chapter 3: Machine-Level ISA

- **RISC-V Calling Convention**: https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf
  - Function calling conventions (ABI)
  - Register usage conventions
  - Stack management

### Hardware Specifications
- **QEMU RISC-V virt Machine**: https://www.qemu.org/docs/master/system/riscv/virt.html
  - Memory map (RAM at 0x80000000)
  - Device addresses (UART, PLIC, etc.)

- **16550 UART Specification**: https://www.ti.com/lit/ds/symlink/pc16550d.pdf
  - Register map and operation
  - Used for serial console I/O

- **RISC-V PLIC Specification**: https://github.com/riscv/riscv-plic-spec
  - Platform-Level Interrupt Controller
  - Interrupt routing and handling

### Tools
- **GNU Binutils Documentation**: https://sourceware.org/binutils/docs/
  - Assembler (as), Linker (ld), objcopy
  
- **GCC for RISC-V**: https://gcc.gnu.org/onlinedocs/
  - Compiler flags and options
  - Freestanding environment support

## Prerequisites

### Install RISC-V Toolchain (Arch Linux)

```bash
# Install dependencies
sudo pacman -S autoconf automake curl python3 libmpc mpfr gmp gawk \
               base-devel bison flex texinfo gperf libtool patchutils \
               bc zlib expat libusb

# Clone and build toolchain (takes 30-60 minutes)
cd ~
git clone https://github.com/riscv/riscv-gnu-toolchain
cd riscv-gnu-toolchain

# Configure for 32-bit bare metal
./configure --prefix=$HOME/.local/riscv32 \
            --with-arch=rv32imac \
            --with-abi=ilp32

# Build (use all CPU cores)
make -j$(nproc)

# Add to PATH (add to ~/.zshrc or ~/.bashrc)
export PATH="$HOME/.local/riscv32/bin:$PATH"
```

### Install QEMU

```bash
sudo pacman -S qemu-system-riscv
```

### Verify Installation

```bash
riscv32-unknown-elf-gcc --version
riscv32-unknown-elf-as --version
qemu-system-riscv32 --version
```

## Project Structure

```
.
├── kernel.c          # Main kernel code (C)
├── kernel.S          # Low-level assembly code
├── keyboard_map.h    # Keyboard mapping header
├── link.ld           # Linker script
├── Makefile          # Build automation
└── README.md         # This file
```

## Building

### Using Make (Recommended)

```bash
# Build everything
make

# This produces:
# - kernel.o: Compiled C code
# - kernel_asm.o: Assembled assembly code
# - kernel.elf: Linked ELF executable
# - kernel.bin: Raw binary (optional)
```

### Manual Build (for understanding)

```bash
# 1. Assemble the assembly file
riscv32-unknown-elf-as -march=rv32imac_zicsr -mabi=ilp32 \
    kernel.S -o kernel_asm.o

# 2. Compile the C file
riscv32-unknown-elf-gcc -march=rv32imac_zicsr -mabi=ilp32 \
    -ffreestanding -nostdlib -O2 -c kernel.c -o kernel.o

# 3. Link everything together
riscv32-unknown-elf-ld -T link.ld -nostdlib \
    kernel_asm.o kernel.o -o kernel.elf -lgcc
```

## Running

### Run in QEMU

```bash
make run

# Or manually:
qemu-system-riscv32 -machine virt -bios none \
    -kernel kernel.elf -nographic -serial mon:stdio
```

**To exit QEMU**: Press `Ctrl-A` then `X`

### Expected Output

```
RISC-V Kernel with UART Support
Type something...

_
```

Type characters and they will echo back. Press Enter for newlines.

## Debugging

### Using GDB

Terminal 1:
```bash
# Start QEMU with GDB server
make debug

# Or manually:
qemu-system-riscv32 -machine virt -bios none \
    -kernel kernel.elf -nographic -s -S
```

Terminal 2:
```bash
# Connect GDB
riscv32-unknown-elf-gdb kernel.elf

# Inside GDB:
(gdb) target remote localhost:1234
(gdb) break kmain
(gdb) continue
(gdb) step
(gdb) print current_loc
(gdb) x/10i $pc    # Examine 10 instructions at PC
```

### Useful GDB Commands

- `info registers` - Show all register values
- `x/10x $sp` - Examine 10 words at stack pointer
- `disassemble` - Disassemble current function
- `backtrace` - Show call stack
- `layout asm` - Show assembly in TUI mode

## Architecture Flags Explained

### `-march=rv32imac_zicsr`

- `rv32`: RISC-V 32-bit
- `i`: Base Integer instruction set (required)
- `m`: Multiply/Divide extension
- `a`: Atomic instructions extension
- `c`: Compressed (16-bit) instructions extension
- `zicsr`: CSR (Control and Status Register) instructions

### `-mabi=ilp32`

- `i`: Integer calling convention
- `l`: Long is 32-bit
- `p`: Pointer is 32-bit
- `32`: Natural alignment is 32-bit

## How It Works

### 1. Boot Process

1. QEMU loads `kernel.elf` at address `0x80000000`
2. CPU starts executing at `_start` (defined in `kernel.S`)
3. `_start` sets up:
   - Trap vector (`mtvec` CSR) → points to interrupt handler
   - Stack pointer (`sp`) → points to top of 8KB stack
   - Interrupts disabled initially

### 2. Initialization

1. `_start` calls `kmain()` in C
2. `kmain()` initializes:
   - UART (enables receive interrupts)
   - PLIC (routes UART interrupt to CPU)
3. Interrupts enabled via `mstatus.MIE` bit
4. Enter infinite loop with `wfi` (Wait For Interrupt)

### 3. Interrupt Handling

1. Character typed → UART hardware signals interrupt
2. CPU jumps to `trap_vector` (in `kernel.S`)
3. `trap_vector` saves all registers to stack
4. Calls `trap_handler()` in C
5. `trap_handler()` reads character from UART and echoes it
6. `trap_vector` restores registers and returns via `mret`

## Key Differences from x86

| Feature | x86 | RISC-V |
|---------|-----|--------|
| **I/O** | Port I/O (`in`/`out`) | Memory-mapped I/O |
| **Console** | VGA text mode at 0xB8000 | UART at 0x10000000 |
| **Interrupts** | PIC (8259) or APIC | PLIC |
| **IDT** | Interrupt Descriptor Table | Trap vector (`mtvec` CSR) |
| **Return from interrupt** | `iret`/`iretd` | `mret` |
| **Calling convention** | Stack for arguments | Registers a0-a7 for arguments |

## Memory Map (QEMU virt)

| Address Range | Description |
|---------------|-------------|
| `0x00000000` - `0x00000FFF` | Debug device |
| `0x0C000000` - `0x0FFFFFFF` | PLIC (Platform-Level Interrupt Controller) |
| `0x10000000` - `0x100000FF` | UART0 (16550A) |
| `0x80000000` - `0x87FFFFFF` | RAM (128 MB default) |

## Troubleshooting

### "command not found: riscv32-unknown-elf-gcc"

Add toolchain to PATH:
```bash
export PATH="$HOME/.local/riscv32/bin:$PATH"
```

### "Error: unrecognized opcode `csrw`"

Missing `zicsr` extension:
```bash
# Use: -march=rv32imac_zicsr
# Not: -march=rv32imac
```

### "undefined reference to `__umodsi3`"

Missing libgcc (provides software division):
```bash
# Add -lgcc to linker flags
riscv32-unknown-elf-ld ... -lgcc
```

### QEMU hangs / no output

Check that you're using `-nographic` and `-serial mon:stdio`:
```bash
qemu-system-riscv32 -machine virt -bios none \
    -kernel kernel.elf -nographic -serial mon:stdio
```

## Next Steps

### Enhancements to Try

1. **Add more UART features**:
   - Line editing (backspace, delete)
   - Command history
   - ANSI color codes

2. **Implement a simple shell**:
   - Command parsing
   - Built-in commands (help, clear, echo)
   - Memory inspection commands

3. **Add timer interrupts**:
   - RISC-V timer (`mtime` CSR)
   - Periodic tasks
   - Uptime counter

4. **Memory management**:
   - Heap allocator
   - Virtual memory (S-mode)

5. **Additional drivers**:
   - VIRTIO block device (disk)
   - VIRTIO network device

## Learning Resources

### Books
- "Computer Organization and Design: RISC-V Edition" by Patterson & Hennessy
- "The RISC-V Reader" by Patterson & Waterman

### Online Courses
- UC Berkeley CS61C: Great Ideas in Computer Architecture (RISC-V)
- MIT 6.004: Computation Structures

### Tutorials
- RISC-V Assembly Programming Tutorial: https://rv8.io/asm.html
- OSDev Wiki RISC-V: https://wiki.osdev.org/RISC-V

### Communities
- RISC-V International: https://riscv.org/
- OSDev Forums: https://forum.osdev.org/
- r/RISCV: https://reddit.com/r/RISCV

## License

This is educational code. Feel free to use and modify as needed.

## Credits

Adapted from x86 keyboard driver tutorial for RISC-V architecture.
