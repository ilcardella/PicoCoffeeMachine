#include <cstdlib>

#include <lib_coffee_machine/coffee_machine.h>

#include "configuration.h"
#include "ktype_thermocouple.h"
#include "pico_adapter.h"
#include "pico_io_pin.h"
#include "pico_pid_controller.h"
#include "pico_serial.h"
#include "ssd1306_ascii_display.h"

using Adapter = PicoAdapter;

int main()
{
    // Create the custom interfaces
    KTypeThermocouple *water_temp_sensor =
        new KTypeThermocouple(Configuration::SPI_CLK_PIN, Configuration::SPI_DO_PIN,
                              Configuration::WATER_TEMP_PIN);
    KTypeThermocouple *steam_temp_sensor =
        new KTypeThermocouple(Configuration::SPI_CLK_PIN, Configuration::SPI_DO_PIN,
                              Configuration::STEAM_TEMP_PIN);
    PicoIOPin *mode_switch_pin = new PicoIOPin(Configuration::STEAM_SWITCH_PIN);
    PicoIOPin *heater_pin = new PicoIOPin(Configuration::HEATER_SSR_PIN);
    PIDController *pid = new PIDController(Configuration::P_GAIN, Configuration::I_GAIN,
                                           Configuration::D_GAIN);
    PicoSerial *serial = new PicoSerial(Configuration::SERIAL_BAUDRATE);
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
