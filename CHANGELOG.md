# PicoCoffeeMachine Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## []
### Removed
- Removed custom PID controller and use default one included in lib_coffee_machine

## [0.1.0] - 25/03/2021
### Added
- Initial release:
  - Added dependency `lib_coffee_machine`
  - Added implementation of `lib_coffee_machine` interfaces for Raspberry Pico
  - Added schematics and PCB files
  - Added dependency `RPi-Pico-SSD1306-library` to control the OLED
  - Added LED blinking to show board status
  - Added K-type thermocouple support with software SPI
