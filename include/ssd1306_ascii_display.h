#pragma once

#include <lib_coffee_machine/interfaces.h>
#include <ssd1306_ascii_pico/ssd1306_ascii_i2c.h>

#include "configuration.h"

class SSD1306AsciiDisplay : public BaseDisplay
{
  public:
    SSD1306AsciiDisplay()
        : bus(1, Configuration::I2C_SDA_PIN, Configuration::I2C_SCL_PIN, 400000L),
          display(bus)
    {
    }

    bool initialise() override
    {
        display.begin(&Adafruit128x64, 0x3C);
        display.setFont(Adafruit5x7);
        clear();
        return true;
    }

    bool clear() override
    {
        display.clear();
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const char *data) override
    {
        display.setCursor(col, row);
        display.print(data);
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const int &data) override
    {
        display.setCursor(col, row);
        display.print(data);
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const float &data) override
    {
        display.setCursor(col, row);
        display.print(data);
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const double &data) override
    {
        display.setCursor(col, row);
        display.print(data);
        return true;
    }

  private:
    I2CBus bus;
    SSD1306AsciiI2C display;
};
