// Bug in lib_coffee_machine requires this include
#include <cstdlib>

#include <lib_coffee_machine/coffee_machine.h>

#include "configuration.h"
#include "ktype_thermocouple.h"
#include "pico_adapter.h"
#include "pico_io_pin.h"
#include "pico_serial.h"
#include "ssd1306_ascii_display.h"

using Adapter = PicoAdapter;

bool blink_callback(struct repeating_timer *t)
{
    PicoIOPin *led = static_cast<PicoIOPin *>(t->user_data);
    led->is_high() ? led->digital_write_low() : led->digital_write_high();
    return true;
}

void initialise_spi()
{
    // FIXME: Due to a bug in the PCB the sensors are sharing the CLK and the CS
    // lines with separate MISO line. In order to use the hardware SPI on the Pico board
    // this should change to share CLK and MISO and instead having separate CS lines
    // For this reason the SPI is going to be handled from software
    // Once the PCB is fixed, the following code can be uncommented and the software
    // SPI logic can be removed
    // ---------------------------------------------------
    // spi_init(spi0, 500000);
    // gpio_set_function(Configuration::SPI_CLK_PIN, GPIO_FUNC_SPI);
    // gpio_set_function(Configuration::SPI_CS_PIN, GPIO_FUNC_SPI);
    // gpio_set_function(Configuration::WATER_TEMP_PIN, GPIO_FUNC_SPI);
    // gpio_set_function(Configuration::STEAM_TEMP_PIN, GPIO_FUNC_SPI);
    // ---------------------------------------------------
    gpio_init(Configuration::SPI_CLK_PIN);
    gpio_set_dir(Configuration::SPI_CLK_PIN, GPIO_OUT);
    gpio_init(Configuration::WATER_TEMP_PIN);
    gpio_set_dir(Configuration::WATER_TEMP_PIN, GPIO_IN);
    gpio_init(Configuration::STEAM_TEMP_PIN);
    gpio_set_dir(Configuration::STEAM_TEMP_PIN, GPIO_IN);
    // ---------------------------------------------------

    // The CS line is always managed manually even with hardware spi
    gpio_init(Configuration::SPI_CS_PIN);
    gpio_set_dir(Configuration::SPI_CS_PIN, GPIO_OUT);
    // Set CS line high to notify no comms requested for now
    gpio_put(Configuration::SPI_CS_PIN, 1);
}

int main()
{
    initialise_spi();

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
    PicoSerial *serial = new PicoSerial();
    SSD1306AsciiDisplay *display = new SSD1306AsciiDisplay();

    // Create the coffee machine instance
    CoffeeMachine<Adapter, Configuration> machine(serial, mode_switch_pin, display,
                                                  heater_pin, water_temp_sensor,
                                                  steam_temp_sensor);

    while (true)
    {
        machine.spin();
        sleep_ms(1);
    }

    return 0;
}
