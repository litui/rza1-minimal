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
* File Name   : sample_riic_command.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : RIIC sample program
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_riic.h"            /* RIIC Driver Header */
#include "sample_riic.h"
#include "command.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define SAMPLE_RIIC_DEVICE_CODE (0xA0)  /* EEPROM device code         : b'1010 */
#define SAMPLE_RIIC_DEVICE_ADDR (0x00)  /* EEPROM device address code : b'000  */

#define SAMPLE_RIIC_DATA_SIZE   (16)    /* Data receive RAM size */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void    RIIC_PORT_Init(void);
static void    RIIC_InitMemory8bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment);
static int32_t RIIC_VerifyMemory8bit(uint8_t * addr_1, uint8_t * addr_2, uint32_t count);

#ifdef __CC_ARM
#pragma arm section zidata = "BSS_RIIC_SAMPLE"
/* Transmit data buffer */
static uint8_t  riic_trans_data[SAMPLE_RIIC_DATA_SIZE];
/* Receive data buffer  */
static uint8_t  riic_receive_data[SAMPLE_RIIC_DATA_SIZE];
#pragma arm section zidata
#endif
#ifdef __ICCARM__
/* Transmit data buffer */
static uint8_t  riic_trans_data[SAMPLE_RIIC_DATA_SIZE]   @ "BSS_RIIC_SAMPLE";
/* Receive data buffer  */
static uint8_t  riic_receive_data[SAMPLE_RIIC_DATA_SIZE] @ "BSS_RIIC_SAMPLE";
#endif
#ifdef __GNUC__
/* Transmit data buffer */
static uint8_t  riic_trans_data[SAMPLE_RIIC_DATA_SIZE]   __attribute__ ((section ("BSS_RIIC_SAMPLE")));
/* Receive data buffer  */
static uint8_t  riic_receive_data[SAMPLE_RIIC_DATA_SIZE] __attribute__ ((section ("BSS_RIIC_SAMPLE")));
#endif


