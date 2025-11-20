set(HAL_LIB_NAME "hal")

add_library(${HAL_LIB_NAME} STATIC)

add_subdirectory(hal)

target_include_directories(${HAL_LIB_NAME}
    PRIVATE
    ../include/CMSIS/Include/
    ../include/CMSIS/Device/ST/STM32F1xx/Include/
)
