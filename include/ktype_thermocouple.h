#pragma once

#include <lib_coffee_machine/interfaces.h>
#include <pico/stdlib.h>

class KTypeThermocouple : public BaseSensor
{
  public:
    KTypeThermocouple(const uint &clk, const uint &cs, const uint &miso)
        : clk_pin(clk), cs_pin(cs), miso_pin(miso)
    {
        gpio_set_dir(clk_pin, true);
        gpio_set_dir(cs_pin, true);
        gpio_set_dir(miso_pin, false);

        gpio_put(cs_pin, 1);
    }

    bool read_sensor(float *value) override
    {
        uint16_t data;

        gpio_put(cs_pin, 0);
        sleep_us(10);

        data = read_from_spi();
        data <<= 8;
        data |= read_from_spi();

        gpio_put(cs_pin, 1);

        if (data & 0x4)
        {
            return false;
        }

        data >>= 3;
        *value = static_cast<float>(data * 0.25);
        return true;
    }

  private:
    uint8_t read_from_spi()
    {
        uint8_t d = 0;

        for (int i = 7; i >= 0; i--)
        {
            gpio_put(clk_pin, 0);
            sleep_us(10);
            if (gpio_get(miso_pin))
            {
                d |= (1 << i);
            }
            gpio_put(clk_pin, 1);
            sleep_us(10);
        }

        return d;
    }

    uint clk_pin;
    uint cs_pin;
    uint miso_pin;
};
