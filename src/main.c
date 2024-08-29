#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"            /* Device Driver common header */
#include "devdrv_ostm.h"        /* OSTM Driver Header */
#include "devdrv_rspi.h"
#include "main.h"
#include "gpio.h"
#include "unused.h"
#include "board.h"
#include "sio_char.h"

#define MAIN_LED_ON     (1)
#define MAIN_LED_OFF    (0)

static bool main_led_flg = false;      /* LED lighting/turning off */

extern int     _write(int, char *, int);

void SystemInit(void);

int main(void)
{
    init_gpio_as_output(R_LED1_PORT, R_LED1_PIN);
    init_gpio_as_output(R_LED2_PORT, R_LED2_PIN);
    init_gpio_as_output(R_LED3_PORT, R_LED3_PIN);
    init_gpio_as_output(R_LED4_PORT, R_LED4_PIN);

    /* Turn on LED1 to prove SystemInit has begun. */
    set_gpio(R_LED1_PORT, R_LED1_PIN);

    SystemInit();

    printf("\n\nRZA1-Minimal");
#if defined(BOARD_GR_PEACH)
    printf(" (GR-PEACH)\n");
#else
    printf(" (GR-LYCHEE)\n");
#endif
    printf("Completed System Init\n\n");

    /* Turn off LED1 since SystemInit has concluded. */
    clear_gpio(R_LED1_PORT, R_LED1_PIN);
    
    main_led_flg = MAIN_LED_OFF;    /* Initialize LED on/off flag */

    printf("Initializing OS Timer (channel 0)\n");
    /* ==== Initialize OS timer (channel 0) ==== */
    R_OSTM_Init(DEVDRV_CH_0, OSTM_MODE_INTERVAL, 500);
    /* ==== Start OS timer (channel 0) ==== */
    R_OSTM_Open(DEVDRV_CH_0);

    /* Initialize SDCard-related GPIO */
    // init_gpio_as_alt(R_SDCARD_CLK_PORT, R_SDCARD_CLK_PIN, R_SDCARD_CLK_MUX, false);
    // init_gpio_as_alt(R_SDCARD_CMD_PORT, R_SDCARD_CMD_PIN, R_SDCARD_CMD_MUX, false);
    // init_gpio_as_alt(R_SDCARD_DAT0_PORT, R_SDCARD_DAT0_PIN, R_SDCARD_DAT0_MUX, false);
    // init_gpio_as_alt(R_SDCARD_DAT3_PORT, R_SDCARD_DAT3_PIN, R_SDCARD_DAT3_MUX, false);
    // init_gpio_as_output(R_SDCARD_CD_PORT, R_SDCARD_CD_PIN);

    printf("Initializing SDCard SPI Bus\n");
    R_RSPI_Init(R_SDCARD_SPI_CHANNEL);

    /* Turn on LED2 to prove main function has reached its end. */
    set_gpio(R_LED2_PORT, R_LED2_PIN);

    printf("\nReached end of main function.\n");
    return 0;
}

void Sample_OSTM0_Interrupt(uint32_t int_sense)
{
    /* ==== LED blink ==== */
    main_led_flg = !main_led_flg;

    if (MAIN_LED_ON == main_led_flg)
    {
        set_gpio(R_LED4_PORT, R_LED4_PIN);
    }
    else
    {
        clear_gpio(R_LED4_PORT, R_LED4_PIN);
    }

    R_UNUSED(int_sense);
}