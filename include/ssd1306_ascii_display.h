#pragma once

#include <lib_coffee_machine/interfaces.h>

class SSD1306AsciiDisplay : public BaseDisplay
{
  public:
    SSD1306AsciiDisplay()
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
};
