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
* File Name   : rspi_userdef.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Renesas Serial Peripheral Interface(RSPI) device driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdlib.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_rspi.h"            /* RSPI Driver header */
#include "devdrv_intc.h"            /* INTC Driver Header */
#include "sample_rspi.h"
#include "iodefine.h"
#include "rza_io_regrw.h"
#include "unused.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Bit rate setting (SPR register, BRDV bit) ==== */
/* CPU internal clock (I clock) : 400MHz, Internal bus clock (B clock) : 133.33MHz  */
/* Peripheral clock1 (P1 clock) : 66.67MHz, Peripheral clock0 (P0 clock) : 33.33MHz */
/* Setting to operate the bit rate at the speed of 2.78Mbps (=(P1 clock)/24)        */
/* with the condition above                                                         */
#define RSPI_SPR            (5u)         /* Setting value of the SPBR register */
#define RSPI_BRDV           (1u)         /* Setting value of the BRDV bit of   */
                                         /* the SPCMD0 register                */

/* ==== Dummy data ==== */
#define RSPI_DUMMY_DATA_32  (0xFFFFFFFFuL)
#define RSPI_DUMMY_DATA_16  (0xFFFFu)
#define RSPI_DUMMY_DATA_8   (0xFFu)

/* ==== Access to the SPDR register ==== */
#define RSPI_SPDR_ACCESS_8      (1u)     /* 8-bit access  */
#define RSPI_SPDR_ACCESS_16     (2u)     /* 16-bit access */
#define RSPI_SPDR_ACCESS_32     (3u)     /* 32-bit access */

#define RSPI_TOP_ADDRESS    (0)
#define RSPI_PAGE_SIZE      (128u)
#define RSPI_DATA_SIZE      (0x4000u)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static volatile uint8_t rspi0_receive_full_flg;
static volatile uint8_t rspi0_transmit_empty_flg;
static volatile uint8_t rspi1_receive_full_flg;
static volatile uint8_t rspi1_transmit_empty_flg;
static volatile uint8_t rspi2_receive_full_flg;
static volatile uint8_t rspi2_transmit_empty_flg;
static volatile uint8_t rspi3_receive_full_flg;
static volatile uint8_t rspi3_transmit_empty_flg;
static volatile uint8_t rspi4_receive_full_flg;
static volatile uint8_t rspi4_transmit_empty_flg;

uint8_t *rspi0_trans_data;
uint8_t *rspi0_receive_data;
uint8_t *rspi1_trans_data;
uint8_t *rspi1_receive_data;
uint8_t *rspi2_trans_data;
uint8_t *rspi2_receive_data;
uint8_t *rspi3_trans_data;
uint8_t *rspi3_receive_data;
uint8_t *rspi4_trans_data;
uint8_t *rspi4_receive_data;

static void GenericSetTransmitEmpty(devdrv_ch_t channel);
static void GenericWaitTransmitEmpty(devdrv_ch_t channel);
static void GenericInitReceiveFull(devdrv_ch_t channel);
void Userdef_SPRI0_Interrupt(uint32_t int_sense);
void Userdef_SPRI1_Interrupt(uint32_t int_sense);
void Userdef_SPRI2_Interrupt(uint32_t int_sense);
void Userdef_SPRI3_Interrupt(uint32_t int_sense);
void Userdef_SPRI4_Interrupt(uint32_t int_sense);
void Userdef_SPTI0_Interrupt(uint32_t int_sense);
void Userdef_SPTI1_Interrupt(uint32_t int_sense);
void Userdef_SPTI2_Interrupt(uint32_t int_sense);
void Userdef_SPTI3_Interrupt(uint32_t int_sense);
void Userdef_SPTI4_Interrupt(uint32_t int_sense);
static volatile struct st_rspi * RSPI_GetRegAddr(uint32_t channel);

static void InitMemory8Bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment)
{
    uint32_t offset;

    for (offset = 0; offset < count; offset++)
    {
        if (0 == increment)
        {
            *(addr + offset) = data;
        }
        else
        {
            *(addr + offset) = data++;
        }
    }
}

static void SetClockStandby(devdrv_ch_t channel, bool off_on)
{
    const uint8_t mstp_shift = 7U - (uint8_t)channel;
    const uint8_t mstp_mask = 1U << mstp_shift;

    volatile uint8_t dummy_buf;

    /* 
     * Write value (0 or 1) to the bit for the correct SPI module in the Standby Control Register 
     * This logic should work for both RZ/A1LU and RZ/A1H, though the latter has more channels. 
     * (3 SPI channels on RZ/A1LU, 5 on RZ/A1H)
     */
    RZA_IO_RegWrite_8(&CPG.STBCR10, off_on ? 1 : 0, mstp_shift, mstp_mask);
    dummy_buf = CPG.STBCR10;

    R_UNUSED(dummy_buf);
}

