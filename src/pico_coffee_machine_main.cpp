#include <cstdlib>

#include "lib_coffee_machine/default_configuration.h"
#include <lib_coffee_machine/coffee_machine.h>

#include "ktype_thermocouple.h"
#include "pico_adapter.h"
#include "pico_io_pin.h"
#include "pico_pid_controller.h"
#include "pico_serial.h"
#include "ssd1306_ascii_display.h"

using Adapter = PicoAdapter;
using Configuration = DefaultConfiguration;

int main()
{
    // Create the custom interfaces
    KTypeThermocouple *water_temp_sensor = new KTypeThermocouple();
    KTypeThermocouple *steam_temp_sensor = new KTypeThermocouple();
    PicoIOPin *mode_switch_pin = new PicoIOPin(Configuration::STEAM_SWITCH_PIN);
    PicoIOPin *heater_pin = new PicoIOPin(Configuration::HEATER_SSR_PIN);
    PIDController *pid = new PIDController(1, 1, 1);
    PicoSerial *serial = new PicoSerial(9600);
    SSD1306AsciiDisplay *display = new SSD1306AsciiDisplay();

    // Create the coffee machine instance
    CoffeeMachine<Adapter, Configuration> machine(pid, serial, mode_switch_pin, display,
                                                  heater_pin, water_temp_sensor,
                                                  steam_temp_sensor);

    while (true)
    {
        machine.spin();
    }

    return 0;
}
