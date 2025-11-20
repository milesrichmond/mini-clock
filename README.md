# Mini-Clock

## Purpose

Make a cool little clock that can display temperature, humidity and time.
Additionally, it would be great to be as efficient as possible, but that's not
a now problem.

## Procedure

- First boot / backup reset
  - Setup RTC
  - Setup Peripherals
- Wakeup from standby
  - If required, wake peripherals and read from them (caution for setup time)
  - Read RTC / increment time on display
- Entering standby
  - Put peripherals to sleep
    - This is limited to the displays, since the SHT45 sensor does not have one
  - Put mcu into standby

## Build Instructions

Dependencies:

- arm-none-eabi- toolchain (14.3.1)
- CMake (3.23)
- OpenOCD (0.12.0)

1. Setup build (from the source directory)

    ``` bash
    cmake -B build --toolchain TC.cmake
    ```

    | CMake Option                  |                                       |
    |-------------------------------|---------------------------------------|
    | -DCMAKE_BUILD_TYPE=Debug      | Enables select debug info             |
    | -DCMAKE_BUILD_TYPE=Release    | Regular optimizations (default)       |
    | -DNO_STANDBY=1                | Prevent the mcu from entering standby |

2. Compile

    ``` bash
    make -C build/
    ```

3. Program stm32f103c8t6

    ``` bash
    openocd -f interface/stlink.cfg -f board/stm32f103c8_blue_pill.cfg
    ```

    While openocd is technically for debugging, it can still work to flash programs.

## TODO

- Custom linker and startup script
  - Gets rid of heap
  - Fully written by me
- Custom sycalls library
  - Fully written by me
- Make it as small as possible :D
- Better commenting/documentation of internals and header
