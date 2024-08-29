#include "gpio.h"

#include "gpio_iodefine.h"
#include "rza_io_regrw.h"

/*
 * Shorthand function for setting GPIO registers, borrowed from the Synthstrom
 * Deluge project
 */
void gpio_reg_set(volatile uint16_t *reg, uint8_t port, uint8_t pin,
                  uint8_t value) {
  RZA_IO_RegWrite_16((volatile uint16_t *)((uint32_t)reg + (port - 1) * 4),
                     value, pin, (uint16_t)1 << pin);
}

/*
 * Shorthand function for getting the value of GPIO registers, borrowed from the
 * Synthstrom Deluge project
 */
uint16_t gpio_reg_get(volatile uint16_t *reg, uint8_t port, uint8_t pin) {
  return RZA_IO_RegRead_16(
      (volatile uint16_t *)((uint32_t)reg + (port - 1) * 4), pin,
      (uint16_t)1 << pin);
}

void init_gpio_as_alt(uint8_t port, uint8_t pin, uint8_t mux, bool bidirectional) {
  
  /* Clean init of port before mux settings */
  gpio_reg_set(&GPIO.PIBC1, port, pin, 0);
  gpio_reg_set(&GPIO.PBDC1, port, pin, 0);
  gpio_reg_set(&GPIO.PM1, port, pin, 1);
  gpio_reg_set(&GPIO.PMC1, port, pin, 0);
  gpio_reg_set(&GPIO.PIPC1, port, pin, 0);

  /* Mux settings for alternate function */
  gpio_reg_set(&GPIO.PBDC1, port, pin, bidirectional ? 1 : 0);
  gpio_reg_set(&GPIO.PFCAE1, port, pin, mux >= 5);
  gpio_reg_set(&GPIO.PFCE1, port, pin, ((mux - 1) >> 1) & 1);
  gpio_reg_set(&GPIO.PFC1, port, pin, (mux - 1) & 1);
  gpio_reg_set(&GPIO.PMC1, port, pin, 1);
  gpio_reg_set(&GPIO.PIPC1, port, pin, 1);
}

void init_gpio_as_output(uint8_t port, uint8_t pin) {
  gpio_reg_set(&GPIO.PMC1, port, pin, 0);
  gpio_reg_set(&GPIO.PM1, port, pin, 0);
  gpio_reg_set(&GPIO.PIPC1, port, pin, 0);
}

void init_gpio_as_input(uint8_t port, uint8_t pin) {
  gpio_reg_set(&GPIO.PMC1, port, pin, 0);
  gpio_reg_set(&GPIO.PM1, port, pin, 1);
  gpio_reg_set(&GPIO.PIBC1, port, pin, 1);
}

void set_gpio(uint8_t port, uint8_t pin) {
  gpio_reg_set(&GPIO.P1, port, pin, 1);
}

void clear_gpio(uint8_t port, uint8_t pin) {
  gpio_reg_set(&GPIO.P1, port, pin, 0);
}

uint8_t get_gpio_value(uint8_t port, uint8_t pin) {
  return gpio_reg_get(&GPIO.PPR1, port, pin);
}