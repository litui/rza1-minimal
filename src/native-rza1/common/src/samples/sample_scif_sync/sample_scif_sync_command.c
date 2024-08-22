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
* File Name   : sample_scif_sync_command.c
* $Rev: 1335 $
* $Date:: 2015-02-17 18:03:28 +0900#$
* Description : Serial Communication Interface with FIFO(SCIF) Clock synchronous 
*             : serial communication sample program
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_scif_sync.h"       /* SCIF SYNC Driver Header */
#include "command.h"
#include "sample_scif_sync.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define SCIF_SYNC_DATA_SIZE (1024)  /* Size of transmit/receive data           */
#define SCIF_SYNC_FIFO_SIZE (16)    /* Maximum data size transmitted/received  */
                                    /* at one time (Max data count is 16 bytes */
                                    /* in FIFO buffer) when transmission/      */
                                    /* reception is executed by using loop     */
                                    /* back processing.                        */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void    SCIF_SYNC_PORT_Init(void);
static void    SCIF_SYNC_InitMemory8bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment);
static int32_t SCIF_SYNC_VerifyMemory8bit(uint8_t * addr_1, uint8_t * addr_2, uint32_t count);

#ifdef __CC_ARM
    #pragma arm section zidata = "BSS_SCIF_SYNC_SAMPLE"
    static uint8_t scif_sync_trans_data[SCIF_SYNC_DATA_SIZE];   /* Transmit data buffer */
    static uint8_t scif_sync_receive_data[SCIF_SYNC_DATA_SIZE]; /* Receive data buffer  */
    #pragma arm section zidata
#endif

#ifdef __ICCARM__
    static uint8_t scif_sync_trans_data[SCIF_SYNC_DATA_SIZE] @ "BSS_SCIF_SYNC_SAMPLE";
    static uint8_t scif_sync_receive_data[SCIF_SYNC_DATA_SIZE] @ "BSS_SCIF_SYNC_SAMPLE";
#endif

#ifdef __GNUC__
    static uint8_t scif_sync_trans_data[SCIF_SYNC_DATA_SIZE] __attribute__ ((section ("BSS_SCIF_SYNC_SAMPLE")));
    static uint8_t scif_sync_receive_data[SCIF_SYNC_DATA_SIZE] __attribute__ ((section ("BSS_SCIF_SYNC_SAMPLE")));
#endif



