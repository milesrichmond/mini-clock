target_sources(${HAL_LIB_NAME}
    PRIVATE
    i2c.c
    rtc.c
    gpio.c

    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES
    i2c.h
    rtc.h
    gpio.h
)
