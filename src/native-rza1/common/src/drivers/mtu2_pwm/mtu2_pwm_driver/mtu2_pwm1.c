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
* File Name   : mtu2_pwm1.c
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description : MTU2 PWM1 driver
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_mtu2_pwm1.h"       /* MTU2 PWM Driver Header      */
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
* Function Name: R_MTU2_PWM1_RegTblConv
* Description  : Converts the cycle and duty information of the table specified 
*              : by the argument* mtu2_pwm_usertbl[] into the values of the cycle 
*              : and duty to be set in the registers for the amount specified 
*              : by the argument tbl_num, and stores them in the table specified 
*              : by the argument* mtu2_pwm_regtbl[].
*              : The cycle and duty register setting values are converted from 
*              : the values of cycle (us) and duty (%) in the specified table 
*              : using the P0  frequency (Hz) specified by the argument p0clk_freq 
*              : and the count clock specified by the argument cks. The duty is 
*              : defined as percentage for High width in cycle, and the conversion 
*              : expression for the register setting values differs depending on 
*              : the argument tior_mode specification.
*              : When the cycle (us) or the duty (%) that cannot be converted into 
*              : register setting values is specified, DEVDRV_ERROR is returned.
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
*              : uint32_t p0clk_freq: Frequency of P0 clock (Hz)
*              : uint32_t tbl_num   : Amount of cycle and duty information
*              : uint16_t (* mtu2_pwm_usertbl)[2]
*              :                    : Cycle and duty setting table
*              : uint16_t (* mtu2_pwm_regtbl)[2]
*              :                    : Setting table for TGRA_4 and TGRB_4 registers
* Return Value : DEVDRV_SUCCESS   : Success in converting into register setting values
*              : DEVDRV_ERROR     : Failure in converting into register setting values
******************************************************************************/
int32_t R_MTU2_PWM1_RegTblConv(uint8_t cks, uint8_t tior_mode, uint32_t p0clk_freq, uint32_t tbl_num, 
                                uint32_t (* mtu2_pwm_usertbl)[2], uint16_t (* mtu2_pwm_regtbl)[2])
{
    uint32_t micro_sec_per_clk;         /* Micro second per count of timer                                */
    uint32_t mtu2_clk_freq;             /* Count clock of MTU2                                            */
    uint32_t micro_sec_min;             /* Lower limit of us which can be set by P0 clock and count clock */
    uint32_t duty_micro_sec;            /* Value converted to us from duty                                */
    uint32_t cycle_reg;                 /* Cycle register setting value                                   */
    uint32_t duty_reg;                  /* Duty register setting value                                    */
    uint32_t index;                     /* Table number                                                   */
    int32_t  ret;

    ret = DEVDRV_SUCCESS;

    /* ==== Argument check ==== */
    if ((cks > MTU2_CKS_DIVISION_1024) || ((tior_mode != MTU2_CMP_MATCH_A0_B1) && (tior_mode != MTU2_CMP_MATCH_A1_B0))
        || (tbl_num <= 1))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Check duty at the start of the specified table ==== */
    /* ---- Set initial output level to "0", output level at TGRA compare match to "0", and ---- */
    /* ---- output level at TGRB compare match to "1" to perform waveform output            ---- */
    if (MTU2_CMP_MATCH_A0_B1 == tior_mode)
    {
        /* Initial output is duty 100% */
        if (100 == mtu2_pwm_usertbl[0][MTU2_TABLE_DUTY])
        {
            ret = DEVDRV_ERROR;
        }
    }
    /* ---- Set initial  output level to "1", output level at TGRA compare match to "1", and ---- */
    /* ---- output level at TGRB compare match to "0" to perform waveform output             ---- */
    else
    {
        /* Initial output is duty 0% */
        if (0 == mtu2_pwm_usertbl[0][MTU2_TABLE_DUTY])
        {
            ret = DEVDRV_ERROR;
        }
    }

    if (DEVDRV_ERROR == ret)
    {
        return DEVDRV_ERROR;
    }

    /* ==== Calculate lower limit of time (us) that can be generated by the specified P0 clock and count clock ==== */
    micro_sec_per_clk = ((1 << (cks * 2)) * 1000000) / p0clk_freq;
    micro_sec_min     = micro_sec_per_clk + 1;

    /* ==== Calculate count clock of MTU2 by using the specified P0 clock and count clock ==== */
    mtu2_clk_freq = p0clk_freq / (1 << (cks * 2));

    index = 0;
    do {
        /* ==== Check if the specified cycle (us) indicates the time that can be generated ==== */
        if (mtu2_pwm_usertbl[index][MTU2_TABLE_CYCLE] < micro_sec_min)
        {
            ret = DEVDRV_ERROR;
            break;
        }

        /* ==== Convert the specified cycle (us) to the value to be set in the register ==== */
        cycle_reg = (((uint64_t)mtu2_clk_freq * (uint64_t)mtu2_pwm_usertbl[index][MTU2_TABLE_CYCLE]) / 1000000) - 1;

        /* ==== Check the range of register values to be set as a cycle ==== */
        /* ---- The maximum setting value of the cycle register should be "H'FFFD" because "H'FFFE" ---- */
        /* ---- and "H'FFFF" are used for register setting values when the duty is 0% or 100%       ---- */
        if (cycle_reg > 0xfffd)
        {
            ret = DEVDRV_ERROR;
            break;
        }

        /* ==== Store the converted cycle value in the setting table for TGRA and TGRB registers ==== */
        mtu2_pwm_regtbl[index][MTU2_TABLE_CYCLE] = (uint16_t)cycle_reg;


        /* ==== Check the range of specified duty ==== */
        if (mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY] > 100)      /* Duty is larger than 100% */
        {
            ret = DEVDRV_ERROR;
            break;
        }

        /* ==== Check the combination of specified duty and the next duty ==== */
        if (index < (tbl_num - 1))
        {
            /* ---- When "0" is output at TGRA compare match, and "1" is output at TGRB compare match ---- */
            if (MTU2_CMP_MATCH_A0_B1 == tior_mode)
            {
                /* Specified duty is 0%, and the next duty is 100% */
                if ((  0 == mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY]) && 
                    (100 == mtu2_pwm_usertbl[index + 1][MTU2_TABLE_DUTY]))
                {
                    ret = DEVDRV_ERROR;
                    break;
                }
            }
            /* ---- When "1" is output at TGRA compare match and "0" is output at TGRB compare match ---- */
            else
            {
                /* Specified duty is 100%, and the next duty is 0% */
                if ((100 == mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY]) && 
                    (  0 == mtu2_pwm_usertbl[index + 1][MTU2_TABLE_DUTY]))
                {
                    ret = DEVDRV_ERROR;
                    break;
                }
            }
        }

        /* ==== Convert the specified duty (%) to the value to be set in the register, and ==== */
        /* ==== store it to the setting table of TGRA and TGRB registers                   ==== */
        /* ---- 0% when "0" is output at TGRA compare match and "1" is output at TGRB compare match   ---- */
        /* ---- 100% when "1" is output at TGRA compare match and "0" is output at TGRB compare match ---- */
        if (((MTU2_CMP_MATCH_A0_B1 == tior_mode) && (  0 == mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY])) ||
            ((MTU2_CMP_MATCH_A1_B0 == tior_mode) && (100 == mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY])))
        {
            /* Change the duty to the value where 1 is added to the cycle register setting value, and */
            /* store it in the setting table for TGRA and TGRB registers                              */
            /* (to avoid a change in PWM output by TGRB compare match)                                */
            mtu2_pwm_regtbl[index][MTU2_TABLE_DUTY] = mtu2_pwm_regtbl[index][MTU2_TABLE_CYCLE] + 1;
        }
        /* ---- Duty is between 1 and 99% ---- */
        else if ((mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY] >= 1) && 
                 (mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY] <= 99))
        {
            /* ---- When "0" is output at TGRA compare match, and "1" is output at TGRB compare match ---- */
            if (MTU2_CMP_MATCH_A0_B1 == tior_mode)
            {
                /* Using cycle (us) and duty (%), calculate the value converted from duty into us */
                duty_micro_sec = (mtu2_pwm_usertbl[index][MTU2_TABLE_CYCLE] * 
                                    (100 - mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY])) / 100;
            }
            /* ---- When "1" is output at TGRA compare match, and "0" is output at TGRB compare match ---- */
            else
            {
                /* Using cycle (us) and duty (%), calculate the value converted from duty into us */
                duty_micro_sec = (mtu2_pwm_usertbl[index][MTU2_TABLE_CYCLE] * 
                                  mtu2_pwm_usertbl[index][MTU2_TABLE_DUTY]) / 100;
            }

            /* ---- Check if the duty indicates the time that can be generated ---- */
            if (duty_micro_sec < micro_sec_min)
            {
                ret = DEVDRV_ERROR;
                break;
            }

            /* ==== Convert the specified duty to the value to be set in the register ==== */
            duty_reg = ((mtu2_clk_freq * (uint64_t)duty_micro_sec) / 1000000) - 1;

            /* ==== Store the converted duty value in the setting table for TGRA and TGRB registers ==== */
            mtu2_pwm_regtbl[index][MTU2_TABLE_DUTY] = (uint16_t)duty_reg;
        }
        /* ---- 100% when "0" is output at TGRA compare match and "1" is output at TGRB compare match ---- */
        /* ---- 0% when "1" is output at TGRA compare match and "0" is output at TGRB compare match   ---- */
        else
        {
            /* When the cycle and duty is not the last information in the table */
            if (index < (tbl_num - 1))
            {
                /* ---- Next duty is 100% when "0" is output at TGRA compare match and ---- */
                /* ---- "1" is output at TGRB compare match                            ---- */
                /* ---- Next duty is 0% when "1" is output at TGRA compare match and   ---- */
                /* ---- "0" is output at TGRB compare match                            ---- */
                if (((MTU2_CMP_MATCH_A0_B1 == tior_mode) && (100 == mtu2_pwm_usertbl[index + 1][MTU2_TABLE_DUTY])) ||
                    ((MTU2_CMP_MATCH_A1_B0 == tior_mode) && (  0 == mtu2_pwm_usertbl[index + 1][MTU2_TABLE_DUTY])))
                {
                    /* Make the same setting value for duty and cycle, and store it in the setting table */
                    /* for TGRA and TGRB registers                                                       */
                    mtu2_pwm_regtbl[index][MTU2_TABLE_DUTY] = mtu2_pwm_regtbl[index][MTU2_TABLE_CYCLE];
                }
                /* ---- Next duty is in the range between 1 and 99% when "0" is output at TGRA compare match  ---- */
                /* ---- and "1" is output at TGRB compare match                                               ---- */
                /* ---- Next duty is in the range between 1 and 100% when "1" is output at TGRA compare match ---- */
                /* ---- and "0" is output at TGRB compare match                                               ---- */
                else
                {
                    /* Change the duty setting value to the value where 2 is added to the cycle setting value and */
                    /* store it in the setting table for TGRA and TGRB registers                                  */
                    mtu2_pwm_regtbl[index][MTU2_TABLE_DUTY] = mtu2_pwm_regtbl[index][MTU2_TABLE_CYCLE] + 2;
                }
            }
            /* When the cycle and duty is the last information in the table */
            else
            {
                /* Change the duty setting value to the value where 2 is added to the cycle setting value and */
                /* store it in the setting table for TGRA and TGRB registers                                  */
                mtu2_pwm_regtbl[index][MTU2_TABLE_DUTY] = mtu2_pwm_regtbl[index][MTU2_TABLE_CYCLE] + 2;
            }
        }

        index++;

    } while (tbl_num > index);      /* Loop until the cycle and duty information reaches to the last in the table */

    return ret;
}


