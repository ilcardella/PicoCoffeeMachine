#pragma once

#include <functional>

#include <hardware/i2c.h>
#include <pico/stdlib.h>

#include <lib_coffee_machine/common.h>
#include <lib_coffee_machine/interfaces.h>

extern "C"
{
#include "ssd1306.h"
}

#include "icons.h"
#include "configuration.h"
#include "pico_adapter.h"

class SSD1306AsciiDisplay : public BaseDisplay
{
public:
    SSD1306AsciiDisplay()
    {
        i2c_init(i2c1, 400000); // Initialize I2C on i2c1 port at 400KHz
        gpio_set_function(Configuration::I2C_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(Configuration::I2C_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(Configuration::I2C_SDA_PIN);
        gpio_pull_up(Configuration::I2C_SCL_PIN);
    }

    ~SSD1306AsciiDisplay()
    {
        if (oled)
        {
            ssd1306_deinit(oled);
            delete oled;
            oled = nullptr;
        }
    }

    bool initialise() override
    {
        oled = new ssd1306_t();
        oled->external_vcc = false;
        ssd1306_init(oled, 128, 64, 0x3C, i2c1);
        ssd1306_clear(oled);

        // TODO Show initial logo
        // ssd1306_bmp_show_image(oled, ICON_RPI_PICO, sizeof(ICON_RPI_PICO) / sizeof(ICON_RPI_PICO[0]));
        ssd1306_draw_string(oled, 0, 0, 3, "STARTING...");
        ssd1306_show(oled);

        // Record the start time
        start_time = PicoAdapter::millis();

        return true;
    }

    bool clear() override
    {
        if (oled && !is_logo_time())
        {
            ssd1306_clear(oled);
        }
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const char *data) override
    {
        if (oled)
        {
            ssd1306_draw_string(oled, col, row, 1, data);
        }
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const int &data) override
    {
        // TODO ssd1306_draw_string accepts a char* so the data must be converted first
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const float &data) override
    {
        // TODO ssd1306_draw_string accepts a char* so the data must be converted first
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const double &data) override
    {
        // TODO ssd1306_draw_string accepts a char* so the data must be converted first
        return true;
    }

    bool display() override
    {
        if (oled)
        {
            ssd1306_show(oled);
        }
        return true;
    }

    bool use_custom_display() override
    {
        return true;
    }

    bool print_custom_display(const Machine::Status &status) override;

private:
    bool is_logo_time()
    {
        return (PicoAdapter::millis() - start_time) < LOGO_TIME;
    }

    /**
     * @brief Alternate 2 different action based on the specified period to simulate a
     * blinking effect
     *
     * @param period The period of the blinking effect
     * @param timestamp The current up time timestamp (system millis())
     * @param blink_on The function to call when the blink should be on
     * @param blink_off The function to call when the blink should be off
     * @param predicate (optional) condition to enable/disable the blinking
     */
    void apply_blinking(
        const uint32_t &period, const unsigned long &timestamp,
        std::function<void(void)> blink_on, std::function<void(void)> blink_off,
        std::function<bool(void)> predicate = []()
        { return true; })
    {
        if (predicate())
        {
            ((timestamp % (period * 2)) > period) ? blink_on() : blink_off();
        }
        else
        {
            blink_on();
        }
    }

    template <typename T>
    void draw(const uint32_t &col, const uint32_t &row, const T &value);

    ssd1306_t *oled;
    static constexpr unsigned long LOGO_TIME = 2000; // ms
    unsigned long start_time = 0.0;
};

template <>
void SSD1306AsciiDisplay::draw(const uint32_t &col, const uint32_t &row,
                               const double &value)
{
    char output[10];
    char buffer[6];
    PicoAdapter::dtostrf(value, 4, 1, buffer);
    snprintf(output, 10, "%s", buffer);

    ssd1306_draw_string(oled, col, row, 1, output);
}

template <>
void SSD1306AsciiDisplay::draw(const uint32_t &col, const uint32_t &row,
                               const uint32_t &value)
{
    char output[10];
    snprintf(output, 10, "%d", value);

    ssd1306_draw_string(oled, col, row, 1, output);
}

bool SSD1306AsciiDisplay::print_custom_display(const Machine::Status &status)
{
    // Show the initial log for the configured amount of time
    if (is_logo_time())
    {
        return true;
    }

    const long now = PicoAdapter::millis();
    const Machine::Mode mode = status.machine_mode;
    const double temperature = status.current_temperature;
    const long up_time = now - status.start_timestamp;
    const long eco_countdown = std::max<long>(0, Configuration::SAFETY_TIMEOUT - up_time);

    std::function<bool(void)> temp_not_ready = [&status]()
    {
        return std::abs(status.target_temperature - status.current_temperature) > 3.0;
    };

    // If eco_countdown is 0, then show a specific icon
    if (eco_countdown < 1)
    {
        ssd1306_draw_string(oled, 0, 0, 3, "TIMEOUT");
    }
    // Change the main icon based on the machine mode and make them blink if the
    // temperature is not within a "ready" range
    // TODO ssd1306_bmp_show_image does not work with the images in icons.h
    else if (mode == Machine::Mode::WATER_MODE)
    {
        apply_blinking(
            1000, now,
            [this]()
            {
                // ssd1306_bmp_show_image(oled, ICON_COFFEE, sizeof(ICON_COFFEE) / sizeof(ICON_COFFEE[0]));
                ssd1306_draw_string(oled, 0, 0, 3, "COFFEE");
            },
            [this]()
            {
                // ssd1306_bmp_show_image(oled, ICON_EMPTY, sizeof(ICON_EMPTY) / sizeof(ICON_EMPTY[0]));
                ssd1306_draw_string(oled, 0, 0, 3, "");
            },
            temp_not_ready);
    }
    else
    {
        apply_blinking(
            1000, now,
            [this]()
            {
                // ssd1306_bmp_show_image(oled, ICON_STEAM, sizeof(ICON_STEAM) / sizeof(ICON_STEAM[0]));
                ssd1306_draw_string(oled, 0, 0, 3, "STEAM");
            },
            [this]()
            {
                // ssd1306_bmp_show_image(oled, ICON_EMPTY, sizeof(ICON_EMPTY) / sizeof(ICON_EMPTY[0]));
                ssd1306_draw_string(oled, 0, 0, 3, "");
            },
            temp_not_ready);
    }

    // Draw secondary information after the icons
    draw<double>(1, 56, temperature);
    draw<uint32_t>(60, 56, static_cast<uint32_t>(eco_countdown / (60 * 1000)));
    auto extraction_counter = (static_cast<uint32_t>(PicoAdapter::millis() / 1000) % 30) + 1;
    draw<uint32_t>(110, 56, extraction_counter);

    return true;
}
