#pragma once

#include <pico/stdlib.h>
#include <string>

class PicoAdapter
{
  public:
    inline static void delay(unsigned long ms)
    {
        sleep_ms(ms);
    }

    inline static unsigned long millis()
    {
        return get_absolute_time();
    }

    inline static char *dtostrf(double val, signed char width, unsigned char prec,
                                char *s)
    {
        char fmt[20];
        sprintf(fmt, "%%%d.%df", width, prec);
        sprintf(s, fmt, val);
        return s;
    }
};