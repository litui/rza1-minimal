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
* File Name   : siochar.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Serial I/O character R/W (SCIF 2-ch process)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_scif_uart.h"       /* UART Driver header */
#include "sio_char.h"
#include "iodefine.h"
#include "rza_io_regrw.h"
#include "board.h"
#include "gpio.h"

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


/******************************************************************************
* Function Name: IoInitScif
* Description  : This function initializes SCIF in UART mode.
*              : The transmit and the receive of SCIF channel are enabled.
* Arguments    : none
* Return Value : none
******************************************************************************/
void IoInitScif(void)
{
    devdrv_ch_t channel = R_SERIAL_UART;
    volatile struct st_scif * scif = SCIF_GetRegAddr(channel);

    /* === Initialization of SCIF2 ==== */
    /* P1 clock=66.67MHz CKS=0 SCBRR=17 Bit rate error=0.46% => Baud rate=115200bps */
    R_SCIF_UART_Init(channel, SCIF_UART_MODE_RW, SCIF_CKS_DIVISION_1, 17);

    init_gpio_as_alt(R_SERIAL_TX_PORT, R_SERIAL_TX_PIN, R_SERIAL_TX_MUX, false);
    init_gpio_as_alt(R_SERIAL_RX_PORT, R_SERIAL_RX_PIN, R_SERIAL_RX_MUX, true);

    /* === Enable SCIF transmission/reception ==== */
    scif->SCSCR = 0x0030u;
}

/******************************************************************************
* Function Name: IoGetchar
* Description  : One character is received from SCIF, and it's data is returned.
*              : This function keeps waiting until it can obtain the receiving data.
* Arguments    : none
* Return Value : Character to receive (Byte).
******************************************************************************/
int_t IoGetchar(void)
{
    volatile struct st_scif * scif = SCIF_GetRegAddr(R_SERIAL_UART);

    int_t data;

    /* Confirming receive error(ER,BRK,FER,PER,ORER) */
    if (((scif->SCFSR & 0x09Cu) != 0) ||
        (1 == RZA_IO_RegRead_16(&(scif->SCLSR), SCIFn_SCLSR_ORER_SHIFT, SCIFn_SCLSR_ORER)))
    {
        /* ---- Detect receive error ---- */
        /* Disable reception             */
        /* Reset receiving FIFO          */
        /* Clearing FIFO reception reset */
        /* Error bit clear               */
        /* Enable reception              */
        RZA_IO_RegWrite_16(&(scif->SCSCR), 0, SCIFn_SCSCR_RE_SHIFT,    SCIFn_SCSCR_RE);
        RZA_IO_RegWrite_16(&(scif->SCFCR), 1, SCIFn_SCFCR_RFRST_SHIFT, SCIFn_SCFCR_RFRST);
        RZA_IO_RegWrite_16(&(scif->SCFCR), 0, SCIFn_SCFCR_RFRST_SHIFT, SCIFn_SCFCR_RFRST);
        scif->SCFSR &= ~0x9Cu;
        RZA_IO_RegWrite_16(&(scif->SCLSR), 0, SCIFn_SCLSR_ORER_SHIFT,  SCIFn_SCLSR_ORER);
        RZA_IO_RegWrite_16(&(scif->SCSCR), 1, SCIFn_SCSCR_RE_SHIFT,    SCIFn_SCSCR_RE);

        return -1;
    }

    /* Is there receive FIFO data? */
    while (0 == RZA_IO_RegRead_16(&(scif->SCFSR), SCIFn_SCFSR_RDF_SHIFT, SCIFn_SCFSR_RDF))
    {
        /* WAIT */
    }

    /* Read receive data */
    data = (int_t)(scif->SCFRDR & 0xFFu);
    /* Clear DR,RDF */
    RZA_IO_RegWrite_16(&(scif->SCFSR), 0, SCIFn_SCFSR_DR_SHIFT, SCIFn_SCFSR_DR);
    RZA_IO_RegWrite_16(&(scif->SCFSR), 0, SCIFn_SCFSR_RDF_SHIFT, SCIFn_SCFSR_RDF);

    return data;
}

/******************************************************************************
* Function Name: IoPutchar
* Description  : Character "buffer" is output to SCIF2.
*              : This function keeps waiting until it becomes the transmission
*              : enabled state.
* Arguments    : int_t buffer : character to output
* Return Value : None
******************************************************************************/
void IoPutchar(int_t buffer)
{
    volatile struct st_scif * scif = SCIF_GetRegAddr(R_SERIAL_UART);

    /* Check if it is possible to transmit (TDFE flag) */
    while (0 == RZA_IO_RegRead_16(&(scif->SCFSR), SCIFn_SCFSR_TDFE_SHIFT, SCIFn_SCFSR_TDFE))
    {
        /* Wait */
    }

    /* Write the receiving data in TDR */
    scif->SCFTDR = (uint8_t)buffer;

    /* Clear TDRE and TEND flag */
    scif->SCFSR &= ~0x0060u;
}


/* End of File */

