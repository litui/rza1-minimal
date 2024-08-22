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
* File Name   : mtu2_pwm1_userdef.c
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description : MTU2 PWM1 driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_mtu2_pwm1.h"       /* MTU2 PWM1 Driver Header     */
#include "devdrv_intc.h"            /* INTC Driver Header          */
#include "sample_mtu2.h"
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


/******************************************************************************
* Function Name: Userdef_MTU2_PWM1_Ch0_Init
* Description  : This is a user-defined function. Initial settings for the MTU2 
*              : channel 0 and settings to use interrupts are required when the 
*              : channel 0 is applied.
*              : In the sample code, this function returns after providing only 
*              : STB initial settings to supply a clock to MTU2 if the clock has 
*              : not been supplied.
* Arguments    : uint8_t cks        : TCNT count clock
*              :                    :   MTU2_CKS_DIVISION_1    : (P0 clock)/1 clock
*              :                    :   MTU2_CKS_DIVISION_4    : (P0 clock)/4 clock
*              :                    :   MTU2_CKS_DIVISION_16   : (P0 clock)/16 clock
*              :                    :   MTU2_CKS_DIVISION_64   : (P0 clock)/64 clock
*              : uint8_t tior_mode  : TGRA and TGRB operations
*              :                    :   MTU2_CMP_MATCH_A0_B1
*              :                    :     : Select "0" for initial output level of output pin
*              :                    :     : Select "0" for output level at TGRA compare match
*              :                    :     : Select "1" for output level at TGRB compare match
*              :                    :   MTU2_CMP_MATCH_A1_B0
*              :                    :     : Select "1" for initial output level of output pin
*              :                    :     : Select "1" for output level at TGRA compare match
*              :                    :     : Select "0" for output level at TGRB compare match
*              : uint16_t (* mtu2_pwm_regtbl)[2]
*              :                    : Setting table for TGRA and TGRB registers
* Return Value : none
******************************************************************************/
void Userdef_MTU2_PWM1_Ch0_Init(uint8_t cks, uint8_t tior_mode, uint16_t (* mtu2_pwm_regtbl)[2])
{
    volatile uint8_t dummy_buf;

    /* ==== Processing when MTU2 module standby mode is not cancelled ==== */
    {
        /* ==== Module standby clear ==== */
        /* ---- Supply clock to the MTU2 ---- */
        RZA_IO_RegWrite_8(&CPG.STBCR3, 0, CPG_STBCR3_MSTP33_SHIFT, CPG_STBCR3_MSTP33);
        dummy_buf = CPG.STBCR3;     /* Dummy read */
    }

    /* Additional processing is required to use the MTU2 Ch0 */
}

/******************************************************************************
* Function Name: Userdef_MTU2_PWM1_Ch3_Init
* Description  : This is a user-defined function. Initial settings for the MTU2 
*              : channel 3 and settings to use interrupts are required when the 
*              : channel 3 is applied.
*              : In the sample code, this function returns after providing only 
*              : STB initial settings to supply a clock to MTU2 if the clock has 
*              : not been supplied.
* Arguments    : uint8_t cks        : TCNT count clock
*              :                    :   MTU2_CKS_DIVISION_1    : (P0 clock)/1 clock
*              :                    :   MTU2_CKS_DIVISION_4    : (P0 clock)/4 clock
*              :                    :   MTU2_CKS_DIVISION_16   : (P0 clock)/16 clock
*              :                    :   MTU2_CKS_DIVISION_64   : (P0 clock)/64 clock
*              :                    :   MTU2_CKS_DIVISION_256  : (P0 clock)/256 clock
*              :                    :   MTU2_CKS_DIVISION_1024 : (P0 clock)/1024 clock
*              : uint8_t tior_mode  : TGRA and TGRB operations
*              :                    :   MTU2_CMP_MATCH_A0_B1
*              :                    :     : Select "0" for initial output level of output pin
*              :                    :     : Select "0" for output level at TGRA compare match
*              :                    :     : Select "1" for output level at TGRB compare match
*              :                    :   MTU2_CMP_MATCH_A1_B0
*              :                    :     : Select "1" for initial output level of output pin
*              :                    :     : Select "1" for output level at TGRA compare match
*              :                    :     : Select "0" for output level at TGRB compare match
*              : uint16_t (* mtu2_pwm_regtbl)[2]
*              :                    : Setting table for TGRA and TGRB registers
* Return Value : none
******************************************************************************/
void Userdef_MTU2_PWM1_Ch3_Init(uint8_t cks, uint8_t tior_mode, uint16_t (* mtu2_pwm_regtbl)[2])
{
    volatile uint8_t dummy_buf;

    /* ==== Processing when MTU2 module standby mode is not cancelled ==== */
    {
        /* ==== Module standby clear ==== */
        /* ---- Supply clock to the MTU2 ---- */
        RZA_IO_RegWrite_8(&CPG.STBCR3, 0, CPG_STBCR3_MSTP33_SHIFT, CPG_STBCR3_MSTP33);
        dummy_buf = CPG.STBCR3;     /* Dummy read */
    }

    /* Additional processing is required to use the MTU2 Ch3 */
}

