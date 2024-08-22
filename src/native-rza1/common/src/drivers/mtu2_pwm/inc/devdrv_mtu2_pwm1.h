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
/******************************************************************************
* File Name    : devdrv_mtu2_pwm1.h
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description  : MTU2 PWM1 driver header
******************************************************************************/
#ifndef DEVDRV_MTU2_H
#define DEVDRV_MTU2_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/

/* ==== Select TCNT count clock ==== */
#define MTU2_CKS_DIVISION_1     (0)             /* (P0 clock)/1    */
#define MTU2_CKS_DIVISION_4     (1)             /* (P0 clock)/4    */
#define MTU2_CKS_DIVISION_16    (2)             /* (P0 clock)/16   */
#define MTU2_CKS_DIVISION_64    (3)             /* (P0 clock)/64   */
#define MTU2_CKS_DIVISION_256   (4)             /* (P0 clock)/256  */
#define MTU2_CKS_DIVISION_1024  (5)             /* (P0 clock)/1024 */

/* ==== Select switching method for output levels of output pins ==== */
#define MTU2_CMP_MATCH_A0_B1    (0x21)          /* Initial output level of output pin is "0" */
                                                /* Output level at TGRA compare match is "0" */
                                                /* Output level at TGRB compare match is "1" */

#define MTU2_CMP_MATCH_A1_B0    (0x56)          /* Initial output level of output pin is "1" */
                                                /* Output level at TGRA compare match is "1" */
                                                /* Output level at TGRB compare match is "0" */
/* ==== Select cycle and duty in table ==== */
#define MTU2_TABLE_CYCLE        (0)
#define MTU2_TABLE_DUTY         (1)

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* ==== API functions ==== */
int32_t R_MTU2_PWM1_RegTblConv(uint8_t cks, uint8_t tior_mode, uint32_t p0clk_freq, uint32_t tbl_num,
                                uint32_t (* mtu2_pwm_usertbl)[2], uint16_t (* mtu2_pwm_regtbl)[2]);
int32_t R_MTU2_PWM1_Init(uint32_t channel, uint8_t cks, uint8_t tior_mode, uint16_t (* mtu2_pwm_regtbl)[2]);
int32_t R_MTU2_PWM1_Open(uint32_t channel);
int32_t R_MTU2_PWM1_Close(uint32_t channel);
int32_t R_MTU2_PWM1_TGInA_Interrupt(uint32_t channel, uint16_t cycle_reg, uint16_t duty_reg);
int32_t R_MTU2_PWM1_TGInB_Interrupt(uint32_t channel);

/* ==== User-defined functions ==== */
void Userdef_MTU2_PWM1_Ch0_Init(uint8_t cks, uint8_t tior_mode, uint16_t (* mtu2_pwm_regtbl)[2]);
void Userdef_MTU2_PWM1_Ch3_Init(uint8_t cks, uint8_t tior_mode, uint16_t (* mtu2_pwm_regtbl)[2]);
void Userdef_MTU2_PWM1_Ch4_Init(uint8_t cks, uint8_t tior_mode, uint16_t (* mtu2_pwm_regtbl)[2]);


#endif  /* DEVDRV_MTU2_H */

/* End of File */
