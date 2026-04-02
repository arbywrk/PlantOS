add_library(plantos_arch STATIC)
add_library(PlantOS::arch ALIAS plantos_arch)

target_link_libraries(
    plantos_arch
    PRIVATE
        PlantOS::project_options
        PlantOS::warnings
)

target_sources(
    plantos_arch
    PRIVATE
        boot/start.S
        boot/trap_entry.S
        cpu/context_switch.S
        cpu/irq.c
        trap/trap.c
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            include/arch/context.h
            include/arch/cpu.h
            include/arch/csr.h
            include/arch/trap.h
    PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
        FILES
            cpu/idle.h
            timer/cycle.h
            trap/trapframe.h
)

target_include_directories(
    plantos_arch
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/kernel/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
