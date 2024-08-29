#ifndef NATIVE_RZA1_PINS_H
#define NATIVE_RZA1_PINS_H

#define R_LED1_PORT 2
#define R_LED1_PIN  4
#define R_LED2_PORT 2
#define R_LED2_PIN  5
#define R_LED3_PORT 7
#define R_LED3_PIN  0
#define R_LED4_PORT 2
#define R_LED4_PIN  2

/* I2C */
#define R_I2C_CHANNEL  3
#define R_I2C_SDA_PORT 1
#define R_I2C_SDA_PIN  7
#define R_I2C_SDA_MUX  1
#define R_I2C_SCL_PORT 1
#define R_I2C_SCL_PIN  6
#define R_I2C_SCL_MUX  1

/* SD Card (SPI2) */
/* Note that these pins do not map to the RZA1 SD Host interface; just SPI2 */
#define R_SDCARD_SPI_CHANNEL 2
#define R_SDCARD_CLK_PORT    5  /* SCK */
#define R_SDCARD_CLK_PIN     4  /* SCK */
#define R_SDCARD_CLK_MUX     2
#define R_SDCARD_CMD_PORT    5  /* MOSI */
#define R_SDCARD_CMD_PIN     6  /* MOSI */
#define R_SDCARD_CMD_MUX     2
#define R_SDCARD_DAT0_PORT   5  /* MISO */
#define R_SDCARD_DAT0_PIN    7  /* MISO */
#define R_SDCARD_DAT0_MUX    2
#define R_SDCARD_DAT3_PORT   5  /* SSL */
#define R_SDCARD_DAT3_PIN    5  /* SSL */
#define R_SDCARD_DAT3_MUX    2
#define R_SDCARD_CD_PORT     3
#define R_SDCARD_CD_PIN      8
#define R_SDCARD_CD_MUX      0

/* USB Serial (UART1) */
#define R_USB_SERIAL_UART    1
#define R_USB_SERIAL_RX_PORT 0
#define R_USB_SERIAL_RX_PIN  2
#define R_USB_SERIAL_RX_MUX  2
#define R_USB_SERIAL_TX_PORT 3
#define R_USB_SERIAL_TX_PIN  15
#define R_USB_SERIAL_TX_MUX  5

/* Main Serial (UART4, pins D0 and D1) */
#define R_SERIAL_UART    4
#define R_SERIAL_RX_PORT 5
#define R_SERIAL_RX_PIN  14
#define R_SERIAL_RX_MUX  3
#define R_SERIAL_TX_PORT 5
#define R_SERIAL_TX_PIN  15
#define R_SERIAL_TX_MUX  3

/* ESP32 Interface (UART2) */
#define R_ESP_UART      2
#define R_ESP_TX_PORT   7
#define R_ESP_TX_PIN    1
#define R_ESP_TX_MUX    4
#define R_ESP_RX_PORT   0
#define R_ESP_RX_PIN    1
#define R_ESP_RX_MUX    2
#define R_ESP_IO0_PORT  3
#define R_ESP_IO0_PIN   14
#define R_ESP_IO0_MUX   0
#define R_ESP_IO2_PORT  3
#define R_ESP_IO2_PIN   13
#define R_ESP_IO2_MUX   0
#define R_ESP_EN_PORT   5
#define R_ESP_EN_PIN    3
#define R_ESP_EN_MUX    0
#define R_ESP_RTS_PORT  7
#define R_ESP_RTS_PIN   6
#define R_ESP_RTS_MUX   5
#define R_ESP_CTS_PORT  7
#define R_ESP_CTS_PIN   7
#define R_ESP_CTS_MUX   5
#define R_ESP_IO15_PORT 5
#define R_ESP_IO15_PIN  13
#define R_ESP_IO15_MUX  0


#endif /* NATIVE_RZA1_PINS_H */