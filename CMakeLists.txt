cmake_minimum_required(VERSION 3.12)

# PICO_SDK_PATH env var must point to the sdk installation path
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(PicoCoffeeMachine)

# initialize the Pico SDK
pico_sdk_init()

set(SOURCES src/hello_world.cpp)

# rest of your project
add_executable(${PROJECT_NAME} ${SOURCES})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT_NAME} pico_stdlib)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})
