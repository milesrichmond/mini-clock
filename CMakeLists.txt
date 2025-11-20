# Project settings
cmake_minimum_required(VERSION 3.23.0)
project("mini-clock")
enable_language(ASM)

# Source tree
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/startup_stm32f103c8tx.s)

if(NO_STANDBY)
    add_definitions(-DNO_STANDBY)
endif()

add_subdirectory(hal)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PUBLIC hal)

# Include
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ./include/CMSIS/Include
    ./include/CMSIS/Device/ST/STM32F1xx/Include

    ${PROJECT_SOURCE_DIR}/hal
)
