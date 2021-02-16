#include "pico/stdlib.h"
#include <stdio.h>

#include <lib_coffee_machine/coffee_machine.h>

int main()
{
    setup_default_uart();
    printf("Hello, world!\n");
    return 0;
}