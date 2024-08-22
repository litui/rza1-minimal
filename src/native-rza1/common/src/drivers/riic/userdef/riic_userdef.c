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
* File Name   : riic_userdef.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : RIIC driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_riic.h"            /* RIIC Driver header */
#include "devdrv_intc.h"            /* INTC Driver Header */
#include "riic_rskrza1h.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Transfer speed setting (CKS bit, BRL bit, and BRH bit) ==== */
/* CPU internal clock (I(clock)) : 400MHz, Internal bus clock (B(clock)) : 133.33MHz  */
/* Peripheral clock1 (P1(clock)) : 66.67MHz, Peripheral clock0 (P0(clock)) : 33.33MHz */
/* RIIC digital noise filter : Used (4 stages)                                        */
/* Setting for the RIIC transfer at the speed of 100kbps (Duty cycle: 0.524)          */
/* when the RIIC internal reference clock (IIC(clock)) is P0(clock)/8 with            */
/* the conditionsabove.                                                               */
#define RIIC_CKS    RIIC_CKS_DIVISION_8 /* I2C clock source : P0(clock)/8 */
#define RIIC_BRL    (14)                /* SCL Low width                  */
#define RIIC_BRH    (16)                /* SCL High width                 */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static volatile uint8_t riic_receive_full_flg;      /* Receive data full flag   */
static volatile uint8_t riic_transmit_empty_flg;    /* Transmit data empty flag */
static volatile uint8_t riic_transmit_end_flg;      /* Transmit end flag        */


/******************************************************************************
* Function Name: Userdef_RIIC0_Init
* Description  : The RIIC initial setting and the setting for the interrupts 
*              : (receive data full interrupt, transmit data empty interrupt, and 
*              : transmit end interrupt) are required when the RIIC channel 0 is used. 
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_Init(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_Init
* Description  : The RIIC initial setting and the setting for the interrupts 
*              : (receive data full interrupt, transmit data empty interrupt, and 
*              : transmit end interrupt) are required when the RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_Init(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_Init
* Description  : The initial setting is required to use the RIIC channel 2.
*              : In the sample code, the STB initial setting is executed to supply
*              : clock to the RIIC channel 2. The RIIC initial settings such as
*              : RIIC internal reference clock, digital noise filter stages, SCL 
*              : clock High width and Low width are executed to enable the transfer
*              : in the speed of 100kbps.
*              : Also, setting for the interrupts (receive data full interrupt, 
*              : the transmit data empty interrupt, and the transmit end interrupt)
*              : and the INTC setting are executed.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_Init(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_Init
* Description  : The RIIC initial setting and the setting for the interrupts 
*              : (receive data full interrupt, transmit data empty interrupt, and 
*              : transmit end interrupt) are required when the RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_Init(void)
{
    volatile uint8_t dummy_buf;

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 3) ---- */
    RZA_IO_RegWrite_8(&CPG.STBCR9, 0, CPG_STBCR9_MSTP94_SHIFT, CPG_STBCR9_MSTP94);
    dummy_buf = CPG.STBCR9;     /* Dummy read */

    /* ==== RIIC initial setting ==== */
    /* SCL and SDA pins: Not driven */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]), 0, RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);
    /* RIIC reset */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]), 1, RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);
    /* Internal reset */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]), 1, RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);
    /* Clock source setting for the internal reference clock */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnMR1.UINT8[0]), RIIC_CKS, RIICn_RIICnMR1_CKS_SHIFT, RIICn_RIICnMR1_CKS);
    /* SCL clock Low width */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnBRL.UINT8[0]), RIIC_BRL, RIICn_RIICnBRL_BRL_SHIFT, RIICn_RIICnBRL_BRL);
    /* SCL clock High width */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnBRH.UINT8[0]), RIIC_BRH, RIICn_RIICnBRH_BRH_SHIFT, RIICn_RIICnBRH_BRH);
    /* Noise filter stages : 4 stages */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnMR3.UINT8[0]), 3, RIICn_RIICnMR3_NF_SHIFT, RIICn_RIICnMR3_NF);
    /* Transfer stop due to the NACK reception during transmission: Enabled */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnFER.UINT8[0]), 1, RIICn_RIICnFER_NACKE_SHIFT, RIICn_RIICnFER_NACKE);
    /* Digital noise-filter circuit: Used */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnFER.UINT8[0]), 1, RIICn_RIICnFER_NFE_SHIFT, RIICn_RIICnFER_NFE);
    /* Receive data full interrupt: Enabled */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]), 1, RIICn_RIICnIER_RIE_SHIFT, RIICn_RIICnIER_RIE);
    /* Transmit data empty interrupt: Enabled */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]), 1, RIICn_RIICnIER_TIE_SHIFT, RIICn_RIICnIER_TIE);
    /* Transmit end interrupt: Enabled */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnIER.UINT8[0]), 1, RIICn_RIICnIER_TEIE_SHIFT, RIICn_RIICnIER_TEIE);
    /* Clear internal reset */
    RZA_IO_RegWrite_8(&(RIIC3.RIICnCR1.UINT8[0]), 0, RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* ==== Interrupt setting ==== */
    R_INTC_RegistIntFunc(INTC_ID_INTIICRI3,  Sample_RIIC_Ri3_Interrupt);    /* Register INTIICRI3 interrupt function  */
    R_INTC_RegistIntFunc(INTC_ID_INTIICTI3,  Sample_RIIC_Ti3_Interrupt);    /* Register INTIICTI3 interrupt function  */
    R_INTC_RegistIntFunc(INTC_ID_INTIICTEI3, Sample_RIIC_Tei3_Interrupt);   /* Register INTIICTEI3 interrupt function */

    R_INTC_SetPriority(INTC_ID_INTIICRI3,  9);  /* INTIICRI3 interrupt priority = Set to 9  */
    R_INTC_SetPriority(INTC_ID_INTIICTI3,  9);  /* INTIICTI3 interrupt priority = Set to 9  */
    R_INTC_SetPriority(INTC_ID_INTIICTEI3, 9);  /* INTIICTEI3 interrupt priority = Set to 9 */

    R_INTC_Enable(INTC_ID_INTIICRI3);           /* INTIICRI3 interrupt enabled  */
    R_INTC_Enable(INTC_ID_INTIICTI3);           /* INTIICTI3 interrupt enabled  */
    R_INTC_Enable(INTC_ID_INTIICTEI3);          /* INTIICTEI3 interrupt enabled */
}

