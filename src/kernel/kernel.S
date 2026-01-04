# RISC-V Bare Metal Kernel - Assembly Startup Code
#
# Provides low-level CPU initialization, memory access primitives,
# and trap (interrupt/exception) handling for M-mode execution.
#
# Documentation References:
# - RISC-V ISA Manual: https://riscv.org/technical/specifications/
#   Instruction set, pseudo-instructions, register conventions
# - RISC-V Privileged Spec: https://github.com/riscv/riscv-isa-manual
#   Machine mode, CSR registers, trap handling (Chapter 3)
# - RISC-V Calling Convention: https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf
#   ABI, register usage, stack conventions

.section .text
.align 4

# Exported symbols
.global _start
.global read_byte
.global write_byte
.global read_word
.global write_word

# Imported C functions
.extern kmain
.extern trap_handler

# ============================================================================
# _start - Kernel Entry Point
# ============================================================================
# First code executed after bootloader. Initializes CPU state and calls kmain.
#
# Initialization sequence:
# 1. Disable interrupts (mstatus.MIE = 0)
# 2. Set trap vector (mtvec = address of trap_vector)
# 3. Set stack pointer (sp = stack_top from linker script)
# 4. Enable external interrupts (mie.MEIE = 1)
# 5. Enable global interrupts (mstatus.MIE = 1)
# 6. Call kmain()
# 7. Enter infinite low-power loop (should never reach here)
#
# Reference: RISC-V Privileged Spec, Chapter 3.1.6 (mstatus) and 3.1.9 (mie)

_start:
    # Disable interrupts during initialization
    # mstatus CSR bit 3 (MIE) = Machine Interrupt Enable
    csrci mstatus, 0x8

    # Set trap vector to point to our handler
    # mtvec CSR holds trap handler address
    # Bits [1:0] = mode: 00 = Direct (all traps go to same handler)
    # Reference: RISC-V Privileged Spec, Section 3.1.7
    la t0, trap_vector
    csrw mtvec, t0

    # Initialize stack pointer
    # Stack grows downward from stack_top (defined in linker script)
    # Reference: RISC-V Calling Convention, stack frame layout
    la sp, stack_top

    # Enable Machine External Interrupts
    # mie CSR bit 11 (MEIE) = Machine External Interrupt Enable
    # This allows PLIC interrupts to reach the CPU
    # Reference: RISC-V Privileged Spec, Section 3.1.9
    li t0, 0x800
    csrs mie, t0

    # Enable global interrupts
    # Set mstatus.MIE = 1 to allow interrupts
    li t0, 0x8
    csrs mstatus, t0

    # Call C kernel main function
    call kmain

    # If kmain returns (shouldn't happen), enter low-power loop
halt_loop:
    wfi              # Wait For Interrupt - sleep until interrupt
    j halt_loop

# ============================================================================
# Memory Access Primitives
# ============================================================================
# RISC-V uses memory-mapped I/O - hardware registers are accessed like
# regular memory. These functions provide a clean C interface.
#
# RISC-V Calling Convention:
# - Arguments: a0-a7 (a0 = first arg, a1 = second arg, etc.)
# - Return value: a0
# - Caller-saved: a0-a7, t0-t6 (can be modified by callee)
# - Callee-saved: s0-s11 (must be preserved by callee)
#
# Reference: RISC-V Calling Convention document

# read_byte(addr) -> byte
# Reads an 8-bit value from memory address, zero-extended to 32 bits
read_byte:
    lbu a0, 0(a0)    # Load Byte Unsigned: a0 = mem[a0], zero-extend
    ret

# write_byte(addr, data)
# Writes lower 8 bits of data to memory address
write_byte:
    sb a1, 0(a0)     # Store Byte: mem[a0] = a1[7:0]
    ret

# read_word(addr) -> word
# Reads a 32-bit word from memory (must be 4-byte aligned)
read_word:
    lw a0, 0(a0)     # Load Word: a0 = mem[a0]
    ret

# write_word(addr, data)
# Writes a 32-bit word to memory (must be 4-byte aligned)
write_word:
    sw a1, 0(a0)     # Store Word: mem[a0] = a1
    ret

# ============================================================================
# trap_vector - Trap (Interrupt/Exception) Entry Point
# ============================================================================
# Hardware jumps here when a trap occurs. We must:
# 1. Save all caller-saved registers
# 2. Call C trap handler
# 3. Restore all registers
# 4. Return from trap (mret instruction)
#
# Trap Flow (Reference: RISC-V Privileged Spec, Chapter 3.1):
# 1. Hardware saves current PC to mepc CSR
# 2. Hardware writes trap cause to mcause CSR
# 3. Hardware disables interrupts (mstatus.MIE = 0)
# 4. Hardware sets PC to mtvec (this function)
# 5. We execute this handler
# 6. mret instruction restores PC from mepc and re-enables interrupts

.align 4
trap_vector:
    # Save context - all caller-saved registers
    # Allocate 64 bytes (16 registers × 4 bytes) on stack
    addi sp, sp, -64

    # Save registers (offset from sp)
    sw ra,  0(sp)    # Return address
    sw t0,  4(sp)    # Temporary registers
    sw t1,  8(sp)
    sw t2, 12(sp)
    sw t3, 16(sp)
    sw t4, 20(sp)
    sw t5, 24(sp)
    sw t6, 28(sp)
    sw a0, 32(sp)    # Argument/return registers
    sw a1, 36(sp)
    sw a2, 40(sp)
    sw a3, 44(sp)
    sw a4, 48(sp)
    sw a5, 52(sp)
    sw a6, 56(sp)
    sw a7, 60(sp)

    # Call C trap handler
    # Follows calling convention: ra saved above, sp aligned
    call trap_handler

    # Restore context - reload all saved registers
    lw a7, 60(sp)
    lw a6, 56(sp)
    lw a5, 52(sp)
    lw a4, 48(sp)
    lw a3, 44(sp)
    lw a2, 40(sp)
    lw a1, 36(sp)
    lw a0, 32(sp)
    lw t6, 28(sp)
    lw t5, 24(sp)
    lw t4, 20(sp)
    lw t3, 16(sp)
    lw t2, 12(sp)
    lw t1,  8(sp)
    lw t0,  4(sp)
    lw ra,  0(sp)

    # Deallocate stack space
    addi sp, sp, 64

    # Return from trap
    # mret does: PC = mepc, mstatus.MIE = mstatus.MPIE, privilege = mstatus.MPP
    # Reference: RISC-V Privileged Spec, Section 3.3.2
    mret
