#pragma once

#include <lib_coffee_machine/interfaces.h>

class PicoSerial : public BaseSerialInterface
{
  public:
    PicoSerial(const unsigned short &baudrate)
    {
        // TODO
    }

    void begin(const unsigned long &baudrate) override
    {
        // TODO
    }

    int available() override
    {
        // TODO
        return 0;
    }

    void read_string_until(const char &terminator, char *data) override
    {
        // TODO
    }

    size_t println(const char *value) override
    {
        // TODO
        return 0;
    }
};