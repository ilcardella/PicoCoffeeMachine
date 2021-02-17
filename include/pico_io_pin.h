#pragma once

#include <lib_coffee_machine/interfaces.h>

class PicoIOPin : public IOPin
{
  public:
    PicoIOPin(const unsigned char &pin) : pin(pin)
    {
    }

    void set_mode(const IOPin::Modes &mode) override
    {
        // TODO
    }

    bool is_high() override
    {
        return false;
    }

    bool is_low() override
    {
        return not is_high();
    }

    void digital_write_high() override
    {
        // TODO
    }

    void digital_write_low() override
    {
        // TODO
    }

  private:
    unsigned char pin;
};