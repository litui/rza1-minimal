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
* File Name   : bsc.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Common driver
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "devdrv_bsc.h"         /* Common Driver Header */

#ifdef __CC_ARM
#pragma arm section code   = "CODE_BASIC_SETUP"
#pragma arm section rodata = "CONST_BASIC_SETUP"
#pragma arm section rwdata = "DATA_BASIC_SETUP"
#pragma arm section zidata = "BSS_BASIC_SETUP"
#endif

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


/******************************************************************************
* Function Name: R_BSC_Init
* Description  : Executes the setting to initialize the BSC function.
*              : Sets the BSC channels by calling the Userdef_BSC_CSnInit of
*              : the user-defined function.
* Arguments    : uint8 area : Specifies the CS space to be initialized
*              :     <Bit 0> 1 : Initialize CS0 space
*              :             0 : Do not initialize CS0 space
*              :     <Bit 1> 1 : Initialize CS1 space
*              :             0 : Do not initialize CS1 space
*              :     <Bit 2> 1 : Initialize CS2 space
*              :             0 : Do not initialize CS2 space
*              :     <Bit 3> 1 : Initialize CS3 space
*              :             0 : Do not initialize CS3 space
*              :     <Bit 4> 1 : Initialize CS4 space
*              :             0 : Do not initialize CS4 space
*              :     <Bit 5> 1 : Initialize CS5 space
*              :             0 : Do not initialize CS5 space
*              :     <Bit 6-7> Reserved
* Return Value : none
******************************************************************************/
void R_BSC_Init(uint8_t area)
{
    /* WARNING:                                                           */
    /* The section area for the RW data or the ZI data is not initialized */
    /* because this function is called by the Peripheral_BasicInit        */
    /* function. Do not use the variables allocated to the section area   */
    /* for the RW data or the ZI data within this function and the user-  */
    /* defined function called by this function.                          */

    /* ==== BSC initialize ==== */
    if ((area & BSC_AREA_CS0) != 0)     /* Initialize CS0 space */
    {
        Userdef_BSC_CS0Init();
    }
    if ((area & BSC_AREA_CS1) != 0)     /* Initialize CS1 space */
    {
        Userdef_BSC_CS1Init();
    }
    if ((area & BSC_AREA_CS2) != 0)     /* Initialize CS2 space */
    {
        Userdef_BSC_CS2Init();
    }
    if ((area & BSC_AREA_CS3) != 0)     /* Initialize CS3 space */
    {
        Userdef_BSC_CS3Init();
    }
    if ((area & BSC_AREA_CS4) != 0)     /* Initialize CS4 space */
    {
        Userdef_BSC_CS4Init();
    }
    if ((area & BSC_AREA_CS5) != 0)     /* Initialize CS5 space */
    {
        Userdef_BSC_CS5Init();
    }
}


/* End of File */

