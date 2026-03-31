/*
 * trap_entry.S - RISC-V low-level trap entry/exit assembly.
 *
 * Responsibilities:
 * - save machine state on trap entry
 * - build or reference the trap frame layout
 * - transfer control to the C trap handler
 * - restore state and return from trap
 *
 * (not implemented)
 */