/******************************************************************************
* Function Name: Sample_SCIF_SYNC_LoopBack
* Description  : In the sample code, this function set the bit rate to 250kbps, 
*              : and enables the loop-back testing to transmit/receive the data
*              : of 1KB allocated to the large-capacity on-chip RAM space using 
*              : the SCIF clock synchronous serial communication.
*              : For the transmit/receive operation, the data is transmitted in 
*              : the 16-byte units (data count in the SCIF transmit FIFO buffer), 
*              : and the data transmitted by the loop-back testing is received. 
*              : The received data is written to the large-capacity on-chip RAM
*              : space every 16 bytes (data count in the receive FIFO buffer).
*              : This transmit/receive operation is repeated until the data size
*              : reaches 1KB.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal. Not used in this function.
*              : char_t ** argv : Pointer to the command input from the terminal.
*              :                : Not used in this function.
* Return Value : COMMAND_SUCCESS : Success of SCIF clock synchronous serial
*              :                 : communication sample code processing
*              : COMMAND_ERROR   : Failure of SCIF clock synchronous serial 
*              :                 : communication sample code processing
******************************************************************************/
int32_t Sample_SCIF_SYNC_LoopBack(int32_t argc, char_t ** argv)
{
    int32_t         ret;
    uint32_t        channel;
    uint32_t        mode;
    uint32_t        byte;
    static uint32_t offset;

    printf("\n");

    /* ==== Initialize memory ==== */
    channel = DEVDRV_CH_3;
    mode    = SCIF_SYNC_MODE_RW;
    SCIF_SYNC_InitMemory8bit(scif_sync_trans_data, 0x00, SCIF_SYNC_DATA_SIZE, 1);   /* Generate transmit data    */
    SCIF_SYNC_InitMemory8bit(scif_sync_receive_data, 0x00, SCIF_SYNC_DATA_SIZE, 0); /* Clear receive data buffer */

    /* ==== Initialize SCIF ==== */
    R_SCIF_SYNC_Init(channel, mode);

    /* ==== Setting for PORT functions ==== */
    SCIF_SYNC_PORT_Init();

    /* ==== Enable SCIF functions ==== */
    R_SCIF_SYNC_Open(channel, mode);

    for (offset = 0; offset < SCIF_SYNC_DATA_SIZE; offset += SCIF_SYNC_FIFO_SIZE)
    {
        /* ---- The remaining transmit data count is the FIFO buffer size or more ---- */
        if ((SCIF_SYNC_DATA_SIZE - offset) >= SCIF_SYNC_FIFO_SIZE)
        {
            /* Specify the FIFO buffer size for the transmit/receive data count */
            byte = SCIF_SYNC_FIFO_SIZE;
        }
        /* ---- The remaining transmit data count is smaller than the FIFO buffer size ---- */
        else
        {
            /* Specify the remaining transmit data count for the transmit/receive data count */
            byte = SCIF_SYNC_DATA_SIZE - offset;
        }

        /* ==== Transmit data ==== */
        R_SCIF_SYNC_Write(channel, scif_sync_trans_data + offset, byte);

        /* ==== Receive data ==== */
        R_SCIF_SYNC_Read(channel, scif_sync_receive_data + offset, byte);
    }

    /* ==== Disable SCIF functions ==== */
    R_SCIF_SYNC_Close(channel, mode);

    printf("SCIF Write and Read complete.\n");
    printf("\n");

    /* ==== Verify ==== */
    ret = SCIF_SYNC_VerifyMemory8bit(scif_sync_trans_data, scif_sync_receive_data, SCIF_SYNC_DATA_SIZE);
    if (-1 == ret)
    {
        printf("error: Verify error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    printf("Verify complete.\n");
    printf("\n");

    return COMMAND_SUCCESS;
}

/******************************************************************************
* Function Name: SCIF_SYNC_PORT_Init
* Description  : In the sample code, the PORT function setting is not required
*              : because the transmission/reception is executed by using the 
*              : SCIF channel 3 loop-back test function. However, the PORT 
*              : initialization such to set the P3_4 pin, the P3_5 pin, and 
*              : the P3_6 pin to SCK3, TxD3, and RxD3 respectively is required 
*              : in order to check the waveform of the serial communication. 
* Arguments    : none
* Return Value : none
******************************************************************************/
static void SCIF_SYNC_PORT_Init(void)
{
    /* ---- P3_4 : SCK3 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC34_SHIFT,   GPIO_PIBC3_PIBC34);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC34_SHIFT,   GPIO_PBDC3_PBDC34);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM34_SHIFT,       GPIO_PM3_PM34);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC34_SHIFT,     GPIO_PMC3_PMC34);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC34_SHIFT,   GPIO_PIPC3_PIPC34);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC34_SHIFT,   GPIO_PBDC3_PBDC34);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC34_SHIFT,     GPIO_PFC3_PFC34);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE34_SHIFT,   GPIO_PFCE3_PFCE34);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 1, GPIO_PFCAE3_PFCAE34_SHIFT, GPIO_PFCAE3_PFCAE34);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC34_SHIFT,   GPIO_PIPC3_PIPC34);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC34_SHIFT,     GPIO_PMC3_PMC34);

    /* ---- P3_5 : TxD3 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC35_SHIFT,   GPIO_PIBC3_PIBC35);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC35_SHIFT,   GPIO_PBDC3_PBDC35);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM35_SHIFT,       GPIO_PM3_PM35);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC35_SHIFT,     GPIO_PMC3_PMC35);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC35_SHIFT,   GPIO_PIPC3_PIPC35);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC35_SHIFT,   GPIO_PBDC3_PBDC35);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC35_SHIFT,     GPIO_PFC3_PFC35);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE35_SHIFT,   GPIO_PFCE3_PFCE35);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 1, GPIO_PFCAE3_PFCAE35_SHIFT, GPIO_PFCAE3_PFCAE35);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC35_SHIFT,   GPIO_PIPC3_PIPC35);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC35_SHIFT,     GPIO_PMC3_PMC35);

    /* ---- P3_6 : RxD3 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC36_SHIFT,   GPIO_PIBC3_PIBC36);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC36_SHIFT,   GPIO_PBDC3_PBDC36);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM36_SHIFT,       GPIO_PM3_PM36);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC36_SHIFT,     GPIO_PMC3_PMC36);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC36_SHIFT,   GPIO_PIPC3_PIPC36);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC36_SHIFT,   GPIO_PBDC3_PBDC36);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC36_SHIFT,     GPIO_PFC3_PFC36);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE36_SHIFT,   GPIO_PFCE3_PFCE36);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 1, GPIO_PFCAE3_PFCAE36_SHIFT, GPIO_PFCAE3_PFCAE36);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC36_SHIFT,   GPIO_PIPC3_PIPC36);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC36_SHIFT,     GPIO_PMC3_PMC36);
}

/******************************************************************************
* Function Name: SCIF_SYNC_InitMemory8bit
* Description  : Writes the value specified by the argument data in the 8-bit 
*              : units from the address specified by the argument *addr for 
*              : the times specified by the argument count. 
*              : When the value other than 0 is set for the argument increment, 
*              : increment the argument data every time writing to the memory.
* Arguments    : uint8_t * addr    : Start address of the memory for writing
*              : uint8_t data      : Data to be written
*              : uint32_t count    : Writing count (in the 8-bit units)
*              : uint8_t increment : Increment setting for write data
*              :                       0 : Write the data specified by the 
*              :                         : argument data
*              :            Other than 0 : Write the data incremented from 
*              :                         : the argument data
* Return Value : none
******************************************************************************/
static void SCIF_SYNC_InitMemory8bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment)
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

/******************************************************************************
* Function Name: SCIF_SYNC_VerifyMemory8bit
* Description  : Compares the area with the size specified by the argument 
*              : count starting with the address specified by the argument 
*              : *addr_1 and the area starting with the address specified by 
*              : the argument *addr_2. Reads it in the8-bit units, and sets 
*              : the compare result for the return value.
* Arguments    : uint8_t * addr_1 : Start address of the memory for comparison
*              : uint8_t * addr_2 : Start address of the memory for comparison
*              : uint32_t count   : Comparison count(in the 8-bit units)
* Return Value :  0 : Verify OK
*              : -1 : Verify Error
******************************************************************************/
static int32_t SCIF_SYNC_VerifyMemory8bit(uint8_t * addr_1, uint8_t * addr_2, uint32_t count)
{
    uint32_t offset;
    int32_t  ret;

    ret = 0;

    for (offset = 0; offset < count; offset++)
    {
        if (*(addr_1 + offset) != *(addr_2 + offset))
        {
            ret = -1;   /* Verify error */
            break;
        }
    }

    return ret;
}


/* End of File */

