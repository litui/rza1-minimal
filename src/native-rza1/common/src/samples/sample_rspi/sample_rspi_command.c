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
* File Name   : sample_rspi_command.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Renesas Serial Peripheral Interface(RSPI) sample program
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_rspi.h"            /* RSPI Driver Header */
#include "command.h"
#include "sample_rspi.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define RSPI_EEP_TOP_ADDRESS    (0)         /* Start address of EEPROM */
#define RSPI_EEP_PAGE_SIZE      (128)       /* Page size of EEPROM     */
#define RSPI_EEP_DATA_SIZE      (0x4000)    /* EEPROM size (16KB)      */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void    RSPI_PORT_Init(void);
static void    RSPI_InitMemory8bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment);
static int32_t RSPI_VerifyMemory8bit(uint8_t * addr_1, uint8_t * addr_2, uint32_t count);

#ifdef __CC_ARM
#pragma arm section zidata = "BSS_RSPI_SAMPLE"
/* Transmit data buffer */
static uint8_t rspi_trans_data[RSPI_EEP_DATA_SIZE];
/* Receive data buffer  */
static uint8_t rspi_receive_data[RSPI_EEP_DATA_SIZE];
#pragma arm section zidata
#endif
#ifdef __ICCARM__
/* Transmit data buffer */
static uint8_t rspi_trans_data[RSPI_EEP_DATA_SIZE]   @ "BSS_RSPI_SAMPLE";
/* Receive data buffer  */
static uint8_t rspi_receive_data[RSPI_EEP_DATA_SIZE] @ "BSS_RSPI_SAMPLE";
#endif
#ifdef __GNUC__
/* Transmit data buffer */
static uint8_t rspi_trans_data[RSPI_EEP_DATA_SIZE]   __attribute__ ((section ("BSS_RSPI_SAMPLE")));
/* Receive data buffer  */
static uint8_t rspi_receive_data[RSPI_EEP_DATA_SIZE] __attribute__ ((section ("BSS_RSPI_SAMPLE")));
#endif


