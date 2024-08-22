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
* File Name   : scif_sync_userdef.c
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description : Serial Communication Interface with FIFO(SCIF) Clock synchronous 
*             : serial communication device driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_scif_sync.h"       /* SCIF SYNC Driver header */
#include "devdrv_intc.h"            /* INTC Driver Header */
#include "sample_scif_sync.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Setting for bit rate (SCBRR register, CKS bit) ==== */
/* CPU internal clock (I clock) : 400MHz, Internal bus clock (B clock) : 133.33MHz   */
/* Peripheral clock1 (P1 clock) : 66.67MHz, Peripheral clock0 (P0 clock) : 33.33MHz  */
/* Setting to operate the bit rate at the speed of 250kbps with the conditions above */
#define SCIF_SYNC_SCBRR     (66)    /* Setting value of the SCBRR register (0 to 255) */
#define SCIF_SYNC_CKS       (0)     /* Setting value of the CKS bit in the SCSMR      */
                                    /* register (0 to 3)                              */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static volatile uint8_t scif_sync_transmit_empty_flg;   /* Transmit FIFO data empty flag */
static volatile uint8_t scif_sync_receive_full_flg;     /* Receive FIFO data full flag   */


/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_Init
* Description  : The SCIF initial setting and the setting for the interrupts are 
*              : required to execute the transmit, receive, and 
*              : transmit/receive operations specified by the argument mode when 
*              : the SCIF channel 0 is used. 
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : uint32_t mode : Setting for transfer direction 
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
void Userdef_SCIF0_SYNC_Init(uint32_t mode)
{
    /* Add a processing when the SCIF0 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_Init
* Description  : The SCIF initial setting and the setting for the interrupts are
*              : required to execute the transmit, receive, and 
*              : transmit/receive operations specified by the argument mode when
*              : the SCIF channel 1 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : uint32_t mode : Setting for transfer direction  
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
void Userdef_SCIF1_SYNC_Init(uint32_t mode)
{
    /* Add a processing when the SCIF1 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_Init
* Description  : The SCIF initial setting and the setting for the interrupts are 
*              : required to execute the transmit, receive, and 
*              : transmit/receive operations specified by the argument mode when
*              : the SCIF channel 2 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : uint32_t mode : Setting for transfer direction 
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
void Userdef_SCIF2_SYNC_Init(uint32_t mode)
{
    /* Add a processing when the SCIF2 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_Init
* Description  : Initial setting is required to use the SCIF channel 3.
*              : In the sample code, the loop-back testing in the clock 
*              : synchronous mode is enabled, and the SCIF transmission/reception
*              : can be performed with the bit rate of 250kbps. Internal clock
*              : is used for the clock source, the SCK pin is set to output the 
*              : synchronous clock. When the transmit FIFO buffer has the data 
*              : less than 8 bytes, the transmit FIFO data empty flag is set. 
*              : When the receive FIFO buffer has the data of 8 bytes or more, 
*              : the SCIF is configured for the receive FIFO data full flag to
*              : be set.
*              : The INTC is set to use the transmit FIFO data empty interrupt, 
*              : the receive FIFO data full interrupt. In this function, the 
*              : SCIF is set to disable the transmit interrupt and the receive
*              : interrupt.
* Arguments    : uint32_t mode : Setting for transfer direction  
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
void Userdef_SCIF3_SYNC_Init(uint32_t mode)
{
    volatile uint8_t dummy_buf;

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the SCIF(channel 3) ---- */
    RZA_IO_RegWrite_8(&CPG.STBCR4, 0, CPG_STBCR4_MSTP44_SHIFT, CPG_STBCR4_MSTP44);
    dummy_buf = CPG.STBCR4;     /* Dummy read */

    /* ==== SCIF initial setting ==== */
    /* SCSCR - Serial control register
    b7 TIE  - Transmit FIFO data empty interrupt (TXI) : Disabled
    b6 RIE  - Receive FIFO data full interrupt (RXI)   : Disabled
    b5 TE   - Transmit operation : Disabled
    b4 RE   - Receive operation : Disabled
    b3 REIE - Receive error interrupt (ERI), Break interrupt (BRI) : Disabled */
    SCIF3.SCSCR = 0x0000u;

    /* SCSMR - Serial mode register
    b7    C/A - Communication mode : Clock synchronous mode
    b1:b0 CKS - Clock select       : SCIF_SYNC_CKS */
    SCIF3.SCSMR = 0x0080u | SCIF_SYNC_CKS;

    /* SCSCR - Serial control register
    b1:b0 CKE - Clock source : Internal clock, SCK pin : Synchronous clock output */
    RZA_IO_RegWrite_16(&(SCIF3.SCSCR), 0, SCIFn_SCSCR_CKE_SHIFT, SCIFn_SCSCR_CKE);

    /* SCBRR - Bit rate register */
    SCIF3.SCBRR = SCIF_SYNC_SCBRR;

    /* SCFCR - FIFO control register
    b0     LOOP  - Loop-back testing     : Enabled */
    RZA_IO_RegWrite_16(&(SCIF3.SCFCR), 1, SCIFn_SCFCR_LOOP_SHIFT, SCIFn_SCFCR_LOOP);

    if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
    {
        /* SCFCR - FIFO control register
        b5:b4  TTRG  - Transmit FIFO data triggering number : 8 bytes or less */
        RZA_IO_RegWrite_16(&(SCIF3.SCFCR), 0, SCIFn_SCFCR_TTRG_SHIFT, SCIFn_SCFCR_TTRG);

        /* ==== Interrupt setting ==== */
        /* Register the TXIn interrupt function */
        R_INTC_RegistIntFunc(INTC_ID_TXI3, Sample_SCIF_SYNC_Txi3_Interrupt);
        R_INTC_SetPriority(INTC_ID_TXI3, 11);   /* TXIn interrupt priority = 11 is specified */
        R_INTC_Enable(INTC_ID_TXI3);            /* TXIn interrupt enabled */
    }
    if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
    {
        /* SCFCR - FIFO control register
        b7:b6  RTRG  - Receive FIFO data triggering number : 8 bytes or more */
        RZA_IO_RegWrite_16(&(SCIF3.SCFCR), 2, SCIFn_SCFCR_RTRG_SHIFT, SCIFn_SCFCR_RTRG);

        /* ==== Interrupt setting ==== */
        /* Register the RXIn interrupt function */
        R_INTC_RegistIntFunc(INTC_ID_RXI3, Sample_SCIF_SYNC_Rxi3_Interrupt);
        R_INTC_SetPriority(INTC_ID_RXI3, 11);   /* RXIn interrupt priority = 11 is specified */
        R_INTC_Enable(INTC_ID_RXI3);            /* RXIn interrupt enabled */
    }
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_Init
* Description  : The SCIF initial setting and the setting for the interrupts are 
*              : required to execute the transmit, receive, and 
*              : transmit/receive operations specified by the argument mode when
*              : the SCIF channel 4 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : uint32_t mode : Setting for transfer direction 
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
void Userdef_SCIF4_SYNC_Init(uint32_t mode)
{
    /* Add a processing when the SCIF4 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_Init
* Description  : The SCIF initial setting and the setting for the interrupts are 
*              : required to execute the transmit, receive, and 
*              : transmit/receive operations specified by the argument mode when 
*              : the SCIF channel 5 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : uint32_t mode : Setting for transfer direction  
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
void Userdef_SCIF5_SYNC_Init(uint32_t mode)
{
    /* Add a processing when the SCIF5 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_Init
* Description  : The SCIF initial setting and the setting for the interrupts are 
*              : required to execute the transmit, receive, and 
*              : transmit/receive operations specified by the argument mode when 
*              : the SCIF channel 6 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : uint32_t mode : Setting for transfer direction  
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
void Userdef_SCIF6_SYNC_Init(uint32_t mode)
{
    /* Add a processing when the SCIF6 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_Init
* Description  : The SCIF initial setting and the setting for the interrupts are 
*              : required to execute the transmit, receive, and 
*              : transmit/receive operations specified by the argument mode when
*              : the SCIF channel 7 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : uint32_t mode : Setting for transfer direction  
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
void Userdef_SCIF7_SYNC_Init(uint32_t mode)
{
    /* Add a processing when the SCIF7 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_InitTransmitEmpty
* Description  : Execute this function to initialize the SCIF channel 0 transmit
*              : FIFO data empty notification information when the SCIF channel 
*              : 0 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF0_SYNC_InitTransmitEmpty(void)
{
    /* Add a processing when the SCIF0 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_InitTransmitEmpty
* Description  : Execute this function to initialize the SCIF channel 1 transmit
*              : FIFO data empty notification information when the SCIF channel 
*              : 1 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF1_SYNC_InitTransmitEmpty(void)
{
    /* Add a processing when the SCIF1 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_InitTransmitEmpty
* Description  : Execute this function to initialize the SCIF channel 2 transmit
*              : FIFO data empty notification information when the SCIF channel 
*              : 2 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF2_SYNC_InitTransmitEmpty(void)
{
    /* Add a processing when the SCIF2 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_InitTransmitEmpty
* Description  : Execute this function to initialize the SCIF channel 3 transmit
*              : FIFO data empty notification information.
*              : In the sample code, 0 is set to the software flag to notify
*              : that the SCIF channel 3 transmit FIFO data empty interrupt has
*              : been generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF3_SYNC_InitTransmitEmpty(void)
{
    scif_sync_transmit_empty_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_InitTransmitEmpty
* Description  : Execute this function to initialize the SCIF channel 4 transmit
*              : FIFO data empty notification information when the SCIF channel 
*              : 4 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF4_SYNC_InitTransmitEmpty(void)
{
    /* Add a processing when the SCIF4 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_InitTransmitEmpty
* Description  : Execute this function to initialize the SCIF channel 5 transmit
*              : FIFO data empty notification information when the SCIF channel 
*              : 5 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF5_SYNC_InitTransmitEmpty(void)
{
    /* Add a processing when the SCIF5 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_InitTransmitEmpty
* Description  : Execute this function to initialize the SCIF channel 6 transmit
*              : FIFO data empty notification information when the SCIF channel 
*              : 6 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF6_SYNC_InitTransmitEmpty(void)
{
    /* Add a processing when the SCIF6 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_InitTransmitEmpty
* Description  : Execute this function to initialize the SCIF channel 7 transmit
*              : FIFO data empty notification information when the SCIF channel 
*              : 7 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF7_SYNC_InitTransmitEmpty(void)
{
    /* Add a processing when the SCIF7 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_InitReceiveFull
* Description  : Execute this function to initialize the SCIF channel 0 receive
*              : FIFO data full notification information when the SCIF channel 
*              : 0 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF0_SYNC_InitReceiveFull(void)
{
    /* Add a processing when the SCIF0 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_InitReceiveFull
* Description  : Execute this function to initialize the SCIF channel 1 receive
*              : FIFO data full notification information when the SCIF channel 
*              : 1 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF1_SYNC_InitReceiveFull(void)
{
    /* Add a processing when the SCIF1 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_InitReceiveFull
* Description  : Execute this function to initialize the SCIF channel 2 receive
*              : FIFO data full notification information when the SCIF channel 
*              : 2 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF2_SYNC_InitReceiveFull(void)
{
    /* Add a processing when the SCIF2 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_InitReceiveFull
* Description  : Execute this function to initialize the SCIF channel 3 receive
*              : FIFO data full notification information.
*              : In the sample code, 0 is set to the software flag to notify 
*              : that the SCIF channel 3 receive FIFO data full interrupt has 
*              : been generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF3_SYNC_InitReceiveFull(void)
{
    scif_sync_receive_full_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_InitReceiveFull
* Description  : Execute this function to initialize the SCIF channel 4 receive
*              : FIFO data full notification information when the SCIF channel 
*              : 4 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF4_SYNC_InitReceiveFull(void)
{
    /* Add a processing when the SCIF4 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_InitReceiveFull
* Description  : Execute this function to initialize the SCIF channel 5 receive
*              : FIFO data full notification information when the SCIF channel 
*              : 5 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF5_SYNC_InitReceiveFull(void)
{
    /* Add a processing when the SCIF5 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_InitReceiveFull
* Description  : Execute this function to initialize the SCIF channel 6 receive
*              : FIFO data full notification information when the SCIF channel 
*              : 6 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF6_SYNC_InitReceiveFull(void)
{
    /* Add a processing when the SCIF6 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_InitReceiveFull
* Description  : Execute this function to initialize the SCIF channel 7 receive
*              : FIFO data full notification information when the SCIF channel 
*              : 7 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF7_SYNC_InitReceiveFull(void)
{
    /* Add a processing when the SCIF7 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 0 transmit FIFO data empty notification information  
*              : when the SCIF channel 0 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF0_SYNC_SetTransmitEmpty(void)
{
    /* Add a processing when the SCIF0 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 1 transmit FIFO data empty notification information  
*              : when the SCIF channel 1 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF1_SYNC_SetTransmitEmpty(void)
{
    /* Add a processing when the SCIF1 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 2 transmit FIFO data empty notification information  
*              : when the SCIF channel 2 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF2_SYNC_SetTransmitEmpty(void)
{
    /* Add a processing when the SCIF2 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the SCIF
*              : channel 3 transmit FIFO data empty notification information.
*              : In the sample code, 1 is set to the software flag to notify 
*              : that the SCIF channel 3 transmit FIFO data empty interrupt 
*              : has been generated.
*              : The transmit FIFO data empty interrupt is disabled.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF3_SYNC_SetTransmitEmpty(void)
{
    /* Disable the transmit interrupt request to be issued */
    RZA_IO_RegWrite_16(&SCIF3.SCSCR, 0, SCIFn_SCSCR_TIE_SHIFT, SCIFn_SCSCR_TIE);

    scif_sync_transmit_empty_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 4 transmit FIFO data empty notification information  
*              : when the SCIF channel 4 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF4_SYNC_SetTransmitEmpty(void)
{
    /* Add a processing when the SCIF4 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 5 transmit FIFO data empty notification information  
*              : when the SCIF channel 5 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF5_SYNC_SetTransmitEmpty(void)
{
    /* Add a processing when the SCIF5 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 6 transmit FIFO data empty notification information  
*              : when the SCIF channel 6 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF6_SYNC_SetTransmitEmpty(void)
{
    /* Add a processing when the SCIF6 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 7 transmit FIFO data empty notification information  
*              : when the SCIF channel 7 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF7_SYNC_SetTransmitEmpty(void)
{
    /* Add a processing when the SCIF7 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 0 receive FIFO data full notification information  
*              : when the SCIF channel 0 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF0_SYNC_SetReceiveFull(void)
{
    /* Add a processing when the SCIF0 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 1 receive FIFO data full notification information  
*              : when the SCIF channel 1 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF1_SYNC_SetReceiveFull(void)
{
    /* Add a processing when the SCIF1 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 2 receive FIFO data full notification information  
*              : when the SCIF channel 2 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF2_SYNC_SetReceiveFull(void)
{
    /* Add a processing when the SCIF2 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the SCIF
*              : channel 3 receive FIFO data full notification information.
*              : In the sample code, 1 is set to the software flag to notify 
*              : that the SICF channel 3 receive FIFO data full interrupt has 
*              : been generated. 
*              : The receive FIFO data full interrupt is disabled.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF3_SYNC_SetReceiveFull(void)
{
    /* Disable the receive interrupt request to be issued */
    RZA_IO_RegWrite_16(&SCIF3.SCSCR, 0, SCIFn_SCSCR_RIE_SHIFT, SCIFn_SCSCR_RIE);

    scif_sync_receive_full_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 4 receive FIFO data full notification information  
*              : when the SCIF channel 4 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF4_SYNC_SetReceiveFull(void)
{
    /* Add a processing when the SCIF4 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 5 receive FIFO data full notification information  
*              : when the SCIF channel 5 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF5_SYNC_SetReceiveFull(void)
{
    /* Add a processing when the SCIF5 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 6 receive FIFO data full notification information  
*              : when the SCIF channel 6 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF6_SYNC_SetReceiveFull(void)
{
    /* Add a processing when the SCIF6 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the SCIF 
*              : channel 7 receive FIFO data full notification information  
*              : when the SCIF channel 7 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF7_SYNC_SetReceiveFull(void)
{
    /* Add a processing when the SCIF7 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the SCIF 
*              : channel 0 transmit FIFO data empty notification information are 
*              : satisfied when the SCIF channel 0 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF0_SYNC_WaitTransmitEmpty(void)
{
    /* Add a processing when the SCIF0 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the SCIF 
*              : channel 1 transmit FIFO data empty notification information are 
*              : satisfied when the SCIF channel 1 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF1_SYNC_WaitTransmitEmpty(void)
{
    /* Add a processing when the SCIF1 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the SCIF 
*              : channel 2 transmit FIFO data empty notification information are 
*              : satisfied when the SCIF channel 2 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF2_SYNC_WaitTransmitEmpty(void)
{
    /* Add a processing when the SCIF2 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the SCIF
*              : channel 3 transmit FIFO data empty notification information are
*              : satisfied.
*              : In the sample code, enables the transmit FIFO data empty 
*              : interrupt and waits until 1 is set to the software flag to notify
*              : that the SCIF channel 3 transmit FIFO data empty interrupt has 
*              : been generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF3_SYNC_WaitTransmitEmpty(void)
{
    /* ==== Enable the transmit interrupt request to be issued ==== */
    RZA_IO_RegWrite_16(&SCIF3.SCSCR, 1, SCIFn_SCSCR_TIE_SHIFT, SCIFn_SCSCR_TIE);

    while (DEVDRV_FLAG_OFF == scif_sync_transmit_empty_flg)
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the SCIF 
*              : channel 4 transmit FIFO data empty notification information are 
*              : satisfied when the SCIF channel 4 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF4_SYNC_WaitTransmitEmpty(void)
{
    /* Add a processing when the SCIF4 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the SCIF 
*              : channel 5 transmit FIFO data empty notification information are 
*              : satisfied when the SCIF channel 5 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF5_SYNC_WaitTransmitEmpty(void)
{
    /* Add a processing when the SCIF5 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the SCIF 
*              : channel 6 transmit FIFO data empty notification information are 
*              : satisfied when the SCIF channel 6 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF6_SYNC_WaitTransmitEmpty(void)
{
    /* Add a processing when the SCIF6 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the SCIF 
*              : channel 7 transmit FIFO data empty notification information are 
*              : satisfied when the SCIF channel 7 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF7_SYNC_WaitTransmitEmpty(void)
{
    /* Add a processing when the SCIF7 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : SCIF channel 0 receive FIFO data full notification information
*              : are satisfied when the SCIF channel 0 is used.
*              : In the sample code, DEVDRV_SUCCESS is set to the return value.
*              : The receive processing for the API function R_SCIF_SYNC_Read 
*              : can be suspended by setting DEVDRV_ERROR to the return value 
*              : to terminate this function. 
*              : If the system cannot receive the data specified by the argument
*              : byte, set DEVDRV_ERROR to the return value by such implementing
*              : timeout processing.
* Arguments    : uint32_t byte      : Byte count of data to be received
*              : uint32_t less_rtrg : Setting for operation
*              :                    :   0 : Normal operation 
*              :                    :     : (Wait until the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information are satisfied.)
*              :                    :   1 : Assume that the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information has been satisfied when 
*              :                    :     : the byte count specified by the 
*              :                    :     : argument byte is received.
* Return Value : DEVDRV_SUCCESS : Success in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              : DEVDRV_ERROR   : Failure in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              :                : (Suspended before the data is received for
*              :                : the byte count specified by the argument byte)
******************************************************************************/
int32_t Userdef_SCIF0_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : SCIF channel 1 receive FIFO data full notification information
*              : are satisfied when the SCIF channel 1 is used.
*              : In the sample code, DEVDRV_SUCCESS is set to the return value.
*              : The receive processing for the API function R_SCIF_SYNC_Read 
*              : can be suspended by setting DEVDRV_ERROR to the return value 
*              : to terminate this function. 
*              : If the system cannot receive the data specified by the argument
*              : byte, set DEVDRV_ERROR to the return value by such implementing
*              : timeout processing.
* Arguments    : uint32_t byte      : Byte count of data to be received
*              : uint32_t less_rtrg : Setting for operation
*              :                    :   0 : Normal operation 
*              :                    :     : (Wait until the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information are satisfied.)
*              :                    :   1 : Assume that the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information has been satisfied when 
*              :                    :     : the byte count specified by the 
*              :                    :     : argument byte is received.
* Return Value : DEVDRV_SUCCESS : Success in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              : DEVDRV_ERROR   : Failure in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              :                : (Suspended before the data is received for
*              :                : the byte count specified by the argument byte)
******************************************************************************/
int32_t Userdef_SCIF1_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : SCIF channel 2 receive FIFO data full notification information
*              : are satisfied when the SCIF channel 2 is used.
*              : In the sample code, DEVDRV_SUCCESS is set to the return value.
*              : The receive processing for the API function R_SCIF_SYNC_Read 
*              : can be suspended by setting DEVDRV_ERROR to the return value 
*              : to terminate this function. 
*              : If the system cannot receive the data specified by the argument
*              : byte, set DEVDRV_ERROR to the return value by such implementing
*              : timeout processing.
* Arguments    : uint32_t byte      : Byte count of data to be received
*              : uint32_t less_rtrg : Setting for operation
*              :                    :   0 : Normal operation 
*              :                    :     : (Wait until the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information are satisfied.)
*              :                    :   1 : Assume that the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information has been satisfied when 
*              :                    :     : the byte count specified by the 
*              :                    :     : argument byte is received.
* Return Value : DEVDRV_SUCCESS : Success in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              : DEVDRV_ERROR   : Failure in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              :                : (Suspended before the data is received for
*              :                : the byte count specified by the argument byte)
******************************************************************************/
int32_t Userdef_SCIF2_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : SCIF channel 3 receive FIFO data full notification information
*              : are satisfied.
*              : In the sample code, enables the SCIF channel 3 receive FIFO
*              : data full interrupt and waits until 1 is set to the software
*              : flag to notify that the receive FIFO data full interrupt has 
*              : been generated. However, if 1 is specified for the argument 
*              : less_rtrg, the conditions for the receive FIFO data full 
*              : notification information are satisfied when the bytes 
*              : specified by the argument byte is stored in the receive FIFO
*              : buffer, and terminate this function.
*              : When receiving the byte count less than that specified by 
*              : the RTRG bit in the FIFO control register (SCFCR), effective
*              : receive processing can be performed by setting 1 to the 
*              : argument less_rtrg.
*              : The receive processing for the API function R_SCIF_SYNC_Read 
*              : can be suspended by setting DEVDRV_ERROR to the return value 
*              : to terminate this function. 
*              : If the system cannot receive the data specified by the argument
*              : byte, set DEVDRV_ERROR to the return value by such implementing
*              : timeout processing.
* Arguments    : uint32_t byte      : Byte count of data to be received
*              : uint32_t less_rtrg : Setting for operation
*              :                    :   0 : Normal operation 
*              :                    :     : (Wait until the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information are satisfied.)
*              :                    :   1 : Assume that the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information has been satisfied when 
*              :                    :     : the byte count specified by the 
*              :                    :     : argument byte is received.
* Return Value : DEVDRV_SUCCESS : Success in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              : DEVDRV_ERROR   : Failure in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              :                : (Suspended before the data is received for
*              :                : the byte count specified by the argument byte)
******************************************************************************/
int32_t Userdef_SCIF3_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg)
{
    uint16_t scfrdr_byte;
    int32_t  ret;

    ret = DEVDRV_SUCCESS;

    /* ==== Enable the receive interrupt request to be issued ==== */
    RZA_IO_RegWrite_16(&SCIF3.SCSCR, 1, SCIFn_SCSCR_RIE_SHIFT, SCIFn_SCSCR_RIE);

    /* ==== Processing to satisfy the conditions for receive FIFO full without waiting the trigger (RTRG) ==== */
    if (1 == less_rtrg)     /* When receiving the data smaller than the RTRG */
    {
        /* Wait until the data is received for the bytes specified by the argument byte */
        while (1)
        {
            /* Monitor data count in the receive buffer */
            scfrdr_byte = RZA_IO_RegRead_16(&SCIF3.SCFDR, SCIFn_SCFDR_R_SHIFT, SCIFn_SCFDR_R);
            if (scfrdr_byte >= byte)    /* When the data reaches to the specified data count */
            {
                /* Assume that the conditions for receive FIFO full are satisfied */
                Userdef_SCIF3_SYNC_SetReceiveFull();
                break;
            }
        }
    }
    /* ==== Wait until the conditions for receive FIFO full are satisfied ==== */
    else    /* When receiving the data larger than the RTRG */
    {
        /* Wait until the receive FIFO data full interrupt is generated */
        while (DEVDRV_FLAG_OFF == scif_sync_receive_full_flg)
        {
            /* Wait */
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : SCIF channel 4 receive FIFO data full notification information
*              : are satisfied when the SCIF channel 4 is used.
*              : In the sample code, DEVDRV_SUCCESS is set to the return value.
*              : The receive processing for the API function R_SCIF_SYNC_Read 
*              : can be susupended by setting DEVDRV_ERROR to the return value 
*              : to terminate this function. 
*              : If the system cannot receive the data specified by the argument
*              : byte, set DEVDRV_ERROR to the return value by such implementing
*              : timeout processing.
* Arguments    : uint32_t byte      : Byte count of data to be received
*              : uint32_t less_rtrg : Setting for operation
*              :                    :   0 : Normal operation 
*              :                    :     : (Wait until the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information are satisfied.)
*              :                    :   1 : Assume that the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information has been satisfied when 
*              :                    :     : the byte count specified by the 
*              :                    :     : argument byte is received.
* Return Value : DEVDRV_SUCCESS : Success in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              : DEVDRV_ERROR   : Failure in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              :                : (Suspended before the data is received for
*              :                : the byte count specified by the argument byte)
******************************************************************************/
int32_t Userdef_SCIF4_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : SCIF channel 5 receive FIFO data full notification information
*              : are satisfied when the SCIF channel 5 is used.
*              : In the sample code, DEVDRV_SUCCESS is set to the return value.
*              : The receive processing for the API function R_SCIF_SYNC_Read 
*              : can be susupended by setting DEVDRV_ERROR to the return value 
*              : to terminate this function. 
*              : If the system cannot receive the data specified by the argument
*              : byte, set DEVDRV_ERROR to the return value by such implementing
*              : timeout processing.
* Arguments    : uint32_t byte      : Byte count of data to be received
*              : uint32_t less_rtrg : Setting for operation
*              :                    :   0 : Normal operation 
*              :                    :     : (Wait until the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information are satisfied.)
*              :                    :   1 : Assume that the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information has been satisfied when 
*              :                    :     : the byte count specified by the 
*              :                    :     : argument byte is received.
* Return Value : DEVDRV_SUCCESS : Success in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              : DEVDRV_ERROR   : Failure in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              :                : (Suspended before the data is received for
*              :                : the byte count specified by the argument byte)
******************************************************************************/
int32_t Userdef_SCIF5_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : SCIF channel 6 receive FIFO data full notification information
*              : are satisfied when the SCIF channel 6 is used.
*              : In the sample code, DEVDRV_SUCCESS is set to the return value.
*              : The receive processing for the API function R_SCIF_SYNC_Read 
*              : can be susupended by setting DEVDRV_ERROR to the return value 
*              : to terminate this function. 
*              : If the system cannot receive the data specified by the argument
*              : byte, set DEVDRV_ERROR to the return value by such implementing
*              : timeout processing.
* Arguments    : uint32_t byte      : Byte count of data to be received
*              : uint32_t less_rtrg : Setting for operation
*              :                    :   0 : Normal operation 
*              :                    :     : (Wait until the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information are satisfied.)
*              :                    :   1 : Assume that the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information has been satisfied when 
*              :                    :     : the byte count specified by the 
*              :                    :     : argument byte is received.
* Return Value : DEVDRV_SUCCESS : Success in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              : DEVDRV_ERROR   : Failure in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              :                : (Suspended before the data is received for
*              :                : the byte count specified by the argument byte)
******************************************************************************/
int32_t Userdef_SCIF6_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : SCIF channel 7 receive FIFO data full notification information
*              : are satisfied when the SCIF channel 7 is used.
*              : In the sample code, DEVDRV_SUCCESS is set to the return value.
*              : The receive processing for the API function R_SCIF_SYNC_Read 
*              : can be susupended by setting DEVDRV_ERROR to the return value 
*              : to terminate this function. 
*              : If the system cannot receive the data specified by the argument
*              : byte, set DEVDRV_ERROR to the return value by such implementing
*              : timeout processing.
* Arguments    : uint32_t byte      : Byte count of data to be received
*              : uint32_t less_rtrg : Setting for operation
*              :                    :   0 : Normal operation 
*              :                    :     : (Wait until the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information are satisfied.)
*              :                    :   1 : Assume that the conditions for the 
*              :                    :     : receive FIFO data full notification
*              :                    :     : information has been satisfied when 
*              :                    :     : the byte count specified by the 
*              :                    :     : argument byte is received.
* Return Value : DEVDRV_SUCCESS : Success in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              : DEVDRV_ERROR   : Failure in waiting until the conditions for 
*              :                : the receive FIFO data full notification 
*              :                : information to be satisfied
*              :                : (Suspended before the data is received for
*              :                : the byte count specified by the argument byte)
******************************************************************************/
int32_t Userdef_SCIF7_SYNC_WaitReceiveFull(uint32_t byte, uint32_t less_rtrg)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_WaitTransmitEnd
* Description  : Execute this function to wait for the SCIF channel 0 transmit
*              : end when the SCIR channel 0 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF0_SYNC_WaitTransmitEnd(void)
{
    /* Add a processing when the SCIF0 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_WaitTransmitEnd
* Description  : Execute this function to wait for the SCIF channel 1 transmit
*              : end when the SCIR channel 1 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF1_SYNC_WaitTransmitEnd(void)
{
    /* Add a processing when the SCIF1 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_WaitTransmitEnd
* Description  : Execute this function to wait for the SCIF channel 2 transmit
*              : end when the SCIR channel 2 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF2_SYNC_WaitTransmitEnd(void)
{
    /* Add a processing when the SCIF2 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_WaitTransmitEnd
* Description  : Execute this function to wait for the SCIF channel 3 transmit
*              : end.
*              : In the sample code, waits until 1 is set to the TEND bit in 
*              : serial status register (SCFSR).
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF3_SYNC_WaitTransmitEnd(void)
{
    while (0 == RZA_IO_RegRead_16(&SCIF3.SCFSR, SCIFn_SCFSR_TEND_SHIFT, SCIFn_SCFSR_TEND))
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_WaitTransmitEnd
* Description  : Execute this function to wait for the SCIF channel 4 transmit
*              : end when the SCIR channel 4 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF4_SYNC_WaitTransmitEnd(void)
{
    /* Add a processing when the SCIF4 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_WaitTransmitEnd
* Description  : Execute this function to wait for the SCIF channel 5 transmit
*              : end when the SCIR channel 5 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF5_SYNC_WaitTransmitEnd(void)
{
    /* Add a processing when the SCIF5 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_WaitTransmitEnd
* Description  : Execute this function to wait for the SCIF channel 6 transmit
*              : end when the SCIR channel 6 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF6_SYNC_WaitTransmitEnd(void)
{
    /* Add a processing when the SCIF6 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_WaitTransmitEnd
* Description  : Execute this function to wait for the SCIF channel 7 transmit
*              : end when the SCIR channel 7 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_SCIF7_SYNC_WaitTransmitEnd(void)
{
    /* Add a processing when the SCIF7 is used */
}

/******************************************************************************
* Function Name: Userdef_SCIF0_SYNC_ErrReceiveOverrun
* Description  : Use this function to check the SCIF channel 0 overrun error
*              : occurrence and execute the processing for an error occurrence
*              : when the SCIF channel 0 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : DEVDRV_SUCCESS : No overrun error
*              : DEVDRV_ERROR   : Overrun error occurred
******************************************************************************/
int32_t Userdef_SCIF0_SYNC_ErrReceiveOverrun(void)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF1_SYNC_ErrReceiveOverrun
* Description  : Use this function to check the SCIF channel 1 overrun error
*              : occurrence and execute the processing for an error occurrence
*              : when the SCIF channel 1 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : DEVDRV_SUCCESS : No overrun error
*              : DEVDRV_ERROR   : Overrun error occurred
******************************************************************************/
int32_t Userdef_SCIF1_SYNC_ErrReceiveOverrun(void)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF2_SYNC_ErrReceiveOverrun
* Description  : Use this function to check the SCIF channel 2 overrun error
*              : occurrence and execute the processing for an error occurrence
*              : when the SCIF channel 2 is used.
*              : In the sample code, this function returns without any 
*              : processing.
* Arguments    : None
* Return Value : DEVDRV_SUCCESS : No overrun error
*              : DEVDRV_ERROR   : Overrun error occurred
******************************************************************************/
int32_t Userdef_SCIF2_SYNC_ErrReceiveOverrun(void)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF3_SYNC_ErrReceiveOverrun
* Description  : Use this function to check the SCIF channel 3 overrun error
*              : occurrence and execute the processing for an error occurrence.
*              : In the sample code, the ORER bit in the line status register 
*              : (SCLSR) is used to check the overrun error occurrence.
*              : If an error occurs, the ORER bit is cleared, and DEVDRV_ERROR 
*              : is returned.
* Arguments    : None
* Return Value : DEVDRV_SUCCESS : No overrun error
*              : DEVDRV_ERROR   : Overrun error occurred
******************************************************************************/
int32_t Userdef_SCIF3_SYNC_ErrReceiveOverrun(void)
{
    int32_t ret;

    ret = DEVDRV_SUCCESS;

    /* ==== Check overrun error ==== */
    /* ---- When the overrun error has been occurred ---- */
    if (1 == RZA_IO_RegRead_16(&SCIF3.SCLSR, SCIFn_SCLSR_ORER_SHIFT, SCIFn_SCLSR_ORER))
    {
        ret = DEVDRV_ERROR;

        /* Clear overrun error flag */
        RZA_IO_RegWrite_16(&SCIF3.SCLSR, 0, SCIFn_SCLSR_ORER_SHIFT, SCIFn_SCLSR_ORER);
    }

    return ret;
}

/******************************************************************************
* Function Name: Userdef_SCIF4_SYNC_ErrReceiveOverrun
* Description  : Use this function to check the SCIF channel 4 overrun error
*              : occurrence and execute the processing for an error occurrence
*              : when the SCIF channel 4 is used.
*              : In the sample code, this function is returned without any 
*              : processing.
* Arguments    : None
* Return Value : DEVDRV_SUCCESS : No overrun error
*              : DEVDRV_ERROR   : Overrun error occurred
******************************************************************************/
int32_t Userdef_SCIF4_SYNC_ErrReceiveOverrun(void)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF5_SYNC_ErrReceiveOverrun
* Description  : Use this function to check the SCIF channel 5 overrun error
*              : occurrence and execute the processing for an error occurrence
*              : when the SCIF channel 5 is used.
*              : In the sample code, this function is returned without any 
*              : processing.
* Arguments    : None
* Return Value : DEVDRV_SUCCESS : No overrun error
*              : DEVDRV_ERROR   : Overrun error occurred
******************************************************************************/
int32_t Userdef_SCIF5_SYNC_ErrReceiveOverrun(void)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF6_SYNC_ErrReceiveOverrun
* Description  : Use this function to check the SCIF channel 6 overrun error
*              : occurrence and execute the processing for an error occurrence
*              : when the SCIF channel 6 is used.
*              : In the sample code, this function is returned without any 
*              : processing.
* Arguments    : None
* Return Value : DEVDRV_SUCCESS : No overrun error
*              : DEVDRV_ERROR   : Overrun error occurred
******************************************************************************/
int32_t Userdef_SCIF6_SYNC_ErrReceiveOverrun(void)
{
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: Userdef_SCIF7_SYNC_ErrReceiveOverrun
* Description  : Use this function to check the SCIF channel 7 overrun error
*              : occurrence and execute the processing for an error occurrence
*              : when the SCIF channel 7 is used.
*              : In the sample code, this function is returned without any 
*              : processing.
* Arguments    : None
* Return Value : DEVDRV_SUCCESS : No overrun error
*              : DEVDRV_ERROR   : Overrun error occurred
******************************************************************************/
int32_t Userdef_SCIF7_SYNC_ErrReceiveOverrun(void)
{
    return DEVDRV_SUCCESS;
}


/* End of File */

