project(GD32E50x_standard_peripheral LANGUAGES C CXX ASM)

# Comment-out unused source files.
add_library(GD32E50x_standard_peripheral OBJECT
	${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_adc.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_bkp.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_can.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_cmp.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_crc.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_ctc.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_dac.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_dbg.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_dma.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_enet.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_exmc.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_exti.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_fmc.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_fwdgt.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_gpio.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_i2c.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_misc.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_pmu.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_rcu.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_rtc.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_sdio.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_shrtimer.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_spi.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_sqpi.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_timer.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_tmu.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_usart.c
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Source/gd32e50x_wwdgt.c
    )

target_include_directories(GD32E50x_standard_peripheral PUBLIC
    ${DRIVERS_DIR}/GD32E50x_standard_peripheral/Include
    )

# CMSIS header only library is linked.
target_link_libraries(GD32E50x_standard_peripheral PUBLIC CMSIS)
