cmake_minimum_required(VERSION 3.12)

# PICO_SDK_PATH env var must point to the sdk installation path
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(PicoCoffeeMachine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Make lib_coffee_machine header available for the target
find_package(lib_coffee_machine CONFIG REQUIRED)

# initialize the Pico SDK
pico_sdk_init()

# Create the binary from sources
set(SOURCES src/pico_coffee_machine_main.cpp
            src/ssd1306/GFX.cpp
            src/ssd1306/SSD1306.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link against lib_coffee_machine
# pico_stdlib library adds commonly used features
target_link_libraries(${PROJECT_NAME}
                        pico_stdlib
                        hardware_i2c
                        lib_coffee_machine::lib_coffee_machine
)

# Enable USB serial output over UART ports
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})
