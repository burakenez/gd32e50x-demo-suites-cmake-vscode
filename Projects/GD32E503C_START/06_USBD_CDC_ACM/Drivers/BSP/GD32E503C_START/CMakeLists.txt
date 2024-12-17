project(GD32E503C_START LANGUAGES C CXX ASM)

add_library(GD32E503C_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32E503C_START/gd32e503c_start.c
    )

target_include_directories(GD32E503C_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E503C_START
    )

target_link_libraries(GD32E503C_START PUBLIC GD32E50x_standard_peripheral)
