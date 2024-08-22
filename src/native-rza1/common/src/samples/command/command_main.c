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
* File Name   : command_main.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Command process
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "r_typedefs.h"
#include "command.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define COMMAND_ARGC            (8)     /* Maximum number of arguments is 8           */
#define COMMAND_FLG_ON          (1)
#define COMMAND_FLG_OFF         (0)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ==== Prototype declaration ==== */
static int32_t CommandCmdlineSplit(char_t * cmdline, char_t * argv[]);
static command_list_t * CommandGetCmdList(void);
static int32_t CommandCmdlinePars(int32_t argc, char_t * argv[]);

/* ==== Global variable ==== */
static char_t command_arg[COMMAND_ARGC][COMMAND_MAX_ARGLENGTH]; /* Storage for input commands */
static command_list_t * command_current_cmd;


/******************************************************************************
* Function Name: CommandExe
* Description  : execute command with buff
* Arguments    : char_t * buff : character code
* Return Value : result
******************************************************************************/
int32_t CommandExe(char_t * buff)
{
    uint32_t cnt;
    int32_t  argc;
    char_t * argv[COMMAND_ARGC];

    argc = CommandCmdlineSplit(buff, argv);

    for (cnt = 0; cnt < COMMAND_ARGC; cnt++)
    {
        argv[cnt] = command_arg[cnt];
    }

    if (argc)
    {
        return CommandCmdlinePars(argc, argv);
    }
    
    return COMMAND_SUCCESS;
}

/******************************************************************************
* Function Name: CommandCmdlineSplit
* Description  : separate character code in command line and set it up to argv
* Arguments    : char_t * cmdline;
*              : char_t * argv[];
* Return Value : int
******************************************************************************/
static int32_t CommandCmdlineSplit(char_t * cmdline, char_t * argv[])
{
    uint32_t  cnt;
    int32_t   argc;
    char_t  * buff;
    char_t  * ptr;

    for (cnt = 0; cnt < COMMAND_ARGC; cnt++)
    {
        command_arg[cnt][0] = '\0';
    }

    buff = cmdline;
    if ('>' == *buff)
    {
        buff++;
    }

    /* Analysis command line */
    for (argc = 0; argc < COMMAND_ARGC; )
    {
        ptr = command_arg[argc];
        /* Delete the white space before the string */
        while ((*buff != '\0') && (isspace((int_t)(*buff & 0xFFu))))
        {
            buff++;
        }

        cnt = 0;

        /* With double quotation marks */
        if ('\"' == *buff)
        {
            buff++;
            while ((*buff != '\"') && (*buff != '\0'))
            {
                *ptr++ = *buff++;
                cnt++;
                if (cnt >= COMMAND_MAX_ARGLENGTH)
                {
                    printf("command line is too long\n\n");
                    return COMMAND_SUCCESS;
                }
            }
            buff++;
        }

        /* Detect the white space after the string */
        while ((*buff != '\0') && (!isspace((int_t)(*buff & 0xFFu))))
        {
            *ptr++ = *buff++;
            cnt++;
            if (cnt >= COMMAND_MAX_ARGLENGTH)
            {
                printf("command line is too long\n\n");
                return COMMAND_SUCCESS;
            }
        }
        *ptr = '\0';
        argc++;
        if (('\0' == *buff) || ('\n' == *buff))
        {
            if (0 == cnt)
            {
                argc--;
            }
            break;
        }
    }

    return argc;
}

/******************************************************************************
* Function Name: CommandCmdlinePars
* Description  : Analysis command line
* Arguments    : int32_t  argc;
*              : char_t * argv[];
* Return Value : >0 ; success depend on command
*              : 0  ; success
*              : -1 ; error
******************************************************************************/
static int32_t CommandCmdlinePars(int32_t argc, char_t * argv[])
{
    command_list_t * pcmd;
    uint32_t  offset;
    char_t  * buff;

    /* Convert command strings in upper case */
    for (buff = argv[0]; *buff != '\0'; buff++)
    {
        *buff = (char_t)toupper((int_t)(*buff & 0xFFu));
    }

    /* Convert option strings to upper case */
    for (offset = 1; offset < argc; offset++)
    {
        if (('/' == argv[offset][0]) || ('+' == argv[offset][0]) || ('-' == argv[offset][0]))
        {
            for (buff = argv[offset]; *buff != '\0'; buff++)
            {
                *buff = (char_t)toupper((int_t)(*buff & 0xFFu));
            }
        }
    }

    pcmd = CommandGetCmdList();
    if (0 == pcmd)
    {
        return COMMAND_ERROR;
    }

    /* Analysis and execute request command */
    while (pcmd->cmd_str != 0)
    {
        if (!strcmp(pcmd->cmd_str, command_arg[0]))
        {
            return (*pcmd->cmdexe)(argc, argv);
        }
        pcmd++;
    }

    printf("error: %s is unsupport command\n\n", command_arg[0]);

    return COMMAND_SUCCESS;
}

/******************************************************************************
* Function Name: CommandGetCmdList
* Description  : Getting command list
* Arguments    : none
* Return Value : Top address of command list
******************************************************************************/
static command_list_t * CommandGetCmdList(void)
{
    command_list_t * pcmd;

    pcmd = command_current_cmd;

    return pcmd;
}

/******************************************************************************
* Function Name: CommandSetCmdList
* Description  : Setting command list
* Arguments    : command_list_t * cmd
* Return Value : none
******************************************************************************/
void CommandSetCmdList(const command_list_t * cmd)
{
    command_current_cmd = (command_list_t *)cmd;
}


/* End of File */

