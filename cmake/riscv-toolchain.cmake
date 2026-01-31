# RISC-V 32-bit Toolchain File
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)

# Toolchain prefix
set(PREFIX riscv32-unknown-elf-)

# Compilers
set(CMAKE_C_COMPILER ${PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${PREFIX}gcc)
set(CMAKE_OBJCOPY ${PREFIX}objcopy)
set(CMAKE_OBJDUMP ${PREFIX}objdump)
set(CMAKE_SIZE ${PREFIX}size)

# Don't try to run executables on the build machine
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)