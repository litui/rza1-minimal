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
* File Name   : scif_sync.c
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description : Serial Communication Interface with FIFO(SCIF) Clock synchronous 
*             : serial communication device drive
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_scif_sync.h"       /* SCIF SYNC Driver header */
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== SCIF H/W specification ==== */
#define SCIF_SYNC_CH_TOTAL      (8)     /* Number of SCIF channels */

/******************************************************************************
Imported global variables and functions (from other files)
s******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ====  Prototype function ==== */
static void    SCIF_SYNC_Open(volatile struct st_scif * scif, uint32_t mode);
static void    SCIF_SYNC_Close(volatile struct st_scif * scif, uint32_t mode);
static void    SCIF_SYNC_Transmit(uint32_t channel, uint8_t * buffer, uint32_t byte);
static int32_t SCIF_SYNC_Receive(uint32_t channel, uint8_t * buffer, uint32_t byte);
static volatile struct st_scif * SCIF_SYNC_GetRegAddr(uint32_t channel);

/* ==== Global variable ==== */
static uint32_t scif_sync_ttrg_table[4] = { 8, 12, 14, 16 };    /* Transmit data triggering number (empty byte count) */
static uint32_t scif_sync_rtrg_table[4] = { 1,  2,  8, 14 };    /* Receive data triggering number (byte count)        */

