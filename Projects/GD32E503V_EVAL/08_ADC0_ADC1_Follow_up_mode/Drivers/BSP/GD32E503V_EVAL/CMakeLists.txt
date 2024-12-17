project(GD32E503V_EVAL LANGUAGES C CXX ASM)

add_library(GD32E503V_EVAL OBJECT
    ${DRIVERS_DIR}/BSP/GD32E503V_EVAL/gd32e503v_eval.c
    ${DRIVERS_DIR}/BSP/GD32E503V_EVAL/gd32e503v_lcd_eval.c
    )

target_include_directories(GD32E503V_EVAL PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E503V_EVAL
    )

target_link_libraries(GD32E503V_EVAL PUBLIC GD32E50x_standard_peripheral)
