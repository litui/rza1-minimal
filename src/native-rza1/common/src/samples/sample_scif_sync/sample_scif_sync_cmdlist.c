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
* File Name   : sample_scif_sync_cmdlist.c
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description : Serial Communication Interface with FIFO(SCIF) Clock synchronous 
*             : serial communication sample program - Command list
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "command.h"
#include "sample_scif_sync.h"

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
static const command_list_t sample_scif_sync_cmd_list[] = 
{
    {"1",       Sample_SCIF_SYNC_LoopBack,  NULL},  /* 1: SCIF SYNC loop back sample */
    {"HELP",    Sample_SCIF_SYNC_Help,      NULL},  /* Display command help          */
    {"EXIT",    Sample_SCIF_SYNC_Exit,      NULL},  /* Exit SCIF SYNC sample         */
    {NULL,      NULL,                       NULL}
};

/******************************************************************************
* Function Name: Sample_SCIF_SYNC_Help
* Description  : Outputs command descriptions to the terminal.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal
*              : char_t ** argv : Pointer to the command input from the terminal
* Return Value : COMMAND_SUCCESS : Success
*              : COMMAND_ERROR   : Error
******************************************************************************/
int32_t Sample_SCIF_SYNC_Help(int32_t argc, char_t ** argv)
{
    /* ==== Command help ==== */
    if (1 == argc)
    {
        printf("\n");
        printf("  1    : Loop back sample\n");
        printf("          - Bit rate : 250kbps\n");
        printf("  EXIT : Exit SCIF Clock synchronous serial communication sample\n");
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
* Function Name: Sample_SCIF_SYNC_GetCmdList
* Description  : Obtains the command list.
* Arguments    : none
* Return Value : command_list_t * : Start address of the command list
******************************************************************************/
const command_list_t * Sample_SCIF_SYNC_GetCmdList(void)
{
    return sample_scif_sync_cmd_list;
}

/* End of File */

