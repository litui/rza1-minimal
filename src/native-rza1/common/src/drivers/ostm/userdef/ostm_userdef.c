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
* File Name   : ostm_userdef.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : OS timer driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_ostm.h"            /* OSTM Driver header */
#include "devdrv_intc.h"            /* INTC Driver Header */
#include "iodefine.h"
#include "main.h"
#include "rza_io_regrw.h"
#include "unused.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ---- P0 clock (Unit : kHz) ---- */
#define P0_CLOCK_FREQUENCY_kHz  (long long int)(33.333F * 1000U)     /* Assume 33.333MHz */

/* ---- Timer cycle specification(Argument cycle)(Unit : ms) maximum value ---- */
    /* The maximum value is H'FFFF_FFFF because CMP is the register which has 32 bits */
    /* 128,850msec * 33.333MHz = 4,294,957,050 (H'FFFF_D7FA)   : No Error             */
    /* 128,851msec * 33.333MHz = 4,294,990,383 (H'1_0000_5A2F) : Error                */
    /* Timer cycle specification > Error when the cycle is 128850ms                   */
#define MAX_CYCLE_msec          (0xFFFFFFFF / P0_CLOCK_FREQUENCY_kHz)

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
* Function Name: Userdef_OSTM0_Init
* Description  : This function is a user-defined function. Run the processing to
*              : execute initial setting for the count value of the OSTM channel 0 
*              : using the timer cycle specified by the argument cycle in the 
*              : operating mode specified by the argument mode. In this sample code,
*              : the timer count value is set assuming that the P0 clock is set to
*              : 33.33MH. The initial setting for the use of the OSTM0 interrupt
*              : is also executed.
* Arguments    : uint32_t mode    : OSTM operating mode
*              :                  :   OSTM_MODE_INTERVAL : Interval timer mode
*              :                  :   OSTM_MODE_COMPARE  : Free running compare mode
*              : uint32_t cycle   : Timer cycle (msec)
* Return Value : DEVDRV_SUCCESS   : Success to initialize OSTM
*              : DEVDRV_ERROR     : Failure to initialize OSTM
******************************************************************************/
int32_t Userdef_OSTM0_Init(uint32_t mode, uint32_t cycle)
{
    volatile uint8_t dummy_buf;

    /* ==== Argument check ==== */
    if (cycle > MAX_CYCLE_msec)
    {
        return DEVDRV_ERROR;
    }

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the OSTM(channel 0) ---- */
    RZA_IO_RegWrite_8(&CPG.STBCR5, 0, CPG_STBCR5_MSTP51_SHIFT, CPG_STBCR5_MSTP51);
    dummy_buf = CPG.STBCR5;     /* Dummy read */

    /* ==== Start counting with OS timer 500ms ===== */
    /* ---- OSTM count stop trigger register (TT) setting ---- */
    OSTM0.OSTMnTT = 0x01u;      /* Stop counting */

    /* ---- OSTM control register (CTL) setting ---- */
    /* Counter operating mode                */
    /* Disable interrupt when start counting */
    RZA_IO_RegWrite_8(&OSTM0.OSTMnCTL, (uint8_t)mode, OSTM0_OSTMnCTL_MD1_SHIFT, OSTM0_OSTMnCTL_MD1);
    RZA_IO_RegWrite_8(&OSTM0.OSTMnCTL,             0, OSTM0_OSTMnCTL_MD0_SHIFT, OSTM0_OSTMnCTL_MD0);

    /* ---- OSTM compare register (CMP) setting ---- */
    OSTM0.OSTMnCMP = ((uint32_t)P0_CLOCK_FREQUENCY_kHz * cycle);
        /* Set timer in the cycle specified by the cycle */
        /* CMP = P0 clock (kHz) * cycle (msec)           */
        /* (Assume that P0 clock has 33.333MHz)          */

    R_INTC_Disable(INTC_ID_OSTM0TINT);

    /* ==== Register OS timer interrupt handler ==== */
    R_INTC_RegistIntFunc(INTC_ID_OSTM0TINT, Sample_OSTM0_Interrupt);

    /* ==== Set priority of OS timer interrupt to 5 ==== */
    R_INTC_SetPriority(INTC_ID_OSTM0TINT, 5);

    /* ==== Validate OS timer interrupt ==== */
    R_INTC_Enable(INTC_ID_OSTM0TINT);

    R_UNUSED(dummy_buf);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_OSTM1_Init
* Description  : This function is a user-defined function. Run the processing 
*              : to execute initial setting for the count value of the OSTM 
*              : channel 1 using the timer cycle specified by the argument cycle
*              : in the operating mode specified by the argument mode.
* Arguments    : uint32_t mode    : OSTM operating mode
*              :                  :   OSTM_MODE_INTERVAL : Interval timer mode
*              :                  :   OSTM_MODE_COMPARE  : Free running compare mode
*              : uint32_t cycle   : Timer cycle
* Return Value : DEVDRV_SUCCESS   : Success to initialize OSTM
*              : DEVDRV_ERROR     : Failure to initialize OSTM
******************************************************************************/
int32_t Userdef_OSTM1_Init(uint32_t mode, uint32_t cycle)
{
    /* Change processing when using OSTM1 */
    R_UNUSED(mode);
    R_UNUSED(cycle);
    return 0;
}


/* End of File */