static uint8_t GetReceiveFull(devdrv_ch_t channel)
{
    switch (channel)
    {
    case DEVDRV_CH_0:
        return rspi0_receive_full_flg;
    case DEVDRV_CH_1:
        return rspi1_receive_full_flg;
    case DEVDRV_CH_2:
        return rspi2_receive_full_flg;
    case DEVDRV_CH_3:
        return rspi3_receive_full_flg;
    case DEVDRV_CH_4:
        return rspi4_receive_full_flg;
    default:
        return DEVDRV_FLAG_OFF;
    }
}

static uint8_t GetTransmitEmpty(devdrv_ch_t channel)
{
    switch (channel)
    {
    case DEVDRV_CH_0:
        return rspi0_transmit_empty_flg;
    case DEVDRV_CH_1:
        return rspi1_transmit_empty_flg;
    case DEVDRV_CH_2:
        return rspi2_transmit_empty_flg;
    case DEVDRV_CH_3:
        return rspi3_transmit_empty_flg;
    case DEVDRV_CH_4:
        return rspi4_transmit_empty_flg;
    default:
        return DEVDRV_FLAG_OFF;
    }
}

/* Generic init */
static void GenericSPIInit(devdrv_ch_t channel)
{
    SetClockStandby(channel, false);

    volatile struct st_rspi *spi_block = RSPI_GetRegAddr(channel);

    /* ==== RSPI initial setting ==== */
    /* SPCR - Control register
    b6 SPE - Function enable (0: disabled) */
    spi_block->SPCR = 0x00u;

    /* SPPCR - Pin control register
    b5 MOIFE - MOSI idle value fixing enable : Value set in the MOIFV bit
    b4 MOIFV - MOSI idle fixed value         : Fixed to 1 when idle
    b0 SPLP  - Loop back                     : Do not loop back */
    spi_block->SPPCR = 0x30u;

    /* SPBR - Bit rate register
    b7:b0 SPR - Bit rate : RSPI_SPR */
    spi_block->SPBR   = RSPI_SPR;

    /* SPDCR - Data control register
    b7    TXDMY - Dummy data transmission enabled : Dummy data transmission disabled
    b6:b5 SPLW  - Access width                    : Byte access to the SPDR register */
    spi_block->SPDCR  = 0x20u;

    /* SPCKD - Clock delay register
    b2:b0 SCKDL - RSPCK delay : 1RSPCK */
    spi_block->SPCKD  = 0x00u;

    /* SSLND - Slave select negation delay register
    b2:b0 SLNDL - SSL negation delay : 1RSPCK */
    spi_block->SSLND  = 0x00u;

    /* SPND - Next-access delay register
    b2:b0 SPNDL - Next access delay 1RSPCK + 2P1 clock */
    spi_block->SPND   = 0x00u;

    /* SPSCR - Sequence control register
    b1:b0 SPSLN - Sequence length : Referenced SPCMD register 0=>0=>... */
    spi_block->SPSCR  = 0x00u;

    /* SPCMD0 - Command register 0
    b15     SCKDEN - RSPCK delay setting enable        : Use SPCKD setting value
    b14     SLNDEN - SSL negation delay setting enable : Use SSLND setting value
    b13     SPNDEN - Next-access delay enable          : Use SPND setting value
    b12     LSBF   - LSB first                         : Change to MSB first
    b11:b8  SPB    - Data length                       : 8 bits
    b7      SSLKP  - SSL signal level keeping
                        : Keep the SSL signal level from the end of transfer
                        : until the beginning of the next access
    b3:b2   BRDV   - Bit rate division setting         : RSPI_BRDV
    b1      CPOL   - RSPCK polarity setting            : RSPCK=1 when idle
    b0      CPHA   - RSPCK phase setting
                        : Data variation on odd edge, data sampling on even edge */
    spi_block->SPCMD0 = 0xE783u | (RSPI_BRDV << 2);

    /* SPBFCR - Buffer control register
    b7    TXRST - Transmit buffer data reset             : Enable reset operation
    b6    RXRST - Receive buffer data reset              : Enable reset operation
    b5:b4 TXTRG - Transmit buffer data triggering number : 0 byte or less
    b2:b0 RXTRG - Receive buffer data triggering number  : 24 bytes or more */
    spi_block->SPBFCR = 0xF5u;

    /* SPBFCR - Buffer control register
    b7    TXRST - Transmit buffer data reset : Disable reset operation
    b6    RXRST - Receive buffer data reset  : Disable reset operation */
    spi_block->SPBFCR &= ~0xC0u;

    /* SSLP - Slave select polarity register
    b0 SSL0P - SSL signal polarity : 0 active */
    spi_block->SSLP   = 0x00u;

    /* SPCR - Control register
    b7 SPRIE  - Receive interrupt enable           : Disabled
    b6 SPE    - Function enable                    : Disabled
    b5 SPTIE  - Transmit interrupt enable          : Disabled
    b4 SPEIE  - Error interrupt enable             : Disabled
    b3 MSTR   - Master/slave mode select           : Master mode
    b2 MODFEN - Mode fault error detection enable  : Disabled */
    spi_block->SPCR   = 0x08u;

    switch (channel)
    {
    case DEVDRV_CH_0:
        R_INTC_RegistIntFunc(INTC_ID_SPRI0, Userdef_SPRI0_Interrupt);
        R_INTC_RegistIntFunc(INTC_ID_SPTI0, Userdef_SPTI0_Interrupt);
        R_INTC_SetPriority(INTC_ID_SPRI0, 10);  /* SPRIn interrupt priority= 10 is specified */
        R_INTC_SetPriority(INTC_ID_SPTI0, 10);  /* SPTIn interrupt priority= 10 is specified */
        R_INTC_Enable(INTC_ID_SPRI0);   /* SPRIn interrupt enabled */
        R_INTC_Enable(INTC_ID_SPTI0);   /* SPTIn interrupt enabled */
        rspi0_trans_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        rspi0_receive_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        InitMemory8Bit(rspi0_trans_data, 0x00, RSPI_DATA_SIZE, 1);      /* Generate transmit data    */
        InitMemory8Bit(rspi0_receive_data, 0x00, RSPI_DATA_SIZE, 0);    /* Clear receive data buffer */
        break;
    case DEVDRV_CH_1:
        R_INTC_RegistIntFunc(INTC_ID_SPRI1, Userdef_SPRI1_Interrupt);
        R_INTC_RegistIntFunc(INTC_ID_SPTI1, Userdef_SPTI1_Interrupt);
        R_INTC_SetPriority(INTC_ID_SPRI1, 10);  /* SPRIn interrupt priority= 10 is specified */
        R_INTC_SetPriority(INTC_ID_SPTI1, 10);  /* SPTIn interrupt priority= 10 is specified */
        R_INTC_Enable(INTC_ID_SPRI1);   /* SPRIn interrupt enabled */
        R_INTC_Enable(INTC_ID_SPTI1);   /* SPTIn interrupt enabled */
        rspi1_trans_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        rspi1_receive_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        InitMemory8Bit(rspi1_trans_data, 0x00, RSPI_DATA_SIZE, 1);      /* Generate transmit data    */
        InitMemory8Bit(rspi1_receive_data, 0x00, RSPI_DATA_SIZE, 0);    /* Clear receive data buffer */
        break;
    case DEVDRV_CH_2:
        R_INTC_RegistIntFunc(INTC_ID_SPRI2, Userdef_SPRI2_Interrupt);
        R_INTC_RegistIntFunc(INTC_ID_SPTI2, Userdef_SPTI2_Interrupt);
        R_INTC_SetPriority(INTC_ID_SPRI2, 10);  /* SPRIn interrupt priority= 10 is specified */
        R_INTC_SetPriority(INTC_ID_SPTI2, 10);  /* SPTIn interrupt priority= 10 is specified */
        R_INTC_Enable(INTC_ID_SPRI2);   /* SPRIn interrupt enabled */
        R_INTC_Enable(INTC_ID_SPTI2);   /* SPTIn interrupt enabled */
        rspi2_trans_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        rspi2_receive_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        InitMemory8Bit(rspi2_trans_data, 0x00, RSPI_DATA_SIZE, 1);      /* Generate transmit data    */
        InitMemory8Bit(rspi2_receive_data, 0x00, RSPI_DATA_SIZE, 0);    /* Clear receive data buffer */
        break;
    case DEVDRV_CH_3:
        R_INTC_RegistIntFunc(INTC_ID_SPRI3, Userdef_SPRI3_Interrupt);
        R_INTC_RegistIntFunc(INTC_ID_SPTI3, Userdef_SPTI3_Interrupt);
        R_INTC_SetPriority(INTC_ID_SPRI3, 10);  /* SPRIn interrupt priority= 10 is specified */
        R_INTC_SetPriority(INTC_ID_SPTI3, 10);  /* SPTIn interrupt priority= 10 is specified */
        R_INTC_Enable(INTC_ID_SPRI3);   /* SPRIn interrupt enabled */
        R_INTC_Enable(INTC_ID_SPTI3);   /* SPTIn interrupt enabled */
        rspi3_trans_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        rspi3_receive_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        InitMemory8Bit(rspi3_trans_data, 0x00, RSPI_DATA_SIZE, 1);      /* Generate transmit data    */
        InitMemory8Bit(rspi3_receive_data, 0x00, RSPI_DATA_SIZE, 0);    /* Clear receive data buffer */
        break;
    case DEVDRV_CH_4:
        R_INTC_RegistIntFunc(INTC_ID_SPRI4, Userdef_SPRI4_Interrupt);
        R_INTC_RegistIntFunc(INTC_ID_SPTI4, Userdef_SPTI4_Interrupt);
        R_INTC_SetPriority(INTC_ID_SPRI4, 10);  /* SPRIn interrupt priority= 10 is specified */
        R_INTC_SetPriority(INTC_ID_SPTI4, 10);  /* SPTIn interrupt priority= 10 is specified */
        R_INTC_Enable(INTC_ID_SPRI4);   /* SPRIn interrupt enabled */
        R_INTC_Enable(INTC_ID_SPTI4);   /* SPTIn interrupt enabled */
        rspi4_trans_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        rspi4_receive_data = (uint8_t*) malloc(RSPI_DATA_SIZE);
        InitMemory8Bit(rspi4_trans_data, 0x00, RSPI_DATA_SIZE, 1);      /* Generate transmit data    */
        InitMemory8Bit(rspi4_receive_data, 0x00, RSPI_DATA_SIZE, 0);    /* Clear receive data buffer */
        break;
    default:
        break;
    }
}
/******************************************************************************
* Function Name: Userdef_RSPI0_Init
* Description  : The RSPI initial setting and setting for interrupts are required
*              : when the RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_Init(void)
{
    GenericSPIInit(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Userdef_RSPI1_Init
* Description  : The RSPI initial setting and setting for interrupts are required
*              : when the RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_Init(void)
{
    GenericSPIInit(DEVDRV_CH_1);
}

/******************************************************************************
* Function Name: Userdef_RSPI2_Init
* Description  : The RSPI initial setting and setting for interrupts are required
*              : when the RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_Init(void)
{
    GenericSPIInit(DEVDRV_CH_2);
}

/******************************************************************************
* Function Name: Userdef_RSPI3_Init
* Description  : The RSPI initial setting and setting for interrupts are required
*              : when the RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_Init(void)
{
    GenericSPIInit(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Userdef_RSPI4_Init
* Description  : Initial setting is required to use the RSPI channel 4.
*              : In the sample code, the RSPI transfer is executed in the master
*              : mode by setting the bit rate of 2.78Mbps (=(P1 clock)/24), and 
*              : the MSB first, and the 8-bit length. 
*              : 1 is set to the RSPCK signal as polarity when idle. The driving 
*              : of valid data commenses at the falling edge, and the data is 
*              : sampled at the rising edge. 0 is set to the SSL signal as active.
*              : The RSPI setting is executed to set the transmit buffer empty 
*              : flag when the data in the transmit buffer has 0 byte or smaller, 
*              : or the receive buffer full flag is set when the data in the 
*              : receive buffer has 24 bytes or more.
*              : The INTC setting is also executed to use the transmit buffer
*              : empty interrupt and the receive buffer full interrupt. 
*              : In this function, the RSPI is set to disable the transmission 
*              : interrupt and the reception interrupt.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_Init(void)
{
    GenericSPIInit(DEVDRV_CH_4);
}

static void GenericInitReceiveFull(devdrv_ch_t channel)
{
    switch (channel)
    {
    case DEVDRV_CH_0:
        rspi0_receive_full_flg = DEVDRV_FLAG_OFF;
        break;
    case DEVDRV_CH_1:
        rspi1_receive_full_flg = DEVDRV_FLAG_OFF;
        break;
    case DEVDRV_CH_2:
        rspi2_receive_full_flg = DEVDRV_FLAG_OFF;
        break;
    case DEVDRV_CH_3:
        rspi3_receive_full_flg = DEVDRV_FLAG_OFF;
        break;
    case DEVDRV_CH_4:
        rspi4_receive_full_flg = DEVDRV_FLAG_OFF;
        break;
    default:
        break;
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 0 receive
*              : buffer full notification information when the RSPI channel 0 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_InitReceiveFull(void)
{
    GenericInitReceiveFull(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Userdef_RSPI1_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 1 receive
*              : buffer full notification information when the RSPI channel 1 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_InitReceiveFull(void)
{
    GenericInitReceiveFull(DEVDRV_CH_1);
}

/******************************************************************************
* Function Name: Userdef_RSPI2_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 2 receive
*              : buffer full notification information when the RSPI channel 2 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_InitReceiveFull(void)
{
    GenericInitReceiveFull(DEVDRV_CH_2);
}

/******************************************************************************
* Function Name: Userdef_RSPI3_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 3 receive
*              : buffer full notification information when the RSPI channel 3 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_InitReceiveFull(void)
{
    GenericInitReceiveFull(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Userdef_RSPI4_InitReceiveFull
* Description  : Execute this function to initialize the RSPI channel 4 receive
*              : buffer full notification information when the RSPI channel 4 is 
*              : used. 
*              : In the sample code, 0 is set to the software flag to notify 
*              : that the RSPI channel 4 receive buffer full interrupt has been 
*              : generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_InitReceiveFull(void)
{
    GenericInitReceiveFull(DEVDRV_CH_4);
}

static void GenericInitTransmitEmpty(devdrv_ch_t channel)
{
    switch (channel)
    {
    case DEVDRV_CH_0:
        rspi0_transmit_empty_flg = DEVDRV_FLAG_OFF;
        break;
    case DEVDRV_CH_1:
        rspi1_transmit_empty_flg = DEVDRV_FLAG_OFF;
        break;
    case DEVDRV_CH_2:
        rspi2_transmit_empty_flg = DEVDRV_FLAG_OFF;
        break;
    case DEVDRV_CH_3:
        rspi3_transmit_empty_flg = DEVDRV_FLAG_OFF;
        break;
    case DEVDRV_CH_4:
        rspi4_transmit_empty_flg = DEVDRV_FLAG_OFF;
        break;
    default:
        break;
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 0 transmit
*              : buffer empty notification information when the RSPI channel 0 is
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_InitTransmitEmpty(void)
{
    GenericInitTransmitEmpty(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Userdef_RSPI1_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 1 transmit
*              : buffer empty notification information when the RSPI channel 1 is
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_InitTransmitEmpty(void)
{
    GenericInitTransmitEmpty(DEVDRV_CH_1);
}

/******************************************************************************
* Function Name: Userdef_RSPI2_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 2 transmit
*              : buffer empty notification information when the RSPI channel 2 is
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_InitTransmitEmpty(void)
{
    GenericInitTransmitEmpty(DEVDRV_CH_2);
}

/******************************************************************************
* Function Name: Userdef_RSPI3_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 3 transmit
*              : buffer empty notification information when the RSPI channel 3 is
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_InitTransmitEmpty(void)
{
    GenericInitTransmitEmpty(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Userdef_RSPI4_InitTransmitEmpty
* Description  : Execute this function to initialize the RSPI channel 4 transmit
*              : buffer empty notification information.
*              : In the sample code, 0 is set to the software flag to notify
*              : that the RSPI channel 4 transmit buffer empty interrupt has 
*              : bee generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_InitTransmitEmpty(void)
{
    GenericInitTransmitEmpty(DEVDRV_CH_4);
}

static void GenericSetReceiveFull(devdrv_ch_t channel)
{
    volatile struct st_rspi *spi_block = RSPI_GetRegAddr(channel);

    /* Disable the receive interrupt request to be issued */
    RZA_IO_RegWrite_8(&(spi_block->SPCR), 0, RSPIn_SPCR_SPRIE_SHIFT, RSPIn_SPCR_SPRIE);

    switch (channel)
    {
    case DEVDRV_CH_0:
        rspi0_receive_full_flg = DEVDRV_FLAG_ON;
        break;
    case DEVDRV_CH_1:
        rspi1_receive_full_flg = DEVDRV_FLAG_ON;
        break;
    case DEVDRV_CH_2:
        rspi2_receive_full_flg = DEVDRV_FLAG_ON;
        break;
    case DEVDRV_CH_3:
        rspi3_receive_full_flg = DEVDRV_FLAG_ON;
        break;
    case DEVDRV_CH_4:
        rspi4_receive_full_flg = DEVDRV_FLAG_ON;
        break;
    default:
        break;
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 0 receive buffer full notification information when 
*              : the RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_SetReceiveFull(void)
{
    GenericSetReceiveFull(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Userdef_RSPI1_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 1 receive buffer full notification information when 
*              : the RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_SetReceiveFull(void)
{
    GenericSetReceiveFull(DEVDRV_CH_1);
}

/******************************************************************************
* Function Name: Userdef_RSPI2_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 2 receive buffer full notification information when 
*              : the RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_SetReceiveFull(void)
{
    GenericSetReceiveFull(DEVDRV_CH_2);
}

/******************************************************************************
* Function Name: Userdef_RSPI3_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 3 receive buffer full notification information when 
*              : the RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_SetReceiveFull(void)
{
    GenericSetReceiveFull(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Userdef_RSPI4_SetReceiveFull
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 4 receive buffer full notification information when 
*              : the RSPI channel 4 is used.
*              : In the sample code, 1 is set to the software flag to notify
*              : that the RSPI channel 4 receive buffer full interrupt has been 
*              : generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_SetReceiveFull(void)
{
    GenericSetReceiveFull(DEVDRV_CH_4);
}

static void GenericSetTransmitEmpty(devdrv_ch_t channel)
{
    volatile struct st_rspi *spi_block = RSPI_GetRegAddr(channel);
    RZA_IO_RegWrite_8(&(spi_block->SPCR), 0, RSPIn_SPCR_SPTIE_SHIFT, RSPIn_SPCR_SPTIE);

    switch (channel)
    {
    case DEVDRV_CH_0:
        rspi0_transmit_empty_flg = DEVDRV_FLAG_ON;
        break;
    case DEVDRV_CH_1:
        rspi1_transmit_empty_flg = DEVDRV_FLAG_ON;
        break;
    case DEVDRV_CH_2:
        rspi2_transmit_empty_flg = DEVDRV_FLAG_ON;
        break;
    case DEVDRV_CH_3:
        rspi3_transmit_empty_flg = DEVDRV_FLAG_ON;
        break;
    case DEVDRV_CH_4:
        rspi4_transmit_empty_flg = DEVDRV_FLAG_ON;
        break;
    default:
        break;
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 0 transmit buffer empty notification information when 
*              : the RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_SetTransmitEmpty(void)
{
    GenericSetTransmitEmpty(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Userdef_RSPI1_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 1 transmit buffer empty notification information when 
*              : the RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_SetTransmitEmpty(void)
{
    GenericSetTransmitEmpty(DEVDRV_CH_1);
}

/******************************************************************************
* Function Name: Userdef_RSPI2_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 2 transmit buffer empty notification information when 
*              : the RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_SetTransmitEmpty(void)
{
    GenericSetTransmitEmpty(DEVDRV_CH_2);
}

/******************************************************************************
* Function Name: Userdef_RSPI3_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 3 transmit buffer empty notification information when 
*              : the RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_SetTransmitEmpty(void)
{
    GenericSetTransmitEmpty(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Userdef_RSPI4_SetTransmitEmpty
* Description  : Execute this function to satisfy the conditions for the RSPI
*              : channel 4 transmit buffer empty notification information.
*              : In the sample code, 1 is set to the software flag to notify 
*              : that the RSPI channel 4 transmit buffer empty interrupt has
*              : been generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_SetTransmitEmpty(void)
{
    GenericSetTransmitEmpty(DEVDRV_CH_4);
}

static void GenericWaitReceiveFull(devdrv_ch_t channel, uint32_t byte, uint32_t less_rxtrg)
{
    uint32_t tx_byte;
    uint16_t sprx_byte;
    uint8_t  splw;
    uint8_t  txtrg_backup;

    tx_byte = byte;

    volatile struct st_rspi *spi_block = RSPI_GetRegAddr(channel);

    /* ==== Save TXTRG ==== */
    txtrg_backup = RZA_IO_RegRead_8(&(spi_block->SPBFCR), RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);

    /* ==== Obtain setting information for the access width ==== */
    splw = RZA_IO_RegRead_8(&(spi_block->SPDCR), RSPIn_SPDCR_SPLW_SHIFT, RSPIn_SPDCR_SPLW);

   /* ==== Set the TXTRG value according to the access width settings ==== */
    if (RSPI_SPDR_ACCESS_32 == splw)
    {
        RZA_IO_RegWrite_8(&(spi_block->SPBFCR), 2, RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);
    }
    else if (RSPI_SPDR_ACCESS_16 == splw)
    {
        RZA_IO_RegWrite_8(&(spi_block->SPBFCR), 1, RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);
    }
    else
    {
        RZA_IO_RegWrite_8(&(spi_block->SPBFCR), 0, RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);
    }

    /* ==== Enable the receive interrupt request to be issued ==== */
    RZA_IO_RegWrite_8(&(spi_block->SPCR), 1, RSPIn_SPCR_SPRIE_SHIFT, RSPIn_SPCR_SPRIE);

    while (tx_byte > 0)
    {
        GenericWaitTransmitEmpty(channel);
        GenericInitTransmitEmpty(channel);

        if (RSPI_SPDR_ACCESS_32 == splw)
        {
            spi_block->SPDR.UINT32 = RSPI_DUMMY_DATA_32;
            tx_byte -= 4;
        }
        else if (RSPI_SPDR_ACCESS_16 == splw)
        {
            spi_block->SPDR.UINT16[0] = RSPI_DUMMY_DATA_16;
            tx_byte -= 2;
        }
        else
        {
            spi_block->SPDR.UINT8[0] = RSPI_DUMMY_DATA_8;
            tx_byte--;
        }
    }

    /* ==== Processing to satisfy the conditions for receive buffer full  without waiting the trigger (RXTRG) ==== */
    if (1 == less_rxtrg)    /* When receiving the data smaller than the RXTRG */
    {
        while (1)
        {
            /* Monitor data count of the receive buffer */
            sprx_byte = RZA_IO_RegRead_16(&(spi_block->SPBFDR), RSPIn_SPBFDR_R_SHIFT, RSPIn_SPBFDR_R);
            if (sprx_byte >= byte)  /* When the data receives to the specified data count */
            {
                /* Assume that the conditions for receive buffer full are satisfied */
                GenericSetReceiveFull(channel);
                break;
            }
        }
    }
    /* ==== Wait until the conditions for receive buffer full are satisfied ==== */
    else    /* When receiving the data larger than the RXTRG */
    {
        while (DEVDRV_FLAG_OFF == GetReceiveFull(channel))
        {
            /* Wait */
        }
    }

    /* ==== Restore TXTRG ==== */
    RZA_IO_RegWrite_8(&(spi_block->SPBFCR), txtrg_backup, RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);
}

/******************************************************************************
* Function Name: Userdef_RSPI0_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 0 receive buffer full notification information are 
*              : satisfied when the RSPI channel 0 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    GenericWaitReceiveFull(DEVDRV_CH_0, byte, less_rxtrg);
}

/******************************************************************************
* Function Name: Userdef_RSPI1_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 1 receive buffer full notification information are 
*              : satisfied when the RSPI channel 1 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    GenericWaitReceiveFull(DEVDRV_CH_1, byte, less_rxtrg);
}

/******************************************************************************
* Function Name: Userdef_RSPI2_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 2 receive buffer full notification information are 
*              : satisfied when the RSPI channel 2 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    GenericWaitReceiveFull(DEVDRV_CH_2, byte, less_rxtrg);
}

/******************************************************************************
* Function Name: Userdef_RSPI3_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 3 receive buffer full notification information are 
*              : satisfied when the RSPI channel 3 is used.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    GenericWaitReceiveFull(DEVDRV_CH_3, byte, less_rxtrg);
}

/******************************************************************************
* Function Name: Userdef_RSPI4_WaitReceiveFull
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 4 receive buffer full notification information are 
*              : satisfied.
*              : In the sample code, the dummy data is transmitted to output
*              : the RSPCK clock in order to receive the data for the bytes
*              : specified by the argument byte. 
*              : Enables the RSPI channel 4 receive buffer full interrupt, and 
*              : waits until 1 is set to the software flag to notify that the 
*              : interrupt has been generated. However, if 1 is specified to the 
*              : argument less_rxtrg, terminate this function after satisfying 
*              : the conditions for the receive buffer full notification 
*              : information when the data is stored in the receieve buffer 
*              : for the bytes specified by the argument byte.
*              : When receiving the byte count which is smaller than the one  
*              : specified by the RXTRG bit of the buffer control register 
*              : (SPBFCR), efficient receive processing can be executed by setting
*              : 1 to the argument less_rxtrg.
* Arguments    : uint32_t byte       : Byte count of dummy data to be transmitted
*              : uint32_t less_rxtrg : Setting for operation
*              :                     :   0 : Normal operation 
*              :                     :     : (Wait until the conditions for the  
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied.)
*              :                     :   1 : Wait until the conditions for the
*              :                     :     : receive buffer full notification 
*              :                     :     : information are satisfied, but the
*              :                     :     : conditions are determined to be
*              :                     :     : satisfied when the data is received
*              :                     :     : for the bytes specified by the argument
*              :                     :     : byte.
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_WaitReceiveFull(uint32_t byte, uint32_t less_rxtrg)
{
    GenericWaitReceiveFull(DEVDRV_CH_4, byte, less_rxtrg);
}

static void GenericWaitTransmitEmpty(devdrv_ch_t channel)
{
    volatile struct st_rspi *spi_block = RSPI_GetRegAddr(channel);
    RZA_IO_RegWrite_8(&(spi_block->SPCR), 1, RSPIn_SPCR_SPTIE_SHIFT, RSPIn_SPCR_SPTIE);
    while (DEVDRV_FLAG_OFF == GetTransmitEmpty(channel))
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 0 transmit buffer empty notification information are
*              : satisfied when the RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_WaitTransmitEmpty(void)
{
    GenericWaitTransmitEmpty(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Userdef_RSPI1_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 1 transmit buffer empty notification information are
*              : satisfied when the RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_WaitTransmitEmpty(void)
{
    GenericWaitTransmitEmpty(DEVDRV_CH_1);
}

/******************************************************************************
* Function Name: Userdef_RSPI2_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 2 transmit buffer empty notification information are
*              : satisfied when the RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_WaitTransmitEmpty(void)
{
    GenericWaitTransmitEmpty(DEVDRV_CH_2);
}

/******************************************************************************
* Function Name: Userdef_RSPI3_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 3 transmit buffer empty notification information are
*              : satisfied when the RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_WaitTransmitEmpty(void)
{
    GenericWaitTransmitEmpty(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Userdef_RSPI4_WaitTransmitEmpty
* Description  : Execute this function to wait until the conditions for the 
*              : RSPI channel 4 transmit buffer empty notification information 
*              : are satisfied.
*              : In the sample code, this function enables the transfer buffer 
*              : empty interrupt and waits until 1 is set to the software flag
*              : to notify that the interrupt has been generated.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_WaitTransmitEmpty(void)
{
    GenericWaitTransmitEmpty(DEVDRV_CH_4);
}

static void GenericWaitTransmitEnd(devdrv_ch_t channel)
{
    volatile struct st_rspi *spi_block = RSPI_GetRegAddr(channel);
    while (0 == RZA_IO_RegRead_8(&(spi_block->SPSR), RSPIn_SPSR_TEND_SHIFT, RSPIn_SPSR_TEND))
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 0 transmit end are satisfied when the RSPI channel 0 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_WaitTransmitEnd(void)
{
    GenericWaitTransmitEnd(DEVDRV_CH_0);
}

/******************************************************************************
* Function Name: Userdef_RSPI1_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 1 transmit end are satisfied when the RSPI channel 1 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_WaitTransmitEnd(void)
{
    GenericWaitTransmitEnd(DEVDRV_CH_1);
}

/******************************************************************************
* Function Name: Userdef_RSPI2_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 2 transmit end are satisfied when the RSPI channel 2 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_WaitTransmitEnd(void)
{
    GenericWaitTransmitEnd(DEVDRV_CH_2);
}

/******************************************************************************
* Function Name: Userdef_RSPI3_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 3 transmit end are satisfied when the RSPI channel 3 is 
*              : used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_WaitTransmitEnd(void)
{
    GenericWaitTransmitEnd(DEVDRV_CH_3);
}

/******************************************************************************
* Function Name: Userdef_RSPI4_WaitTransmitEnd
* Description  : Execute this function to wait until the conditions for the RSPI
*              : channel 4 transmit end are satisfied.
*              : In the sample code, this function waits until 1 is set to the
*              : TEND bit of the status register (SPSR).
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_WaitTransmitEnd(void)
{
    GenericWaitTransmitEnd(DEVDRV_CH_4);
}

static void GenericDelayNextAccess()
{
    volatile int32_t cnt;

    cnt = 10;
    while (cnt-- > 0)
    {
        /* Delay */
    }
}