/******************************************************************************
* Function Name: Sample_RSPI_EepAccess
* Description  : In the sample code, the bit rate is set to 2.78 Mbps and the 
*              : 16 KB of data which has been allocated to the large-capacity 
*              : on-chip RAM space is written to the EEPROM. Then the data is 
*              : read from the EEPROM and written to the large-capacity on-chip 
*              : RAM space.
* Arguments    : int32_t   argc : Command arguments input from the terminal. 
*              :                : Not used in this function.
*              : char_t ** argv : Pointer to the command input from the terminal.
*              :                : Not used in this function.
* Return Value : COMMAND_SUCCESS : Success of RSPI sample code processing
*              : COMMAND_ERROR   : Failure of RSPI sample code processing
******************************************************************************/
int32_t Sample_RSPI_EepAccess(int32_t argc, char_t ** argv)
{
    int32_t         ret;
    uint32_t        channel;
    static uint32_t addr;

    printf("\n");

    /* ==== Initialize memory ==== */
    channel = DEVDRV_CH_4;
    RSPI_InitMemory8bit(rspi_trans_data, 0x00, RSPI_EEP_DATA_SIZE, 1);      /* Generate transmit data    */
    RSPI_InitMemory8bit(rspi_receive_data, 0x00, RSPI_EEP_DATA_SIZE, 0);    /* Clear receive data buffer */

    /* ==== Set PORT functions ==== */
    RSPI_PORT_Init();

    /* ==== Initialize RSPI ==== */
    ret = R_RSPI_Init(channel);
    if (DEVDRV_ERROR == ret)
    {
        printf("error: RSPI Initialize error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    printf("RSPI Initialize complete.\n");
    printf("\n");

    /* ==== Release protection of EEPROM ==== */
    Sample_RSPI_EepControlProtect_8bit(channel, SAMPLE_RSPI_REQ_UNPROTECT);

    /* ==== Write data ==== */
    for (addr = RSPI_EEP_TOP_ADDRESS;
         addr < (RSPI_EEP_TOP_ADDRESS + RSPI_EEP_DATA_SIZE);
         addr += RSPI_EEP_PAGE_SIZE)
    {
        Sample_RSPI_EepWrite_8bit(channel, addr, rspi_trans_data + addr, RSPI_EEP_PAGE_SIZE);
    }

    /* ==== Read data ==== */
    Sample_RSPI_EepRead_8bit(channel, RSPI_EEP_TOP_ADDRESS, rspi_receive_data, RSPI_EEP_DATA_SIZE);

    /* ==== Protection of EEPROM ==== */
    Sample_RSPI_EepControlProtect_8bit(channel, SAMPLE_RSPI_REQ_PROTECT);

    printf("RSPI Write and Read complete.\n");
    printf("\n");

    /* ==== Verify ==== */
    ret = RSPI_VerifyMemory8bit(rspi_trans_data, rspi_receive_data, RSPI_EEP_DATA_SIZE);
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
* Function Name: RSPI_PORT_Init
* Description  : Initializes the PORT used in RSPI transmission/reception. 
*              : In the sample code, the PORT is initialized to allow the P4_0 pin, 
*              : the P4_1 pin, the P4_2 pin and the P4_3 pin for the RSPCK4 function, 
*              : the SSL40 function, the MOSI4 function and the MISO4 function 
*              : respectively.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void RSPI_PORT_Init(void)
{
    /* ---- P4_0 : RSPCK4 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC40_SHIFT,   GPIO_PIBC4_PIBC40);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC40_SHIFT,   GPIO_PBDC4_PBDC40);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM40_SHIFT,       GPIO_PM4_PM40);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC40_SHIFT,     GPIO_PMC4_PMC40);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC40_SHIFT,   GPIO_PIPC4_PIPC40);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  1, GPIO_PBDC4_PBDC40_SHIFT,   GPIO_PBDC4_PBDC40);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC40_SHIFT,     GPIO_PFC4_PFC40);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE40_SHIFT,   GPIO_PFCE4_PFCE40);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE40_SHIFT, GPIO_PFCAE4_PFCAE40);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC40_SHIFT,   GPIO_PIPC4_PIPC40);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC40_SHIFT,     GPIO_PMC4_PMC40);

    /* ---- P4_1 : SSL40 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC41_SHIFT,   GPIO_PIBC4_PIBC41);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC41_SHIFT,   GPIO_PBDC4_PBDC41);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM41_SHIFT,       GPIO_PM4_PM41);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC41_SHIFT,     GPIO_PMC4_PMC41);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC41_SHIFT,   GPIO_PIPC4_PIPC41);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  1, GPIO_PBDC4_PBDC41_SHIFT,   GPIO_PBDC4_PBDC41);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC41_SHIFT,     GPIO_PFC4_PFC41);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE41_SHIFT,   GPIO_PFCE4_PFCE41);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE41_SHIFT, GPIO_PFCAE4_PFCAE41);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC41_SHIFT,   GPIO_PIPC4_PIPC41);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC41_SHIFT,     GPIO_PMC4_PMC41);

    /* ---- P4_2 : MOSI4 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC42_SHIFT,   GPIO_PIBC4_PIBC42);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC42_SHIFT,   GPIO_PBDC4_PBDC42);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM42_SHIFT,       GPIO_PM4_PM42);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC42_SHIFT,     GPIO_PMC4_PMC42);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC42_SHIFT,   GPIO_PIPC4_PIPC42);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  1, GPIO_PBDC4_PBDC42_SHIFT,   GPIO_PBDC4_PBDC42);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC42_SHIFT,     GPIO_PFC4_PFC42);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE42_SHIFT,   GPIO_PFCE4_PFCE42);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE42_SHIFT, GPIO_PFCAE4_PFCAE42);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC42_SHIFT,   GPIO_PIPC4_PIPC42);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC42_SHIFT,     GPIO_PMC4_PMC42);

    /* ---- P4_3 : MISO4 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC43_SHIFT,   GPIO_PIBC4_PIBC43);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC43_SHIFT,   GPIO_PBDC4_PBDC43);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM43_SHIFT,       GPIO_PM4_PM43);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC43_SHIFT,     GPIO_PMC4_PMC43);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC43_SHIFT,   GPIO_PIPC4_PIPC43);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  1, GPIO_PBDC4_PBDC43_SHIFT,   GPIO_PBDC4_PBDC43);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC43_SHIFT,     GPIO_PFC4_PFC43);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE43_SHIFT,   GPIO_PFCE4_PFCE43);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 1, GPIO_PFCAE4_PFCAE43_SHIFT, GPIO_PFCAE4_PFCAE43);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC43_SHIFT,   GPIO_PIPC4_PIPC43);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC43_SHIFT,     GPIO_PMC4_PMC43);
}

/******************************************************************************
* Function Name: RSPI_InitMemory8bit
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
static void RSPI_InitMemory8bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment)
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
* Function Name: RSPI_VerifyMemory8bit
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
static int32_t RSPI_VerifyMemory8bit(uint8_t * addr_1, uint8_t * addr_2, uint32_t count)
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

