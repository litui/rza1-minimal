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
* File Name   : resetprg.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Sub Main
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#ifdef __ICCARM__
#include <intrinsics.h>
#endif
#ifdef __GNUC__

#define SEGGER_RTT_ASM
#include "SEGGER_RTT.h"

#undef printf
#define printf SEGGER_RTT_printf

#include "irq.h"
#endif
#include "r_typedefs.h"
#include "devdrv_bsc.h"         /* Common Driver Header */
#include "devdrv_intc.h"        /* INTC Driver Header   */
#include "resetprg.h"
#include "cache.h"
#include "sio_char.h"
#include "stb_init.h"
#include "port_init.h"

#ifdef __CC_ARM
#pragma arm section code   = "CODE_RESET"
#pragma arm section rodata = "CONST_RESET"
#pragma arm section rwdata = "DATA_RESET"
#pragma arm section zidata = "BSS_RESET"
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#ifdef __CC_ARM
#define SystemInit  $Sub$$main
#endif


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
* Function Name: SystemInit
* Description  : Executes initial setting for the peripheral functions, and 
*              : jumps to the main function by calling the $Super$$main of the 
*              : library function. In this sample code, initial settings for the
*              : STB, PORT, INTC, and L1 cache are executed, and the vector 
*              : base address is set to the on-chip RAM area. The IRQ and the
*              : FIQ interrupts are enabled by calling the __enable_irq and 
*              : __enable_fiq of the library function.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void SystemInit(void)
{
    printf(0, "Vbar Init.\n");
    /* ==== Vector base address setting ==== */
    VbarInit();

    printf(0, "STB Init.\n");
    /* ==== Start clock supply of the peripheral functions ==== */
    STB_Init();

    /* ==== PORT setting ==== */
    PORT_Init();

    printf(0, "BSC CS2 and CS3 Init.\n");
    /* ==== BSC setting ==== */
    /* Initialize the CS2 and the CS3 spaces */
    R_BSC_Init((uint8_t)BSC_AREA_CS3);
    R_BSC_Init((uint8_t)BSC_AREA_CS2);

    __disable_fiq();
    __disable_irq();
    /* ==== INTC setting ==== */
    printf(0, "INTC Init.\n");
    R_INTC_Init();

    /* ==== Initial setting of the level 2 cache ==== */
    
    // printf(0, "L2 Cache Init.\n");
    // L2CacheInit();

    printf(0, "L1 Cache Init.\n");
    /* ==== Initial setting of the level 1 cache ==== */
    L1CacheInit();

    printf(0, "Enabling IRQ and FIQ\n");
    __enable_irq();         /* Enable IRQ interrupt */
    __enable_fiq();         /* Enable FIQ interrupt */

    /* ==== Initialize Terminal ==== */
    /* SCIF 2ch */
    /* P1 clock=66.67MHz CKS=0 SCBRR=17 Bit rate error=0.46% => Baud rate=115200bps */
    // IoInitScif2();
    printf(0, "SystemInit Complete.\n");

#ifdef __CC_ARM
    /* ==== Function call of main function ==== */
    $Super$$main();
#endif
}


/* END of File */

