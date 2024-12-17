project(GD32E507Z_EVAL LANGUAGES C CXX ASM)

add_library(GD32E507Z_EVAL OBJECT
    ${DRIVERS_DIR}/BSP/GD32E507Z_EVAL/gd32e507z_eval.c
    )

target_include_directories(GD32E507Z_EVAL PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E507Z_EVAL
    )

target_link_libraries(GD32E507Z_EVAL PUBLIC GD32E50x_standard_peripheral)
