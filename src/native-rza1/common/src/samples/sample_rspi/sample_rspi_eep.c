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
* File Name   : sample_rspi_eep.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Renesas Serial Peripheral Interface(RSPI) sample program - EEPROM read/write
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_rspi.h"            /* RSPI Driver Header */
#include "sample_rspi.h"


/******************************************************************************
Typedef definitions
******************************************************************************/
#define EEP_nBYTE_READ              (0)
#define EEP_01BYTE_READ             (1)

/* EEPROM commands */
#define EEP_CMD_WRITE_ENABLE        (0x06u)
#define EEP_CMD_WRITE_DISABLE       (0x04u)
#define EEP_CMD_READ_STATUS         (0x05u)
#define EEP_CMD_WRITE_STATUS        (0x01u)
#define EEP_CMD_READ_ARRAY          (0x03u)
#define EEP_CMD_WRITE_ARRAY         (0x02u)

/* EEPROM status bits */
#define EEP_STATUS_UNPROTECT        (0x00u)
#define EEP_STATUS_PROTECT          (0x0Cu)
#define EEP_STATUS_BUSY             (0x01u)

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
static void    RSPI_EepWriteStatus_8bit(uint32_t channel, uint8_t status);
static uint8_t RSPI_EepReadStatus_8bit(uint32_t channel);
static void    RSPI_EepWriteEnable_8bit(uint32_t channel);
static void    RSPI_EepBusyWait_8bit(uint32_t channel);
static void    RSPI_EepWrite(uint32_t channel, void * ope_data, uint32_t ope_count, 
                    void * w_data, uint32_t w_data_count);
static void    RSPI_EepRead(uint32_t channel, void * ope_data, uint32_t ope_count, 
                    void * r_data, uint32_t r_data_count, uint32_t r_byte_flg);


/******************************************************************************
* Function Name: Sample_RSPI_EepControlProtect_8bit
* Description  : Sets or releases the protection to/from the EEPROM using the 
*              : argument req.
*              : In the sample code, the protection for entire EEPROM area is set 
*              : or released by writing "B'11" or "B'00" to the BP0 and BP1 bits 
*              : of the EEPROM status register.
*              : The RSPI transfer is executed using the data of 8-bit length.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
*              : uint32_t req     : Request for protection release
*              :                  :   RSPI_EEP_REQ_PROTECT   : Request for protection
*              :                  :   RSPI_EEP_REQ_UNPROTECT : Request for protection 
*              :                  :                          : release
* Return Value : None
******************************************************************************/
void Sample_RSPI_EepControlProtect_8bit(uint32_t channel, uint32_t req)
{
    uint8_t status;

    if (SAMPLE_RSPI_REQ_UNPROTECT == req)
    {
        status = EEP_STATUS_UNPROTECT;  /* Release protection for the entire area */
    }
    else
    {
        status = EEP_STATUS_PROTECT;    /* Protection for the entire area         */
    }
    RSPI_EepWriteStatus_8bit(channel, status);
}

/******************************************************************************
* Function Name: Sample_RSPI_EepWrite_8bit
* Description  : Writes the memory contents specified by the argument buffer to 
*              : the EEPROM for the byte count specified by the argument byte 
*              : using the memory address specified by the argument addr.
*              : In the sample code, the sample function RSPI_EepWriteEnable_8bit
*              : is called to enable the write operation. This function transmits
*              : the data with the EEPROM page write function using the EEPROM 
*              : instruction code "WRITE(H'02)", and waits until the write 
*              : operation is completed. 
*              : The values should be specified for the arguments addr and byte 
*              : to make the area specified the said arguments to have the same 
*              : range as the page specified in the EEPROM.
*              : The RSPI transfer is executed using the data of 8-bit length.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
*              : uint32_t addr    : Memory address
*              : uint8_t * buffer : Store buffer for the data written to the
*              :                  : EEPROM
*              : uint32_t byte    : Byte count to be written to the EEPROM
* Return Value : None
******************************************************************************/
void Sample_RSPI_EepWrite_8bit(uint32_t channel, uint32_t addr, uint8_t * buffer, uint32_t byte)
{
    uint8_t cmd[3];

    cmd[0] = EEP_CMD_WRITE_ARRAY;
    cmd[1] = (uint8_t)((addr >>  8) & 0xFF);
    cmd[2] = (uint8_t)( addr        & 0xFF);
    RSPI_EepWriteEnable_8bit(channel);
    RSPI_EepWrite(channel, cmd, 3, buffer, byte);
    RSPI_EepBusyWait_8bit(channel);
}

