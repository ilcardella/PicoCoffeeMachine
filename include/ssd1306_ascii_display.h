#pragma once

#include <hardware/i2c.h>
#include <pico/stdlib.h>

#include <lib_coffee_machine/interfaces.h>

#include "ssd1306/GFX.hpp"
#include "ssd1306/logo.hpp"

#include "configuration.h"

class SSD1306AsciiDisplay : public BaseDisplay
{
  public:
    SSD1306AsciiDisplay()
    {
        i2c_init(i2c1, 400000); // Initialize I2C on i2c1 port at 400kHz
        gpio_set_function(Configuration::I2C_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(Configuration::I2C_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(Configuration::I2C_SDA_PIN);
        gpio_pull_up(Configuration::I2C_SCL_PIN);
    }

    ~SSD1306AsciiDisplay()
    {
        delete oled;
        oled = nullptr;
    }

    bool initialise() override
    {
        oled = new GFX(0x3C, 128, 64, i2c1);
        oled->display(logo);
        start_time = to_ms_since_boot(get_absolute_time());
        return true;
    }

    bool clear() override
    {
        if (not is_logo_time())
        {
            oled->clear();
        }
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const char *data) override
    {
        oled->drawString(col, normalise_row(row), data);
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const int &data) override
    {
        // TODO
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const float &data) override
    {
        // TODO
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const double &data) override
    {
        // TODO
        return true;
    }

    bool display() override
    {
        if (not is_logo_time())
        {
            oled->display();
        }
        return true;
    }

  private:
    unsigned normalise_row(const unsigned &row)
    {
        auto actual_row(row);
        if (row > 1)
        {
            actual_row *= 10;
        }
        return actual_row;
    }

    bool is_logo_time()
    {
        auto now = to_ms_since_boot(get_absolute_time());
        if (now - start_time < LOGO_TIME)
        {
            return true;
        }
        return false;
    }

    GFX *oled = nullptr;

    static constexpr unsigned long LOGO_TIME = 2000; // ms
    unsigned long start_time = 0.0;
};
