#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"            /* Device Driver common header */
#include "devdrv_ostm.h"        /* OSTM Driver Header */
#include "devdrv_rspi.h"
#include "main.h"
#include "gpio.h"
#include "sdcard.h"
#include "unused.h"
#include "board.h"

#define MAIN_LED_ON     (1)
#define MAIN_LED_OFF    (0)

static bool main_led_flg = false;      /* LED lighting/turning off */

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

    sdcard_init();

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