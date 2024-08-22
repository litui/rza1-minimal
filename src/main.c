#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"            /* Device Driver common header */
// #include "devdrv_ostm.h"        /* OSTM Driver Header */
#include "main.h"
#include "gpio.h"

int main(void)
{
    init_gpio_as_output(6, 12);

    while(1) {
        set_gpio(6, 12);
        clear_gpio(6, 12);
    }
}
