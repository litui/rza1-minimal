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
* File Name   : sample_mtu2_pwm1_command.c
* $Rev: 1532 $
* $Date:: 2015-09-08 12:13:42 +0900#$
* Description : MTU2 PWM1 sample program
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_mtu2_pwm1.h"       /* MTU2 PWM Driver Header      */
#include "sample_mtu2.h"
#include "command.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define MTU2_USER_TABLE_NUM     (6)                     /* Number of information in cycle and duty setting table */

#define MTU2_P0_CLOCK_FREQ_Hz   (33.333333*1000000)     /* Definition of P0 clock frequency */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/

/* ==== Global variables ==== */
/* Cycle (us) and duty (%) setting table (Used in sample operation 1) */
static uint32_t mtu2_pwm_usertbl1[MTU2_USER_TABLE_NUM][2] =
{
/*   cycle(us),duty(%) */
    {    10000,  70},
    {    12000,  10},
    {    14000,   0},
    {    16000,  40},
    {     8000, 100},
    {     4000, 100},
};

/* Cycle (us) and duty (%) setting table (Used in sample operation 2) */
static uint32_t mtu2_pwm_usertbl2[MTU2_USER_TABLE_NUM][2] =
{
/*   cycle(us),duty(%) */
    {    10000,  30},
    {    12000,  90},
    {    14000, 100},
    {    16000,  60},
    {     8000,   0},
    {     4000,   0},
};

static uint32_t mtu2_pwm_num;                               /* Table number of the setting table for */
                                                            /* TGRA and TGRB registers               */
static uint16_t mtu2_pwm_regtbl[MTU2_USER_TABLE_NUM][2];    /* Setting table for TGRA and TGRB registers */

static void     MTU2_PWM1_PORT_Init(void);

