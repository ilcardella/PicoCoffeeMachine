#pragma once

#include <lib_coffee_machine/interfaces.h>

class KTypeThermocouple : public BaseSensor
{
  public:
    KTypeThermocouple()
    {
    }

    bool read_sensor(float *value) override
    {
        // TODO
        return false;
    }

  private:
};
