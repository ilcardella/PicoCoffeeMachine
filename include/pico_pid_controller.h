#pragma once

#include <lib_coffee_machine/interfaces.h>

class PIDController : public Controller
{
  public:
    PIDController(const double &kp, const double &ki, const double &kd)
    {
        // TODO
    }

    void set_output_limits(const double &min, const double &max) override
    {
        // TODO
    }

    bool compute(const double &input, const double &setpoint, double &output) override
    {
        // TODO
        return false;
    }

    bool update_settings(char *raw) override
    {
        // TODO
        return false;
    }

  private:
};