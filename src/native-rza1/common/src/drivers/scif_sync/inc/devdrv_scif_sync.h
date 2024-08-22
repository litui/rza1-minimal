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
* File Name    : devdrv_scif_sync.h
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description  : Serial Communication Interface with FIFO(SCIF) Clock synchronous 
*              : serial communication device driver header
******************************************************************************/
#ifndef DEVDRV_SCIF_SYNC_H
#define DEVDRV_SCIF_SYNC_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/
/* ---- Mode ---- */
#define SCIF_SYNC_MODE_W    (0x00000001uL)                          /* Transfer         */
#define SCIF_SYNC_MODE_R    (0x00000002uL)                          /* Receive          */
#define SCIF_SYNC_MODE_RW   (SCIF_SYNC_MODE_W | SCIF_SYNC_MODE_R)   /* Transfer/receive */

/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* ==== API functions ==== */
int32_t R_SCIF_SYNC_Init(uint32_t channel, uint32_t mode);
int32_t R_SCIF_SYNC_Open(uint32_t channel, uint32_t mode);
int32_t R_SCIF_SYNC_Close(uint32_t channel, uint32_t mode);
int32_t R_SCIF_SYNC_Write(uint32_t channel, uint8_t * buffer, uint32_t byte);
int32_t R_SCIF_SYNC_Read(uint32_t channel, uint8_t * buffer, uint32_t byte);
int32_t R_SCIF_SYNC_TxiInterrupt(uint32_t channel);
int32_t R_SCIF_SYNC_RxiInterrupt(uint32_t channel);


/* ==== User-defined functions ==== */
void Userdef_SCIF0_SYNC_Init(uint32_t mode);
void Userdef_SCIF1_SYNC_Init(uint32_t mode);
void Userdef_SCIF2_SYNC_Init(uint32_t mode);
void Userdef_SCIF3_SYNC_Init(uint32_t mode);
void Userdef_SCIF4_SYNC_Init(uint32_t mode);
void Userdef_SCIF5_SYNC_Init(uint32_t mode);
void Userdef_SCIF6_SYNC_Init(uint32_t mode);
void Userdef_SCIF7_SYNC_Init(uint32_t mode);

void Userdef_SCIF0_SYNC_InitTransmitEmpty(void);
void Userdef_SCIF1_SYNC_InitTransmitEmpty(void);
void Userdef_SCIF2_SYNC_InitTransmitEmpty(void);
void Userdef_SCIF3_SYNC_InitTransmitEmpty(void);
void Userdef_SCIF4_SYNC_InitTransmitEmpty(void);
void Userdef_SCIF5_SYNC_InitTransmitEmpty(void);
void Userdef_SCIF6_SYNC_InitTransmitEmpty(void);
void Userdef_SCIF7_SYNC_InitTransmitEmpty(void);

void Userdef_SCIF0_SYNC_InitReceiveFull(void);
void Userdef_SCIF1_SYNC_InitReceiveFull(void);
void Userdef_SCIF2_SYNC_InitReceiveFull(void);
void Userdef_SCIF3_SYNC_InitReceiveFull(void);
void Userdef_SCIF4_SYNC_InitReceiveFull(void);
void Userdef_SCIF5_SYNC_InitReceiveFull(void);
void Userdef_SCIF6_SYNC_InitReceiveFull(void);
void Userdef_SCIF7_SYNC_InitReceiveFull(void);

void Userdef_SCIF0_SYNC_SetTransmitEmpty(void);
void Userdef_SCIF1_SYNC_SetTransmitEmpty(void);
void Userdef_SCIF2_SYNC_SetTransmitEmpty(void);
void Userdef_SCIF3_SYNC_SetTransmitEmpty(void);
void Userdef_SCIF4_SYNC_SetTransmitEmpty(void);
void Userdef_SCIF5_SYNC_SetTransmitEmpty(void);
void Userdef_SCIF6_SYNC_SetTransmitEmpty(void);
void Userdef_SCIF7_SYNC_SetTransmitEmpty(void);

void Userdef_SCIF0_SYNC_SetReceiveFull(void);
void Userdef_SCIF1_SYNC_SetReceiveFull(void);
void Userdef_SCIF2_SYNC_SetReceiveFull(void);
void Userdef_SCIF3_SYNC_SetReceiveFull(void);
void Userdef_SCIF4_SYNC_SetReceiveFull(void);
void Userdef_SCIF5_SYNC_SetReceiveFull(void);
void Userdef_SCIF6_SYNC_SetReceiveFull(void);
void Userdef_SCIF7_SYNC_SetReceiveFull(void);

void Userdef_SCIF0_SYNC_WaitTransmitEmpty(void);
void Userdef_SCIF1_SYNC_WaitTransmitEmpty(void);
void Userdef_SCIF2_SYNC_WaitTransmitEmpty(void);
void Userdef_SCIF3_SYNC_WaitTransmitEmpty(void);
void Userdef_SCIF4_SYNC_WaitTransmitEmpty(void);
void Userdef_SCIF5_SYNC_WaitTransmitEmpty(void);
void Userdef_SCIF6_SYNC_WaitTransmitEmpty(void);
void Userdef_SCIF7_SYNC_WaitTransmitEmpty(void);

int32_t Userdef_SCIF0_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg);
int32_t Userdef_SCIF1_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg);
int32_t Userdef_SCIF2_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg);
int32_t Userdef_SCIF3_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg);
int32_t Userdef_SCIF4_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg);
int32_t Userdef_SCIF5_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg);
int32_t Userdef_SCIF6_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg);
int32_t Userdef_SCIF7_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg);

void Userdef_SCIF0_SYNC_WaitTransmitEnd(void);
void Userdef_SCIF1_SYNC_WaitTransmitEnd(void);
void Userdef_SCIF2_SYNC_WaitTransmitEnd(void);
void Userdef_SCIF3_SYNC_WaitTransmitEnd(void);
void Userdef_SCIF4_SYNC_WaitTransmitEnd(void);
void Userdef_SCIF5_SYNC_WaitTransmitEnd(void);
void Userdef_SCIF6_SYNC_WaitTransmitEnd(void);
void Userdef_SCIF7_SYNC_WaitTransmitEnd(void);

int32_t Userdef_SCIF0_SYNC_ErrReceiveOverrun(void);
int32_t Userdef_SCIF1_SYNC_ErrReceiveOverrun(void);
int32_t Userdef_SCIF2_SYNC_ErrReceiveOverrun(void);
int32_t Userdef_SCIF3_SYNC_ErrReceiveOverrun(void);
int32_t Userdef_SCIF4_SYNC_ErrReceiveOverrun(void);
int32_t Userdef_SCIF5_SYNC_ErrReceiveOverrun(void);
int32_t Userdef_SCIF6_SYNC_ErrReceiveOverrun(void);
int32_t Userdef_SCIF7_SYNC_ErrReceiveOverrun(void);


#endif  /* DEVDRV_SCIF_SYNC_H */

/* End of File */
