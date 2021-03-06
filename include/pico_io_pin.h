#pragma once

#include <pico/stdlib.h>

#include <lib_coffee_machine/interfaces.h>

class PicoIOPin : public IOPin
{
  public:
    PicoIOPin(const uint &p) : pin(p)
    {
        gpio_init(pin);
    }

    PicoIOPin(const uint &p, const IOPin::Modes &mode) : pin(p)
    {
        gpio_init(pin);
        set_mode(mode);
    }

    void set_mode(const IOPin::Modes &mode) override
    {
        switch (mode)
        {
        case IOPin::Modes::IN:
            gpio_set_dir(pin, false);
            break;
        case IOPin::Modes::OUT:
            gpio_set_dir(pin, true);
            break;
        case IOPin::Modes::IN_PU:
            gpio_set_dir(pin, false);
            gpio_pull_up(pin);
            break;
        default:
            break;
        }
    }

    bool is_high() override
    {
        return gpio_get(pin);
    }

    bool is_low() override
    {
        return not is_high();
    }

    void digital_write_high() override
    {
        gpio_put(pin, 1);
    }

    void digital_write_low() override
    {
        gpio_put(pin, 0);
    }

  private:
    uint pin;
};