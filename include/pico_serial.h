#pragma once

#include <lib_coffee_machine/interfaces.h>

#include <pico/stdlib.h>
#include <stdio.h>

class PicoSerial : public BaseSerialInterface
{
  public:
    PicoSerial()
    {
        stdio_init_all();
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
        printf(value);
        return 0;
    }
};