project(GD32E503R_START LANGUAGES C CXX ASM)

add_library(GD32E503R_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32E503R_START/gd32e503r_start.c
    )

target_include_directories(GD32E503R_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E503R_START
    )

target_link_libraries(GD32E503R_START PUBLIC GD32E50x_standard_peripheral)