/******************************************************************************
* Function Name: Sample_MTU2_PWM1_Output
* Description  : The MTU2 channel 4 is used to output waveforms in PWM mode 1.
*              : Converts the information specified by the cycle (us) and duty (%) 
*              : into the register setting values using the P0 /64 clock source 
*              : for the count clock to output PWM waveforms from the TIOC4A pin.
*              : In sample operation 1, the initial output level of output pin 
*              : is set to "0" and output levels for TGRA_4 compare match and 
*              : TGRB_4 compare match to "0" and "1" respectively, and the waveforms 
*              : with the cycle and duty of 10ms/70%, 12ms/10%, 14ms/0%, 16ms/40%, 
*              : 8ms/100%, and 4ms/100% are continuously output.
*              : In sample operation 2, the initial output level of output pin 
*              : is set to "1" and output levels for TGRA_4 compare match and 
*              : TGRB_4 compare match to "1" and "0" respectively, and the waveforms 
*              : with the cycle and duty of 10ms/30%, 12ms/90%, 14ms/100%, 16ms/60%, 
*              : 8ms/0%, and 4ms/0% are continuously output.
* Arguments    : int32_t   argc : Number of command arguments input from the terminal
*              :                : Not used in this function.
*              : char_t ** argv : Pointer to commands input from the terminal
*              :                :   When argv[0][0] is "1", sample operation 1 is executed.
*              :                :   When argv[0][0] is "2", sample operation 2 is executed
* Return Value : COMMAND_SUCCESS : Success of MTU2 PWM waveform output
*              : COMMAND_ERROR   : Failure of MTU2 PWM waveform output
******************************************************************************/
int32_t Sample_MTU2_PWM1_Output(int32_t argc, char_t ** argv)
{
    int32_t  ret;
    uint32_t channel;                   /* MTU2 channel                 */
    uint8_t  cks;                       /* TCNT count clock             */
    uint8_t  tior_mode;                 /* Setting for TGRA_4 and TGRB_4 operations */
    uint32_t (* tmp_usertbl)[2];

#if MTU2_USER_TABLE_NUM == 1
    printf("error: MTU2 MTU2_USER_TABLE_NUM error.\n");
    printf("\n");
    return COMMAND_ERROR;
#else

    printf("\n");

    channel = DEVDRV_CH_4;
    cks = MTU2_CKS_DIVISION_64;         /* Specify (P0 clock)/64 for count clock */

    /* When the sample operation 1 is specified */
    if('1' == argv[0][0])
    {
        tior_mode = MTU2_CMP_MATCH_A0_B1;   /* Initial output level of output pin is "0"   */
                                            /* Output level at TGRA_4 compare match is "0" */
                                            /* Output level at TGRB_4 compare match is "1" */

        tmp_usertbl = mtu2_pwm_usertbl1;    /* Specify cycle and duty setting table for sample operation 1 */
    }
    /* When the sample operation 2 is specified */
    else if('2' == argv[0][0])
    {
        tior_mode = MTU2_CMP_MATCH_A1_B0;   /* Initial output level of output pin is "1"   */
                                            /* Output level at TGRA_4 compare match is "1" */
                                            /* Output level at TGRB_4 compare match is "0" */

        tmp_usertbl = mtu2_pwm_usertbl2;    /* Specify cycle and duty setting table for sample operation 2 */
    }
    else
    {
        /* This function is executed only when argv[0][0] specifies "1" or "2".                 */
        /* If argv[0][0] indicates an illegal parameter, this function should not be called.    */
        /* Do not reach here based on the assumption.                                           */
        /* It is assumed that this processing should not be executed, but the variables used in */
        /* this function are initialized to avoid their contents to be indefinite.              */
        
        tior_mode = MTU2_CMP_MATCH_A0_B1;   /* Initial output level of output pin is "0"   */
                                            /* Output level at TGRA_4 compare match is "0" */
                                            /* Output level at TGRB_4 compare match is "1" */

        tmp_usertbl = mtu2_pwm_usertbl1;    /* Specify cycle and duty setting table for sample operation 1 */
    }

    /* ==== Convert the cycle and duty information to the register setting values and ==== */
    /* ==== store them in the register setting table                                  ==== */
    ret = R_MTU2_PWM1_RegTblConv(cks,                               /* TCNT count clock                              */
                                 tior_mode,                         /* Switching method for output pin               */
                                 (uint32_t)MTU2_P0_CLOCK_FREQ_Hz,   /* P0 clock frequency (in Hz)                    */
                                 (uint32_t)MTU2_USER_TABLE_NUM,     /* Number of information in cycle and            */
                                                                    /* duty setting table                            */
                                 tmp_usertbl,                       /* Setting table for cycle and duty              */
                                 mtu2_pwm_regtbl);                  /* Setting table for TGRA_4 and TGRB_4 registers */

    if (DEVDRV_ERROR == ret)
    {
        printf("error: MTU2 Register value convert error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }
    

    /* ==== MTU2 initialization ==== */
    ret = R_MTU2_PWM1_Init(channel,             /* MTU2 channel                                  */
                           cks,                 /* TCNT count clock                              */
                           tior_mode,           /* Setting for TGRA_4 and TGRB_4 operations      */
                           mtu2_pwm_regtbl);    /* Setting table for TGRA_4 and TGRB_4 registers */

    if (DEVDRV_ERROR == ret)
    {
        printf("error: MTU2 Initialize error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== PORT function setting ==== */
    MTU2_PWM1_PORT_Init();

    /* ==== Initialize table number in the setting table for TGRA and TGRB registers ==== */
#if MTU2_USER_TABLE_NUM >= 3
    /* ---- Number of cycle and duty information in the setting table is 3 or more ---- */
    mtu2_pwm_num = 2;   /* Set "2" to the initial value of the table number referred at */
                        /* the first TGRA_4 compare match interrupt                     */
#else
    /* ---- Number of cycle and duty information in the setting table is less than 3 ---- */
    mtu2_pwm_num = 0;   /* Set "0" to the initial value of the table number referred at   */
                        /* the first TGRA_4 compare match interrupt                       */
#endif

    /* ==== Start TCNT_4 count operation ==== */
    ret = R_MTU2_PWM1_Open(channel);

    if (DEVDRV_ERROR == ret)
    {
        printf("error: MTU2 count start error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    printf("MTU2 PWM1 SAMPLE CODE %c Initialize complete.\n", argv[0][0]);
    printf("\n");

    return COMMAND_SUCCESS;
#endif
}

/******************************************************************************
* Function Name: Sample_MTU2_PWM1_Stop
* Description  : Stops the timer count on MTU2 channel 4 to stop the PWM waveform 
*              : output. The level at the time the timer count stopped is continuously 
*              : output using the TIOC4A pin.
* Arguments    : int32_t   argc : Number of command arguments input from the terminal 
*              :                : Not used in this function.
*              : char_t ** argv : Pointer to commands input from the terminal
*              :                : Not used in this function.
* Return Value : COMMAND_SUCCESS : Success in stopping timer used in PWM waveform output
*              : COMMAND_ERROR   : Failure in stopping timer used in PWM waveform output
******************************************************************************/
int32_t Sample_MTU2_PWM1_Stop(int32_t argc, char_t ** argv)
{
    int32_t  ret;
    uint32_t channel;                   /* MTU2 channel */

    channel = DEVDRV_CH_4;

    /* ==== Stop TCNT_4 count operation ==== */
    ret = R_MTU2_PWM1_Close(channel);
    if (DEVDRV_ERROR == ret)
    {
        printf("error: MTU2 count stop error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    printf("MTU2 count stop complete.\n");
    printf("\n");

    return COMMAND_SUCCESS;
}

/******************************************************************************
* Function Name: MTU2_PWM1_PORT_Init
* Description  : Initializes the PORT used to output PWM waveforms of MTU2.
*              : In the sample code, the PORT is initialized to use P4_4 pin as 
*              : the TIOC4A function.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void MTU2_PWM1_PORT_Init(void)
{
    /* ---- P4_4 : TIOC4A ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC44_SHIFT,   GPIO_PIBC4_PIBC44);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC44_SHIFT,   GPIO_PBDC4_PBDC44);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM44_SHIFT,       GPIO_PM4_PM44);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC44_SHIFT,     GPIO_PMC4_PMC44);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC44_SHIFT,   GPIO_PIPC4_PIPC44);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC44_SHIFT,   GPIO_PBDC4_PBDC44);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC44_SHIFT,     GPIO_PFC4_PFC44);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE44_SHIFT,   GPIO_PFCE4_PFCE44);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 0, GPIO_PFCAE4_PFCAE44_SHIFT, GPIO_PFCAE4_PFCAE44);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC44_SHIFT,     GPIO_PMC4_PMC44);
    RZA_IO_RegWrite_16(&GPIO.PM4,    0, GPIO_PM4_PM44_SHIFT,       GPIO_PM4_PM44);
}

/******************************************************************************
* Function Name: Sample_MTU2_PWM1_TGI4A_Interrupt
* Description  : This is an interrupt handler executed when an interrupt generated 
*              : by TGRA_4 compare match has been accepted.
*              : Sets the setting values for the next cycle and duty to the 
*              : argument and executes the processing to output specified PWM 
*              : waveforms during the next cycle by calling the API function 
*              : R_MTU2_PWM1_TGInA_Interrupt.
*              : Also, increments the variables which indicate table number for 
*              : cycle and duty.
*              : In the sample code, this function is registered as INTC handler 
*              : by using the user-defined function Userdef_MTU2_PWM1_Ch4_Init.
* Arguments    : uint32_t int_sense : Interrupt detection (Not used)
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge
* Return Value : none
******************************************************************************/
void Sample_MTU2_PWM1_TGI4A_Interrupt(uint32_t int_sense)
{
    uint32_t channel;       /* MTU2 channel            */
    uint16_t cycle_reg;     /* Setting value of cycle  */
    uint16_t duty_reg;      /* Setting value of duty   */

    channel = DEVDRV_CH_4;

    /* ==== Obtain setting value of cycle from the setting table for TGRA and TGRAB registers ==== */
    cycle_reg = mtu2_pwm_regtbl[mtu2_pwm_num][MTU2_TABLE_CYCLE];

    /* ==== Obtain setting value of duty from the setting table for TGRA and TGRAB registers ==== */
    duty_reg = mtu2_pwm_regtbl[mtu2_pwm_num][MTU2_TABLE_DUTY];

    /* ==== TGInA interrupt processing ==== */
    R_MTU2_PWM1_TGInA_Interrupt(channel, cycle_reg, duty_reg);

    mtu2_pwm_num++;

    /* ==== Initialize table number of cycle and duty setting table ==== */
    if (mtu2_pwm_num >= MTU2_USER_TABLE_NUM)
    {
        mtu2_pwm_num = 0;
    }
}

/******************************************************************************
* Function Name: Sample_MTU2_PWM1_TGI4B_Interrupt
* Description  : This is an interrupt handler executed when an interrupt generated 
*              : by TGRB_4 compare match has been accepted.
*              : Execute the processing to avoid a change in PWM waveforms at 
*              : the next TGRA_4 compare match by calling the API function 
*              : R_MTU2_PWM1_TGInB_Interrupt.
*              : In the sample code, this function is registered as INTC handler 
*              : by using the user-defined function Userdef_MTU2_PWM1_Ch4_Init.
* Arguments    : uint32_t int_sense : Interrupt detection (Not used)
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
******************************************************************************/
void Sample_MTU2_PWM1_TGI4B_Interrupt(uint32_t int_sense)
{
    uint32_t channel;       /* MTU2 channel */

    channel = DEVDRV_CH_4;

    /* ==== TGInB interrupt processing ==== */
    R_MTU2_PWM1_TGInB_Interrupt(channel);
}


/* End of File */

