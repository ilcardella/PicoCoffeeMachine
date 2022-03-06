#pragma once

#include <algorithm>
#include <functional>

#include <hardware/i2c.h>
#include <pico/stdlib.h>

#include <lib_coffee_machine/common.h>
#include <lib_coffee_machine/interfaces.h>

#include "ssd1306.h"
#include "textRenderer/TextRenderer.h"

#include "icons.h"

#include "configuration.h"
#include "pico_adapter.h"

bool extraction_timer_callback(struct repeating_timer *t)
{
    unsigned int *counter = static_cast<unsigned int *>(t->user_data);
    *counter = *counter + 1;
    if (*counter > 30)
    {
        *counter = 1;
    }
    return true;
}

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

        // TODO This is unnecessary, we can use millis() % 30 to have a 30s counter
        add_repeating_timer_ms(1000, extraction_timer_callback, &extraction_counter,
                               &extraction_timer);

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
        // TODO drawText accepts a char* so the data must be converted first
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const float &data) override
    {
        // TODO drawText accepts a char* so the data must be converted first
        return true;
    }

    bool print(const unsigned &col, const unsigned &row, const double &data) override
    {
        // TODO drawText accepts a char* so the data must be converted first
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
    struct repeating_timer extraction_timer;

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

    /**
     * @brief Alternate 2 different action based on the specified period to simulate a
     * blinking effect
     *
     * @param period The period of the blinking effect
     * @param timestamp The current up time timestamp (system millis())
     * @param blink_on The function to call when the blink should be on
     * @param blink_off The function to call when the blink should be off
     * @param enable_blinking (optional) condition to enable/disable the blinking
     */
    void apply_blinking(
        const unsigned int &period, const unsigned long &timestamp,
        std::function<void(void)> blink_on, std::function<void(void)> blink_off,
        std::function<bool(void)> enable_blinking = []() { return true; })
    {
        if (enable_blinking())
        {
            if ((timestamp % (period * 2)) > period)
            {
                blink_on();
            }
            else
            {
                blink_off();
            }
        }
        else
        {
            blink_on();
        }
    }

    template <typename T>
    void draw(const unsigned &col, const unsigned &row, const T &value);

    pico_ssd1306::SSD1306 *oled = nullptr;
    const unsigned char *FONT = font_8x8;

    static constexpr unsigned long LOGO_TIME = 2000; // ms
    unsigned long start_time = 0.0;
    unsigned int extraction_counter = 0;
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

    const long now = PicoAdapter::millis();
    const Machine::Mode mode = status.machine_mode;
    const double temperature = status.current_temperature;
    const long up_time = now - status.start_timestamp;
    const long eco_countdown = std::max<long>(0, Configuration::SAFETY_TIMEOUT - up_time);

    std::function<bool(void)> temp_not_ready = [&status]() {
        return std::abs(status.target_temperature - status.current_temperature) > 3.0;
    };

    // TODO if eco_countdown is 0, then we should show a specific icon

    // Change the main icon based on the machine mode and make them blink if the
    // temperature is not within a "ready" range
    if (mode == Machine::Mode::WATER_MODE)
    {
        apply_blinking(
            1000, now,
            [this]() {
                oled->addBitmapImage(0, 0, ICON_COFFEE_WIDTH, ICON_COFFEE_HEIGHT,
                                     ICON_COFFEE);
            },
            [this]() {
                oled->addBitmapImage(0, 0, ICON_EMPTY_WIDTH, ICON_EMPTY_HEIGHT,
                                     ICON_EMPTY);
            },
            temp_not_ready);
    }
    else
    {
        apply_blinking(
            1000, now,
            [this]() {
                oled->addBitmapImage(0, 0, ICON_STEAM_WIDTH, ICON_STEAM_HEIGHT,
                                     ICON_STEAM);
            },
            [this]() {
                oled->addBitmapImage(0, 0, ICON_EMPTY_WIDTH, ICON_EMPTY_HEIGHT,
                                     ICON_EMPTY);
            },
            temp_not_ready);
    }

    // Draw secondary information after the icons
    draw<double>(1, 56, temperature);
    draw<unsigned int>(60, 56, static_cast<unsigned int>(eco_countdown / (60 * 1000)));
    draw<unsigned int>(110, 56, extraction_counter);

    return true;
}
