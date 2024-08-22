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
* Copyright (C) 2014 - 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : sample_flctl_main.c
* $Rev: 1536 $
* $Date:: 2015-09-10 17:39:18 +0900#$
* Description : RZ/A1H NAND Sample Program
*******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "r_typedefs.h"
#include "sample_flctl.h"


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
//extern int32_t flctl_main(void);

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ==== Global variable ==== */
static char_t sample_flctl_main_command_buf[COMMAND_MAX_ARGLENGTH];

/******************************************************************************
* Function Name: int32_t Sample_FLCTL_Main(int32_t argc, char_t **argv);
* Description  : Waits the character input from the terminal on the host PC 
*              : connected to the GENMAI board by the serial interface. When 
*              : the following commands are input, this function executes the 
*              : respective FLCTL sample code.
*              : Enter "INIT"+"Enter"   : Initialize the FLCTL
*              : Enter "RESET"+"Enter"  : Issue RESET command to the NAND-type flash memory.
*              : Enter "ERASE"+"Enter " : Issue BLOCK ERASE command to the NAND-type flash memory.
*              : Enter "WRITE"+"Enter " : Issue PAGE PROGRAM command to the NAND-type flash memory.
*              : Enter "READ"+"Enter "  : Issue READ command to the NAND-type flash memory.
*              : Enter "INFO"+"Enter "  : Issue READ ID command to the NAND-type flash memory.
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : 0 -   Succeed to execute
******************************************************************************/
int32_t Sample_FLCTL_Main(int32_t argc, char_t **argv)
{
    const command_list_t *command_list;

    printf("\n");
    printf("RZ/A1H NAND Flash Memory Controller(FLCTL) Sample Program. Ver.1.01\n");
    printf("Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.\n");
    printf( "\n" );

    /* ==== Command receive processing ==== */
    printf("select sample program.\n");
    printf("\n");

    command_list = Sample_FLCTL_GetCmdList();
    CommandSetCmdList(command_list);

    while (1)
    {
        printf("FLCTL> ");
        if (gets(sample_flctl_main_command_buf) == NULL)
        {
            printf("error: gets error\n");
            while (1)
            {
                /* Do Nothing */
            }
        }

        /* ---- Analyze and execute commands ---- */
        if (CommandExe(sample_flctl_main_command_buf) == COMMAND_EXIT)
        {
            return COMMAND_EXIT;
        }
    }
}

/***************************************************************************** 
* Function Name: Sample_FLCTL_Exit
* Description  : Exit from the NAND Command mode
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : COMMAND_EXIT -   Succeed to execute
******************************************************************************/
int32_t Sample_FLCTL_Exit(int32_t argc, char_t ** argv)
{
    return COMMAND_EXIT;
}

/* End of File */
