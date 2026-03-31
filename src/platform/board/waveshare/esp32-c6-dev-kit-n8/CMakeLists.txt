add_library(plantos_board STATIC)
add_library(PlantOS::board ALIAS plantos_board)

target_link_libraries(
    plantos_board
    PUBLIC
        PlantOS::soc
    PRIVATE
        PlantOS::project_options
        PlantOS::warnings
)

target_sources(
    plantos_board
    PRIVATE
        board.c
)