/******************************************************************************
* Function Name: Userdef_RIIC0_InitReceiveFull
* Description  : Execute this function to initialize the RIIC channel 0 receive
*              : data full notification information when the RIIC channel 0 is used. 
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_InitReceiveFull(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_InitReceiveFull
* Description  : Execute this function to initialize the RIIC channel 1 receive
*              : data full notification information when the RIIC channel 1 is used. 
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_InitReceiveFull(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_InitReceiveFull
* Description  : Execute this function to initialize the RIIC channel 2 receive
*              : data full notification information when the RIIC channel 2 is 
*              : used. In the sample code, 0 is set to the software flag to 
*              : notify that the RIIC channel 2 receive data full interrupt has
*              : been generated.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_InitReceiveFull(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_InitReceiveFull
* Description  : Execute this function to initialize the RIIC channel 3 receive
*              : data full notification information when the RIIC channel 3 isused. 
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_InitReceiveFull(void)
{
    riic_receive_full_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_InitTransmitEmpty
* Description  : Execute this function to initialize the RIIC channel 0 transmit
*              : data empty notification information when the RIIC channel 0 is used. 
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_InitTransmitEmpty(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_InitTransmitEmpty
* Description  : Execute this function to initialize the RIIC channel 1 transmit
*              : data empty notification information when the RIIC channel 1 isused. 
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_InitTransmitEmpty(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_InitTransmitEmpty
* Description  : Execute this function to initialize the RIIC channel 2 transmit
*              : data empty notification information when the RIIC channel 2 isused. 
*              : In the sample code, 0 is set to the software flag to notify that
*              : the RIIC channel 2 transmit data empty interrupt has been generated.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_InitTransmitEmpty(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_InitTransmitEmpty
* Description  : Execute this function to initialize the RIIC channel 3 transmit
*              : data empty notification information when the RIIC channel 3 is used. 
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_InitTransmitEmpty(void)
{
    riic_transmit_empty_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_InitTransmitEnd
* Description  : When the RIIC channel 0 is used, execute this function to 
*              : initialize the RIIC channel 0 transmit end notification 
*              : information if the argument mode specifies RIIC_TEND_WAIT_TRANSMIT, 
*              : or initialize the RIIC channel 0 receive data full notification 
*              : information if the argument mode specifies RIIC_TEND_WAIT_RECEIVE.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode : Continuation of transmission mode or 
*              :               : transition to reception mode
*              :               :   RIIC_TEND_WAIT_TRANSMIT : Continuation of transmission mode
*              :               :   RIIC_TEND_WAIT_RECEIVE  : Transition to reception mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_InitTransmitEnd(uint32_t mode)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_InitTransmitEnd
* Description  : When the RIIC channel 1 is used, execute this function to 
*              : initialize the RIIC channel 1 transmit end notification 
*              : information if the argument mode specifies RIIC_TEND_WAIT_TRANSMIT, 
*              : or initialize the RIIC channel 1 receive data full notification 
*              : information if the argument mode specifies RIIC_TEND_WAIT_RECEIVE.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode : Continuation of transmission mode or 
*              :               : transition to reception mode
*              :               :   RIIC_TEND_WAIT_TRANSMIT : Continuation of transmission mode
*              :               :   RIIC_TEND_WAIT_RECEIVE  : Transition to reception mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_InitTransmitEnd(uint32_t mode)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_InitTransmitEnd
* Description  : Execute this function to initialize the RIIC channel 2 
*              : transmit end notification information.
*              : In the sample code, 0 is set to the software flag to notify 
*              : that the RIIC channel 2 transmit end interrupt has been generated
*              : if the argument mode specifies RIIC_TEND_WAIT_TRANSMIT.
*              : When the said argument specifies RIIC_TEND_WAIT_RECEIVE, 0 is set 
*              : to the software flag to notify that the RIIC channel 2 receive 
*              : data full interrupt has been generated.
* Arguments    : uint32_t mode : Continuation of transmission mode or 
*              :               : transition to reception mode
*              :               :   RIIC_TEND_WAIT_TRANSMIT : Continuation of transmission mode
*              :               :   RIIC_TEND_WAIT_RECEIVE  : Transition to reception mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_InitTransmitEnd(uint32_t mode)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_InitTransmitEnd
* Description  : When the RIIC channel 1 is used, execute this function to 
*              : initialize the RIIC channel 3 transmit end notification 
*              : information if the argument mode specifies RIIC_TEND_WAIT_TRANSMIT, 
*              : or initialize the RIIC channel 3 receive data full notification 
*              : information if the said argument specifies RIIC_TEND_WAIT_RECEIVE.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode : Continuation of transmission mode or 
*              :               : transition to reception mode
*              :               :   RIIC_TEND_WAIT_TRANSMIT : Continuation of transmission mode
*              :               :   RIIC_TEND_WAIT_RECEIVE  : Transition to reception mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_InitTransmitEnd(uint32_t mode)
{
    /* ==== Continue transmission mode ==== */
    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
        riic_transmit_end_flg = DEVDRV_FLAG_OFF;
    }
    /* ==== Transit to reception mode ==== */
    else
    {
        riic_receive_full_flg = DEVDRV_FLAG_OFF;
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 0 receive data full notification information when the 
*              : RIIC channel 0 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_SetReceiveFull(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 1 receive data full notification information when the 
*              : RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_SetReceiveFull(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 2 receive data full notification information when the 
*              : RIIC channel 2 is used.
*              : In the sample code, 1 is set to the software flag to notify 
*              : that the RIIC channel 2 receive data full interrupt has been 
*              : generated.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_SetReceiveFull(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 3 receive data full notification information when the 
*              : RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_SetReceiveFull(void)
{
    riic_receive_full_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 0 transmit data empty notification information when the 
*              : RIIC channel 0 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_SetTransmitEmpty(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 1 transmit data full notification information when the 
*              : RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_SetTransmitEmpty(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 2 transmit data empty notification information when 
*              : the RIIC channel 2 is used.
*              : In the sample code, 1 is set to the software flag to notify 
*              : that the RIIC channel 2 transmit data empty interrupt has been 
*              : generated.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_SetTransmitEmpty(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 3 transmit data empty notification information when the 
*              : RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_SetTransmitEmpty(void)
{
    riic_transmit_empty_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_SetTransmitEnd
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 0 transmit end notification information when the 
*              : RIIC channel 0 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_SetTransmitEnd(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_SetTransmitEnd
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 1 transmit end notification information when the 
*              : RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_SetTransmitEnd(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_SetTransmitEnd
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 2 transmit end notification information when the 
*              : RIIC channel 2 is used.
*              : In the sample code, 1 is set to the software flag to notify 
*              : that the RIIC channel 2 transmit end interrupt has been generated.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_SetTransmitEnd(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_SetTransmitEnd
* Description  : Execute this function to satisfy the conditions for the RIIC
*              : channel 3 transmit end notification information when the 
*              : RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_SetTransmitEnd(void)
{
    riic_transmit_end_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 0 receive data full notification information are 
*              : satisfied when the RIIC channel 0 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitReceiveFull(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 1 receive data full notification information are 
*              : satisfied when the RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitReceiveFull(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 2 receive data full notification information are 
*              : satisfied when the RIIC channel 2 is used.
*              : In the sample code, waits until 1 is set to the software flag 
*              : to notify that the RIIC channel 2 receive data full interrupt 
*              : has been generated.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitReceiveFull(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 3 receive data full notification information are 
*              : satisfied when the RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitReceiveFull(void)
{
    while (DEVDRV_FLAG_OFF == riic_receive_full_flg)
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 0 transmit data empty notification information are 
*              : satisfied when the RIIC channel 0 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitTransmitEmpty(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 1 transmit data empty notification information are 
*              : satisfied when the RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitTransmitEmpty(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 2 transmit data empty notification information are 
*              : satisfied when the RIIC channel 2 is used.
*              : In the sample code, waits until 1 is set to the software flag 
*              : to notify that the RIIC channel 2 transmit data empty interrupt
*              : has been generated.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitTransmitEmpty(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 3 transmit data empty notification information are 
*              : satisfied when the RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitTransmitEmpty(void)
{
    while (DEVDRV_FLAG_OFF == riic_transmit_empty_flg)
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 0 transmit end notification information are 
*              : satisfied when the RIIC channel 0 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode : Selection for wait operation after transmission
*              :               :   RIIC_TEND_WAIT_TRANSMIT : Continuation of transmission mode
*              :               :   RIIC_TEND_WAIT_RECEIVE  : Transition to reception mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitTransmitEnd(uint32_t mode)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 1 transmit end notification information are 
*              : satisfied when the RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode : Selection for wait operation after transmission
*              :               :   RIIC_TEND_WAIT_TRANSMIT : Continuation of transmission mode
*              :               :   RIIC_TEND_WAIT_RECEIVE  : Transition to reception mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitTransmitEnd(uint32_t mode)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 2 transmit end notification information are 
*              : satisfied when the RIIC channel 2 is used.
*              : In the sample code, waits until 1 is set to the software flag
*              : to notify that the RIIC channel 2 transmit end interrupt has 
*              : been generated if the argument mode specifies RIIC_TEND_WAIT_TRANSMIT.
*              : When the said argument specifies RIIC_TEND_WAIT_RECEIVE, waits
*              : until 1 is set to the software flag to notify that the RIIC 
*              : channel 2 receive data full interrupt has been generated.
* Arguments    : uint32_t mode : Selection for wait operation after transmission
*              :               :   RIIC_TEND_WAIT_TRANSMIT : Continuation of transmission mode
*              :               :   RIIC_TEND_WAIT_RECEIVE  : Transition to reception mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitTransmitEnd(uint32_t mode)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the 
*              : RIIC channel 3 transmit end notification information are 
*              : satisfied when the RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode : Selection for wait operation after transmission
*              :               :   RIIC_TEND_WAIT_TRANSMIT : Continuation of transmission mode
*              :               :   RIIC_TEND_WAIT_RECEIVE  : Transition to reception mode
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitTransmitEnd(uint32_t mode)
{
    /* NOTE:                                                                       */
    /* When transmitting the slave address with the R/W# bit "0" (Write operation) */
    /* and the data transmission has been completed, the transmit end flag (TEND)  */
    /* is set to 1. Therefore, the transmit end waiting processing is executed     */
    /* if the argument mode specifies RIIC_TEND_WAIT_TRANSMIT.                     */
    /* When transmitting the slave address with the R/W# bit "1" (Read operation), */
    /* the master transfer mode is changed to the master receive mode and the      */
    /* receive data full flag (RDRF) is set to 1. Therefore, the receive data      */
    /* waiting processing is executed if the argument mode specifies               */
    /* RIIC_TEND_WAIT_RECEIVE.                                                     */

    /* ==== Continuation of transmission mode ==== */
    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
        while (DEVDRV_FLAG_OFF == riic_transmit_end_flg)
        {
            /* Wait */
        }
    }
    /* ==== Transit to reception mode ==== */
    else
    {
        while (DEVDRV_FLAG_OFF == riic_receive_full_flg)
        {
            /* Wait */
        }
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitBusMastership
* Description  : Execute this function to wait the RIIC channel 0 bus free and 
*              : bus busy when the RIIC channel 0 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode
*              :            : Mode selection for bus free or bus busy
*              :            :   RIIC_BUS_MASTERSHIP_WAIT_FREE : Wait for bus free
*              :            :   RIIC_BUS_MASTERSHIP_WAIT_BUSY : Wait for bus busy
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitBusMastership(uint32_t mode)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitBusMastership
* Description  : Execute this function to wait the RIIC channel 1 bus free and 
*              : bus busy when the RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode
*              :            : Mode selection for bus free or bus busy
*              :            :   RIIC_BUS_MASTERSHIP_WAIT_FREE : Wait for bus free
*              :            :   RIIC_BUS_MASTERSHIP_WAIT_BUSY : Wait for bus busy
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitBusMastership(uint32_t mode)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitBusMastership
* Description  : Execute this function to wait the RIIC channel 2 bus free and 
*              : bus busy when the RIIC channel 2 is used.
*              : In the sample code, waits until 0 is set to the BBSY bit of the 
*              : RIIC2CR2 register if the argument mode specifies 
*              : RIIC_BUS_MASTERSHIP_WAIT_FREE. 
*              : When the said argument specifies RIIC_BUS_MASTERSHIP_WAIT_BUSY, 
*              : waits until 1 is set to bit. 
* Arguments    : uint32_t mode
*              :            : Mode selection for bus free or bus busy
*              :            :   RIIC_BUS_MASTERSHIP_WAIT_FREE : Wait for bus free
*              :            :   RIIC_BUS_MASTERSHIP_WAIT_BUSY : Wait for bus busy
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitBusMastership(uint32_t mode)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitBusMastership
* Description  : Execute this function to wait the RIIC channel 3 bus free and 
*              : bus busy when the RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : uint32_t mode
*              :            : Mode selection for bus free or bus busy
*              :            :   RIIC_BUS_MASTERSHIP_WAIT_FREE : Wait for bus free
*              :            :   RIIC_BUS_MASTERSHIP_WAIT_BUSY : Wait for bus busy
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitBusMastership(uint32_t mode)
{
    /* ==== Bus free wait mode ==== */
    if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
        while (1 == RZA_IO_RegRead_8(&(RIIC3.RIICnCR2.UINT8[0]), RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY))
        {
            /* Wait */
        }
    }
    /* ==== Bus busy wait mode ==== */
    else
    {
        while (0 == RZA_IO_RegRead_8(&(RIIC3.RIICnCR2.UINT8[0]), RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY))
        {
            /* Wait */
        }
    }
}

/******************************************************************************
* Function Name: Userdef_RIIC0_WaitStop
* Description  : Execute this function to wait until the RIIC channel 0 stop 
*              : condition is detected when the RIIC channel 0 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC0_WaitStop(void)
{
    /* Add a processing when the RIIC0 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC1_WaitStop
* Description  : Execute this function to wait until the RIIC channel 1 stop 
*              : condition is detected when the RIIC channel 1 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC1_WaitStop(void)
{
    /* Add a processing when the RIIC1 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC2_WaitStop
* Description  : Execute this function to wait until the RIIC channel 2 stop 
*              : condition is detected when the RIIC channel 2 is used.
*              : In the sample code, waits until the STOP bit of the RIIC2SR2 
*              : register is set to 1.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC2_WaitStop(void)
{
    /* Add a processing when the RIIC2 is used */
}

/******************************************************************************
* Function Name: Userdef_RIIC3_WaitStop
* Description  : Execute this function to wait until the RIIC channel 3 stop 
*              : condition is detected  when the RIIC channel 3 is used.
*              : In the sample code, this function returns without any processing.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_RIIC3_WaitStop(void)
{
    /* === Wait for stop condition detection ==== */
    while (0 == RZA_IO_RegRead_8(&(RIIC3.RIICnSR2.UINT8[0]), RIICn_RIICnSR2_STOP_SHIFT, RIICn_RIICnSR2_STOP))
    {
        /* Wait */
    }
}


/* End of File */