/******************************************************************************
* Function Name: Userdef_MTU2_PWM1_Ch4_Init
* Description  : This is a user-defined function. Initial settings for the MTU2 
*              : channel 4 are required.
*              : In the sample code, STB initial settings are provided to supply 
*              : a clock to MTU2 if the clock has not been supplied. The arguments 
*              : cks and tior_mode are respectively specified for the TPSC bit 
*              : in the TCR_4 register and the TIORH_4 register.
*              : By using the argument* mtu2_pwm_regtbl[], the cycle and duty of 
*              : the waveform to be output first is set to TGRA_4 and TGRB_4 
*              : registers from the first information in the setting table for 
*              : TGRA and TGRB registers. The cycle and duty of the second waveform 
*              : to be output is set to TGRC_4 and TGRD_4 registers from the second 
*              : information in the setting table for TGRA and TGRB registers.
*              : If the argument tior_mode is MTU2_CMP_MATCH_A0_B1, the TGRB_4 
*              : interrupt is enabled when the duty of the second waveform to be 
*              : output is set to 100%. If the argument tior_mode is MTU2_CMP_MATCH_A1_B0, 
*              : the TGRB_4 interrupt is enabled when the duty of the second waveform 
*              : to be output is set to 0%.
*              : Settings to use TGRA_4 and TGRB_4 interrupts and INTC setting 
*              : are made by this function.
* Arguments    : uint8_t cks        : TCNT count clock
*              :                    :   MTU2_CKS_DIVISION_1    : (P0 clock)/1 clock
*              :                    :   MTU2_CKS_DIVISION_4    : (P0 clock)/4 clock
*              :                    :   MTU2_CKS_DIVISION_16   : (P0 clock)/16 clock
*              :                    :   MTU2_CKS_DIVISION_64   : (P0 clock)/64 clock
*              :                    :   MTU2_CKS_DIVISION_256  : (P0 clock)/256 clock
*              :                    :   MTU2_CKS_DIVISION_1024 : (P0 clock)/1024 clock
*              : uint8_t tior_mode  : TGRA and TGRB operations
*              :                    :   MTU2_CMP_MATCH_A0_B1
*              :                    :     : Select "0" for initial output level of output pin
*              :                    :     : Select "0" for output level at TGRA compare match
*              :                    :     : Select "1" for output level at TGRB compare match
*              :                    :   MTU2_CMP_MATCH_A1_B0
*              :                    :     : Select "1" for initial output level of output pin
*              :                    :     : Select "1" for output level at TGRA compare match
*              :                    :     : Select "0" for output level at TGRB compare match
*              : uint16_t (* mtu2_pwm_regtbl)[2]
*              :                    : Setting table for TGRA and TGRB registers
* Return Value : none
******************************************************************************/
void Userdef_MTU2_PWM1_Ch4_Init(uint8_t cks, uint8_t tior_mode, uint16_t (* mtu2_pwm_regtbl)[2])
{
    volatile uint8_t dummy_buf;

    /* ==== Module standby clear ==== */
    {
        /* ---- Supply clock to the MTU2 ---- */
        RZA_IO_RegWrite_8(&CPG.STBCR3, 0, CPG_STBCR3_MSTP33_SHIFT, CPG_STBCR3_MSTP33);
        dummy_buf = CPG.STBCR3;     /* Dummy read */
    }

    /* ==== Stop MTU2 channel 4 timer count ==== */
    RZA_IO_RegWrite_8(&MTU2.TSTR, 0, MTU2_TSTR_CST4_SHIFT, MTU2_TSTR_CST4);

    /* ==== Clear MTU2 channel 4 timer counter ==== */
    MTU2.TCNT_4 = 0x0000;

    /* ==== MTU2 Ch4 initial settings ==== */
    /* Select count clock : (P0 clock)/64 */
    RZA_IO_RegWrite_8(&MTU2.TCR_4, cks, MTU2_TCR_n_TPSC_SHIFT, MTU2_TCR_n_TPSC);
    /* Select input clock edge : Count at rising edge */
    RZA_IO_RegWrite_8(&MTU2.TCR_4, 0, MTU2_TCR_n_CKEG_SHIFT, MTU2_TCR_n_CKEG);
    /* Select counter clear source : TGRA compare match */
    RZA_IO_RegWrite_8(&MTU2.TCR_4, 1, MTU2_TCR_n_CCLR_SHIFT, MTU2_TCR_n_CCLR);
    /* Setting for output pin : Enable output of TIOC4A pin */
    RZA_IO_RegWrite_8(&MTU2.TOER, 1, MTU2_TOER_OE4A_SHIFT, MTU2_TOER_OE4A);

    /* ---- Settings for TGRA_4 and TGRB_4 operations ---- */
    /* tior_mode is MTU2_CMP_MATCH_A0_B1 : Initial output level is "0", output level at TGRA compare match is "0", */
    /*                                     and output level at TGRB compare match is "1"                           */
    /* tior_mode is MTU2_CMP_MATCH_A1_B0 : Initial output level is "1", output level at TGRA compare match is "1", */
    /*                                     and  output level at TGRB compare match is "0"                          */
    MTU2.TIORH_4 = tior_mode;

    /* ---- Settings for cycle and duty of the first output waveform ---- */
    /* Setting for TGRA_4 : Cycle */
    RZA_IO_RegWrite_16(&MTU2.TGRA_4, mtu2_pwm_regtbl[0][MTU2_TABLE_CYCLE], MTU2_TGRA_n_D_SHIFT, MTU2_TGRA_n_D);
    /* Setting for TGRB_4 : Duty */
    RZA_IO_RegWrite_16(&MTU2.TGRB_4, mtu2_pwm_regtbl[0][MTU2_TABLE_DUTY], MTU2_TGRB_n_D_SHIFT, MTU2_TGRB_n_D);

    /* ---- Settings for buffer operation ---- */
    /* Setting for buffer operation A : Combine TGRA_4 and TGRC_4, and set it in buffer operation */
    RZA_IO_RegWrite_8(&MTU2.TMDR_4, 1, MTU2_TMDR_4_BFA_SHIFT, MTU2_TMDR_4_BFA);
    /* Setting for buffer operation B : Combine TGRB_4 and TGRD_4, and set it in buffer operation */
    RZA_IO_RegWrite_8(&MTU2.TMDR_4, 1, MTU2_TMDR_4_BFB_SHIFT, MTU2_TMDR_4_BFB);

    /* ---- Settings for transfer timing from buffer registers to TGR ---- */
    /* Transfer timing from TGRC to TGRA : Transfer when clearing TCNT on channel 4 */
    RZA_IO_RegWrite_8(&MTU2.TBTM_4, 1, MTU2_TBTM_4_TTSA_SHIFT, MTU2_TBTM_4_TTSA);
    /* Transfer timing from TGRD to TGRB : Transfer when clearing TCNT on channel 4 */
    RZA_IO_RegWrite_8(&MTU2.TBTM_4, 1, MTU2_TBTM_4_TTSB_SHIFT, MTU2_TBTM_4_TTSB);

    /* ---- Setting for cycle and duty of the second output waveform ---- */
    /* Setting for TGRC_4 : Cycle */
    RZA_IO_RegWrite_16(&MTU2.TGRC_4, mtu2_pwm_regtbl[1][MTU2_TABLE_CYCLE], MTU2_TGRC_4_D_SHIFT, MTU2_TGRC_4_D);
    /* Setting for TGRD_4 : Duty */
    RZA_IO_RegWrite_16(&MTU2.TGRD_4, mtu2_pwm_regtbl[1][MTU2_TABLE_DUTY], MTU2_TGRD_4_D_SHIFT, MTU2_TGRD_4_D);

    /* PWM mode setting : PWM mode 1 */
    RZA_IO_RegWrite_8(&MTU2.TMDR_4, 2, MTU2_TMDR_n_MD_SHIFT, MTU2_TMDR_n_MD);

    /* Settings for TCNT independent operation : TCNT_4 has no relationship with TCNT preset/clear on other channels */
    RZA_IO_RegWrite_8(&MTU2.TSYR, 0, MTU2_TSYR_SYNC4_SHIFT, MTU2_TSYR_SYNC4);
    /* Setting for TGRA_4 interrupt : Interrupt enabled */
    RZA_IO_RegWrite_8(&MTU2.TIER_4, 1, MTU2_TIER_n_TGIEA_SHIFT, MTU2_TIER_n_TGIEA);

    /* ==== Enable TGRB interrupt to avoid a change in waveform at TGRA compare match by setting ==== */
    /* ==== the second duty to be output                                                         ==== */
    /* ---- The second duty is 100% when "0" is output at TGRA compare match and ---- */
    /* ---- "1" is output at TGRB compare match                                  ---- */
    /* ---- The second duty is 0% when "1" is output at TGRA compare match and   ---- */
    /* ---- "0" is output at TGRB compare match                                  ---- */
    if ((mtu2_pwm_regtbl[1][MTU2_TABLE_DUTY] ==  mtu2_pwm_regtbl[1][MTU2_TABLE_CYCLE]) || 
        (mtu2_pwm_regtbl[1][MTU2_TABLE_DUTY] == (mtu2_pwm_regtbl[1][MTU2_TABLE_CYCLE] + 2)))
    {
        /* Setting for TGRB_4 interrupt : Interrupt enabled */
        RZA_IO_RegWrite_8(&MTU2.TIER_4, 1, MTU2_TIER_n_TGIEB_SHIFT, MTU2_TIER_n_TGIEB);
    }
    /* ---- The second duty is in the range between 0 and 99% when "0" is output at TGRA compare match and  ---- */
    /* ---- "1" is output at TGRB compare match                                                             ---- */
    /* ---- The second duty is in the range between 1 and 100% when "1" is output at TGRA compare match and ---- */
    /* ---- "0" is output at TGRB compare match                                                             ---- */
    else
    {
        /* Setting for TGRB_4 interrupt : Interrupt disabled */
        RZA_IO_RegWrite_8(&MTU2.TIER_4, 0, MTU2_TIER_n_TGIEB_SHIFT, MTU2_TIER_n_TGIEB);
    }

    /* ==== Interrupt settings ==== */
    R_INTC_RegistIntFunc(INTC_ID_TGI4A, Sample_MTU2_PWM1_TGI4A_Interrupt);      /* Register INTC_ID_TGI4A interrupt */
                                                                                /* function                         */
    R_INTC_RegistIntFunc(INTC_ID_TGI4B, Sample_MTU2_PWM1_TGI4B_Interrupt);      /* Register INTC_ID_TGI4B interrupt */
                                                                                /* function                         */

    R_INTC_SetPriority(INTC_ID_TGI4A, 1);       /* Set INTC_ID_TGI4A interrupt priority level to "1" */
    R_INTC_SetPriority(INTC_ID_TGI4B, 1);       /* Set INTC_ID_TGI4B interrupt priority level to "1" */

    R_INTC_Enable(INTC_ID_TGI4A);               /* Enable INTC_ID_TGI4A interrupt */
    R_INTC_Enable(INTC_ID_TGI4B);               /* Enable INTC_ID_TGI4B interrupt */
}


/* End of File */

