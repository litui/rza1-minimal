#ifndef NATIVE_RZA1_GPIO_H
#define NATIVE_RZA1_GPIO_H

#include "r_typedefs.h"

void init_gpio_as_output(uint8_t port, uint8_t pin);
void init_gpio_as_input(uint8_t port, uint8_t pin);
void init_gpio_as_alt(uint8_t port, uint8_t pin, uint8_t mux);
void set_gpio(uint8_t port, uint8_t pin);
void clear_gpio(uint8_t port, uint8_t pin);
uint8_t get_gpio_value(uint8_t port, uint8_t pin);

#endif /* NATIVE_RZA1_GPIO_H */