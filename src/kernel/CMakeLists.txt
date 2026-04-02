add_library(plantos_kernel STATIC)
add_library(PlantOS::kernel ALIAS plantos_kernel)

target_link_libraries(
    plantos_kernel
    PUBLIC
        PlantOS::arch
    PRIVATE
        PlantOS::project_options
        PlantOS::warnings
)

target_sources(
    plantos_kernel
    PRIVATE
        init/kmain.c
        lib/kprintf.c
        lib/panic.c
        console/console.c
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            include/kernel/console.h
            include/kernel/gpio.h
            include/kernel/irq.h
            include/kernel/panic.h
            include/kernel/platform.h
            include/kernel/printk.h
            include/kernel/timer.h
            include/kernel/types.h
    PRIVATE
        FILE_SET private_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
        FILES
            lib/kstdarg.h
)

target_include_directories(
    plantos_kernel
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
