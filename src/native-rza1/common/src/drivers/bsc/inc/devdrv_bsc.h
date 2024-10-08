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
* File Name    : devdrv_bsc.h
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description  : Common driver header
******************************************************************************/
#ifndef DEVDRV_COMMON_H
#define DEVDRV_COMMON_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define BSC_AREA_CS0    (0x01)          /* CS0 space */
#define BSC_AREA_CS1    (0x02)          /* CS1 space */
#define BSC_AREA_CS2    (0x04)          /* CS2 space */
#define BSC_AREA_CS3    (0x08)          /* CS3 space */
#define BSC_AREA_CS4    (0x10)          /* CS4 space */
#define BSC_AREA_CS5    (0x20)          /* CS5 space */

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* ==== API function ==== */
void R_BSC_Init(uint8_t area);

/* ==== User-defined functions ==== */
void Userdef_BSC_CS0Init(void);
void Userdef_BSC_CS1Init(void);
void Userdef_BSC_CS2Init(void);
void Userdef_BSC_CS3Init(void);
void Userdef_BSC_CS4Init(void);
void Userdef_BSC_CS5Init(void);


#endif  /* DEVDRV_COMMON_H */

/* End of File */
