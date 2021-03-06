#pragma once

#include <lib_coffee_machine/default_configuration.h>

struct Configuration : public DefaultConfiguration
{
    // Input pin of the water temperature sensor
    inline static constexpr unsigned char WATER_TEMP_PIN = 4;
    // Input pin of the steam temperature sensor
    inline static constexpr unsigned char STEAM_TEMP_PIN = 16;
    // Output PWM pin to control the boiler
    inline static constexpr unsigned char HEATER_SSR_PIN = 8;
    // Input pin to detect steam mode
    inline static constexpr unsigned char STEAM_SWITCH_PIN = 9;

    // SPI interface common pins
    inline static constexpr unsigned char SPI_CLK_PIN = 6;
    inline static constexpr unsigned char SPI_DO_PIN = 5;

    // I2C interface pins
    inline static constexpr unsigned char I2C_SDA_PIN = 2;
    inline static constexpr unsigned char I2C_SCL_PIN = 3;

    // Serial interface
    inline static constexpr unsigned long SERIAL_BAUDRATE = 115200;
    inline static constexpr bool ENABLE_OUTPUT = true;
};
