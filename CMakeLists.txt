cmake_minimum_required(VERSION 3.31)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(
        CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/riscv-toolchain.cmake"
        CACHE FILEPATH
        "Toolchain file used to cross-compile PlantOS"
    )
endif()

project(PlantOS VERSION 0.1.0 LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CONSOLE "usb" CACHE STRING "Console backend (uart or usb)")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(PlantOSOptions)
include(PlantOSBuildOptions)

message(
    STATUS
    "PlantOS target: arch=${PLANTOS_ARCH} soc=${PLANTOS_SOC_VENDOR}/${PLANTOS_SOC} board=${PLANTOS_BOARD_VENDOR}/${PLANTOS_BOARD}"
)

add_subdirectory(src)

# create anchor file to allow linking
set(PLANTOS_IMAGE_ANCHOR "${CMAKE_CURRENT_BINARY_DIR}/plantos_image_anchor.c")
file(GENERATE OUTPUT "${PLANTOS_IMAGE_ANCHOR}" CONTENT "void plantos_image_anchor(void) {}\n")

add_executable(plantos_image "${PLANTOS_IMAGE_ANCHOR}")
add_executable(PlantOS::image ALIAS plantos_image)

target_link_libraries(
    plantos_image
    PRIVATE
        PlantOS::project_options
        # Keep searching these libraries repeatedly until all symbols are resolved.
        # Because of the circular static dependency:
        # arch[_start] → soc[platform_start] → kernel → arch
        "-Wl,--start-group"
        PlantOS::board
        PlantOS::arch
        PlantOS::kernel
        PlantOS::soc
        "-Wl,--end-group"
)

set_target_properties(
    plantos_image
    PROPERTIES
        OUTPUT_NAME "${PLANTOS_IMAGE_NAME}"
        SUFFIX ".elf"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

set(PLANTOS_MAP_FILE "${CMAKE_BINARY_DIR}/${PLANTOS_IMAGE_NAME}.map")
set(PLANTOS_BIN_FILE "${CMAKE_BINARY_DIR}/${PLANTOS_IMAGE_NAME}.bin")
set(PLANTOS_OBJCOPY "${CMAKE_OBJCOPY}")

if(PLANTOS_OBJCOPY AND NOT IS_ABSOLUTE "${PLANTOS_OBJCOPY}")
    get_filename_component(_plantos_toolchain_bindir "${CMAKE_C_COMPILER}" DIRECTORY)
    if(EXISTS "${_plantos_toolchain_bindir}/${PLANTOS_OBJCOPY}")
        set(PLANTOS_OBJCOPY "${_plantos_toolchain_bindir}/${PLANTOS_OBJCOPY}")
    endif()
endif()

target_link_options(
    plantos_image
    PRIVATE
        "LINKER:-T,${PLANTOS_LINKER_SCRIPT}"
        "LINKER:-Map,${PLANTOS_MAP_FILE}"
        "LINKER:--gc-sections"
)

set_property(TARGET plantos_image APPEND PROPERTY LINK_DEPENDS "${PLANTOS_LINKER_SCRIPT}")

if(PLANTOS_OBJCOPY)
    add_custom_command(
        OUTPUT "${PLANTOS_BIN_FILE}"
        COMMAND "${PLANTOS_OBJCOPY}" -O binary "$<TARGET_FILE:plantos_image>" "${PLANTOS_BIN_FILE}"
        DEPENDS plantos_image
        VERBATIM
    )

    add_custom_target(plantos_binary ALL DEPENDS "${PLANTOS_BIN_FILE}")
endif()
