# Kernel build configuration for the PlantOS kernel image (RV32).

set(KERNEL_TARGET kernel.elf)

# Linker script
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/qemu_virt.ld)

# Source files organized by component
set(ARCH_ASM_SRCS
    arch/${ARCH}/boot/entry.S
)

set(ARCH_C_SRCS
    arch/${ARCH}/boot/start.c
)

set(CONSOLE_C_SRCS
    console/console.c
)

set(CORE_C_SRCS
    core/kmain.c
    core/kprintf.c
    core/panic.c
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
    ${ARCH_C_SRCS}
    ${CONSOLE_C_SRCS}
    ${CORE_C_SRCS}
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

# Component-specific includes
# target_include_directories(${KERNEL_TARGET} PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/console
#     ${CMAKE_CURRENT_SOURCE_DIR}/drivers/uart
# )
