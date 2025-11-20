add_subdirectory(./dev/)

target_sources(${PROJECT_NAME}
    PRIVATE
    ./main.c
    ./syscalls.c
)