/******************************************************************************
* Function Name: Sample_RSPI_EepRead_8bit
* Description  : Reads the data from the EEPROM for the byte count specified by
*              : the argument byte using the memory address specified by the 
*              : argument addr, and stores it to the area specified by the 
*              : argument buffer.
*              : In the sample code, the data is read by using the EEPROM
*              : instruction code "READ(H'03)".
*              : The RSPI transfer is executed using the data of 8-bit length.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
*              : uint32_t addr    : Memory address
*              : uint8_t * buffer : Store buffer for the data read from the 
*              :                  : EEPROM
*              : uint32_t byte    : Byte count to be read from the EEPROM
* Return Value : None
******************************************************************************/
void Sample_RSPI_EepRead_8bit(uint32_t channel, uint32_t addr, uint8_t * buffer, uint32_t byte)
{
    uint8_t cmd[3];

    cmd[0] = EEP_CMD_READ_ARRAY;
    cmd[1] = (uint8_t)((addr >>  8) & 0xff);
    cmd[2] = (uint8_t)( addr        & 0xff);
    RSPI_EepRead(channel, cmd, 3, buffer, byte, EEP_nBYTE_READ);
}

/******************************************************************************
* Function Name: RSPI_EepWriteStatus_8bit
* Description  : Writes the value specified by the argument status to the EEPROM
*              : status register.
*              : In the sample code, the sample function RSPI_EepWriteEnable_8bit 
*              : is called to enable the write operation. Writes the status by 
*              : using the EEPROM instruction code "WRSR(H'01)" and waits until
*              : the write operation is completed.
*              : The RSPI transfer is executed using the data of 8-bit length.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
*              : uint8_t status   : Value of the status to be written
* Return Value : None
******************************************************************************/
static void RSPI_EepWriteStatus_8bit(uint32_t channel, uint8_t status)
{
    uint8_t cmd[2];

    cmd[0] = EEP_CMD_WRITE_STATUS;
    cmd[1] = status;

    RSPI_EepWriteEnable_8bit(channel);
    RSPI_EepWrite(channel, cmd, 2, NULL, 0);
    RSPI_EepBusyWait_8bit(channel);
}

/******************************************************************************
* Function Name: RSPI_EepReadStatus_8bit
* Description  : Read the EEPROM status register.
*              : In the sample code, the status is read by using the EEPROM
*              : instruction code "RDSR(H'05)". Obtains the read value as a 
*              : return value.
*              : The RSPI transfer is executed using the data of 8-bit length.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : uint8_t buffer   : Value read from the status register
******************************************************************************/
static uint8_t RSPI_EepReadStatus_8bit(uint32_t channel)
{
    uint8_t buffer;
    uint8_t cmd[1];

    cmd[0] = EEP_CMD_READ_STATUS;
    RSPI_EepRead(channel, cmd, 1, &buffer, 1, EEP_01BYTE_READ);

    return buffer;
}

/******************************************************************************
* Function Name: RSPI_EepWriteEnable_8bit
* Description  : Enables the EEPROM write operation.
*              : In the sample code, the write operation is enabled by using the
*              : EEPROM instruction code "WREN(H'06)" to set the "WEL" bit.
*              : The RSPI transfer is executed using the data of 8-bit length.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : None
******************************************************************************/
static void RSPI_EepWriteEnable_8bit(uint32_t channel)
{
    uint8_t cmd[1];

    cmd[0] = EEP_CMD_WRITE_ENABLE;
    RSPI_EepWrite(channel, cmd, 1, NULL, 0);
}

/******************************************************************************
* Function Name: RSPI_EepBusyWait_8bit
* Description  : Reads the EEPROM status register and waits until the "WIP" bit
*              : is cleared to "0". In the sample code, the RSPI transfer is 
*              : executed using the data of 8-bit length.
* Arguments    : uint32_t channel : RSPI channel (0 to 4)
* Return Value : None
******************************************************************************/
static void RSPI_EepBusyWait_8bit(uint32_t channel)
{
    /* Loop when the EEPROM is in the write operation */
    while ((RSPI_EepReadStatus_8bit(channel) & EEP_STATUS_BUSY) != 0)
    {
        /* Wait */
    }
}

