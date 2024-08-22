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
* File Name    : devdrv_rspi.h
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description  : Renesas Serial Peripheral Interface(RSPI) device driver header
******************************************************************************/
#ifndef DEVDRV_RSPI_H
#define DEVDRV_RSPI_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* Transmit buffer data triggering number (Setting values for the TXTRG bit) */
#define RSPI_TXTRG_CURRENT      (0xFFu) /* Use current setting value */
#define RSPI_TXTRG_07_BYTE      (0u)    /* Change to  7 bytes        */
#define RSPI_TXTRG_06_BYTE      (1u)    /* Change to  6 bytes        */
#define RSPI_TXTRG_04_BYTE      (2u)    /* Change to  4 bytes        */
#define RSPI_TXTRG_00_BYTE      (3u)    /* Change to  0 byte         */

/* Receive buffer data triggering number (Setting values for the RXTRG bit) */
#define RSPI_RXTRG_CURRENT      (0xFFu) /* Use current setting value */
#define RSPI_RXTRG_01_BYTE      (0u)    /* Change to  1 byte         */
#define RSPI_RXTRG_02_BYTE      (1u)    /* Change to  2 bytes        */
#define RSPI_RXTRG_04_BYTE      (2u)    /* Change to  4 bytes        */
#define RSPI_RXTRG_08_BYTE      (3u)    /* Change to  8 bytes        */
#define RSPI_RXTRG_16_BYTE      (4u)    /* Change to 16 bytes        */
#define RSPI_RXTRG_24_BYTE      (5u)    /* Change to 24 bytes        */
#define RSPI_RXTRG_32_BYTE      (6u)    /* Change to 32 bytes        */
#define RSPI_RXTRG_05_BYTE      (7u)    /* Change to  5 bytes        */

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* ==== API functions ==== */
int32_t R_RSPI_Init(uint32_t channel);
int32_t R_RSPI_Open(uint32_t channel);
int32_t R_RSPI_Close(uint32_t channel);
int32_t R_RSPI_Read(uint32_t channel, void * buffer, uint32_t count, uint8_t rxtrg);
int32_t R_RSPI_Write(uint32_t channel, void * buffer, uint32_t count, uint8_t txtrg);
int32_t R_RSPI_ResetBuffer(uint32_t channel);
int32_t R_RSPI_SpriInterrupt(uint32_t channel);
int32_t R_RSPI_SptiInterrupt(uint32_t channel);
int32_t R_RSPI_WaitTransmitEnd(uint32_t channel);


/* ==== User-defined functions ==== */
void Userdef_RSPI0_Init(void);
void Userdef_RSPI1_Init(void);
void Userdef_RSPI2_Init(void);
void Userdef_RSPI3_Init(void);
void Userdef_RSPI4_Init(void);
void Userdef_RSPI0_InitReceiveFull(void);
void Userdef_RSPI1_InitReceiveFull(void);
void Userdef_RSPI2_InitReceiveFull(void);
void Userdef_RSPI3_InitReceiveFull(void);
void Userdef_RSPI4_InitReceiveFull(void);
void Userdef_RSPI0_InitTransmitEmpty(void);
void Userdef_RSPI1_InitTransmitEmpty(void);
void Userdef_RSPI2_InitTransmitEmpty(void);
void Userdef_RSPI3_InitTransmitEmpty(void);
void Userdef_RSPI4_InitTransmitEmpty(void);
void Userdef_RSPI0_SetReceiveFull(void);
void Userdef_RSPI1_SetReceiveFull(void);
void Userdef_RSPI2_SetReceiveFull(void);
void Userdef_RSPI3_SetReceiveFull(void);
void Userdef_RSPI4_SetReceiveFull(void);
void Userdef_RSPI0_SetTransmitEmpty(void);
void Userdef_RSPI1_SetTransmitEmpty(void);
void Userdef_RSPI2_SetTransmitEmpty(void);
void Userdef_RSPI3_SetTransmitEmpty(void);
void Userdef_RSPI4_SetTransmitEmpty(void);
void Userdef_RSPI0_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg);
void Userdef_RSPI1_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg);
void Userdef_RSPI2_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg);
void Userdef_RSPI3_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg);
void Userdef_RSPI4_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg);
void Userdef_RSPI0_WaitTransmitEmpty(void);
void Userdef_RSPI1_WaitTransmitEmpty(void);
void Userdef_RSPI2_WaitTransmitEmpty(void);
void Userdef_RSPI3_WaitTransmitEmpty(void);
void Userdef_RSPI4_WaitTransmitEmpty(void);
void Userdef_RSPI0_WaitTransmitEnd(void);
void Userdef_RSPI1_WaitTransmitEnd(void);
void Userdef_RSPI2_WaitTransmitEnd(void);
void Userdef_RSPI3_WaitTransmitEnd(void);
void Userdef_RSPI4_WaitTransmitEnd(void);
void Userdef_RSPI0_DelayNextAccess(void);
void Userdef_RSPI1_DelayNextAccess(void);
void Userdef_RSPI2_DelayNextAccess(void);
void Userdef_RSPI3_DelayNextAccess(void);
void Userdef_RSPI4_DelayNextAccess(void);


#endif  /* DEVDRV_RSPI_H */

/* End of File */
