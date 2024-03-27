cmake_minimum_required(VERSION 3.12)

# Make lib_coffee_machine header available for the target
find_package(lib_coffee_machine CONFIG REQUIRED)

# PICO_SDK_PATH env var must point to the sdk installation path
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(PicoCoffeeMachine C CXX ASM)

# initialize the Pico SDK
pico_sdk_init()

set(LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/extern/daschr/pico-ssd1306/ssd1306.c)
add_library(pico_ssd1306 STATIC ${LIB_SOURCES})
target_include_directories(pico_ssd1306 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/daschr/pico-ssd1306)
target_link_libraries(pico_ssd1306 pico_stdlib hardware_i2c)

# Create the binary from sources
set(SOURCES src/pico_coffee_machine_main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link against lib_coffee_machine
# pico_stdlib library adds commonly used features
target_link_libraries(${PROJECT_NAME}
                        pico_stdlib
                        hardware_i2c
                        lib_coffee_machine::lib_coffee_machine
                        pico_ssd1306
)

# Enable USB serial output over UART ports
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})
