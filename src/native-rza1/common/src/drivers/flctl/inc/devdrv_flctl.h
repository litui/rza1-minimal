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
/******************************************************************************
* File Name    : devdrv_flctl.h
* $Rev: 1516 $
* $Date:: 2015-08-25 15:47:15 +0900#$
* Description  : NAND FLASH Memory Controller(FLCTL)  device driver header
******************************************************************************/
#ifndef DEVDRV_FLCTL_H
#define DEVDRV_FLCTL_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Macro definitions ==== */
/* ---- Definition of sector unit size ---- */
#define SECTOR_SZ_DATA_ONLY     (512)             /* Bytes of data only */
#define SECTOR_SZ_EXT_ONLY      (16)              /* Bytes of control code only */
#define SECTOR_SZ_DATA_AND_EXT  (SECTOR_SZ_DATA_ONLY + SECTOR_SZ_EXT_ONLY) /* Bytes of all area */
/* ---- Definition of page unit size ---- */
#define SECTOR_OF_PAGE          (4)               /* Number of sectors per 1page  */
#define PAGE_SZ_DATA_ONLY       (SECTOR_SZ_DATA_ONLY * SECTOR_OF_PAGE)
#define PAGE_SZ_EXT_ONLY        (SECTOR_SZ_EXT_ONLY  * SECTOR_OF_PAGE)
#define PAGE_SZ_DATA_AND_EXT    (PAGE_SZ_DATA_ONLY + PAGE_SZ_EXT_ONLY)
/* ---- Definition of block unit size ---- */
#define PAGE_OF_BLOCK           (64)              /* Number of pages per 1block       */
#define SECTOR_OF_BLOCK         (PAGE_OF_BLOCK * SECTOR_OF_PAGE)
#define BLOCK_SZ_DATA_ONLY      (SECTOR_SZ_DATA_ONLY * SECTOR_OF_BLOCK)
#define BLOCK_SZ_EXT_ONLY       (SECTOR_SZ_EXT_ONLY  * SECTOR_OF_BLOCK)
#define BLOCK_SZ_DATA_AND_EXT   (BLOCK_SZ_DATA_ONLY + BLOCK_SZ_EXT_ONLY)


/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
/* ==== API functions ==== */
int32_t R_FLCTL_Init(void);
int32_t R_FLCTL_Open(void);
int32_t R_FLCTL_Close(void);
int32_t R_FLCTL_Reset(void);
int32_t R_FLCTL_Erase (int32_t sctr);
int32_t R_FLCTL_Read  (int32_t sctr, int32_t num, uint32_t *data);
int32_t R_FLCTL_Write (int32_t sctr, int32_t num, uint32_t *data);
int32_t R_FLCTL_Read_Id (uint8_t *data);


/* ==== User-defined functions ==== */
void Userdef_FLCTL_Init(void);


#endif  /* DEVDRV_FLCTL_H */

/* End of File */
