#define SEGGER_RTT_ASM
#include "SEGGER_RTT.h"

#undef printf
#define printf SEGGER_RTT_printf

#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"            /* Device Driver common header */
#include "devdrv_ostm.h"        /* OSTM Driver Header */
#include "main.h"
#include "gpio.h"
#include "devdrv_intc.h"

#define MAIN_LED_ON     (1)
#define MAIN_LED_OFF    (0)

static uint32_t main_led_flg;      /* LED lighting/turning off */

void SystemInit(void);

void runfor(uint32_t iterations)
{
    for (uint32_t i = 0; i < iterations; i++) {
        ;
    }
}

int main(void)
{
    SEGGER_RTT_Init();
    SystemInit();
    
    init_gpio_as_output(6, 12); /* USER LED */
    init_gpio_as_output(6, 13); /* RED LED */
    init_gpio_as_output(6, 14); /* GREEN LED */
    init_gpio_as_output(6, 15); /* BLUE LED */
    
    main_led_flg = MAIN_LED_OFF;    /* Initialize LED on/off flag */
    printf(0, "Initializing OS Timer\n");
    /* ==== Initialize OS timer (channel 0) ==== */
    R_OSTM_Init(DEVDRV_CH_0, OSTM_MODE_INTERVAL, 500);

    /* Try forcing the remap of the interrupt function here */
    R_INTC_RegistIntFunc(INTC_ID_OSTM0TINT, Sample_OSTM0_Interrupt);

    printf(0, "Starting OS Timer 0\n");
    /* ==== Start OS timer (channel 0) ==== */
    R_OSTM_Open(DEVDRV_CH_0);

    printf(0, "Starting main program loop.\n");

    uint32_t loop_count = 0;

    while(1) {
        // // printf(0, "Setting RED LED.\n");
        // set_gpio(6, 13);
        // runfor(1000);
        // clear_gpio(6, 13);
        // runfor(1000);
        // // printf(0, "Setting GREEN LED.\n");
        // set_gpio(6, 14);
        // runfor(1000);
        // clear_gpio(6, 14);
        // runfor(1000);
        // // printf(0, "Setting BLUE LED.\n");
        // set_gpio(6, 15);
        // runfor(1000);
        // clear_gpio(6, 15);
        printf(0, "Loop count %lu.\n", loop_count);
        runfor(20000);
        loop_count++;
    }
}

void Sample_OSTM0_Interrupt(uint32_t int_sense)
{
    // printf(0, "Running Interrupt Function\n");
    /* ==== LED blink ==== */
    main_led_flg ^= 1;

    if (MAIN_LED_ON == main_led_flg)
    {
        set_gpio(6, 12);
    }
    else
    {
        clear_gpio(6, 12);
    }
}