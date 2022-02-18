#pragma once

#include <hardware/i2c.h>
#include <pico/stdlib.h>

#include <lib_coffee_machine/common.h>
#include <lib_coffee_machine/interfaces.h>

#include "ssd1306.h"
#include "textRenderer/TextRenderer.h"

#include "icons.h"

#include "configuration.h"
#include "pico_adapter.h"

class SSD1306AsciiDisplay : public BaseDisplay
{
  public:
    SSD1306AsciiDisplay()
    {
        i2c_init(i2c1, 1000000); // Initialize I2C on i2c1 port at 1MHz
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
        oled = new pico_ssd1306::SSD1306(i2c1, 0x3C, pico_ssd1306::Size::W128xH64);
        oled->setOrientation(0);
        oled->setBuffer(ICON_RPI_PICO);
        oled->sendBuffer();
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
        drawText(oled, FONT, data, col, normalise_row(row));
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
        oled->sendBuffer();
        return true;
    }

    bool use_custom_display() override
    {
        return true;
    }

    bool print_custom_display(const Machine::Status &status) override;

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

    template <typename T>
    void draw(const unsigned &col, const unsigned &row, const T &value);

    pico_ssd1306::SSD1306 *oled = nullptr;
    const unsigned char *FONT = font_8x8;

    static constexpr unsigned long LOGO_TIME = 2000; // ms
    unsigned long start_time = 0.0;
};

template <>
void SSD1306AsciiDisplay::draw(const unsigned &col, const unsigned &row,
                               const double &value)
{
    char output[10];
    char buffer[6];
    PicoAdapter::dtostrf(value, 4, 1, buffer);
    snprintf(output, 10, "%s", buffer);

    drawText(oled, FONT, output, col, row);
}

template <>
void SSD1306AsciiDisplay::draw(const unsigned &col, const unsigned &row,
                               const unsigned int &value)
{
    char output[10];
    snprintf(output, 10, "%d", value);

    drawText(oled, FONT, output, col, row);
}

bool SSD1306AsciiDisplay::print_custom_display(const Machine::Status &status)
{
    // Show the initial log for the configured amount of time
    if (is_logo_time())
    {
        return true;
    }

    Machine::Mode mode = status.machine_mode;
    double temperature = status.current_temperature;
    unsigned int eco_countdown = 2400;
    unsigned int timer = 30;

    // Change the main icon based on the machine status
    if (mode == Machine::Mode::WATER_MODE)
    {
        oled->addBitmapImage(0, 0, 128, 40, ICON_COFFEE);
    }
    else
    {
        oled->addBitmapImage(0, 0, 128, 40, ICON_STEAM);
    }

    // Draw secondary information after the icons
    draw<double>(1, 56, temperature);
    draw<unsigned int>(60, 56, static_cast<unsigned int>(eco_countdown / 60));
    draw<unsigned int>(110, 56, timer);

    return true;
}
