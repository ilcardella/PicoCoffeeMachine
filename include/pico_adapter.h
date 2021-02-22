#pragma once

#include <pico/stdlib.h>

class PicoAdapter
{
  public:
    inline static void pinMode(uint8_t pin, uint8_t mode)
    {
        // TODO
    }

    inline static int digitalRead(uint8_t pin)
    {
        // TODO
        return 0;
    }

    inline static void digitalWrite(uint8_t pin, uint8_t val)
    {
        // TODO
    }

    inline static void delay(unsigned long ms)
    {
        sleep_ms(ms);
    }

    inline static unsigned long millis()
    {
        // TODO
        return 0;
    }

    inline static void SerialBegin(const unsigned long &baudrate)
    {
        // TODO
    }

    inline static int SerialAvailable()
    {
        // TODO
        return 0;
    }

    inline static void SerialReadStringUntil(const char &terminator, char *data)
    {
        // TODO
    }

    inline static size_t SerialPrintln(const char *value)
    {
        // TODO
        return 0;
    }

    inline static void WireBegin()
    {
        // TODO
    }

    inline static void WireSetClock(const uint32_t &clock)
    {
        // TODO
    }

    inline static char *dtostrf(double val, signed char width, unsigned char prec,
                                char *s)

    {
        // TODO
        return nullptr;
    }
};