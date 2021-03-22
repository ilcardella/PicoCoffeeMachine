#pragma once

#include <pico/stdlib.h>

#include <lib_coffee_machine/interfaces.h>

class KTypeThermocouple : public BaseSensor
{
  public:
    KTypeThermocouple(const uint &clk, const uint &cs, const uint &miso)
        : clk_pin(clk), cs_pin(cs), miso_pin(miso)
    {
    }

    bool read_sensor(float *value) override
    {
        uint16_t data;

        // Notify start of comms
        gpio_put(cs_pin, 0);
        sleep_us(2);

        data = read_byte_from_spi();
        data <<= 8;
        data |= read_byte_from_spi();

        // Notify end of comms
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
    uint8_t read_byte_from_spi()
    {
        uint8_t d(0);

        // FIXME: See main.cpp for a detailed description of this bug
        // The following for loop can be replaced with the hardware spi read below
        // spi_read_blocking(spi0, 0, &d, 1);

        int i;

        for (i = 7; i >= 0; i--)
        {
            gpio_put(clk_pin, 0);
            sleep_us(2);
            if (gpio_get(miso_pin))
            {
                // set the bit to 0 no matter what
                d |= (1 << i);
            }

            gpio_put(clk_pin, 1);
            sleep_us(2);
        }

        return d;
    }

    uint clk_pin;
    uint cs_pin;
    uint miso_pin;
};
