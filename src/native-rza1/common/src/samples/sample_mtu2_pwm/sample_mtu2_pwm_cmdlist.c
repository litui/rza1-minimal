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
* File Name   : sample_mtu2_pwm_cmdlist.c
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description : MTU2 PWM sample program - Command list
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "command.h"
#include "sample_mtu2.h"

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
static const command_list_t sample_MTU2_PWM1_cmd_list[] = 
{
    {"1",       Sample_MTU2_PWM1_Output,    NULL},  /* 1: sample operation 1         */
    {"2",       Sample_MTU2_PWM1_Output,    NULL},  /* 2: sample operation 2         */
    {"STOP",    Sample_MTU2_PWM1_Stop,      NULL},  /* STOP MTU2 channel TCNT        */
    {"HELP",    Sample_MTU2_PWM1_Help,      NULL},  /* Display command help          */
    {"EXIT",    Sample_MTU2_PWM1_Exit,      NULL},  /* Exit MTU2 PWM sample          */
    {NULL,      NULL,                       NULL}
};

/******************************************************************************
* Function Name: Sample_MTU2_PWM_Help
* Description  : Outputs the command descriptions to the terminal.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal
*              : char_t ** argv : Pointer to the command input from the terminal
* Return Value : COMMAND_SUCCESS : Success
*              : COMMAND_ERROR   : Error
******************************************************************************/
int32_t Sample_MTU2_PWM1_Help(int32_t argc, char_t ** argv)
{
    /* ==== Command help ==== */
    if (1 == argc)
    {
        printf("\n");
        printf("  1    : PWM waveform output\n");
        printf("          - Initial output     : Output 0\n");
        printf("          - TGRA compare match : Output 1\n");
        printf("          - TGRB compare match : Output 1\n");
        printf("  2    : PWM waveform output\n");
        printf("          - Initial output     : Output 1\n");
        printf("          - TGRA compare match : Output 0\n");
        printf("          - TGRB compare match : Output 0\n");
        printf("  STOP : Stop MTU2_PWM1 sample\n");
        printf("  EXIT : Exit MTU2_PWM1 sample\n");
        printf("\n");
    }
    else
    {
        printf("error: Command failure.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    return COMMAND_SUCCESS;
}

/******************************************************************************
* Function Name: Sample_MTU2_PWM1_GetCmdList
* Description  : Obtains the command list.
* Arguments    : none
* Return Value : command_list_t * : Start address of the command list
******************************************************************************/
const command_list_t * Sample_MTU2_PWM1_GetCmdList(void)
{
    return sample_MTU2_PWM1_cmd_list;
}

/* End of File */

