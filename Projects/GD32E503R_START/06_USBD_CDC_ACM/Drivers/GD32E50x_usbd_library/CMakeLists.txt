project(GD32E50x_usbd_library LANGUAGES C CXX ASM)

add_library(GD32E50x_usbd_library OBJECT
    ${DRIVERS_DIR}/GD32E50x_usbd_library/class/device/cdc/Source/cdc_acm_core.c
    ${DRIVERS_DIR}/GD32E50x_usbd_library/device/Source/usbd_core.c
    ${DRIVERS_DIR}/GD32E50x_usbd_library/device/Source/usbd_enum.c
    ${DRIVERS_DIR}/GD32E50x_usbd_library/device/Source/usbd_pwr.c
    ${DRIVERS_DIR}/GD32E50x_usbd_library/device/Source/usbd_transc.c
    ${DRIVERS_DIR}/GD32E50x_usbd_library/usbd/Source/usbd_lld_core.c
    ${DRIVERS_DIR}/GD32E50x_usbd_library/usbd/Source/usbd_lld_int.c
    )

target_include_directories(GD32E50x_usbd_library PUBLIC
    ${DRIVERS_DIR}/GD32E50x_usbd_library/usbd/Include
    ${DRIVERS_DIR}/GD32E50x_usbd_library/device/Include
    ${DRIVERS_DIR}/GD32E50x_usbd_library/class/device/cdc/Include
    )

target_link_libraries(GD32E50x_usbd_library PUBLIC GD32E503R_START)