/******************************************************************************
* Function Name: Userdef_RSPI0_DelayNextAccess
* Description  : When the RSPI channel 0 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : RSPI channel 0 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI0_DelayNextAccess(void)
{
    GenericDelayNextAccess();
}

/******************************************************************************
* Function Name: Userdef_RSPI1_DelayNextAccess
* Description  : When the RSPI channel 1 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : RSPI channel 1 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI1_DelayNextAccess(void)
{
    GenericDelayNextAccess();
}

/******************************************************************************
* Function Name: Userdef_RSPI2_DelayNextAccess
* Description  : When the RSPI channel 2 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : RSPI channel 2 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI2_DelayNextAccess(void)
{
    GenericDelayNextAccess();
}

/******************************************************************************
* Function Name: Userdef_RSPI3_DelayNextAccess
* Description  : When the RSPI channel 3 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : RSPI channel 3 is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_RSPI3_DelayNextAccess(void)
{
    GenericDelayNextAccess();
}

/******************************************************************************
* Function Name: Userdef_RSPI4_DelayNextAccess
* Description  : When the RSPI channel 4 is used, execute this function to 
*              : secure the period which is required to start the next access 
*              : after the latest transfer ends. 
*              : In the sample code, this function secures the SSL signal 
*              : negation period (90ns or more) required for the EEPROM by 
*              : using the software wait processing.
* Return Value : None
******************************************************************************/
void Userdef_RSPI4_DelayNextAccess(void)
{
    GenericDelayNextAccess();
}

