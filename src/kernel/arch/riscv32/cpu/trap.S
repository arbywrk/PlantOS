# trap.S - RV32 supervisor trap vector (not wired in yet).

.section .text

.global trap_vector

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
    # sret does: PC = sepc, sstatus.SIE = sstatus.SPIE, privilege = sstatus.SPP
    # Reference: RISC-V Privileged Spec, Section 3.2.2
    sret
