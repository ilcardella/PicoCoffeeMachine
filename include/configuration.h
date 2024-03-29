#pragma once

#include <lib_coffee_machine/default/configuration.h>

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
    inline static constexpr unsigned char SPI_CS_PIN = 5;

    // I2C interface pins
    inline static constexpr unsigned char I2C_SDA_PIN = 2;
    inline static constexpr unsigned char I2C_SCL_PIN = 3;

    // Serial interface
    inline static constexpr unsigned long SERIAL_BAUDRATE = 115200;
    inline static constexpr bool ENABLE_OUTPUT = false;

    // PID gains
    inline static constexpr double P_GAIN = 175.0;
    inline static constexpr double I_GAIN = 0.15;
    inline static constexpr double D_GAIN = 0.5;

    inline static constexpr float WATER_TEMP_OFFSET = -12.0f; // degree celsius
    inline static constexpr float STEAM_TEMP_OFFSET = -12.0f; // degree celsius

    inline static constexpr int SAFETY_MAX_TEMP = 145; // degree celsius

    inline static constexpr double TARGET_STEAM_TEMP = 135.0;
};