void Userdef_SPRI0_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI0_SetReceiveFull();
    R_UNUSED(int_sense);
}

void Userdef_SPRI1_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI1_SetReceiveFull();
    R_UNUSED(int_sense);
}

void Userdef_SPRI2_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI2_SetReceiveFull();
    R_UNUSED(int_sense);
}

void Userdef_SPRI3_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI3_SetReceiveFull();
    R_UNUSED(int_sense);
}

void Userdef_SPRI4_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI4_SetReceiveFull();
    R_UNUSED(int_sense);
}

void Userdef_SPTI0_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI0_SetTransmitEmpty();
    R_UNUSED(int_sense);
}

void Userdef_SPTI1_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI1_SetTransmitEmpty();
    R_UNUSED(int_sense);
}

void Userdef_SPTI2_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI2_SetTransmitEmpty();
    R_UNUSED(int_sense);
}

void Userdef_SPTI3_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI3_SetTransmitEmpty();
    R_UNUSED(int_sense);
}

void Userdef_SPTI4_Interrupt(uint32_t int_sense)
{
    Userdef_RSPI4_SetTransmitEmpty();
    R_UNUSED(int_sense);
}

/******************************************************************************
* Function Name: RSPI_GetRegAddr
* Description  : Obtains the start address of the RSPI-related registers of the 
*              : specified channel.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : struct st_rspi * : Start address of RSPI register by channel
******************************************************************************/
static volatile struct st_rspi * RSPI_GetRegAddr(uint32_t channel)
{
    volatile struct st_rspi * rspi;

    switch (channel)
    {
        case DEVDRV_CH_0:
            rspi = &RSPI0;
        break;
        case DEVDRV_CH_1:
            rspi = &RSPI1;
        break;
        case DEVDRV_CH_2:
            rspi = &RSPI2;
        break;
        case DEVDRV_CH_3:
            rspi = &RSPI3;
        break;
        case DEVDRV_CH_4:
            rspi = &RSPI4;
        break;
        default:
            /* Do not reach here based on the assumption */
            rspi = &RSPI0;
        break;
    }

    return rspi;
}

/* End of File */

