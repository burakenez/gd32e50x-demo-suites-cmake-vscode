project(GD32EPRTV_START LANGUAGES C CXX ASM)

add_library(GD32EPRTV_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32EPRTV_START/gd32eprtv_start.c
    )

target_include_directories(GD32EPRTV_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32EPRTV_START
    )

target_link_libraries(GD32EPRTV_START PUBLIC GD32E50x_standard_peripheral)
