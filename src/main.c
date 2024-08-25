#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"            /* Device Driver common header */
#include "devdrv_ostm.h"        /* OSTM Driver Header */
#include "main.h"
#include "gpio.h"
#include "devdrv_intc.h"

#define MAIN_LED_ON     (1)
#define MAIN_LED_OFF    (0)

static bool main_led_flg = false;      /* LED lighting/turning off */

void SystemInit(void);

void runfor(uint32_t iterations)
{
    for (uint32_t i = 0; i < iterations; i++) {
        ;
    }
}

int main(void)
{
    SystemInit();
    
    init_gpio_as_output(6, 12); /* USER LED (RED, outside eth port) */
    init_gpio_as_output(6, 13); /* RED LED */
    init_gpio_as_output(6, 14); /* GREEN LED */
    init_gpio_as_output(6, 15); /* BLUE LED */
    
    main_led_flg = MAIN_LED_OFF;    /* Initialize LED on/off flag */
    /* ==== Initialize OS timer (channel 0) ==== */
    R_OSTM_Init(DEVDRV_CH_0, OSTM_MODE_INTERVAL, 500);

    /* Try forcing the remap of the interrupt function here */
    R_INTC_RegistIntFunc(INTC_ID_OSTM0TINT, Sample_OSTM0_Interrupt);

    /* ==== Start OS timer (channel 0) ==== */
    R_OSTM_Open(DEVDRV_CH_0);

    while(1) {
        runfor(20000);
    }

    return 0;
}

void Sample_OSTM0_Interrupt(uint32_t int_sense)
{
    // printf(0, "Running Interrupt Function\n");
    /* ==== LED blink ==== */
    main_led_flg = !main_led_flg;

    if (MAIN_LED_ON == main_led_flg)
    {
        set_gpio(6, 15);
    }
    else
    {
        clear_gpio(6, 15);
    }
}