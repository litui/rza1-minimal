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
* File Name   : sample_mtu2_pwm_main.c
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description : MTU2 PWM1 sample program - Main
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stddef.h>
#include <stdio.h>
#include "r_typedefs.h"
#include "riic_rskrza1h.h"
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

/* ==== Global variable ==== */
static char_t sample_mtu2_pwm1_main_command_buf[COMMAND_MAX_ARGLENGTH];


/******************************************************************************
* Function Name: Sample_MTU2_PWM1_Main
* Description  : Waits for character input from the terminal running on the host 
*              : PC connected to the GENMAI board via the serial interface. When 
*              : the following commands are input, this function executes the 
*              : sample codes to output waveforms in PWM mode 1 of MTU2.
*              : Input "1" + "Enter" key: Execute sample operation 1
*              : Input "2" + "Enter" key: Execute sample operation 2
*              : Input "STOP" + "Enter" key: Stop PWM waveform output
* Arguments    : int32_t   argc : Number of command arguments input from the terminal
*              : char_t ** argv : Pointer to commands input from the terminal
* Return Value : COMMAND_EXIT : End of MTU2 sample code processing
******************************************************************************/
int32_t Sample_MTU2_PWM1_Main(int32_t argc, char_t ** argv)
{
    const command_list_t *command_list;

    printf("\n");
    printf("\nRZ/A1H MTU2 PWM1 Sample Program. Ver.1.01\n");
    printf("Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.\n");
    printf("\n");

    /* Initialize the IIC channel for writing to the port expander */
    R_RIIC_rza1h_rsk_init();
    /* ==== Command receive processing ==== */
    printf("select sample program.\n");
    printf("\n");

    command_list = Sample_MTU2_PWM1_GetCmdList();
    CommandSetCmdList(command_list);

    while (1)
    {
        printf("MTU2_PWM1 SAMPLE> ");
        if (gets(sample_mtu2_pwm1_main_command_buf) == NULL)
        {
            printf("error: gets error\n");
            while (1)
            {
                /* Do Nothing */
            }
        }

        /* ---- Analyze and execute commands ---- */
        if (CommandExe(sample_mtu2_pwm1_main_command_buf) == COMMAND_EXIT)
        {
            return COMMAND_EXIT;
        }
    }
}

/******************************************************************************
* Function Name: Sample_MTU2_PWM1_Exit
* Description  : Terminate MTU2 PWM1 sample.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal
*              : char_t ** argv : Pointer to the command input from the terminal
* Return Value : COMMAND_EXIT : Termination of MTU2 PWM1 sample code processing
******************************************************************************/
int32_t Sample_MTU2_PWM1_Exit(int32_t argc, char_t ** argv)
{
    printf("\n");

    return COMMAND_EXIT;
}

/* End of File */

