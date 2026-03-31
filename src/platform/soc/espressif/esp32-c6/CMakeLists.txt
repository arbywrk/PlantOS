add_library(plantos_soc STATIC)
add_library(PlantOS::soc ALIAS plantos_soc)

target_link_libraries(
    plantos_soc
    PUBLIC
        PlantOS::kernel
        PlantOS::arch
    PRIVATE
        PlantOS::project_options
        PlantOS::warnings
)

target_sources(
    plantos_soc
    PRIVATE
        boot/start.c
        clock/clock.c
        console/uart.c
        console/usb_serial_jtag.c
        gpio/gpio.c
        gpio/iomux.c
        irq/intc.c
        platform.c
        timer/systimer.c
        watchdog/wdt.c
    PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            include/soc/gpio.h
            include/soc/iomux.h
            include/soc/irq.h
            include/soc/regs.h
            include/soc/timer.h
            include/soc/uart.h
            include/soc/wdt.h
)

target_include_directories(
    plantos_soc
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
