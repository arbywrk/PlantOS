set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(PLANTOS_TOOLCHAIN_PREFIX "riscv32-esp-elf-" CACHE STRING "Cross-toolchain prefix")

if(NOT DEFINED CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER "${PLANTOS_TOOLCHAIN_PREFIX}gcc")
endif()

if(NOT DEFINED CMAKE_ASM_COMPILER)
    set(CMAKE_ASM_COMPILER "${PLANTOS_TOOLCHAIN_PREFIX}gcc")
endif()

get_filename_component(_plantos_toolchain_bindir "${CMAKE_C_COMPILER}" DIRECTORY)

function(_plantos_set_tool tool_var tool_name)
    if(DEFINED ${tool_var})
        return()
    endif()

    set(_plantos_tool_default "${PLANTOS_TOOLCHAIN_PREFIX}${tool_name}")
    set(_plantos_tool_path "${_plantos_toolchain_bindir}/${_plantos_tool_default}")

    if(EXISTS "${_plantos_tool_path}")
        set(${tool_var} "${_plantos_tool_path}" PARENT_SCOPE)
    else()
        set(${tool_var} "${_plantos_tool_default}" PARENT_SCOPE)
    endif()
endfunction()

_plantos_set_tool(CMAKE_AR ar)
_plantos_set_tool(CMAKE_RANLIB ranlib)
_plantos_set_tool(CMAKE_OBJCOPY objcopy)
_plantos_set_tool(CMAKE_OBJDUMP objdump)
_plantos_set_tool(CMAKE_SIZE size)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
