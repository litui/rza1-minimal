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
* File Name   : scif_uart_userdef.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : SCIF UART driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_scif_uart.h"       /* UART Driver header */
#include "iodefine.h"
#include "rza_io_regrw.h"
#include "unused.h"

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

static void SetClockStandby(devdrv_ch_t channel, bool off_on);
void GenericUARTInit(devdrv_ch_t channel, uint8_t mode, uint16_t cks, uint8_t scbrr);

static void SetClockStandby(devdrv_ch_t channel, bool off_on)
{
    const uint8_t mstp_shift = 7U - (uint8_t)channel;
    const uint8_t mstp_mask = 1U << mstp_shift;

    volatile uint8_t dummy_buf;

    /* 
     * Write value (0 or 1) to the bit for the correct UART module in the Standby Control Register
     */
    RZA_IO_RegWrite_8(&CPG.STBCR4, off_on ? 1 : 0, mstp_shift, mstp_mask);
    dummy_buf = CPG.STBCR4;

    R_UNUSED(dummy_buf);
}

void GenericUARTInit(devdrv_ch_t channel, uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    volatile uint8_t dummy_buf;
    volatile struct st_scif * scif = SCIF_GetRegAddr(channel);

    /* ==== Module standby clear ==== */
    SetClockStandby(channel, false);

    /* ==== SCIF initial setting ==== */
    /* ---- Serial control register (SCSCR) setting ---- */
    scif->SCSCR = 0x0000u;          /* SCIF transmitting and receiving operations stop */

    /* ---- FIFO control register (SCFCR) setting ---- */
    if (SCIF_UART_MODE_W == (mode & SCIF_UART_MODE_W))      /* Use transmit */
    {
        /* Transmit FIFO reset */
        RZA_IO_RegWrite_16(&(scif->SCFCR), 1, SCIFn_SCFCR_TFRST_SHIFT, SCIFn_SCFCR_TFRST);
    }

    if (SCIF_UART_MODE_R == (mode & SCIF_UART_MODE_R))      /* Use receive */
    {
        /* Receive FIFO data register reset */
        RZA_IO_RegWrite_16(&(scif->SCFCR), 1, SCIFn_SCFCR_RFRST_SHIFT, SCIFn_SCFCR_RFRST);
    }

    /* ---- Serial status register (SCFSR) setting ---- */
    scif->SCFSR &= 0xFF6Eu;         /* ER,BRK,DR bit clear */

    /* ---- Line status register (SCLSR) setting ---- */
    /* ORER bit clear */
    RZA_IO_RegWrite_16(&(scif->SCLSR), 0, SCIFn_SCLSR_ORER_SHIFT, SCIFn_SCLSR_ORER);

    /* ---- Serial control register (SCSCR) setting ---- */
    /* B'00 : Internal CLK */
    RZA_IO_RegWrite_16(&(scif->SCSCR), 0, SCIFn_SCSCR_CKE_SHIFT, SCIFn_SCSCR_CKE);

    /* ---- Serial mode register (SCSMR) setting ----
    b7    C/A  - Communication mode : Asynchronous mode
    b6    CHR  - Character length   : 8-bit data
    b5    PE   - Parity enable      : Add and check are disabled
    b3    STOP - Stop bit length    : 1 stop bit
    b1:b0 CKS  - Clock select       : cks(argument) */
    scif->SCSMR = cks & 0x0003u;

    /* ---- Serial extension mode register (SCEMR) setting ---- 
    b7 BGDM - Baud rate generator double-speed mode  : Normal mode
    b0 ABCS - Base clock select in asynchronous mode : Base clock is 16 times the bit rate */
    scif->SCEMR = 0x0000u;

    /* ---- Bit rate register (SCBRR) setting ---- */
    scif->SCBRR = scbrr;

    /* ---- FIFO control register (SCFCR) setting ---- */
    if (SCIF_UART_MODE_RW == mode)
    {
        /* b10:b8 RSTRG - RTS output active trigger         : Initial value
           b7:b6  RTRG  - Receive FIFO data trigger         : 1-data 
           b5:b4  TTRG  - Transmit FIFO data trigger        : 0-data
           b3     MCE   - Modem control enable              : Disabled
           b2     TFRST - Transmit FIFO data register reset : Disabled
           b1     RFRST - Receive FIFO data register reset  : Disabled
           b0     LOOP  - Loop-back test                    : Disabled */
        scif->SCFCR = 0x0030u;
    }
    else
    {
        if (SCIF_UART_MODE_W == (mode & SCIF_UART_MODE_W))      /* Use transmit */
        {
            /* b10:b8 RSTRG - RTS output active trigger         : Initial value
               b7:b6  RTRG  - Receive FIFO data trigger         : 1-data 
               b5:b4  TTRG  - Transmit FIFO data trigger        : 0-data
               b3     MCE   - Modem control enable              : Disabled
               b2     TFRST - Transmit FIFO data register reset : Disabled
               b1     RFRST - Receive FIFO data register reset  : Enabled
               b0     LOOP  - Loop-back test                    : Disabled */
            scif->SCFCR = 0x0032u;
        }
        else if (SCIF_UART_MODE_R == (mode & SCIF_UART_MODE_R))  /* Use receive */
        {
            /* b10:b8 RSTRG - RTS output active trigger         : Initial value
               b7:b6  RTRG  - Receive FIFO data trigger         : 1-data 
               b5:b4  TTRG  - Transmit FIFO data trigger        : 0-data
               b3     MCE   - Modem control enable              : Disabled
               b2     TFRST - Transmit FIFO data register reset : Enabled
               b1     RFRST - Receive FIFO data register reset  : Disabled
               b0     LOOP  - Loop-back test                    : Disabled */
            scif->SCFCR = 0x0034u;
        }
        else
        {
            /* b10:b8 RSTRG - RTS output active trigger         : Initial value
               b7:b6  RTRG  - Receive FIFO data trigger         : 1-data 
               b5:b4  TTRG  - Transmit FIFO data trigger        : 0-data
               b3     MCE   - Modem control enable              : Disabled
               b2     TFRST - Transmit FIFO data register reset : Enabled
               b1     RFRST - Receive FIFO data register reset  : Enabled
               b0     LOOP  - Loop-back test                    : Disabled */
            scif->SCFCR = 0x0036u;
        }
    }

    /* ---- Serial port register (SCSPTR) setting ---- 
    b1 SPB2IO - Serial port break output : Enabled
    b0 SPB2DT - Serial port break data   : High-level */
    scif->SCSPTR |= 0x0003u;

    R_UNUSED(dummy_buf);
}

void Userdef_SCIF0_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    GenericUARTInit(DEVDRV_CH_0, mode, cks, scbrr);
}

void Userdef_SCIF1_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    GenericUARTInit(DEVDRV_CH_1, mode, cks, scbrr);
}

void Userdef_SCIF2_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    GenericUARTInit(DEVDRV_CH_2, mode, cks, scbrr);
}

void Userdef_SCIF3_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    GenericUARTInit(DEVDRV_CH_3, mode, cks, scbrr);
}

void Userdef_SCIF4_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    GenericUARTInit(DEVDRV_CH_4, mode, cks, scbrr);
}

void Userdef_SCIF5_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    GenericUARTInit(DEVDRV_CH_5, mode, cks, scbrr);
}

void Userdef_SCIF6_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    GenericUARTInit(DEVDRV_CH_6, mode, cks, scbrr);
}

void Userdef_SCIF7_UART_Init(uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    GenericUARTInit(DEVDRV_CH_7, mode, cks, scbrr);
}

/* End of File */

