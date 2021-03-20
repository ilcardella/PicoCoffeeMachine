#pragma once

#include <lib_coffee_machine/interfaces.h>

class PIDController : public Controller
{
  public:
    PIDController(const double &kp_gain, const double &ki_gain, const double &kd_gain)
        : kp(kp_gain), ki(ki_gain), kd(kd_gain)
    {
    }

    void set_output_limits(const double &min, const double &max) override
    {
        min_output = min;
        max_output = max;
    }

    bool compute(const double &input, const double &setpoint, double &output) override
    {
        double error(0.0);
        double error_rate(0.0);
        double new_output(0.0);

        error = setpoint - input;
        error_sum += error;
        error_rate = (error - last_error);

        // Sum all the PID terms
        new_output = (kp * error) + (ki * error_sum) - (kd * error_rate);

        // Store error for next iteration
        last_error = error;

        // Constraint output within limits
        output = limit_output(new_output);
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

    double last_error = 0.0;
    double error_sum = 0.0;

    double limit_output(const double &value)
    {
        return std::min<double>(std::max<double>(value, min_output), max_output);
    }
};