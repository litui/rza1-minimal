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
* File Name   : rspi.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Renesas Serial Peripheral Interface(RSPI) device drive
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_rspi.h"            /* RSPI Driver header */
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== RSPI H/W specification ==== */
#if defined(BOARD_GR_PEACH)
#define RSPI_CH_TOTAL           (5)
#else
#define RSPI_CH_TOTAL           (3)
#endif

/* ==== Access to SPDR register ==== */
#define RSPI_SPDR_ACCESS_8      (1)     /* 8-bit access  */
#define RSPI_SPDR_ACCESS_16     (2)     /* 16-bit access */
#define RSPI_SPDR_ACCESS_32     (3)     /* 32-bit access */

/* ==== FIFO buffer stages ==== */
#define RSPI_SPTX_FIFO_STAGE    (8)     /* SPTX (Transmit FIFO buffer) */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void    RSPI_Open(volatile struct st_rspi * rspi);
static void    RSPI_Close(volatile struct st_rspi * rspi);
static int32_t RSPI_Receive(uint32_t channel, void * buffer, uint32_t count, uint8_t rxtrg);
static int32_t RSPI_Transmit(uint32_t channel, void * buffer, uint32_t count, uint8_t txtrg);
static void    RSPI_ResetBuffer(volatile struct st_rspi * rspi);

/* FIFO buffer trigger (TXTRG, RXTRG) setting value conversion table */
static uint32_t rspi_txtrg_table[4] = { 1, 2, 4, 8 };
static uint32_t rspi_rxtrg_table[8] = { 1, 2, 4, 8, 16, 24, 32, 5 };

