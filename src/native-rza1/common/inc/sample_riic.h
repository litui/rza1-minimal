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
* File Name    : sample_riic.h
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description  : RIIC sample program header
******************************************************************************/
#ifndef SAMPLE_RIIC_H
#define SAMPLE_RIIC_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "command.h"

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t Sample_RIIC_Main(int32_t argc, char_t ** argv);
int32_t Sample_RIIC_Help(int32_t argc, char_t ** argv);
int32_t Sample_RIIC_Exit(int32_t argc, char_t ** argv);

int32_t Sample_RIIC_EepAccess(int32_t argc, char_t ** argv);

int32_t Sample_RIIC_EepWrite(uint32_t channel, uint8_t d_code, uint8_t d_adr, 
                             uint16_t w_adr, uint32_t w_byte, const uint8_t * w_buffer);
int32_t Sample_RIIC_EepRead(uint32_t channel, uint8_t d_code, uint8_t d_adr, 
                            uint16_t r_adr, uint32_t r_byte, uint8_t * r_buffer);
void    Sample_RIIC_EepAckPolling(uint32_t channel, uint8_t d_code, uint8_t d_adr);

void Sample_RIIC_Ti3_Interrupt(uint32_t int_sense);
void Sample_RIIC_Tei3_Interrupt(uint32_t int_sense);
void Sample_RIIC_Ri3_Interrupt(uint32_t int_sense);

const command_list_t * Sample_RIIC_GetCmdList(void);

#endif  /* SAMPLE_RIIC_H */

/* End of File */
