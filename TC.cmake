set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(TOOLCHAIN_ROOT "arm-none-eabi-")
set(CMAKE_C_COMPILER "${TOOLCHAIN_ROOT}gcc")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_ROOT}gcc")
set(CMAKE_AR "${TOOLCHAIN_ROOT}ar")
set(CMAKE_LINKER "${TOOLCHAIN_ROOT}ld")
set(CMAKE_OBJCOPY "${TOOLCHAIN_ROOT}objcopy")
set(CMAKE_RANLIB "${TOOLCHAIN_ROOT}ranlib")
set(CMAKE_SIZE "${TOOLCHAIN_ROOT}size")
set(CMAKE_STRIP "${TOOLCHAIN_ROOT}ld")

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker_script.ld")
set(MCPU "-mcpu=Cortex-M3")
set(MFLOAT_ABI "-mfloat-abi=soft")
set(RUNTIME_LIB "--specs=nano.specs")
set(RUNTIME_LIB_SYS "--specs=nosys.specs")

# Build modes
set(CMAKE_C_FLAGS_INIT "-Wall -Wpedantic -Werror -Wshadow -Wextra")
set(CMAKE_C_FLAGS_DEBUG_INIT    "-g")
set(CMAKE_C_FLAGS_RELEASE_INIT  "")

# Build flags
set(CMAKE_C_FLAGS "${MCPU} -std=gnu11 -mthumb ${MFLOAT_ABI} ${RUNTIME_LIB}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} ${RUNTIME_LIB_SYS} -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
