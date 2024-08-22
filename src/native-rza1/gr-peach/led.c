/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 - 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : led.c
* $Rev: 1357 $
* $Date:: 2015-03-10 16:11:29 +0900#$
* Description : LED setting
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "led.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/


/*******************************************************************************
* Function Name: led_init
* Description  : Executes initial setting for the PORT which is connected to  
*              : the LEDs on the CPU board.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void LedInit(void)
{
    /* ---- P7_1 : LED1 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7, 0, GPIO_PIBC7_PIBC71_SHIFT, GPIO_PIBC7_PIBC71);
    RZA_IO_RegWrite_16(&GPIO.PBDC7, 0, GPIO_PBDC7_PBDC71_SHIFT, GPIO_PBDC7_PBDC71);
    RZA_IO_RegWrite_16(&GPIO.PM7,   1, GPIO_PM7_PM71_SHIFT,     GPIO_PM7_PM71);
    RZA_IO_RegWrite_16(&GPIO.PMC7,  0, GPIO_PMC7_PMC71_SHIFT,   GPIO_PMC7_PMC71);
    RZA_IO_RegWrite_16(&GPIO.PIPC7, 0, GPIO_PIPC7_PIPC71_SHIFT, GPIO_PIPC7_PIPC71);

    /* Mode : Port mode                          */
    /* Terminal output level : High level output */
    /* Port mode : Output mode                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7, 0, GPIO_PBDC7_PBDC71_SHIFT, GPIO_PBDC7_PBDC71);
    RZA_IO_RegWrite_16(&GPIO.P7,    1, GPIO_P7_P71_SHIFT,       GPIO_P7_P71);
    RZA_IO_RegWrite_16(&GPIO.PM7,   0, GPIO_PM7_PM71_SHIFT,     GPIO_PM7_PM71);
}

/*******************************************************************************
* Function Name: led_on
* Description  : Turns on the LEDs on the CPU board.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void LedOn(void)
{
    RZA_IO_RegWrite_16(&GPIO.P7, 0, GPIO_P7_P71_SHIFT, GPIO_P7_P71);
}

/*******************************************************************************
* Function Name: led_off
* Description  : Turns off the LEDs on the CPU board.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void LedOff(void)
{
    RZA_IO_RegWrite_16(&GPIO.P7, 1, GPIO_P7_P71_SHIFT, GPIO_P7_P71);
}


/* End of File */