/******************************************************************************
* Function Name: R_RSPI_Init
* Description  : Initializes the RSPI specified by the argument channel.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : DEVDRV_SUCCESS : Success of RSPI initialization
*              : DEVDRV_ERROR   : Failure of RSPI initialization
******************************************************************************/
int32_t R_RSPI_Init(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Initialize RSPI ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RSPI0_Init();
        break;
        case DEVDRV_CH_1:
            Userdef_RSPI1_Init();
        break;
        case DEVDRV_CH_2:
            Userdef_RSPI2_Init();
        break;
        case DEVDRV_CH_3:
            Userdef_RSPI3_Init();
        break;
        case DEVDRV_CH_4:
            Userdef_RSPI4_Init();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RSPI_Open
* Description  : Enables the RSPI functions specified by the argument channel.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : DEVDRV_SUCCESS : Success in starting RSPI communication
*              : DEVDRV_ERROR   : Failure in starting RSPI communication
******************************************************************************/
int32_t R_RSPI_Open(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Setting to enable RSPI functions ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            RSPI_Open(&RSPI0);
        break;
        case DEVDRV_CH_1:
            RSPI_Open(&RSPI1);
        break;
        case DEVDRV_CH_2:
            RSPI_Open(&RSPI2);
        break;
        case DEVDRV_CH_3:
            RSPI_Open(&RSPI3);
        break;
        case DEVDRV_CH_4:
            RSPI_Open(&RSPI4);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RSPI_Close
* Description  : Disables the RSPI functions specified by the argument channel, 
*              : and negates the SSL signal.
*              : When using the device which is required to negate the SSL signal
*              : before the next access starts, the processing to secure the SSL
*              : signal negation period by using the user-defined function 
*              : Userdef_RSPIn_DelayNextAccess (n=0 to 4).
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : DEVDRV_SUCCESS : Success in ending RSPI communication
*              : DEVDRV_ERROR   : Failure in ending RSPI communication
******************************************************************************/
int32_t R_RSPI_Close(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Disable RSPI functions ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            RSPI_Close(&RSPI0);

            Userdef_RSPI0_DelayNextAccess();
        break;
        case DEVDRV_CH_1:
            RSPI_Close(&RSPI1);

            Userdef_RSPI1_DelayNextAccess();
        break;
        case DEVDRV_CH_2:
            RSPI_Close(&RSPI2);

            Userdef_RSPI2_DelayNextAccess();
        break;
        case DEVDRV_CH_3:
            RSPI_Close(&RSPI3);

            Userdef_RSPI3_DelayNextAccess();
        break;
        case DEVDRV_CH_4:
            RSPI_Close(&RSPI4);

            Userdef_RSPI4_DelayNextAccess();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RSPI_Read
* Description  : Receives the data using the RSPI specified by the argument 
*              : channel.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
*              : void * buffer    : Receive data
*              : uint32_t count   : Reception counts of data
*              : uint8_t rxtrg    : Receive buffer data triggering number 
*              :                  : (Setting values for the RXTRG bit)
*              :                  :   RSPI_RXTRG_CURRENT : Use current setting value
*              :                  :   RSPI_RXTRG_01_BYTE : Change to  1 byte
*              :                  :   RSPI_RXTRG_02_BYTE : Change to  2 bytes
*              :                  :   RSPI_RXTRG_04_BYTE : Change to  4 bytes
*              :                  :   RSPI_RXTRG_08_BYTE : Change to  8 bytes
*              :                  :   RSPI_RXTRG_16_BYTE : Change to 16 bytes
*              :                  :   RSPI_RXTRG_24_BYTE : Change to 24 bytes
*              :                  :   RSPI_RXTRG_32_BYTE : Change to 32 bytes
*              :                  :   RSPI_RXTRG_05_BYTE : Change to  5 bytes
* Return Value : DEVDRV_SUCCESS : Success of RSPI reception
*              : DEVDRV_ERROR   : Failure of RSPI reception
******************************************************************************/
int32_t R_RSPI_Read(uint32_t channel, void * buffer, uint32_t count, uint8_t rxtrg)
{
    int32_t ret;

    ret = DEVDRV_SUCCESS;

    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL || (rxtrg > RSPI_RXTRG_05_BYTE && rxtrg < RSPI_RXTRG_CURRENT))
    {
        ret = DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== RSPI reception ==== */
    if (ret != DEVDRV_ERROR)
    {
        if (count > 0)
        {
            ret = RSPI_Receive(channel, buffer, count, rxtrg);
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: R_RSPI_Write
* Description  : Transmits the data using the RSPI specified by the argument
*              : channel.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
*              : void * buffer    : Transmit data
*              : uint32_t count   : Transmission counts of data
*              : uint8_t txtrg    : Transmit buffer data triggering number 
*              :                  : (Setting values for the TXTRG bit)
*              :                  :   RSPI_TXTRG_CURRENT : Use current setting value
*              :                  :   RSPI_TXTRG_07_BYTE : Change to 7 bytes
*              :                  :   RSPI_TXTRG_06_BYTE : Change to 6 bytes
*              :                  :   RSPI_TXTRG_04_BYTE : Change to 4 bytes
*              :                  :   RSPI_TXTRG_00_BYTE : Change to 0 byte
* Return Value : DEVDRV_SUCCESS : Success of RSPI transmission
*              : DEVDRV_ERROR   : Failure of RSPI transmission
******************************************************************************/
int32_t R_RSPI_Write(uint32_t channel, void * buffer, uint32_t count, uint8_t txtrg)
{
    int32_t ret;

    ret = DEVDRV_SUCCESS;

    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL || (txtrg > RSPI_TXTRG_00_BYTE && txtrg < RSPI_TXTRG_CURRENT))
    {
        ret = DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== RSPI transmission ==== */
    if (ret != DEVDRV_ERROR)
    {
        if (count > 0)
        {
            ret = RSPI_Transmit(channel, buffer, count, txtrg);
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: R_RSPI_ResetBuffer
* Description  : Resets the transmit and receive buffers of the RSPI specified 
*              : by the argument channel. 
*              : Initializes the transmit buffer empty notification information 
*              : and the receive buffer full notification information by using 
*              : the user-defined functions Userdef_RSPIn_InitTransmitEmpty and 
*              : Userdef_RSPIn_InitReceiveFull (n=0 to 4).
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : DEVDRV_SUCCESS : Success of RSPI buffer reset
*              : DEVDRV_ERROR   : Failure of RSPI buffer reset
******************************************************************************/
int32_t R_RSPI_ResetBuffer(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Clear conditions for receive buffer full and transmit buffer empty ==== */
    /* ==== Reset transmit and receive buffers ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RSPI0_InitReceiveFull();
            Userdef_RSPI0_InitTransmitEmpty();

            RSPI_ResetBuffer(&RSPI0);
        break;
        case DEVDRV_CH_1:
            Userdef_RSPI1_InitReceiveFull();
            Userdef_RSPI1_InitTransmitEmpty();

            RSPI_ResetBuffer(&RSPI1);
        break;
        case DEVDRV_CH_2:
            Userdef_RSPI2_InitReceiveFull();
            Userdef_RSPI2_InitTransmitEmpty();

            RSPI_ResetBuffer(&RSPI2);
        break;
        case DEVDRV_CH_3:
            Userdef_RSPI3_InitReceiveFull();
            Userdef_RSPI3_InitTransmitEmpty();

            RSPI_ResetBuffer(&RSPI3);
        break;
        case DEVDRV_CH_4:
            Userdef_RSPI4_InitReceiveFull();
            Userdef_RSPI4_InitTransmitEmpty();

            RSPI_ResetBuffer(&RSPI4);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RSPI_SpriInterrupt
* Description  : Executes the processing to satisfy the conditions for the receive
*              : buffer full notification information of the RSPI specified by
*              : the argument channel.
*              : This function is called by the interrupt handler of the RSPI
*              : receive buffer full interrupt to notify that an interrupt has
*              : been generated. The processing to satisfy the conditions for the 
*              : receive buffer full notification information should be executed
*              : by using the user-defined function Userdef_RSPIn_SetReceiveFull 
*              : (n=0 to 4).
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : DEVDRV_SUCCESS : Success of RSPI receive buffer full interrupt 
*              :                : notification
*              : DEVDRV_ERROR   : Failure of RSPI receive buffer full interrupt 
*              :                : notification
******************************************************************************/
int32_t R_RSPI_SpriInterrupt(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Set conditions for receive buffer full interrupt ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RSPI0_SetReceiveFull();
        break;
        case DEVDRV_CH_1:
            Userdef_RSPI1_SetReceiveFull();
        break;
        case DEVDRV_CH_2:
            Userdef_RSPI2_SetReceiveFull();
        break;
        case DEVDRV_CH_3:
            Userdef_RSPI3_SetReceiveFull();
        break;
        case DEVDRV_CH_4:
            Userdef_RSPI4_SetReceiveFull();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RSPI_SptiInterrupt
* Description  : Executes the processing to satisfy the conditions for the 
*              : transmit buffer empty notification information of the RSPI
*              : specified by the argument channel.
*              : This function is called by the interrupt handler of the RSPI
*              : transmit buffer empty interrupt to notify that an interrupt 
*              : has been generated.
*              : The processing to satisfy the conditions for the transmit buffer 
*              : empty notification information should be executed by using by
*              : using the user-defined function Userdef_RSPIn_SetTransmitEmpty
*              : (n=0 to 4).
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : DEVDRV_SUCCESS : Success of RSPI transmit buffer empty interrupt
*              :                : notification
*              : DEVDRV_ERROR   : Failure of RSPI transmit buffer empty interrupt 
*              :                : notification
******************************************************************************/
int32_t R_RSPI_SptiInterrupt(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Set conditions for transmit buffer empty interrupt ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RSPI0_SetTransmitEmpty();
        break;
        case DEVDRV_CH_1:
            Userdef_RSPI1_SetTransmitEmpty();
        break;
        case DEVDRV_CH_2:
            Userdef_RSPI2_SetTransmitEmpty();
        break;
        case DEVDRV_CH_3:
            Userdef_RSPI3_SetTransmitEmpty();
        break;
        case DEVDRV_CH_4:
            Userdef_RSPI4_SetTransmitEmpty();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RSPI_WaitTransmitEnd
* Description  : Waits for the transmit end of the RSPI specified by the 
*              : argument channel. 
*              : Waits until the conditions for the transmit end are satisfied
*              : by using the user-defined function Userdef_RSPIn_WaitTransmitEnd
*              : (n=0 to 4).
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : DEVDRV_SUCCESS : Succeed of RSPI transmit end waiting
*              : DEVDRV_ERROR   : Failure of RSPI transmit end waiting
******************************************************************************/
int32_t R_RSPI_WaitTransmitEnd(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= RSPI_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Wait until transmit end conditions are satisfied ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_RSPI0_WaitTransmitEnd();
        break;
        case DEVDRV_CH_1:
            Userdef_RSPI1_WaitTransmitEnd();
        break;
        case DEVDRV_CH_2:
            Userdef_RSPI2_WaitTransmitEnd();
        break;
        case DEVDRV_CH_3:
            Userdef_RSPI3_WaitTransmitEnd();
        break;
        case DEVDRV_CH_4:
            Userdef_RSPI4_WaitTransmitEnd();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/*******************************************************************************
* Function Name: RSPI_Open
* Description  : Enables the RSPI functions.
* Arguments    : struct st_rspi * rspi : Start address of RSPI register by channel
* Return Value : None
*******************************************************************************/
static void RSPI_Open(volatile struct st_rspi * rspi)
{
    /* ==== Enable RSPI functions ==== */
    RZA_IO_RegWrite_8(&(rspi->SPCR), 1, RSPIn_SPCR_SPE_SHIFT, RSPIn_SPCR_SPE);
}

/******************************************************************************
* Function Name: RSPI_Close
* Description  : Disables the RSPI functions.
* Arguments    : struct st_rspi * rspi : Start address of RSPI register by channel
* Return Value : None
******************************************************************************/
static void RSPI_Close(volatile struct st_rspi * rspi)
{
    /* ==== Disable RSPI functions ==== */
    RZA_IO_RegWrite_8(&(rspi->SPCR), 0, RSPIn_SPCR_SPE_SHIFT, RSPIn_SPCR_SPE);
}

/******************************************************************************
* Function Name: RSPI_Receive
* Description  : Receives the data using the RSPI specified by the argument 
*              : channel.
*              : By waiting until the conditions for the receive buffer full 
*              : notification information are satisfied using the user-defined
*              : function Userdef_RSPIn_WaitReceiveFull, the data for the bytes
*              : set to the RXTRG bit of the buffer control register (SPBFCR) is
*              : stored in the receive buffer. Then receives the said data and 
*              : initializes the receive buffer full notification information by 
*              : using the user-defined function Userdef_RSPIn_InitReceiveFull
*              : (n=0 to 4). This receive operation is repeated until the 
*              : reception counts reaches the number specified by the argument
*              : count. If the specified reception counts is smaller than the 
*              : bytes set to the RXTRG bit, the data is received only for the 
*              : bytes specified by the argument count.
*              : When starting this function, the value specified for the argument
*              : rxtrg is set to the RXTRG bit, and the original value of the bit
*              : is restored at the end of this function. However, the value of
*              : the RXTRG bit will not be changed if RSPI_RXTRG_CURRENT is 
*              : specified for the argument rxtrg. The data reception is executed
*              : using the access width set to the SPLW bit of the data control 
*              : register (SPDCR).
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
*              : void * buffer    : Receive data
*              : uint32_t count   : Reception counts of data
*              : uint8_t rxtrg    : Receive buffer data triggering number 
*              :                  : (Setting values for the RXTRG bit)
*              :                  :   RSPI_RXTRG_CURRENT : Use current setting value
*              :                  :   RSPI_RXTRG_01_BYTE : Change to  1 byte
*              :                  :   RSPI_RXTRG_02_BYTE : Change to  2 bytes
*              :                  :   RSPI_RXTRG_04_BYTE : Change to  4 bytes
*              :                  :   RSPI_RXTRG_08_BYTE : Change to  8 bytes
*              :                  :   RSPI_RXTRG_16_BYTE : Change to 16 bytes
*              :                  :   RSPI_RXTRG_24_BYTE : Change to 24 bytes
*              :                  :   RSPI_RXTRG_32_BYTE : Change to 32 bytes
*              :                  :   RSPI_RXTRG_05_BYTE : Change to  5 bytes
* Return Value : DEVDRV_SUCCESS : Success of RSPI reception
*              : DEVDRV_ERROR   : Failure of RSPI reception
******************************************************************************/
static int32_t RSPI_Receive(uint32_t channel, void * buffer, uint32_t count, uint8_t rxtrg)
{
    volatile struct st_rspi * rspi;
    uint8_t    splw;
    uint32_t * buffer_addr_32;
    uint16_t * buffer_addr_16;
    uint8_t  * buffer_addr_8;
    uint8_t    rxtrg_backup;
    uint32_t   access_size;
    uint32_t   trigger_byte;
    uint8_t    rxtrg_value;
    uint32_t   read_byte;
    uint32_t   less_rxtrg;
    uint32_t   remain_byte;
    int32_t    ret;

    buffer_addr_32 = NULL;
    buffer_addr_16 = NULL;
    buffer_addr_8  = NULL;
    rxtrg_backup   = 0;

    rspi = RSPI_GetRegAddr(channel);

    if (RSPI_RXTRG_CURRENT == rxtrg)
    {
        /* Do not set RXTRG (Keep current setting value) */
    }
    else
    {
        /* Save RXTRG */
        rxtrg_backup = RZA_IO_RegRead_8(&(rspi->SPBFCR), RSPIn_SPBFCR_RXTRG_SHIFT, RSPIn_SPBFCR_RXTRG);
        /* Reset RXTRG */
        RZA_IO_RegWrite_8(&(rspi->SPBFCR), rxtrg, RSPIn_SPBFCR_RXTRG_SHIFT, RSPIn_SPBFCR_RXTRG);
    }

    /* Obtain setting information for the access width */
    splw = RZA_IO_RegRead_8(&(rspi->SPDCR), RSPIn_SPDCR_SPLW_SHIFT, RSPIn_SPDCR_SPLW);

    if (RSPI_SPDR_ACCESS_32 == splw)
    {
        buffer_addr_32 = (uint32_t *)buffer;
        access_size = 4;
    }
    else if (RSPI_SPDR_ACCESS_16 == splw)
    {
        buffer_addr_16 = (uint16_t *)buffer;
        access_size = 2;
    }
    else
    {
        buffer_addr_8  = (uint8_t  *)buffer;
        access_size = 1;
    }

    /* Obtain information for the receive buffer data triggering number */
    rxtrg_value = RZA_IO_RegRead_8(&(rspi->SPBFCR), RSPIn_SPBFCR_RXTRG_SHIFT, RSPIn_SPBFCR_RXTRG);
    trigger_byte = rspi_rxtrg_table[rxtrg_value];

    /* ==== Error detection ==== */
    /* When the receive buffer data triggering number is specified, which  */
    /* is smaller than the byte count set for the access width             */
    if (trigger_byte < access_size)
    {
        ret = DEVDRV_ERROR;
    }
    /* When the receive buffer data triggering number is specified,        */
    /* which does not have multiples of the bytes set for the access width */
    else if ((trigger_byte % access_size) != 0)
    {
        ret = DEVDRV_ERROR;
    }
    else    /* No error */
    {
        ret = DEVDRV_SUCCESS;
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* ==== Read for the specified counts ==== */
        while (count > 0)
        {
            remain_byte = count * access_size;

            /* ---- When receiving the data for the bytes smaller than RXTRG ---- */
            if (remain_byte < trigger_byte)
            {
                less_rxtrg = 1;
                read_byte = remain_byte;
            }
            /* ---- When receiving the data for the byte count same as or larger than RXTRG ---- */
            else
            {
                less_rxtrg = 0;
                read_byte = trigger_byte;
            }

            /* ==== Wait until the conditions for receieve data full are satisfied ==== */
            switch (channel)
            {
                case DEVDRV_CH_0:
                    Userdef_RSPI0_WaitReceiveFull(read_byte, less_rxtrg);
                break;
                case DEVDRV_CH_1:
                    Userdef_RSPI1_WaitReceiveFull(read_byte, less_rxtrg);
                break;
                case DEVDRV_CH_2:
                    Userdef_RSPI2_WaitReceiveFull(read_byte, less_rxtrg);
                break;
                case DEVDRV_CH_3:
                    Userdef_RSPI3_WaitReceiveFull(read_byte, less_rxtrg);
                break;
                case DEVDRV_CH_4:
                    Userdef_RSPI4_WaitReceiveFull(read_byte, less_rxtrg);
                break;
                default:
                    /* Do not reach here based on the assumption */
                break;
            }

            while (read_byte >= access_size)
            {
                /* ---- Read bytes for receive buffer data ---- */
                if (RSPI_SPDR_ACCESS_32 == splw)
                {
                    *buffer_addr_32 = rspi->SPDR.UINT32;    /* Read 32 bits */
                    buffer_addr_32++;
                }
                else if (RSPI_SPDR_ACCESS_16 == splw)
                {
                    *buffer_addr_16 = rspi->SPDR.UINT16[0]; /* Read 16 bits */
                    buffer_addr_16++;
                }
                else
                {
                    *buffer_addr_8 = rspi->SPDR.UINT8[0];   /* Read 8 bits  */
                    buffer_addr_8++;
                }

                count--;
                read_byte -= access_size;
            }

            /* ==== Clear the conditions for receive buffer full ==== */
            switch (channel)
            {
                case DEVDRV_CH_0:
                    Userdef_RSPI0_InitReceiveFull();
                break;
                case DEVDRV_CH_1:
                    Userdef_RSPI1_InitReceiveFull();
                break;
                case DEVDRV_CH_2:
                    Userdef_RSPI2_InitReceiveFull();
                break;
                case DEVDRV_CH_3:
                    Userdef_RSPI3_InitReceiveFull();
                break;
                case DEVDRV_CH_4:
                    Userdef_RSPI4_InitReceiveFull();
                break;
                default:
                    /* Do not reach here based on the assumption */
                break;
            }
        }
    }

    if (RSPI_RXTRG_CURRENT == rxtrg)
    {
        /* Do Nothing */
    }
    else
    {
        /* Restore TXTRG */
        RZA_IO_RegWrite_8(&(rspi->SPBFCR), rxtrg_backup, RSPIn_SPBFCR_RXTRG_SHIFT, RSPIn_SPBFCR_RXTRG);
    }

    return ret;
}

/******************************************************************************
* Function Name: RSPI_Transmit
* Description  : Transmits the data using the RSPI specified by the argument 
*              : channel.
*              : By waiting until the conditions for the transmit buffer empty
*              : notification information are satisfied using the user-defined
*              : function Userdef_RSPIn_WaitTransmitEmpty, the empty space for 
*              : the bytes set to the TXTRG bit of the buffer control register
*              : (SPBFCR) can be secured in the RSPI transmit buffer. The data
*              : is written to space in the transmit buffer until the empty 
*              : space becomes full. Then initializes the transmit buffer
*              : empty notification information by using the user-defined 
*              : function Userdef_RSPIn_InitTransmitEmpty (n=0 to 4).
*              : The transmit operation is repeated until the transmission 
*              : counts reaches the number specified by the argument count.
*              : When starting this function, the value specified for the 
*              : argument txtrg is set to the TXTRG bit, and the original value
*              : of the bit is restored at the end of this function. However, 
*              : the value of the TXTRG bit will not be changed if 
*              : RSPI_TXTRG_CURRENT is specified for the argument txtrg. 
*              : The data transmission is executed using the access width set
*              : to the SPLW bit of the data control register (SPDCR). 
*              : If there is no space in the receive buffer, dummy reads are 
*              : executed for every transmissions to avoid receive overflows.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
*              : void * buffer    : Transmit data
*              : uint32_t count   : Transmission counts of data
*              : uint8_t txtrg    : Transmit buffer data triggering number 
*              :                  : (Setting values for the TXTRG bit)
*              :                  :   RSPI_TXTRG_CURRENT : Use current setting value
*              :                  :   RSPI_TXTRG_07_BYTE : Change to 7 bytes
*              :                  :   RSPI_TXTRG_06_BYTE : Change to 6 bytes
*              :                  :   RSPI_TXTRG_04_BYTE : Change to 4 bytes
*              :                  :   RSPI_TXTRG_00_BYTE : Change to 0 byte
* Return Value : DEVDRV_SUCCESS : Success of RSPI transmission
*              : DEVDRV_ERROR   : Failure of RSPI transmission
******************************************************************************/
static int32_t RSPI_Transmit(uint32_t channel, void * buffer, uint32_t count, uint8_t txtrg)
{
    volatile struct st_rspi * rspi;
    uint8_t    splw;
    uint32_t * buffer_addr_32;
    uint16_t * buffer_addr_16;
    uint8_t  * buffer_addr_8;
    volatile uint32_t dummy_buffer_32;
    volatile uint16_t dummy_buffer_16;
    volatile uint8_t  dummy_buffer_8;
    uint8_t    txtrg_backup;
    uint32_t   access_size;
    uint32_t   trigger_byte;
    uint8_t    txtrg_value;
    uint32_t   tx_buf_empt_byte;
    uint32_t   remain_byte;
    uint32_t   write_byte;
    int32_t    ret;

    buffer_addr_32 = NULL;
    buffer_addr_16 = NULL;
    buffer_addr_8  = NULL;
    txtrg_backup   = 0;

    rspi = RSPI_GetRegAddr(channel);

    if (RSPI_TXTRG_CURRENT == txtrg)
    {
        /* Do not set TXTRG (Keep current setting value) */
    }
    else
    {
        /* Save TXTRG */
        txtrg_backup = RZA_IO_RegRead_8(&(rspi->SPBFCR), RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);
        /* Reset TXTRG */
        RZA_IO_RegWrite_8(&(rspi->SPBFCR), txtrg, RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);
    }

    /* Obtain setting information for the access width */
    splw = RZA_IO_RegRead_8(&(rspi->SPDCR), RSPIn_SPDCR_SPLW_SHIFT, RSPIn_SPDCR_SPLW);

    if (RSPI_SPDR_ACCESS_32 == splw)
    {
        buffer_addr_32 = (uint32_t *)buffer;
        access_size = 4;
    }
    else if (RSPI_SPDR_ACCESS_16 == splw)
    {
        buffer_addr_16 = (uint16_t *)buffer;
        access_size = 2;
    }
    else
    {
        buffer_addr_8  = (uint8_t *)buffer;
        access_size = 1;
    }

    /* Obtain information for transmit buffer data triggering number */
    txtrg_value = RZA_IO_RegRead_8(&(rspi->SPBFCR), RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);
    trigger_byte = rspi_txtrg_table[txtrg_value];

    /* ==== Error detection ==== */
    /* When the transmit buffer data triggering number (byte count for empty space)      */
    /* is specified, which is smaller than the byte count set for the access width       */
    if (trigger_byte < access_size)
    {
        ret = DEVDRV_ERROR;
    }
    /* When the transmit buffer data triggering number (byte count for empty space)      */
    /* is specified, which does not have multiples of the bytes set for the access width */
    else if ((trigger_byte % access_size) != 0)
    {
        ret = DEVDRV_ERROR;
    }
    else    /* No error */
    {
        ret = DEVDRV_SUCCESS;
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* ==== Write the specified counts ==== */
        while (count > 0)
        {
            tx_buf_empt_byte = trigger_byte;

            /* ==== Wait until the conditions for transmit buffer empty are satisfied ==== */
            switch (channel)
            {
                case DEVDRV_CH_0:
                    Userdef_RSPI0_WaitTransmitEmpty();
                break;
                case DEVDRV_CH_1:
                    Userdef_RSPI1_WaitTransmitEmpty();
                break;
                case DEVDRV_CH_2:
                    Userdef_RSPI2_WaitTransmitEmpty();
                break;
                case DEVDRV_CH_3:
                    Userdef_RSPI3_WaitTransmitEmpty();
                break;
                case DEVDRV_CH_4:
                    Userdef_RSPI4_WaitTransmitEmpty();
                break;
                default:
                    /* Do not reach here based on the assumption */
                break;
            }

            /* ==== Write for the empty space in transmit buffer ==== */
            remain_byte = count * access_size;
            if (remain_byte < tx_buf_empt_byte)
            {
                write_byte = remain_byte;
            }
            else
            {
                write_byte = tx_buf_empt_byte;
            }
            while (write_byte >= access_size)
            {
                /* ---- Write transmit data ---- */
                if (RSPI_SPDR_ACCESS_32 == splw)
                {
                    rspi->SPDR.UINT32 = *buffer_addr_32;    /* Write 32 bits */
                    buffer_addr_32++;
                }
                else if (RSPI_SPDR_ACCESS_16 == splw)
                {
                    rspi->SPDR.UINT16[0] = *buffer_addr_16; /* Write 16 bits */
                    buffer_addr_16++;
                }
                else
                {
                    rspi->SPDR.UINT8[0] = *buffer_addr_8;   /* Write 8 bits  */
                    buffer_addr_8++;
                }

                /* ---- When receive data exists ---- */
                if (1 == RZA_IO_RegRead_8(&(rspi->SPSR), RSPIn_SPSR_SPRF_SHIFT, RSPIn_SPSR_SPRF))
                {
                    /* ---- Execute dummy reads to avoid receive data overflows ---- */
                    if (RSPI_SPDR_ACCESS_32 == splw)
                    {
                        dummy_buffer_32 = rspi->SPDR.UINT32;    /* Read 32 bits */
                    }
                    else if (RSPI_SPDR_ACCESS_16 == splw)
                    {
                        dummy_buffer_16 = rspi->SPDR.UINT16[0]; /* Read 16 bits */
                    }
                    else
                    {
                        dummy_buffer_8  = rspi->SPDR.UINT8[0];  /* Read 8 bits  */
                    }
                }

                count--;
                write_byte -= access_size;
            }

            /* ==== Clear the conditions for transmit buffer empty ==== */
            switch (channel)
            {
                case DEVDRV_CH_0:
                    Userdef_RSPI0_InitTransmitEmpty();
                break;
                case DEVDRV_CH_1:
                    Userdef_RSPI1_InitTransmitEmpty();
                break;
                case DEVDRV_CH_2:
                    Userdef_RSPI2_InitTransmitEmpty();
                break;
                case DEVDRV_CH_3:
                    Userdef_RSPI3_InitTransmitEmpty();
                break;
                case DEVDRV_CH_4:
                    Userdef_RSPI4_InitTransmitEmpty();
                break;
                default:
                    /* Do not reach here based on the assumption */
                break;
            }
        }
    }

    if (RSPI_TXTRG_CURRENT == txtrg)
    {
        /* Do Nothing */
    }
    else
    {
        /* Restore TXTRG */
        RZA_IO_RegWrite_8(&(rspi->SPBFCR), txtrg_backup, RSPIn_SPBFCR_TXTRG_SHIFT, RSPIn_SPBFCR_TXTRG);
    }

    return ret;
}

/******************************************************************************
* Function Name: RSPI_ResetBuffer
* Description  : Resets the transmit and receive buffers.
* Arguments    : struct st_rspi * rspi : Start address of RSPI register by channel
* Return Value : None
******************************************************************************/
static void RSPI_ResetBuffer(volatile struct st_rspi * rspi)
{
    /* ==== Buffer reset ==== */
    RZA_IO_RegWrite_8(&(rspi->SPBFCR), 1, RSPIn_SPBFCR_RXRST_SHIFT, RSPIn_SPBFCR_RXRST);
    RZA_IO_RegWrite_8(&(rspi->SPBFCR), 1, RSPIn_SPBFCR_TXRST_SHIFT, RSPIn_SPBFCR_TXRST);

    /* ==== Buffer reset release ==== */
    RZA_IO_RegWrite_8(&(rspi->SPBFCR), 0, RSPIn_SPBFCR_RXRST_SHIFT, RSPIn_SPBFCR_RXRST);
    RZA_IO_RegWrite_8(&(rspi->SPBFCR), 0, RSPIn_SPBFCR_TXRST_SHIFT, RSPIn_SPBFCR_TXRST);
}

/******************************************************************************
* Function Name: RSPI_GetRegAddr
* Description  : Obtains the start address of the RSPI-related registers of the 
*              : specified channel.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : struct st_rspi * : Start address of RSPI register by channel
******************************************************************************/
volatile struct st_rspi * RSPI_GetRegAddr(uint32_t channel)
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