/******************************************************************************
* Function Name: RSPI_EepWrite
* Description  : Resets the RSPI transmit and receive buffers to enable the RSPI 
*              : functions.
*              : Transmits the instruction code specified by the argument 
*              : ope_data for the counts specified by the argument ope_count. 
*              : After the instruction code has been transmitted, the transmit 
*              : data specified by the argument w_data is transmitted for the 
*              : counts specified by the argument w_data_count. Waits until the 
*              : RSPI transmission is completed, and negates the SSL signal 
*              : by disabling the RSPI functions.
* Arguments    : uint32_t channel   : RSPI channel (0 to 4)
*              : void * ope_data    : Instruction code
*              :                    : (Stores instruction code and memory address)
*              : uint32_t ope_count : Transmission counts of instruction code
*              : void * w_data      : Transmit data
*              : int w_data_count   : Transmission counts of data 
* Return Value : None
******************************************************************************/
static void RSPI_EepWrite(uint32_t channel, void * ope_data, uint32_t ope_count, void * w_data, uint32_t w_data_count)
{
    /* ==== Reset buffer ==== */
    R_RSPI_ResetBuffer(channel);

    /* ==== Start transfer ==== */
    R_RSPI_Open(channel);

    /* ==== Write instruction code and data ==== */
    if (ope_count > 0)
    {
        /* ---- RSPI transmission of instruction code ---- */
        R_RSPI_Write(channel, ope_data, ope_count, RSPI_TXTRG_07_BYTE);
    }
    if (w_data_count > 0)
    {
        /* ---- RSPI transmission of data ---- */
        R_RSPI_Write(channel, w_data, w_data_count, RSPI_TXTRG_CURRENT);
    }

    if (ope_count > 0 || w_data_count > 0)
    {
        /* ---- Wait for the end of transmission ---- */
        R_RSPI_WaitTransmitEnd(channel);
    }

    /* ==== Communication completion (SSL negation) ==== */
    R_RSPI_Close(channel);
}

/******************************************************************************
* Function Name: RSPI_EepRead
* Description  : Recives the data from the EEPROM using the RSPI specified by
*              : the argument channel.
*              : Resets the RSPI transmit and receive buffers to enable the RSPI 
*              : functions. Transmits the instruction code specified by the 
*              : argument ope_data for the counts specified by the argument  
*              : ope_count. Then resets the RSPI buffer and receives the data for 
*              : the counts specified by the argument r_data_count, which is  
*              : stored to the area specified by the argument r_data. Waits until  
*              : the RSPI transmission is completed, and negates the SSL signal  
*              : by disabling the RSPI functions. If the receive data is smaller 
*              : than the byte count specified as the RXTRG bit such as the EEPROM 
*              : status register (1 byte) is read, EEP_01BYTE_READ should be set 
*              : to r_byte_flg.
* Arguments    : uint32_t channel    : RSPI channel (0 to 4)
*              : void * ope_data     : Instruction code
*              :                     : (Stores instruction code and memory address)
*              : uint32_t ope_count  : Transmission counts of instruction code
*              : void * r_data       : Receive data
*              : int r_data_count    : Reception counts of data
*              : uint32_t r_byte_flg : Select flag for received byte count
*              :                     :   EEP_nBYTE_READ  : When receiving normal data
*              :                     :   EEP_01BYTE_READ : When receiving data 
*              :                     :                   : smaller than the bytes 
*              :                     :                   : specified by the RXTRG bit
* Return Value : None
******************************************************************************/
static void RSPI_EepRead(uint32_t channel, void * ope_data, uint32_t ope_count, 
                         void * r_data, uint32_t r_data_count, uint32_t r_byte_flg)
{
    uint8_t rxtrg;

    /* ==== Reset buffer ==== */
    R_RSPI_ResetBuffer(channel);

    /* ==== Start transfer ==== */
    R_RSPI_Open(channel);

    /* ==== Write instruction code ==== */
    if (ope_count > 0)
    {
        /* ---- RSPI transmission ---- */
        R_RSPI_Write(channel, ope_data, ope_count, RSPI_TXTRG_07_BYTE);

        /* ---- Wait for transmission end ---- */
        R_RSPI_WaitTransmitEnd(channel);
    }

    /* ==== Reset bufffer ==== */
    R_RSPI_ResetBuffer(channel);

    /* ==== Read data ==== */
    if (r_data_count > 0)
    {
        /* ---- RSPI reception ---- */
        if (EEP_nBYTE_READ == r_byte_flg)
        {
            rxtrg = RSPI_RXTRG_CURRENT;
        }
        else
        {
            rxtrg = RSPI_RXTRG_01_BYTE;
        }
        R_RSPI_Read(channel, r_data, r_data_count, rxtrg);

        /* ---- Wait for the end of dummy data transmission executed to receive the RSPI ---- */
        R_RSPI_WaitTransmitEnd(channel);
    }

    /* ==== Communication completion (SSL negation) ==== */
    R_RSPI_Close(channel);
}


/* End of File */

