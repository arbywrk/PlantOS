# CMakeLists.txt - Kernel subtree build definition.
#
# Responsibilities:
# - define generic kernel sources
# - export kernel include paths
# - participate in the final target build

set(KERNEL_TARGET kernel.elf)

# Linker script
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/boards/${BOARD}/${BOARD}.ld)

# Source files organized by component
set(ARCH_ASM_SRCS
        arch/${ARCH}/boards/${BOARD}/boot/entry.S
        arch/${ARCH}/cpu/trap.S
)

set(ARCH_HDRS
        arch/${ARCH}/boards/${BOARD}/${BOARD}.ld
        arch/${ARCH}/cpu/csr.h
        arch/${ARCH}/cpu/trap.h
        arch/${ARCH}/cpu/interrupt.h
        arch/${ARCH}/cpu/timer.h
        arch/${ARCH}/include/arch/io.h
        arch/${ARCH}/include/arch/param.h
        arch/${ARCH}/include/arch/riscv.h
        arch/${ARCH}/include/arch/platform/interrupt.h
        arch/${ARCH}/include/arch/platform/timer.h
        arch/${ARCH}/include/arch/platform/uart.h
)

set(ARCH_C_SRCS
        arch/${ARCH}/boards/${BOARD}/boot/start.c
        arch/${ARCH}/cpu/trap.c
        arch/${ARCH}/cpu/interrupt.c
        arch/${ARCH}/cpu/timer.c
)

set(CONSOLE_HDRS
        console/console.h
)

set(CONSOLE_C_SRCS
        console/console.c
)

set(CORE_HDRS
        include/kernel/kernel.h
        include/kernel/panic.h
        include/kernel/types.h
        lib/kstdarg.h
)

set(CORE_C_SRCS
        core/kmain.c
        core/kprintf.c
        core/panic.c
)

set(UART_HDRS
        drivers/uart/uart.h
        include/drivers/uart.h
)

set(UART_C_SRCS
        drivers/uart/uart.c
)

set(LIB_ASM_SRCS
        # lib/memset.S
)

set(LIB_C_SRCS
        # lib/string.c
)

# Combine all sources
set(ALL_SOURCES
        ${ARCH_ASM_SRCS}
        ${ARCH_HDRS}
        ${ARCH_C_SRCS}
        ${CONSOLE_HDRS}
        ${CONSOLE_C_SRCS}
        ${CORE_HDRS}
        ${CORE_C_SRCS}
        ${UART_HDRS}
        ${UART_C_SRCS}
        ${LIB_ASM_SRCS}
        ${LIB_C_SRCS}
)

# Create kernel executable
add_executable(${KERNEL_TARGET} ${ALL_SOURCES})

# Apply compiler flags
target_compile_options(${KERNEL_TARGET} PRIVATE
        ${COMMON_FLAGS}
        ${MODE_FLAGS}
)

# Linker flags
target_link_options(${KERNEL_TARGET} PRIVATE
        ${COMMON_FLAGS}
        ${MODE_FLAGS}
        -nostartfiles
        -T ${LINKER_SCRIPT}
)

# Ensure linker script is treated as a dependency
set_target_properties(${KERNEL_TARGET} PROPERTIES
        LINK_DEPENDS ${LINKER_SCRIPT}
)