/******************************************************************************
* Function Name: R_MTU2_PWM1_Init
* Description  : Initializes MTU2 specified by the argument channel by using the 
*              : count clock specified by the argument cks in operating state 
*              : of TGRA and TGRB specified by the argument tior_mode. The initial 
*              : values are set to TGRA and TGRB registers based on the table 
*              : information specified by the argument* mtu2_pwm_regtbl.
*              : Initial settings are made by using the user-defined function 
*              : Userdef_MTU2_Chn_Init (n=0, 3, or 4).
*              : This function should be called after converting the cycle and 
*              : duty information into the register setting values by calling 
*              : the API function R_MTU2_PWM1_RegTblConv and storing them in the 
*              : setting table for TGRA and TGRB registers.
* Arguments    : uint32_t channel   : MTU2 channel (0, 3, or 4)
*              : uint8_t cks        : TCNT count clock
*              :                    :   MTU2_CKS_DIVISION_1    : (P0 clock)/1 clock
*              :                    :   MTU2_CKS_DIVISION_4    : (P0 clock)/4 clock
*              :                    :   MTU2_CKS_DIVISION_16   : (P0 clock)/16 clock
*              :                    :   MTU2_CKS_DIVISION_64   : (P0 clock)/64 clock
*              :                    :   MTU2_CKS_DIVISION_256  : (P0 clock)/256 clock
*              :                    :   MTU2_CKS_DIVISION_1024 : (P0 clock)/1024 clokc
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
* Return Value : DEVDRV_SUCCESS   : Success of MTU2 initialization
*              : DEVDRV_ERROR     : Failure of MTU2 initialization
******************************************************************************/
int32_t R_MTU2_PWM1_Init(uint32_t channel, uint8_t cks, uint8_t tior_mode, uint16_t (* mtu2_pwm_regtbl)[2])
{
    /* ==== Argument check ==== */
    /* ---- channel check ---- */
    if ((channel != DEVDRV_CH_0) && (channel != DEVDRV_CH_3) && (channel != DEVDRV_CH_4))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ---- cks check ---- */
    /* channel 0 */
    if (DEVDRV_CH_0 == channel)
    {
        if (cks > MTU2_CKS_DIVISION_64)
        {
            return DEVDRV_ERROR;    /* Argument error */
        }
    }
    /* channel 3, 4 */
    else
    {
        if (cks > MTU2_CKS_DIVISION_1024)
        {
            return DEVDRV_ERROR;    /* Argument error */
        }
    }

    /* ---- tior_mode check ---- */
    if ((tior_mode != MTU2_CMP_MATCH_A0_B1) && (tior_mode != MTU2_CMP_MATCH_A1_B0))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== MTU2 PWM1 initialization ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_MTU2_PWM1_Ch0_Init(cks, tior_mode, mtu2_pwm_regtbl);
        break;
        case DEVDRV_CH_3:
            Userdef_MTU2_PWM1_Ch3_Init(cks, tior_mode, mtu2_pwm_regtbl);
        break;
        case DEVDRV_CH_4:
            Userdef_MTU2_PWM1_Ch4_Init(cks, tior_mode, mtu2_pwm_regtbl);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_MTU2_PWM1_Open
* Description  : Starts the MTU2 timer count specified by the argument channel. 
*              : Channel 0, 3, or 4 can be specified.
* Arguments    : uint32_t channel : MTU2 channel (0, 3, or 4)
* Return Value : DEVDRV_SUCCESS   : Success in starting MTU2 timer count
*              : DEVDRV_ERROR     : Failure in starting MTU2 timer count
******************************************************************************/
int32_t R_MTU2_PWM1_Open(uint32_t channel)
{
    /* ==== Argument check ==== */
    if ((channel != DEVDRV_CH_0) && (channel != DEVDRV_CH_3) && (channel != DEVDRV_CH_4))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Start TCNT count operation ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            RZA_IO_RegWrite_8(&MTU2.TSTR, 1, MTU2_TSTR_CST0_SHIFT, MTU2_TSTR_CST0);
        break;
        case DEVDRV_CH_3:
            RZA_IO_RegWrite_8(&MTU2.TSTR, 1, MTU2_TSTR_CST3_SHIFT, MTU2_TSTR_CST3);
        break;
        case DEVDRV_CH_4:
            RZA_IO_RegWrite_8(&MTU2.TSTR, 1, MTU2_TSTR_CST4_SHIFT, MTU2_TSTR_CST4);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_MTU2_PWM1_Close
* Description  : Stops the MTU2 timer count specified by the argument channel. 
*              : Channel 0, 3, or 4 can be specified.
* Arguments    : uint32_t channel : MTU2 channel (0, 3, or 4)
* Return Value : DEVDRV_SUCCESS   : Success in stopping MTU2 timer count
*              : DEVDRV_ERROR     : Failure in stopping MTU2 timer count
******************************************************************************/
int32_t R_MTU2_PWM1_Close(uint32_t channel)
{
    /* ==== Argument check ==== */
    if ((channel != DEVDRV_CH_0) && (channel != DEVDRV_CH_3) && (channel != DEVDRV_CH_4))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Stop TCNT timer count operation ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            RZA_IO_RegWrite_8(&MTU2.TSTR, 0, MTU2_TSTR_CST0_SHIFT, MTU2_TSTR_CST0);
        break;
        case DEVDRV_CH_3:
            RZA_IO_RegWrite_8(&MTU2.TSTR, 0, MTU2_TSTR_CST3_SHIFT, MTU2_TSTR_CST3);
        break;
        case DEVDRV_CH_4:
            RZA_IO_RegWrite_8(&MTU2.TSTR, 0, MTU2_TSTR_CST4_SHIFT, MTU2_TSTR_CST4);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_MTU2_PWM1_TGInA_Interrupt
* Description  : Sets the values specified by the arguments cycle_reg and duty_reg 
*              : to the buffer registers (TGRC and TGRD) of the channel specified 
*              : by the argument channel as the cycle and duty information of 
*              : PWM waveform to be output next.
*              : When the values specified by the arguments cycle_reg and duty_reg 
*              : are equal, or the value where "2" is added to the argument cycle_reg 
*              : and the value specified by the argument duty_reg are equal, and 
*              : also when the duty of the current PWM waveform is in the range 
*              : between 1 and 99%, the TGRB interrupt specified by the argument 
*              : channel is enabled to avoid a change in the waveform at the next 
*              : TGRA compare match.
* Arguments    : uint32_t channel   : MTU2 channel (0, 3, or 4)
*              : uint16_t cycle_reg : Setting value for the next cycle
*              : uint16_t duty_reg  : Setting value for the next duty
* Return Value : DEVDRV_SUCCESS     : Success of TGInA interrupt processing
*              : DEVDRV_ERROR       : Failure of TGInA interrupt processing
******************************************************************************/
int32_t R_MTU2_PWM1_TGInA_Interrupt(uint32_t channel, uint16_t cycle_reg, uint16_t duty_reg)
{
    volatile uint16_t * tgra_reg;
    volatile uint16_t * tgrb_reg;
    volatile uint16_t * tgrc_reg;
    volatile uint16_t * tgrd_reg;
    volatile uint8_t  * tsr_reg;
    volatile uint8_t  * tier_reg;
    volatile uint8_t  dummy_buf_8b;
             uint16_t tmp_reg;

    /* ==== Argument check ==== */
    if ((channel != DEVDRV_CH_0) && (channel != DEVDRV_CH_3) && (channel != DEVDRV_CH_4))
     {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Set the next cycle and duty to the buffer register ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            tgra_reg = &MTU2.TGRA_0;
            tgrb_reg = &MTU2.TGRB_0;
            tgrc_reg = &MTU2.TGRC_0;
            tgrd_reg = &MTU2.TGRD_0;
            tsr_reg  = &MTU2.TSR_0;
            tier_reg = &MTU2.TIER_0;
        break;
        case DEVDRV_CH_3:
            tgra_reg = &MTU2.TGRA_3;
            tgrb_reg = &MTU2.TGRB_3;
            tgrc_reg = &MTU2.TGRC_3;
            tgrd_reg = &MTU2.TGRD_3;
            tsr_reg  = &MTU2.TSR_3;
            tier_reg = &MTU2.TIER_3;
        break;
        case DEVDRV_CH_4:
            tgra_reg = &MTU2.TGRA_4;
            tgrb_reg = &MTU2.TGRB_4;
            tgrc_reg = &MTU2.TGRC_4;
            tgrd_reg = &MTU2.TGRD_4;
            tsr_reg  = &MTU2.TSR_4;
            tier_reg = &MTU2.TIER_4;
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    *tgrc_reg = cycle_reg;      /* Set the next cycle information */
    *tgrd_reg = duty_reg;       /* Set the next duty information  */

    tmp_reg = *tgra_reg;

    /* ==== Enable TGRB interrupt to avoid a change in the waveform at the next TGRA compare match ==== */
    /* ---- Next duty is 100% when "0" is output at TGRA compare match and "1" is output at TGRB compare match ---- */
    /* ---- Next duty is 0% when "1" is output at TGRA compare match and "0" is output at TGRB compare match   ---- */
    if ((duty_reg == cycle_reg) || (duty_reg == (cycle_reg + 2)))
    {
        if (tmp_reg > *tgrb_reg)        /* When the current waveform is output with the duty between 1 and 99% */
        {
            /* Clear TGRB interrupt source flag */
            RZA_IO_RegWrite_8(tsr_reg, 0, MTU2_TSR_n_TGFB_SHIFT, MTU2_TSR_n_TGFB);
            /* Perform a dummy read of TGRB interrupt source flag */
            dummy_buf_8b = RZA_IO_RegRead_8(tsr_reg, MTU2_TSR_n_TGFB_SHIFT, MTU2_TSR_n_TGFB);
            /* TGRB interrupt enabled */
            RZA_IO_RegWrite_8(tier_reg, 1, MTU2_TIER_n_TGIEB_SHIFT, MTU2_TIER_n_TGIEB);
        }
    }

    /* ==== Clear TGRA interrupt source flag ==== */
    RZA_IO_RegWrite_8(tsr_reg, 0, MTU2_TSR_n_TGFA_SHIFT, MTU2_TSR_n_TGFA);
    /* ==== Perform a dummy read of TGRA interrupt source flag ==== */
    dummy_buf_8b = RZA_IO_RegRead_8(tsr_reg, MTU2_TSR_n_TGFA_SHIFT, MTU2_TSR_n_TGFA);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_MTU2_PWM1_TGInB_Interrupt
* Description  : When the value of TGRA is specified for the one of TGRB to avoid 
*              : a change in the waveform at TGRA compare match of the channel 
*              : specified by the argument channel, the duty of the current waveform 
*              : is in the range between 1 and 99% and the next waveform to be 
*              : output can be controlled. If PWM waveform is output at the levels 
*              : of "0" at TGRA compare match and "1" at TGRB compare match, the 
*              : waveform with the duty of 100% is output next. If PWM waveform 
*              : is output at the levels of "1" at TGRA compare match and "0" at 
*              : TGRB compare match, the waveform with the duty of 0% is output 
*              : next.
*              : The TGRB interrupt specified by the argument channel is disabled.
* Arguments    : uint32_t channel : MTU2 channel (0, 3, or 4)
* Return Value : DEVDRV_SUCCESS   : Success of TGInB interrupt processing
*              : DEVDRV_ERROR     : Failure of TGInB interrupt processing
******************************************************************************/
int32_t R_MTU2_PWM1_TGInB_Interrupt(uint32_t channel)
{
    volatile uint16_t * tgra_reg;
    volatile uint16_t * tgrb_reg;
    volatile uint8_t  * tsr_reg;
    volatile uint8_t  * tier_reg;
    volatile uint8_t  dummy_buf_8b;

    /* ==== Argument check ==== */
    if ((channel != DEVDRV_CH_0) && (channel != DEVDRV_CH_3) && (channel != DEVDRV_CH_4))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Set values to avoid a change in the waveform at the next TGRA compare match ==== */
    /* ---- Next duty is 100 when "0" is output at TGRA compare match and "1" is output at TGRB compare match ---- */
    /* ---- Next duty is 0% when "1" is output at TGRA compare match and "0" is output at TGRB compare match  ---- */
    switch (channel)
    {
        case DEVDRV_CH_0:
            tgra_reg = &MTU2.TGRA_0;
            tgrb_reg = &MTU2.TGRB_0;
            tsr_reg  = &MTU2.TSR_0;
            tier_reg = &MTU2.TIER_0;
        break;
        case DEVDRV_CH_3:
            tgra_reg = &MTU2.TGRA_3;
            tgrb_reg = &MTU2.TGRB_3;
            tsr_reg  = &MTU2.TSR_3;
            tier_reg = &MTU2.TIER_3;
        break;
        case DEVDRV_CH_4:
            tgra_reg = &MTU2.TGRA_4;
            tgrb_reg = &MTU2.TGRB_4;
            tsr_reg  = &MTU2.TSR_4;
            tier_reg = &MTU2.TIER_4;
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    /* ==== Set values to avoid a change in waveform output at TGRA compare match ==== */
    *tgrb_reg = *tgra_reg;

    /* ==== Disable TGRB interrupt ==== */
    RZA_IO_RegWrite_8(tier_reg, 0, MTU2_TIER_n_TGIEB_SHIFT, MTU2_TIER_n_TGIEB);

    /* ==== Clear TGRB interrupt source flag ==== */
    RZA_IO_RegWrite_8(tsr_reg, 0, MTU2_TSR_n_TGFB_SHIFT, MTU2_TSR_n_TGFB);
    /* ==== Perform a dummy read of TGRB interrupt source flag ==== */
    dummy_buf_8b = RZA_IO_RegRead_8(tsr_reg, MTU2_TSR_n_TGFB_SHIFT, MTU2_TSR_n_TGFB);

    return DEVDRV_SUCCESS;
}


/* End of File */

