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
* File Name   : sample_riic_main.c
* $Rev: 1349 $
* $Date:: 2015-03-03 18:16:15 +0900#$
* Description : RIIC sample program - Main
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stddef.h>
#include <stdio.h>
#include "r_typedefs.h"
#include "command.h"
#include "sample_riic.h"

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
static char_t sample_riic_main_command_buf[COMMAND_MAX_ARGLENGTH];


/******************************************************************************
* Function Name: Sample_RIIC_Main
* Description  : Waits for character input from the terminal on the host PC
*              : connected with the GENMAI board by the serial interface.
*              : Activates the sample code to enable the read/write access to 
*              : the EEPROM.
*              : The sample code is executed to enable the read/write access
*              : to the EEPROM when "1" and "Enter" keys are pressed.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal
*              : char_t ** argv : Pointer to the command input from the terminal
* Return Value : COMMAND_EXIT : Termination of RIIC sample code processing
******************************************************************************/
int32_t Sample_RIIC_Main(int32_t argc, char_t ** argv)
{
    const command_list_t *command_list;
    int32_t ret;

    ret = COMMAND_SUCCESS;

    printf("\n");
    printf("\nRZ/A1H RIIC Sample Program. Ver.1.02\n");
    printf("Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.\n");
    printf("\n");

    /* ==== Command receive processing ==== */
    printf("select sample program.\n");
    printf("\n");

    command_list = Sample_RIIC_GetCmdList();
    CommandSetCmdList(command_list);

    while (1)
    {
        printf("RIIC SAMPLE> ");
        if (gets(sample_riic_main_command_buf) == NULL)
        {
            printf("error: gets error\n");
            while (1)
            {
                /* Do Nothing */
            }
        }

        /* ---- Analyze and execute commands ---- */
        if (CommandExe(sample_riic_main_command_buf) == COMMAND_EXIT)
        {
            ret = COMMAND_EXIT;
            break;
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: Sample_RIIC_Exit
* Description  : Terminate RIIC sample.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal
*              : char_t ** argv : Pointer to the command input from the terminal
* Return Value : COMMAND_EXIT : Termination of RIIC sample code processing
******************************************************************************/
int32_t Sample_RIIC_Exit(int32_t argc, char_t ** argv)
{
    printf("\n");

    return COMMAND_EXIT;
}

/* End of File */

