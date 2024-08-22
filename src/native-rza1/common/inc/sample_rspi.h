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
* File Name    : sample_rspi.h
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description  : Renesas Serial Peripheral Interface(RSPI) sample program header
******************************************************************************/
#ifndef SAMPLE_RSPI_H
#define SAMPLE_RSPI_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "command.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define SAMPLE_RSPI_REQ_PROTECT     (0x00)  /* Request for protection         */
#define SAMPLE_RSPI_REQ_UNPROTECT   (0x01)  /* Request for protection release */


/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t Sample_RSPI_Main(int32_t argc, char_t ** argv);
int32_t Sample_RSPI_Help(int32_t argc, char_t ** argv);
int32_t Sample_RSPI_Exit(int32_t argc, char_t ** argv);

int32_t Sample_RSPI_EepAccess(int32_t argc, char_t ** argv);

void    Sample_RSPI_EepControlProtect_8bit(uint32_t channel, uint32_t req);
void    Sample_RSPI_EepWrite_8bit(uint32_t channel, uint32_t addr, uint8_t * buffer, uint32_t byte);
void    Sample_RSPI_EepRead_8bit(uint32_t channel, uint32_t addr, uint8_t * buffer, uint32_t byte);

void    Sample_RSPI_Spti4_Interrupt(uint32_t int_sense);
void    Sample_RSPI_Spri4_Interrupt(uint32_t int_sense);

const command_list_t * Sample_RSPI_GetCmdList(void);


#endif  /* SAMPLE_RSPI_H */

/* End of File */
