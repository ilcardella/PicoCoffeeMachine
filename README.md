# Pico Coffee Machine

This project is a PID controller (and more) for a `Gaggia Paros` (and possibly any other manual coffee machine) coffee machine using a Raspberry PI Pico and a few other components.

The project is based on [ArduinoCoffeeMachine](https://github.com/ilcardella/ArduinoCoffeeMachine) which is the original project targeting an Arduino Nano.
Both projects are based on [lib_coffee_machine](https://github.com/ilcardella/lib_coffee_machine) which is a C++ library providing an abstraction of a generic coffee machine.

Please refer to the [ArduinoCoffeeMachine documentation](https://arduinocoffeemachine.readthedocs.io/en/latest/?badge=latest) for a detailed explanation of the design and components used.

## Getting Started

The project is configured to build targeting the RP2040 platform which is at the core of the Raspberry PI Pico board.

### Build

The `Makefile` at the project root directory provides targets to create the build docker image and to build the code itself:

```
$ make docker
$ make build
```

The build process runs inside a Docker container allowing isolation and easier dependencies management.

## PCB

Schematics and PCB gerber files can be found in the `pcb` folder. The folder contains:
- .zip archive containing gerber files for the PCB
- .png showing the schematics and the PCB mask
- .json file to open and edit the project in EasyEDA