/******************************************************************************
* Function Name: Sample_RIIC_EepAccess
* Description  : In the sample code, the transfer speed is set to 100kbps, and 
*              : the data of 64 bytes allocated to the write space is written 
*              : to the EEPROM. Then waits until writing has been completed 
*              : inside the EEPROM using the acknowledge polling processing.
*              : Reads the data of 64 bytes from the EEPROM and writes it into 
*              : the large-capacity on-chip RAM space.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal. Not used in this function.
*              : char_t ** argv : Pointer to the command input from the terminal.
*              :                : Not used in this function.
* Return Value : COMMAND_SUCCESS : Success of RIIC sample code processing
*              : COMMAND_ERROR   : Failure of RIIC sample code processing
******************************************************************************/
int32_t Sample_RIIC_EepAccess(int32_t argc, char_t ** argv)
{
    int32_t  ret;
    uint32_t channel;

    printf("\n");

    /* ==== Memory initialization ==== */
    channel = DEVDRV_CH_3;
    RIIC_InitMemory8bit(riic_trans_data, 0x00, SAMPLE_RIIC_DATA_SIZE, 1);   /* Generate transmit data    */
    RIIC_InitMemory8bit(riic_receive_data, 0x00, SAMPLE_RIIC_DATA_SIZE, 0); /* Clear receive data buffer */

    /* ==== PORT function setting ==== */
    RIIC_PORT_Init();

    /* ==== RIIC initialization ==== */
    ret = R_RIIC_Init(channel);
    if (DEVDRV_ERROR == ret)
    {
        printf("error: RIIC Initialize error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    printf("RIIC Initialize complete.\n");
    printf("\n");

    /* ==== Write to EEPROM ==== */
    ret = Sample_RIIC_EepWrite(channel,                 /* RIIC channel        */
                               SAMPLE_RIIC_DEVICE_CODE, /* Device code         */
                               SAMPLE_RIIC_DEVICE_ADDR, /* Device address code */
                               0x0000,                  /* Write start address */
                               SAMPLE_RIIC_DATA_SIZE,   /* Write data size     */
                               riic_trans_data);        /* Write data          */
    /* When the NACK is received at the time of data transmission */
    if (DEVDRV_ERROR_RIIC_NACK == ret)
    {
        printf("error: EEPROM write error (NACK).\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Wait until writing has been completed inside the EEPROM ==== */
    Sample_RIIC_EepAckPolling(channel, SAMPLE_RIIC_DEVICE_CODE, SAMPLE_RIIC_DEVICE_ADDR);

    /* ==== Read from EEPROM ==== */
    ret = Sample_RIIC_EepRead(channel,                  /* RIIC channel        */
                              SAMPLE_RIIC_DEVICE_CODE,  /* Device code         */
                              SAMPLE_RIIC_DEVICE_ADDR,  /* Device address code */
                              0x0000,                   /* Read start address  */
                              SAMPLE_RIIC_DATA_SIZE,    /* Read data size      */
                              riic_receive_data);       /* Read data           */
    /* When the NACK is received at the time data transmission */
    if (DEVDRV_ERROR_RIIC_NACK == ret)
    {
        printf("error: EEPROM read error (NACK).\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    printf("EEPROM write and EEPROM read complete.\n");
    printf("\n");

    /* ==== Verify ==== */
    ret = RIIC_VerifyMemory8bit(riic_trans_data, riic_receive_data, SAMPLE_RIIC_DATA_SIZE);
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
* Function Name: RIIC_PORT_Init
* Description  : Initializes the PORT used in the RIIC transmission/reception.
*              : In the sample code, the PORT is initialized to set the P1_6 and
*              : P1_7 pins to RIIC3SCL function and RIIC3SDA function respectively.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void RIIC_PORT_Init(void)
{
    /* ---- P1_6 : SCL3 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC1,  0,  GPIO_PIBC1_PIBC16_SHIFT,   GPIO_PIBC1_PIBC16);
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1,  GPIO_PBDC1_PBDC16_SHIFT,   GPIO_PBDC1_PBDC16);
    RZA_IO_RegWrite_16(&GPIO.PM1,    1,  GPIO_PM1_PM16_SHIFT,       GPIO_PM1_PM16);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   0,  GPIO_PMC1_PMC16_SHIFT,     GPIO_PMC1_PMC16);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1,  GPIO_PIPC1_PIPC16_SHIFT,   GPIO_PIPC1_PIPC16);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1,  GPIO_PBDC1_PBDC16_SHIFT,   GPIO_PBDC1_PBDC16);
    RZA_IO_RegWrite_16(&GPIO.PFC1,   0,  GPIO_PFC1_PFC16_SHIFT,     GPIO_PFC1_PFC16);
    RZA_IO_RegWrite_16(&GPIO.PFCE1,  0,  GPIO_PFCE1_PFCE16_SHIFT,   GPIO_PFCE1_PFCE16);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0,  GPIO_PFCAE1_PFCAE16_SHIFT, GPIO_PFCAE1_PFCAE16);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1,  GPIO_PIPC1_PIPC16_SHIFT,   GPIO_PIPC1_PIPC16);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   1,  GPIO_PMC1_PMC16_SHIFT,     GPIO_PMC1_PMC16);

    /* ---- P1_7 : SDA3 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC1,  0,  GPIO_PIBC1_PIBC17_SHIFT,   GPIO_PIBC1_PIBC17);
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1,  GPIO_PBDC1_PBDC17_SHIFT,   GPIO_PBDC1_PBDC17);
    RZA_IO_RegWrite_16(&GPIO.PM1,    1,  GPIO_PM1_PM17_SHIFT,       GPIO_PM1_PM17);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   0,  GPIO_PMC1_PMC17_SHIFT,     GPIO_PMC1_PMC17);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1,  GPIO_PIPC1_PIPC17_SHIFT,   GPIO_PIPC1_PIPC17);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  1,  GPIO_PBDC1_PBDC17_SHIFT,   GPIO_PBDC1_PBDC17);
    RZA_IO_RegWrite_16(&GPIO.PFC1,   0,  GPIO_PFC1_PFC17_SHIFT,     GPIO_PFC1_PFC17);
    RZA_IO_RegWrite_16(&GPIO.PFCE1,  0,  GPIO_PFCE1_PFCE17_SHIFT,   GPIO_PFCE1_PFCE17);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0,  GPIO_PFCAE1_PFCAE17_SHIFT, GPIO_PFCAE1_PFCAE17);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1,  GPIO_PIPC1_PIPC17_SHIFT,   GPIO_PIPC1_PIPC17);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   1,  GPIO_PMC1_PMC17_SHIFT,     GPIO_PMC1_PMC17);
}

/******************************************************************************
* Function Name: RIIC_InitMemory8bit
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
static void RIIC_InitMemory8bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment)
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
* Function Name: RIIC_VerifyMemory8bit
* Description  : Compares the area with the size specified by the argument 
*              : count starting with the address specified by the argument 
*              : *addr_1 and the area starting with the address specified by 
*              : the argument *addr_2. Reads it in the8-bit units, and sets 
*              : the compare result for the return value.
* Arguments    : uint8_t * addr_1 : Start address of the memory for comparison
*              : uint8_t * addr_2 : Start address of the memory for comparison
*              : uint32_t count   : Comparison count (in the 8-bit units)
* Return Value :  0 : Verify OK
*              : -1 : Verify Error
******************************************************************************/
static int32_t RIIC_VerifyMemory8bit(uint8_t * addr_1, uint8_t * addr_2, uint32_t count)
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