/******************************************************************************
* Function Name: R_SCIF_SYNC_Init
* Description  : Initializes the SCIF of the channel specified by the argument 
*              : channel to be operated in the clock synchronous mode with the 
*              : transfer direction specified by the argument mode.
*              : The initialization is executed by the user-defined function 
*              : Userdef_SCIFn_SYNC_Init (n=0 to 7).  
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
*              : uint32_t mode    : Setting for transfer direction 
*              :                  :   SCIF_SYNC_MODE_W  : Transmission
*              :                  :   SCIF_SYNC_MODE_R  : Reception
*              :                  :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : DEVDRV_SUCCESS : Success of SCIF initialization
*              : DEVDRV_ERROR   : Failure of SCIF initialization
******************************************************************************/
int32_t R_SCIF_SYNC_Init(uint32_t channel, uint32_t mode)
{
    /* ==== Argument check ==== */
    if ((channel >= SCIF_SYNC_CH_TOTAL) || (mode < SCIF_SYNC_MODE_W) || (mode > SCIF_SYNC_MODE_RW))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== SCIF initialization ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_SCIF0_SYNC_Init(mode);
        break;
        case DEVDRV_CH_1:
            Userdef_SCIF1_SYNC_Init(mode);
        break;
        case DEVDRV_CH_2:
            Userdef_SCIF2_SYNC_Init(mode);
        break;
        case DEVDRV_CH_3:
            Userdef_SCIF3_SYNC_Init(mode);
        break;
        case DEVDRV_CH_4:
            Userdef_SCIF4_SYNC_Init(mode);
        break;
        case DEVDRV_CH_5:
            Userdef_SCIF5_SYNC_Init(mode);
        break;
        case DEVDRV_CH_6:
            Userdef_SCIF6_SYNC_Init(mode);
        break;
        case DEVDRV_CH_7:
            Userdef_SCIF7_SYNC_Init(mode);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_SCIF_SYNC_Open
* Description  : Enables transmit operation, receive operation, or transmit/
*              : receive operation for the SCIF of the channel specified by the 
*              : argument channel according to the transfer direction specified
*              : by the argument mode.
*              : In transmit operation, initializes the notification information
*              : of the transmit FIFO data empty interrupt occurrence by calling 
*              : the user-defined function Userdef_SCIFn_SYNC_InitTransmitEmpty 
*              : (n=0 to 7). In receive operation, initializes the notification
*              : information of the receive FIFO data full interrupt occurrence 
*              : by calling the user-defined function 
*              : Userdef_SCIFn_SYNC_InitReceiveFull (n=0 to 7).
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
*              : uint32_t mode    : Setting for transfer direction 
*              :                  :   SCIF_SYNC_MODE_W  : Transmission
*              :                  :   SCIF_SYNC_MODE_R  : Reception
*              :                  :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : DEVDRV_SUCCESS : Success in starting SCIF communication
*              : DEVDRV_ERROR   : Failure in starting SCIF communication
******************************************************************************/
int32_t R_SCIF_SYNC_Open(uint32_t channel, uint32_t mode)
{
    /* ==== Argument check ==== */
    if ((channel >= SCIF_SYNC_CH_TOTAL) || (mode < SCIF_SYNC_MODE_W) || (mode > SCIF_SYNC_MODE_RW))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Start SCIF communication ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
            {
                Userdef_SCIF0_SYNC_InitTransmitEmpty(); /* Clear transmit FIFO data empty conditions */
            }
            if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
            {
                Userdef_SCIF0_SYNC_InitReceiveFull();   /* Clear receive FIFO data full conditions */
            }

            SCIF_SYNC_Open(&SCIF0, mode);
        break;
        case DEVDRV_CH_1:
            if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
            {
                Userdef_SCIF1_SYNC_InitTransmitEmpty(); /* Clear transmit FIFO data empty conditions */
            }
            if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
            {
                Userdef_SCIF1_SYNC_InitReceiveFull();   /* Clear receive FIFO data full conditions */
            }

            SCIF_SYNC_Open(&SCIF1, mode);
        break;
        case DEVDRV_CH_2:
            if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
            {
                Userdef_SCIF2_SYNC_InitTransmitEmpty(); /* Clear transmit FIFO data empty conditions */
            }
            if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
            {
                Userdef_SCIF2_SYNC_InitReceiveFull();   /* Clear receive FIFO data full conditions */
            }

            SCIF_SYNC_Open(&SCIF2, mode);
        break;
        case DEVDRV_CH_3:
            if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
            {
                Userdef_SCIF3_SYNC_InitTransmitEmpty(); /* Clear transmit FIFO data empty conditions */
            }
            if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
            {
                Userdef_SCIF3_SYNC_InitReceiveFull();   /* Clear receive FIFO data full conditions */
            }

            SCIF_SYNC_Open(&SCIF3, mode);
        break;
        case DEVDRV_CH_4:
            if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
            {
                Userdef_SCIF4_SYNC_InitTransmitEmpty(); /* Clear transmit FIFO data empty conditions */
            }
            if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
            {
                Userdef_SCIF4_SYNC_InitReceiveFull();   /* Clear receive FIFO data full conditions */
            }

            SCIF_SYNC_Open(&SCIF4, mode);
        break;
        case DEVDRV_CH_5:
            if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
            {
                Userdef_SCIF5_SYNC_InitTransmitEmpty(); /* Clear transmit FIFO data empty conditions */
            }
            if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
            {
                Userdef_SCIF5_SYNC_InitReceiveFull();   /* Clear receive FIFO data full conditions */
            }

            SCIF_SYNC_Open(&SCIF5, mode);
        break;
        case DEVDRV_CH_6:
            if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
            {
                Userdef_SCIF6_SYNC_InitTransmitEmpty(); /* Clear transmit FIFO data empty conditions */
            }
            if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
            {
                Userdef_SCIF6_SYNC_InitReceiveFull();   /* Clear receive FIFO data full conditions */
            }

            SCIF_SYNC_Open(&SCIF6, mode);
        break;
        case DEVDRV_CH_7:
            if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
            {
                Userdef_SCIF7_SYNC_InitTransmitEmpty(); /* Clear transmit FIFO data empty conditions */
            }
            if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
            {
                Userdef_SCIF7_SYNC_InitReceiveFull();   /* Clear receive FIFO data full conditions */
            }

            SCIF_SYNC_Open(&SCIF7, mode);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_SCIF_SYNC_Close
* Description  : Disables the transmit operation, receive operation, or transmit/ 
*              : receive operation for the SCIF of the channel specified by the 
*              : argument channel according to the transfer direction specified
*              : by the argument mode. 
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
*              : uint32_t mode    : Setting for transfer direction 
*              :                  :   SCIF_SYNC_MODE_W  : Transmission
*              :                  :   SCIF_SYNC_MODE_R  : Reception
*              :                  :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : DEVDRV_SUCCESS : Success in terminating SCIF communication
*              : DEVDRV_ERROR   : Failure in terminating SCIF communication
******************************************************************************/
int32_t R_SCIF_SYNC_Close(uint32_t channel, uint32_t mode)
{
    /* ==== Argument check ==== */
    if ((channel >= SCIF_SYNC_CH_TOTAL) || (mode < SCIF_SYNC_MODE_W) || (mode > SCIF_SYNC_MODE_RW))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Terminate SCIF communication ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            SCIF_SYNC_Close(&SCIF0, mode);
        break;
        case DEVDRV_CH_1:
            SCIF_SYNC_Close(&SCIF1, mode);
        break;
        case DEVDRV_CH_2:
            SCIF_SYNC_Close(&SCIF2, mode);
        break;
        case DEVDRV_CH_3:
            SCIF_SYNC_Close(&SCIF3, mode);
        break;
        case DEVDRV_CH_4:
            SCIF_SYNC_Close(&SCIF4, mode);
        break;
        case DEVDRV_CH_5:
            SCIF_SYNC_Close(&SCIF5, mode);
        break;
        case DEVDRV_CH_6:
            SCIF_SYNC_Close(&SCIF6, mode);
        break;
        case DEVDRV_CH_7:
            SCIF_SYNC_Close(&SCIF7, mode);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_SCIF_SYNC_Write
* Description  : Transmits the data by using the SCIF of the channel specified 
*              : by the argument channel.
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
*              : uint8_t * buffer : Transmit data
*              : uint32_t byte    : Byte count of the data to be transmitted
* Return Value : DEVDRV_SUCCESS : Success of SCIF transmission
*              : DEVDRV_ERROR   : Failure of SCIF transmission
******************************************************************************/
int32_t R_SCIF_SYNC_Write(uint32_t channel, uint8_t * buffer, uint32_t byte)
{
    /* ==== Argument check ==== */
    if (channel >= SCIF_SYNC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== SCIF transmission ==== */
    if (byte > 0)
    {
        SCIF_SYNC_Transmit(channel, buffer, byte);
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_SCIF_SYNC_Read
* Description  : Received the data using the SCIF of the channel specified by 
*              : the argument channel.
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
*              : uint8_t * buffer : Receive data
*              : uint32_t byte    : Byte count of the data to be received
* Return Value : 0 or over      : Success of SCIF reception 
*              :                : (Byte count of the received data)
*              : DEVDRV_ERROR   : Failure of SCIF reception
******************************************************************************/
int32_t R_SCIF_SYNC_Read(uint32_t channel, uint8_t * buffer, uint32_t byte)
{
    int32_t ret;

    ret = 0;

    /* ==== Argument check ==== */
    if (channel >= SCIF_SYNC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== SCIF reception ==== */
    if (byte > 0)
    {
        ret = SCIF_SYNC_Receive(channel, buffer, byte);
    }

    return ret;
}

/******************************************************************************
* Function Name: R_SCIF_SYNC_TxiInterrupt
* Description  : Executes the processing to satisfy the conditions for the SCIF
*              : transmit FIFO data empty notification information of the 
*              : channel specified by the argument channel.
*              : The interrupt occurrence can be notified by calling this 
*              : function from the interrupt hander for the SICF transmit FIFO
*              : data empty interrupt.
*              : Execute this function to satisfy the conditions for the transmit
*              : FIFO data empty notification information by using the user-
*              : defined function Userdef_SCIFn_SYNC_SetTransmitEmpty (n=0 to 7).
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
* Return Value : DEVDRV_SUCCESS : Success of SCIF transmit FIFO data empty 
*              :                : interrupt notification
*              : DEVDRV_ERROR   : Failure of SCIF transmit FIFO data empty 
*              :                : interrupt notification
******************************************************************************/
int32_t R_SCIF_SYNC_TxiInterrupt(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= SCIF_SYNC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Set conditions for the transmit FIFO data empty ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_SCIF0_SYNC_SetTransmitEmpty();
        break;
        case DEVDRV_CH_1:
            Userdef_SCIF1_SYNC_SetTransmitEmpty();
        break;
        case DEVDRV_CH_2:
            Userdef_SCIF2_SYNC_SetTransmitEmpty();
        break;
        case DEVDRV_CH_3:
            Userdef_SCIF3_SYNC_SetTransmitEmpty();
        break;
        case DEVDRV_CH_4:
            Userdef_SCIF4_SYNC_SetTransmitEmpty();
        break;
        case DEVDRV_CH_5:
            Userdef_SCIF5_SYNC_SetTransmitEmpty();
        break;
        case DEVDRV_CH_6:
            Userdef_SCIF6_SYNC_SetTransmitEmpty();
        break;
        case DEVDRV_CH_7:
            Userdef_SCIF7_SYNC_SetTransmitEmpty();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_SCIF_SYNC_RxiInterrupt
* Description  : Executes the processing to satisfy the conditions for the SCIF  
*              : receive FIFO data full notification information of the channel
*              : specified that argument channel.
*              : The interrupt occurrence can be notified by calling this 
*              : function from interrupt hander for the SCIF receive FIFO data 
*              : full interrupt.
*              : Execute this function to satisfy the conditions for the receive
*              : FIFO data full notification information by using the user-
*              : defined function Userdef_SCIFn_SYNC_SetReceive (N=0 to 7).
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
* Return Value : DEVDRV_SUCCESS : Success of SCIF receive FIFO data full 
*              :                : interrupt notification
*              : DEVDRV_ERROR   : Failure of SCIF receive FIFO data full 
*              :                : interrupt notification
******************************************************************************/
int32_t R_SCIF_SYNC_RxiInterrupt(uint32_t channel)
{
    /* ==== Argument check ==== */
    if (channel >= SCIF_SYNC_CH_TOTAL)
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /* ==== Set conditions for the receive FIFO data full ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_SCIF0_SYNC_SetReceiveFull();
        break;
        case DEVDRV_CH_1:
            Userdef_SCIF1_SYNC_SetReceiveFull();
        break;
        case DEVDRV_CH_2:
            Userdef_SCIF2_SYNC_SetReceiveFull();
        break;
        case DEVDRV_CH_3:
            Userdef_SCIF3_SYNC_SetReceiveFull();
        break;
        case DEVDRV_CH_4:
            Userdef_SCIF4_SYNC_SetReceiveFull();
        break;
        case DEVDRV_CH_5:
            Userdef_SCIF5_SYNC_SetReceiveFull();
        break;
        case DEVDRV_CH_6:
            Userdef_SCIF6_SYNC_SetReceiveFull();
        break;
        case DEVDRV_CH_7:
            Userdef_SCIF7_SYNC_SetReceiveFull();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/*******************************************************************************
* Function Name: SCIF_SYNC_Open
* Description  : Enables the SCIF operation.
* Arguments    : struct st_scif * scif : Start address of SCIF register by channel
*              : uint32_t mode : Setting for transfer direction 
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
*******************************************************************************/
static void SCIF_SYNC_Open(volatile struct st_scif * scif, uint32_t mode)
{
    if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
    {
        /* SCFCR - FIFO control register
        b2 TFRST - Reset transmit FIFO data register : Enable reset operation */
        RZA_IO_RegWrite_16(&(scif->SCFCR), 1, SCIFn_SCFCR_TFRST_SHIFT, SCIFn_SCFCR_TFRST);

        /* SCFCR - FIFO control register
        b2 TFRST - Reset transmit FIFO data register : Disable reset operation */
        RZA_IO_RegWrite_16(&(scif->SCFCR), 0, SCIFn_SCFCR_TFRST_SHIFT, SCIFn_SCFCR_TFRST);

        /* SCSCR - Serial control register
        b5 TE - Transmit operation : Enabled */
        RZA_IO_RegWrite_16(&(scif->SCSCR), 1, SCIFn_SCSCR_TE_SHIFT, SCIFn_SCSCR_TE);
    }

    if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* Use reception */
    {
        /* SCFCR - FIFO control register
        b1 RFRST - Reset receive FIFO data register : Enable reset operation */
        RZA_IO_RegWrite_16(&(scif->SCFCR), 1, SCIFn_SCFCR_RFRST_SHIFT, SCIFn_SCFCR_RFRST);

        /* SCFSR - Serial status register
        b7 ER  - Receive error flag      : Cleared
        b4 BRK - Break detection flag    : Cleared
        b0 DR  - Receive data ready flag : Cleared */
        scif->SCFSR &= ~0x0091u;

        /* SCLSR - Line status register
        b0 ORER - Overrun error flag     : Cleared */
        scif->SCLSR &= ~0x0001u;

        /* SCFCR - FIFO control register
        b1 RFRST - Reset receive FIFO data register : Disable reset operation */
        RZA_IO_RegWrite_16(&(scif->SCFCR), 0, SCIFn_SCFCR_RFRST_SHIFT, SCIFn_SCFCR_RFRST);

        /* SCSCR - Serial control register
        b5 RE - Receive operation : Enabled */
        RZA_IO_RegWrite_16(&(scif->SCSCR), 1, SCIFn_SCSCR_RE_SHIFT, SCIFn_SCSCR_RE);
    }
}

/******************************************************************************
* Function Name: SCIF_SYNC_Close
* Description  : Disables the SCIF operation.
* Arguments    : struct st_scif * scif : Start address of SCIF register by channel
*              : uint32_t mode : Setting for transfer direction 
*              :               :   SCIF_SYNC_MODE_W  : Transmission
*              :               :   SCIF_SYNC_MODE_R  : Reception
*              :               :   SCIF_SYNC_MODE_RW : Transmission/reception
* Return Value : None
******************************************************************************/
static void SCIF_SYNC_Close(volatile struct st_scif * scif, uint32_t mode)
{
    if (SCIF_SYNC_MODE_W == (mode & SCIF_SYNC_MODE_W))  /* Use transmission */
    {
        /* SCSCR - Serial control register
        b5 TE - Transmit operation : Disabled */
        RZA_IO_RegWrite_16(&(scif->SCSCR), 0, SCIFn_SCSCR_TE_SHIFT, SCIFn_SCSCR_TE);
    }

    /* ==== Stop SCIF receive operation ==== */
    if (SCIF_SYNC_MODE_R == (mode & SCIF_SYNC_MODE_R))  /* User reception. */
    {
        /* SCSCR - Serial control register
        b5 RE - Receive operation : Disabled */
        RZA_IO_RegWrite_16(&(scif->SCSCR), 0, SCIFn_SCSCR_RE_SHIFT, SCIFn_SCSCR_RE);
    }
}

/******************************************************************************
* Function Name: SCIF_SYNC_Transmit
* Description  : Transmits the data using the SCIF of the channel specified by 
*              : the argument channel.
*              : When this function waits until the conditions for the transmit
*              : FIFO data empty notification information are satisfied by using
*              : the user-defined function Userdef_SCIFn_SYNC_WaitTransmitEmpty
*              : (n=0 to 7), the empty space can be secured in the SCIF transmit 
*              : FIFO buffer for the bytes set to the TTRG bit in the FIFO 
*              : control register (SCFCR). The data is written to the transmit
*              : FIFO buffer until the space becomes full.
*              : Then initializes the transmit FIFO data empty notification
*              : information by using the user-defined function 
*              : Userdef_SCIFn_SYNC_InitTransmitEmpty (n=0 to 7), and clears
*              : the TDFE and TEND bits in the serial status register (SCFSR). 
*              : The transmit operation is repeated until the data bytes to be 
*              : transmitted reaches to the byte count specified by the argument 
*              : byte. After the data has been transmitted, waits for the transmit 
*              : end by using the user-defined function 
*              : Userdef_SCIFn_SYNC_WaitTransmitEnd (n=0 to 7).
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
*              : void * buffer    : Transmit data
*              : uint32_t count   : Byte count of the data to be transmitted
* Return Value : none
******************************************************************************/
static void SCIF_SYNC_Transmit(uint32_t channel, uint8_t * buffer, uint32_t byte)
{
    volatile struct   st_scif * scif;
    uint16_t          ttrg_value;
    uint32_t          trigger_byte;
    uint32_t          write_byte;
    volatile uint16_t dummy_buf_16b;

    scif = SCIF_SYNC_GetRegAddr(channel);

    /* Obtain transmit FIFO data triggering number setting */
    ttrg_value = RZA_IO_RegRead_16(&(scif->SCFCR), SCIFn_SCFCR_TTRG_SHIFT, SCIFn_SCFCR_TTRG);
    trigger_byte = scif_sync_ttrg_table[ttrg_value];

    /* ==== Write for the specified counts ==== */
    while (byte > 0)
    {
        /* ==== Wait until transmit FIFO data empty conditions are satisfied ==== */
        switch (channel)
        {
            case DEVDRV_CH_0:
                Userdef_SCIF0_SYNC_WaitTransmitEmpty();
            break;
            case DEVDRV_CH_1:
                Userdef_SCIF1_SYNC_WaitTransmitEmpty();
            break;
            case DEVDRV_CH_2:
                Userdef_SCIF2_SYNC_WaitTransmitEmpty();
            break;
            case DEVDRV_CH_3:
                Userdef_SCIF3_SYNC_WaitTransmitEmpty();
            break;
            case DEVDRV_CH_4:
                Userdef_SCIF4_SYNC_WaitTransmitEmpty();
            break;
            case DEVDRV_CH_5:
                Userdef_SCIF5_SYNC_WaitTransmitEmpty();
            break;
            case DEVDRV_CH_6:
                Userdef_SCIF6_SYNC_WaitTransmitEmpty();
            break;
            case DEVDRV_CH_7:
                Userdef_SCIF7_SYNC_WaitTransmitEmpty();
            break;
            default:
                /* Do not reach here based on the assumption */
            break;
        }

        /* ==== Write for the empty space in the transmit buffer ==== */
        if (byte < trigger_byte)
        {
            write_byte = byte;
        }
        else
        {
            write_byte = trigger_byte;
        }
        byte -= write_byte;
        while (write_byte > 0)
        {
            scif->SCFTDR = *buffer;     /* Write */

            buffer++;
            write_byte--;
        }

        /* ==== Clear transmit FIFO data empty conditions ==== */
        switch (channel)
        {
            case DEVDRV_CH_0:
                Userdef_SCIF0_SYNC_InitTransmitEmpty();
            break;
            case DEVDRV_CH_1:
                Userdef_SCIF1_SYNC_InitTransmitEmpty();
            break;
            case DEVDRV_CH_2:
                Userdef_SCIF2_SYNC_InitTransmitEmpty();
            break;
            case DEVDRV_CH_3:
                Userdef_SCIF3_SYNC_InitTransmitEmpty();
            break;
            case DEVDRV_CH_4:
                Userdef_SCIF4_SYNC_InitTransmitEmpty();
            break;
            case DEVDRV_CH_5:
                Userdef_SCIF5_SYNC_InitTransmitEmpty();
            break;
            case DEVDRV_CH_6:
                Userdef_SCIF6_SYNC_InitTransmitEmpty();
            break;
            case DEVDRV_CH_7:
                Userdef_SCIF7_SYNC_InitTransmitEmpty();
            break;
            default:
                /* Do not reach here based on the assumption */
            break;
        }

        /* Clear TDFE and TEND flags */
        RZA_IO_RegWrite_16(&(scif->SCFSR), 0, SCIFn_SCFSR_TDFE_SHIFT, SCIFn_SCFSR_TDFE);
        RZA_IO_RegWrite_16(&(scif->SCFSR), 0, SCIFn_SCFSR_TEND_SHIFT, SCIFn_SCFSR_TEND);
        dummy_buf_16b = scif->SCFSR;    /* Dummy read */
    }

    /* ==== Wait for transmit end ==== */
    switch (channel)
    {
        case DEVDRV_CH_0:
            Userdef_SCIF0_SYNC_WaitTransmitEnd();
        break;
        case DEVDRV_CH_1:
            Userdef_SCIF1_SYNC_WaitTransmitEnd();
        break;
        case DEVDRV_CH_2:
            Userdef_SCIF2_SYNC_WaitTransmitEnd();
        break;
        case DEVDRV_CH_3:
            Userdef_SCIF3_SYNC_WaitTransmitEnd();
        break;
        case DEVDRV_CH_4:
            Userdef_SCIF4_SYNC_WaitTransmitEnd();
        break;
        case DEVDRV_CH_5:
            Userdef_SCIF5_SYNC_WaitTransmitEnd();
        break;
        case DEVDRV_CH_6:
            Userdef_SCIF6_SYNC_WaitTransmitEnd();
        break;
        case DEVDRV_CH_7:
            Userdef_SCIF7_SYNC_WaitTransmitEnd();
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return;
}

/******************************************************************************
* Function Name: SCIF_SYNC_Receive
* Description  : Receives the data using the SCIF of the channel specified by 
*              : the argument channel.
*              : Checks overrun error occurrence by using the user-defined
*              : function Userdef_SCIFn_SYNC_ReceiveOverrun (n=0 to7) and  
*              : executes the error processing. If no overrun error occurs, 
*              : waits until the conditions for the receive FIFO data full 
*              : notification information are satisfied by using the user-
*              : defined function Userdef_SCIFn_SYNC_WaitReceiveFull (n=0 to 7).
*              : Then the data is stored to the receive FIFO buffer for the 
*              : bytes set to the RTRG bit in the FIFO control register (SCFCR).
*              : Receives the data for the byte count and initializes the 
*              : receive FIFO data full notification information by using the
*              : user-defined function serdef_SCIFn_SYNC_InitReceiveFull
*              : (n=0 to 7), and then clears the RDF bit in the serial status 
*              : register (SCFSR). 
*              : The receive operation is repeated until the data bytes to be
*              : received reaches to the byte count specified by the argument
*              : byte. When the return value of Userdef_SCIFn_SYNC_WaitReceiveFul
*              : indicates DEVDRV_ERROR, this function receives the data stored 
*              : in the receive FIFO buffer to terminate the receive processing.
*              : If the reception has been successfully done, the bytes of the
*              : received data is set to the return value.
*              : The receive operation is suspended when an overrun error 
*              : occurs, and DEVDRV_ERROR is set to the return value.
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
*              : uint8_t * buffer : Receive data
*              : uint32_t byte    : Byte count of the data to be received
* Return Value : 0 or over      : Success of SCIF reception 
*              :                : (Byte count of the received data)
*              : DEVDRV_ERROR   : Failure of SCIF reception
******************************************************************************/
static int32_t SCIF_SYNC_Receive(uint32_t channel, uint8_t * buffer, uint32_t byte)
{
    volatile struct   st_scif * scif;
    uint16_t          rtrg_value;
    uint32_t          trigger_byte;
    uint32_t          less_rxtrg;
    uint32_t          read_byte;
    int32_t           byte_count;
    int32_t           userdef_ret;
    uint16_t          scfrdr_byte;
    int32_t           ret;
    volatile uint16_t dummy_buf_16b;

    ret = DEVDRV_SUCCESS;
    userdef_ret = DEVDRV_SUCCESS;
    byte_count = byte;

    scif = SCIF_SYNC_GetRegAddr(channel);

    /* Obtain receive FIFO data triggering number setting */
    rtrg_value = RZA_IO_RegRead_16(&(scif->SCFCR), SCIFn_SCFCR_RTRG_SHIFT, SCIFn_SCFCR_RTRG);
    trigger_byte = scif_sync_rtrg_table[rtrg_value];

    /* ==== Read for the specified counts ==== */
    while (byte > 0)
    {
        /* ==== Check overrun occurrence, execute error processing ==== */
        switch (channel)
        {
            case DEVDRV_CH_0:
                ret = Userdef_SCIF0_SYNC_ErrReceiveOverrun();
            break;
            case DEVDRV_CH_1:
                ret = Userdef_SCIF1_SYNC_ErrReceiveOverrun();
            break;
            case DEVDRV_CH_2:
                ret = Userdef_SCIF2_SYNC_ErrReceiveOverrun();
            break;
            case DEVDRV_CH_3:
                ret = Userdef_SCIF3_SYNC_ErrReceiveOverrun();
            break;
            case DEVDRV_CH_4:
                ret = Userdef_SCIF4_SYNC_ErrReceiveOverrun();
            break;
            case DEVDRV_CH_5:
                ret = Userdef_SCIF5_SYNC_ErrReceiveOverrun();
            break;
            case DEVDRV_CH_6:
                ret = Userdef_SCIF6_SYNC_ErrReceiveOverrun();
            break;
            case DEVDRV_CH_7:
                ret = Userdef_SCIF7_SYNC_ErrReceiveOverrun();
            break;
            default:
                /* Do not reach here based on the assumption */
            break;
        }

        if (DEVDRV_ERROR == ret)
        {
            break;
        }

        /* ---- When receiving the data less than the RTRG ---- */
        if (byte < trigger_byte)
        {
            less_rxtrg = 1;
            read_byte = byte;
        }
        /* ---- When receiving the data same as the RTRG or larger than RTRG ---- */
        else
        {
            less_rxtrg = 0;
            read_byte = trigger_byte;
        }

        /* ==== Wait until the receive FIFO data full conditions are satisfied === */
        switch (channel)
        {
            case DEVDRV_CH_0:
                userdef_ret = Userdef_SCIF0_SYNC_WaitReceiveFull(read_byte, less_rxtrg);
            break;
            case DEVDRV_CH_1:
                userdef_ret = Userdef_SCIF1_SYNC_WaitReceiveFull(read_byte, less_rxtrg);
            break;
            case DEVDRV_CH_2:
                userdef_ret = Userdef_SCIF2_SYNC_WaitReceiveFull(read_byte, less_rxtrg);
            break;
            case DEVDRV_CH_3:
                userdef_ret = Userdef_SCIF3_SYNC_WaitReceiveFull(read_byte, less_rxtrg);
            break;
            case DEVDRV_CH_4:
                userdef_ret = Userdef_SCIF4_SYNC_WaitReceiveFull(read_byte, less_rxtrg);
            break;
            case DEVDRV_CH_5:
                userdef_ret = Userdef_SCIF5_SYNC_WaitReceiveFull(read_byte, less_rxtrg);
            break;
            case DEVDRV_CH_6:
                userdef_ret = Userdef_SCIF6_SYNC_WaitReceiveFull(read_byte, less_rxtrg);
            break;
            case DEVDRV_CH_7:
                userdef_ret = Userdef_SCIF7_SYNC_WaitReceiveFull(read_byte, less_rxtrg);
            break;
            default:
                /* Do not reach here based on the assumption */
            break;
        }
        /* ---- When waiting for the receive buffer full conditions is suspended ---- */
        /* ---- without having byte count to be received                         ---- */
        if (DEVDRV_ERROR == userdef_ret)
        {
            scfrdr_byte = RZA_IO_RegRead_16(&(scif->SCFDR), SCIFn_SCFDR_R_SHIFT, SCIFn_SCFDR_R);
            read_byte = (uint32_t)scfrdr_byte;  /* Read the bytes received */
        }

        /* ==== Read the data in the receive buffer ==== */
        byte -= read_byte;
        while (read_byte > 0)
        {
            *buffer = scif->SCFRDR;     /* Read */

            buffer++;
            read_byte--;
        }

        if (DEVDRV_ERROR == userdef_ret)
        {
            break;  /* Reception is suspended */
        }

        /* ==== Clear receive FIFO data full conditions ==== */
        switch (channel)
        {
            case DEVDRV_CH_0:
                Userdef_SCIF0_SYNC_InitReceiveFull();
            break;
            case DEVDRV_CH_1:
                Userdef_SCIF1_SYNC_InitReceiveFull();
            break;
            case DEVDRV_CH_2:
                Userdef_SCIF2_SYNC_InitReceiveFull();
            break;
            case DEVDRV_CH_3:
                Userdef_SCIF3_SYNC_InitReceiveFull();
            break;
            case DEVDRV_CH_4:
                Userdef_SCIF4_SYNC_InitReceiveFull();
            break;
            case DEVDRV_CH_5:
                Userdef_SCIF5_SYNC_InitReceiveFull();
            break;
            case DEVDRV_CH_6:
                Userdef_SCIF6_SYNC_InitReceiveFull();
            break;
            case DEVDRV_CH_7:
                Userdef_SCIF7_SYNC_InitReceiveFull();
            break;
            default:
                /* Do not reach here based on the assumption */
            break;
        }

        /* Clear RDF flag */
        RZA_IO_RegWrite_16(&(scif->SCFSR), 0, SCIFn_SCFSR_RDF_SHIFT, SCIFn_SCFSR_RDF);
        dummy_buf_16b = scif->SCFSR;    /* Dummy read */
    }

    if (DEVDRV_SUCCESS == ret)
    {
        ret = byte_count - byte;
    }

    return ret;
}

/******************************************************************************
* Function Name: SCIF_SYNC_GetRegAddr
* Description  : Obtains the start address of the SCIF-related register of the 
*              : specified channel.
* Arguments    : uint32_t channel : SCIF channel (0 to 7)
* Return Value : struct st_scif * : Start address of SCIF register by channel
******************************************************************************/
static volatile struct st_scif * SCIF_SYNC_GetRegAddr(uint32_t channel)
{
    volatile struct st_scif * scif;

    switch (channel)
    {
        case DEVDRV_CH_0:
            scif = &SCIF0;
        break;
        case DEVDRV_CH_1:
            scif = &SCIF1;
        break;
        case DEVDRV_CH_2:
            scif = &SCIF2;
        break;
        case DEVDRV_CH_3:
            scif = &SCIF3;
        break;
        case DEVDRV_CH_4:
            scif = &SCIF4;
        break;
        case DEVDRV_CH_5:
            scif = &SCIF5;
        break;
        case DEVDRV_CH_6:
            scif = &SCIF6;
        break;
        case DEVDRV_CH_7:
            scif = &SCIF7;
        break;
        default:
            /* Do not reach here based on the assumption */
            scif = &SCIF0;
        break;
    }

    return scif;
}


/* End of File */

