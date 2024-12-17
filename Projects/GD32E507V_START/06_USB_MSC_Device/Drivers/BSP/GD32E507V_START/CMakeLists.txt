project(GD32E507V_START LANGUAGES C CXX ASM)

add_library(GD32E507V_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32E507V_START/gd32e507v_start.c
    )

target_include_directories(GD32E507V_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E507V_START
    )

target_link_libraries(GD32E507V_START PUBLIC GD32E50x_standard_peripheral)
