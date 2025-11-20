target_sources(${PROJECT_NAME}
    PRIVATE
    ./display.c
    ./sht45.c

    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES
    ./display.h
    ./sht45.h
)
