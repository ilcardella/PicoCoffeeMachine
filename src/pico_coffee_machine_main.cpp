#include <cstdlib>

#include <hardware/spi.h>

#include <lib_coffee_machine/coffee_machine.h>

#include "configuration.h"
#include "ktype_thermocouple.h"
#include "pico_adapter.h"
#include "pico_io_pin.h"
#include "pico_pid_controller.h"
#include "pico_serial.h"
#include "ssd1306_ascii_display.h"

using Adapter = PicoAdapter;

bool blink_callback(struct repeating_timer *t)
{
    PicoIOPin *led = static_cast<PicoIOPin *>(t->user_data);
    led->is_high() ? led->digital_write_low() : led->digital_write_high();
    return true;
}

int main()
{
    // Initialise the SPI controller here as multiple sensors share the controller
    spi_init(spi0, 500000);
    gpio_set_function(Configuration::SPI_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(Configuration::SPI_CS_PIN, GPIO_FUNC_SPI);
    gpio_set_function(Configuration::WATER_TEMP_PIN, GPIO_FUNC_SPI);
    gpio_set_function(Configuration::STEAM_TEMP_PIN, GPIO_FUNC_SPI);
    gpio_init(Configuration::SPI_CS_PIN);
    gpio_set_dir(Configuration::SPI_CS_PIN, GPIO_OUT);
    gpio_put(Configuration::SPI_CS_PIN, 1);

    // Use the on board LED to show the board is on
    PicoIOPin led = PicoIOPin(PICO_DEFAULT_LED_PIN, IOPin::Modes::OUT);
    struct repeating_timer timer;
    add_repeating_timer_ms(500, blink_callback, &led, &timer);

    // Create the custom interfaces
    KTypeThermocouple *water_temp_sensor =
        new KTypeThermocouple(Configuration::SPI_CLK_PIN, Configuration::SPI_CS_PIN,
                              Configuration::WATER_TEMP_PIN);
    KTypeThermocouple *steam_temp_sensor =
        new KTypeThermocouple(Configuration::SPI_CLK_PIN, Configuration::SPI_CS_PIN,
                              Configuration::STEAM_TEMP_PIN);
    PicoIOPin *mode_switch_pin = new PicoIOPin(Configuration::STEAM_SWITCH_PIN);
    PicoIOPin *heater_pin = new PicoIOPin(Configuration::HEATER_SSR_PIN);
    PIDController *pid = new PIDController(Configuration::P_GAIN, Configuration::I_GAIN,
                                           Configuration::D_GAIN);
    PicoSerial *serial = new PicoSerial();
    SSD1306AsciiDisplay *display = new SSD1306AsciiDisplay();

    // Create the coffee machine instance
    CoffeeMachine<Adapter, Configuration> machine(pid, serial, mode_switch_pin, display,
                                                  heater_pin, water_temp_sensor,
                                                  steam_temp_sensor);

    while (true)
    {
        machine.spin();
        sleep_ms(1);
    }

    return 0;
}
