#pragma once

#include <lib_coffee_machine/interfaces.h>
#include <ssd1306_ascii_pico/ssd1306_ascii_i2c.h>

class SSD1306AsciiDisplay : public BaseDisplay
{
  public:
    SSD1306AsciiDisplay() : bus(), display(bus)
    {
    }

    bool initialise() override
    {
        return true;
    }

    bool clear() override
    {
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const char *data) override
    {
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

  private:
    I2CBus bus;
    SSD1306AsciiI2C display;
};
