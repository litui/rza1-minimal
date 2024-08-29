#ifndef NATIVE_RZA1_PINS_H
#define NATIVE_RZA1_PINS_H

#define R_LED1_PORT 6
#define R_LED1_PIN  12
#define R_LED2_PORT 6
#define R_LED2_PIN  13
#define R_LED3_PORT 6
#define R_LED3_PIN  14
#define R_LED4_PORT 6
#define R_LED4_PIN  15

/* I2C */
#define R_I2C_CHANNEL  1
#define R_I2C_SDA_PORT 1
#define R_I2C_SDA_PIN  3
#define R_I2C_SDA_MUX  1
#define R_I2C_SCL_PORT 1
#define R_I2C_SCL_PIN  2
#define R_I2C_SCL_MUX  1

/* Micro-SD Card (SPI2) */
/* Note that these pins do not map to the RZA1 SD Host interface; just SPI2 */
#define R_SDCARD_SPI_CHANNEL 2
#define R_SDCARD_CLK_PORT  8  /* SCK */
#define R_SDCARD_CLK_PIN   3  /* SCK */
#define R_SDCARD_CLK_MUX   3
#define R_SDCARD_CMD_PORT  8  /* MOSI */
#define R_SDCARD_CMD_PIN   5  /* MOSI */
#define R_SDCARD_CMD_MUX   3
#define R_SDCARD_DAT0_PORT 8  /* MISO */
#define R_SDCARD_DAT0_PIN  6  /* MISO */
#define R_SDCARD_DAT0_MUX  3
#define R_SDCARD_DAT3_PORT 8  /* SSL */
#define R_SDCARD_DAT3_PIN  4  /* SSL */
#define R_SDCARD_DAT3_MUX  3
#define R_SDCARD_CD_PORT   7
#define R_SDCARD_CD_PIN    8
#define R_SDCARD_CD_MUX    0

/* Main Serial (UART0, pins D0 and D1) */
#define R_SERIAL_UART    0
#define R_SERIAL_RX_PORT 2
#define R_SERIAL_RX_PIN  15
#define R_SERIAL_RX_MUX  6
#define R_SERIAL_TX_PORT 2
#define R_SERIAL_TX_PIN  14
#define R_SERIAL_TX_MUX  6

/* USB Serial (UART2) */
#define R_USB_SERIAL_UART    2
#define R_USB_SERIAL_RX_PORT 6
#define R_USB_SERIAL_RX_PIN  2
#define R_USB_SERIAL_RX_MUX  7
#define R_USB_SERIAL_TX_PORT 6
#define R_USB_SERIAL_TX_PIN  3
#define R_USB_SERIAL_TX_MUX  7

/* ESP32 Interface not included on GR-PEACH */

#endif /* NATIVE_RZA1_PINS_H */