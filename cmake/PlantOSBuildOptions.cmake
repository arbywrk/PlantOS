add_library(plantos_project_options INTERFACE)
add_library(PlantOS::project_options ALIAS plantos_project_options)

add_library(plantos_warnings INTERFACE)
add_library(PlantOS::warnings ALIAS plantos_warnings)

target_compile_features(plantos_project_options INTERFACE c_std_11)

target_compile_options(
    plantos_project_options
    INTERFACE
        -ffreestanding
        -fno-builtin
        -fno-stack-protector
        -fno-pic
        -ffunction-sections
        -fdata-sections
        -march=${PLANTOS_RISCV_ISA}
        -mabi=${PLANTOS_RISCV_ABI}
        -mcmodel=medany
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Release>:-DNDEBUG>
        $<$<CONFIG:RelWithDebInfo>:-O2>
        $<$<CONFIG:RelWithDebInfo>:-g3>
        $<$<CONFIG:RelWithDebInfo>:-DNDEBUG>
        $<$<CONFIG:MinSizeRel>:-Os>
        $<$<CONFIG:MinSizeRel>:-DNDEBUG>
)

target_link_options(
    plantos_project_options
    INTERFACE
        -nostdlib
        -nostartfiles
        -march=${PLANTOS_RISCV_ISA}
        -mabi=${PLANTOS_RISCV_ABI}
        -mcmodel=medany
)

target_compile_options(
    plantos_warnings
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
)
