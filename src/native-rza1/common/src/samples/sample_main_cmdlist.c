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
* Copyright (C) 2012 - 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : sample_main_cmdlist.c
* $Rev: 1738 $
* $Date:: 2016-10-18 12:05:02 +0900#$
* Description : Main sample program - Command list
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "sample_main.h"
#include "sample_dmac.h"
#include "sample_riic.h"
#include "sample_rspi.h"
#include "sample_scif_sync.h"
#include "sample_mtu2.h"
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
static const command_list_t sample_main_cmd_list[] = 
{
    {"DMAC",        Sample_DMAC_Main,           NULL},  /* DMAC sample          */
    {"RIIC",        Sample_RIIC_Main,           NULL},  /* RIIC sample          */
    {"RSPI",        Sample_RSPI_Main,           NULL},  /* RSPI sample          */
    {"SCIF_SYNC",   Sample_SCIF_SYNC_Main,      NULL},  /* SCIF SYNC sample     */
    {"MTU2_PWM1",   Sample_MTU2_PWM1_Main,      NULL},  /* MTU2 PWM1 sample     */
    {"FLCTL",       Sample_FLCTL_Main,          NULL},  /* FLCTL sample     */
    {"HELP",        Sample_Help,                NULL},  /* Display command help */
    {NULL,          NULL,                       NULL}
};

/******************************************************************************
* Function Name: Sample_Help
* Description  : Outputs command descriptions to the terminal.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal
*              : char_t ** argv : Pointer to the command input from the terminal
* Return Value : COMMAND_SUCCESS : Success
*              : COMMAND_ERROR   : Error
******************************************************************************/
int32_t Sample_Help(int32_t argc, char_t ** argv)
{
    /* ==== Command help ==== */
    if (1 == argc)
    {
        printf("\n");
        printf("  DMAC      : DMAC Sample Program\n");
        printf("  RIIC      : RIIC Sample Program\n");
        printf("  RSPI      : RSPI Sample Program\n");
        printf("  SCIF_SYNC : SCIF Clock synchronous serial communication Sample Program\n");
        printf("  MTU2_PWM1 : MTU2 PWM1 Sample Program\n");
        printf("  FLCTL     : FLCTL Sample Program\n");
        printf("\n");
    }
    /* ==== Help by command ==== */
    else if (2 == argc)
    {
        /* Processing may be added here by using the terminal output */
        /* to display descriptions for the specified command format. */
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
* Function Name: Sample_GetCmdList
* Description  : Obtains command list.
* Arguments    : none
* Return Value : command_list_t * : Start address of the command list
******************************************************************************/
const command_list_t * Sample_GetCmdList(void)
{
    return sample_main_cmd_list;
}

/* End of File */

