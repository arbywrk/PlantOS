#include "../include/arch/param.h"
#include "../include/arch/riscv.h"
#include "../../../include/kernel/types.h"

void kmain();

__attribute__((aligned(16))) char stack0[4096 * HARTS_COUNT];

void start(void) {
        // set Machine Previous Privilege(MPP) mode to Supervisor, for mret.
        uint64 mstatus = read_mstatus();
        // Clear MPP bits.
        mstatus &= ~MSTATUS_MPP_MASK;
        // Set MPP bits to S-Mode bits (01).
        mstatus |= MSTATUS_MPP_S;
        // Set the new mstatus.
        write_mstatus(mstatus);

        /**
         * Machine Exception Program Counter (mepc)
         * On mret, hart jumps to mepc
         * so set mepc to kmain
         * requires gcc -mcmodel=medany
         */
        write_mepc((uint64)kmain);

        /**
         * pagin = hardware virtual memory translation
         * If enabled, the hart will will translate 
         * addresses page tables.
         * Because page tables don't exist at this point
         * disable paging for now.
         * S-mode uses the satp register
         * (Supervisor Address Translation and Protection) 
         * to control the virtual memory paging system.
         * To disable paging set satp to 0;
         */
        write_satp(0);

        /**
         * Delegate all traps (interrupts and exceptions) to supervisor mode.
         * medeleg -> Machine Exception Delegation register
         * In medeleg if bit 'i' is set that means:
         * "delegate exception 'i' to S-mode"
         */
        write_medeleg(0xffff);

        /**
         * mideleg -> Machine Interrupt Delegation register
         * same as medeleg but for interrupts
         */
        write_mideleg(0xffff);

        /**
         * Enable S-mode Timer Interrupts (STI) and External Interrupts (SEI).
         * sie -> Supervisor Interrupt Enable register
         *     -> Mask register that controls which interrupt types 
         *        are allowed if interrupts are enabled.
         * Setting the bits of sie only allows the specific types of interrupts
         * to happen, but does not enable them (a.k.a. does not activate interrupts)
         */
        uint64 sie = read_sie();
        // Flip the bits for Timer Interrupts and External Interrupts.
        sie |= SIE_STIE | SIE_SEIE;
        write_sie(sie);

        /**
         * Physical Memory Protection (PMP) setup using TOR (Top Of Range).
         *
         * PMP is enforced by M-mode and restricts which physical memory
         * regions S-mode and U-mode are allowed to access. Without an
         * explicit PMP entry, S-mode may fault on instruction fetch,
         * loads, or stores—even with paging disabled.
         *
         * TOR mode defines a region [lower_bound, upper_bound):
         *   - For pmp0, the lower bound is implicitly 0x0
         *   - The upper bound is taken from pmpaddr0
         *
         * pmpaddr0 is written with the maximum possible physical address
         * (addresses are shifted right by 2 per the spec), so the region
         * becomes:
         *
         *   [0x00000000, MAX_PHYSICAL_ADDRESS)
         *
         * pmpcfg0 = 0x0f configures pmp0 as:
         *   - R = 1 (read allowed)
         *   - W = 1 (write allowed)
         *   - X = 1 (execute allowed)
         *   - A = TOR (top-of-range addressing)
         *   - L = 0 (not locked, so that M-mode can modify it later)
         *
         * Result:
         *   Supervisor mode has full read/write/execute access to all
         *   physical memory, which is required for the kernel to run
         *   after dropping from M-mode to S-mode.
         */
        write_pmpaddr0(0x3fffffffffffffull);
        write_pmpcfg0(0x0f);

        asm volatile("mret");
}
