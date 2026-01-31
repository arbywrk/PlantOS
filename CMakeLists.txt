cmake_minimum_required(VERSION 3.20)

# Architecture configuration
set(ARCH "riscv32" CACHE STRING "Target architecture")
set(ARCH_ISA "rv32imac_zicsr" CACHE STRING "RISC-V ISA string")
set(ARCH_ABI "ilp32" CACHE STRING "RISC-V ABI string")

# Toolchain configuration
set(PREFIX "riscv32-unknown-elf-")
set(CMAKE_C_COMPILER "${PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${PREFIX}g++")
set(CMAKE_ASM_COMPILER "${PREFIX}gcc")
set(CMAKE_OBJCOPY "${PREFIX}objcopy")

project(PlantOS
        LANGUAGES C ASM
        VERSION 0.1.0
)

# Build type handling
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Prevent CMake from adding default flags
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_MINSIZEREL "")

# Common compiler flags
set(COMMON_FLAGS
    -march=${ARCH_ISA}
    -mabi=${ARCH_ABI}
    -mcmodel=medany
    -ffreestanding
    -fno-builtin
    -fno-stack-protector
    -fno-pic
    -nostdlib
    -Wall
    -Wextra
)

# Build-type specific flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(MODE_FLAGS -g -O0)
else()
    set(MODE_FLAGS -O2)
endif()

# Global include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src/kernel
    ${CMAKE_SOURCE_DIR}/src/kernel/include
    ${CMAKE_SOURCE_DIR}/src/kernel/arch/${ARCH}/include
    ${CMAKE_SOURCE_DIR}/src/kernel/arch/${ARCH}
)

# Build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add kernel subdirectory
add_subdirectory(src/kernel)

# Custom targets for running and debugging
# add_custom_target(run
#     COMMAND qemu-system-riscv32 -machine virt -bios none -kernel $<TARGET_FILE:kernel.elf> -nographic
#     DEPENDS kernel.elf
#     COMMENT "Running kernel in QEMU"
# )
#
# add_custom_target(debug-qemu
#     COMMAND qemu-system-riscv32 -machine virt -bios none -kernel $<TARGET_FILE:kernel.elf> -nographic -S -gdb tcp::1234
#     DEPENDS kernel.elf
#     COMMENT "Starting QEMU in debug mode (waiting for GDB on port 1234)"
# )