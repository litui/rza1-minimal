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
* File Name   : rza_io_regrw.h
* $Rev: 1738 $
* $Date:: 2016-10-18 12:05:02 +0900#$
* Description : Low level register read/write header
*******************************************************************************/
#ifndef RZA_IO_REGRW_H
#define RZA_IO_REGRW_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* ==== includes each bit mask header ==== */
#include "iobitmasks/cpg_iobitmask.h"
#include "iobitmasks/intc_iobitmask.h"
#include "iobitmasks/bsc_iobitmask.h"
#include "iobitmasks/dmac_iobitmask.h"
#include "iobitmasks/mtu2_iobitmask.h"
#include "iobitmasks/ostm_iobitmask.h"
#include "iobitmasks/scif_iobitmask.h"
#include "iobitmasks/rspi_iobitmask.h"
#include "iobitmasks/riic_iobitmask.h"
#include "iobitmasks/gpio_iobitmask.h"
#include "iobitmasks/flctl_iobitmask.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
void     RZA_IO_RegWrite_8 (volatile uint8_t  * ioreg, uint8_t  write_value, uint8_t  shift, uint8_t  mask);
void     RZA_IO_RegWrite_16(volatile uint16_t * ioreg, uint16_t write_value, uint16_t shift, uint16_t mask);
void     RZA_IO_RegWrite_32(volatile uint32_t * ioreg, uint32_t write_value, uint32_t shift, uint32_t mask);
uint8_t  RZA_IO_RegRead_8  (volatile uint8_t  * ioreg, uint8_t  shift, uint8_t  mask);
uint16_t RZA_IO_RegRead_16 (volatile uint16_t * ioreg, uint16_t shift, uint16_t mask);
uint32_t RZA_IO_RegRead_32 (volatile uint32_t * ioreg, uint32_t shift, uint32_t mask);


#endif  /* RZA_IO_REGRW_H */

/* End of File */
