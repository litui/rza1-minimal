#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"            /* Device Driver common header */
// #include "devdrv_ostm.h"        /* OSTM Driver Header */
#include "main.h"
#include "gpio.h"

void SystemInit(void);

int main(void)
{
    SystemInit();
    
    init_gpio_as_output(6, 12); /* USER LED */
    init_gpio_as_output(6, 13); /* RED LED */
    init_gpio_as_output(6, 14); /* GREEN LED */
    init_gpio_as_output(6, 15); /* BLUE LED */

    while(1) {
        set_gpio(6, 12);
        set_gpio(6, 13);
        set_gpio(6, 14);
        set_gpio(6, 15);
        clear_gpio(6, 12);
        clear_gpio(6, 13);
        clear_gpio(6, 14);
        clear_gpio(6, 15);
    }
}
