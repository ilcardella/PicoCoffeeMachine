#pragma once

#include <lib_coffee_machine/interfaces.h>

class PIDController : public Controller
{
  public:
    PIDController(const double &kp, const double &ki, const double &kd)
        : kp(kp), ki(ki), kd(kd)
    {
    }

    void set_output_limits(const double &min, const double &max) override
    {
        min_output = min;
        max_output = max;
    }

    bool compute(const double &input, const double &setpoint, double &output) override
    {
        double error = setpoint - input;

        // Integrative term
        output_sum += (ki * error);

        // Proportional term
        double temp_output = kp * error;

        // Derivative term
        temp_output += output_sum - kd * (input - last_input);

        // Constraint output within limits
        std::min(std::max(temp_output, min_output), max_output);

        output = temp_output;

        last_input = input;
        return true;
    }

    bool update_settings(char *raw) override
    {
        // TODO
        return false;
    }

  private:
    double kp = 1.0;
    double ki = 1.0;
    double kd = 1.0;

    double max_output = 255.0;
    double min_output = 0.0;

    double last_input = 0.0;
    double output_sum = 0.0;
};