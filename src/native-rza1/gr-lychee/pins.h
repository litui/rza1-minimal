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

/* SD Card [SPI2] */
/* Note that these pins do not map to the RZA1 SD Host interface; just SPI2 */
#define R_SDCARD_SPI_CHANNEL 2
#define R_SDCARD_CLK_PORT  5  /* SCK */
#define R_SDCARD_CLK_PIN   4  /* SCK */
#define R_SDCARD_CLK_MUX   2
#define R_SDCARD_CMD_PORT  5  /* MOSI */
#define R_SDCARD_CMD_PIN   6  /* MOSI */
#define R_SDCARD_CMD_MUX   2
#define R_SDCARD_DAT0_PORT 5  /* MISO */
#define R_SDCARD_DAT0_PIN  7  /* MISO */
#define R_SDCARD_DAT0_MUX  2
#define R_SDCARD_DAT3_PORT 5  /* SSL */
#define R_SDCARD_DAT3_PIN  5  /* SSL */
#define R_SDCARD_DAT3_MUX  2
#define R_SDCARD_CD_PORT   3
#define R_SDCARD_CD_PIN    8
#define R_SDCARD_CD_MUX    0

#endif /* NATIVE_RZA1_PINS_H */