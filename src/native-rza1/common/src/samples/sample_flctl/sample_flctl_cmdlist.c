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
* File Name   : sample_flctl_cmdlist.c
* $Rev: 1516 $
* $Date:: 2015-08-25 15:47:15 +0900#$
* Description : FLCTL sample program - Command list
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "command.h"
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


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static const command_list_t sample_FLCTL_cmd_list[] = 
{
    {"INIT",        Sample_FLCTL_Init,      NULL},
    {"RESET",       Sample_FLCTL_Reset,     NULL},
    {"ERASE",       Sample_FLCTL_Erase,     NULL},
    {"WRITE",       Sample_FLCTL_Write,     NULL},
    {"READ",        Sample_FLCTL_Read,      NULL},
    {"INFO",        Sample_FLCTL_Info,      NULL},
    {"HELP",        Sample_FLCTL_Help,      NULL},
    {"EXIT",        Sample_FLCTL_Exit,      NULL},
    {NULL,          NULL,                   NULL}
};

/***************************************************************************** 
* Function Name: Sample_FLCTL_Help
* Description  : Print the HELP message
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : 0 -   Succeed to execute
******************************************************************************/
int32_t Sample_FLCTL_Help(int32_t argc, char_t ** argv)
{
    printf("\n");
    printf("  INIT     : Initialize NAND Driver FLCTL IP\n");
    printf("  RESET    : Issue RESET Command to NAND\n");
    printf("  ERASE    : Issue ERASE Command to NAND\n");
    printf("  WRITE    : Issue WRITE Command to NAND\n");
    printf("  READ     : Issue READ Command to NAND\n");
    printf("  INFO     : Issue READ ID Command to NAND\n");
    printf("  EXIT     : Exit from NAND Command mode\n");
    printf("\n");

    return 0;
}

/******************************************************************************
* Function Name: Sample_FLCTL_GetCmdList
* Description  : Obtains the command list.
* Arguments    : none
* Return Value : command_list_t * : Start address of the command list
******************************************************************************/
const command_list_t * Sample_FLCTL_GetCmdList(void)
{
    return sample_FLCTL_cmd_list;
}

/* End of File */

