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
* File Name   : sample_riic_eep.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : RIIC sample program - EEPROM read/write
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
/* I/O Register root header */
#include "iodefine.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_riic.h"            /* RIIC Driver Header */
#include "sample_riic.h"
/* RIIC Driver Header */
#include "devdrv_riic.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define SAMPLE_RIIC_RWCODE_W        (0)     /* IIC data write code */
#define SAMPLE_RIIC_RWCODE_R        (1)     /* IIC data read code  */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
volatile struct st_riic * RIIC_GetRegAddr(uint32_t channel);


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name: Sample_RIIC_EepWrite
* Description  : Writes the memory specified by the argument *w_buffer for the
*              : byte counts specified by the argument w_byte to the EEPROM 
*              : indicated by the device code specified by the argument d_code, 
*              : the device address code specified by the argument d_adr, and 
*              : the memory address specified by the w_adr using the RIIC of 
*              : the channel specified by the argument channel.
*              : In the sample code, the data is transmitted in the channel 2
*              : RIIC master transmit operation with the EEPROM page write
*              : function.
* Arguments    : uint32_t channel   : RIIC channel (0 to 3)
*              : uint8_t d_code     : Device code
*              : uint8_t d_adr      : Device address code (3 bits)
*              : uint16_t w_adr     : Memory address
*              : uint32_t w_byte    : Byte counts to be written to EEPROM
*              : uint8_t * w_buffer : Store buffer of the data written to EEPROM
* Return Value : DEVDRV_SUCCESS         : Success of RIIC transmit processing
*              : DEVDRV_ERROR           : Failure of RIIC transmit processing
*              : DEVDRV_ERROR_RIIC_NACK : Failure of RIIC transmit processing 
*              :                        : due to NACK reception
******************************************************************************/
int32_t Sample_RIIC_EepWrite(uint32_t channel, uint8_t d_code, uint8_t d_adr, 
                             uint16_t w_adr, uint32_t w_byte, const uint8_t * w_buffer)
{
	int32_t  ret;
	uint8_t  buf[18];
	uint8_t  count;
	uint32_t delay = 200000;
    uint16_t w_saddr;
    uint8_t  maddr[2];

	volatile struct st_riic * priic;

	priic = RIIC_GetRegAddr(channel);

    /* ==== Set slave address and memory address ==== */
    /* 1byte address */
	buf[0] = (uint8_t)(d_code | ((d_adr & 0x7) << 1) | ((w_adr & 0x700) >> 7) | SAMPLE_RIIC_RWCODE_W);;
	buf[1] = (uint8_t)(w_adr  & 0x00ff);

	for (count=2; count < (w_byte + 2); count++)
	{
		buf[count] = (*w_buffer++);
	}

	/* Is the channel's bus free? */
	while (0x00008 == (priic->RIICnCR2.UINT32 & 0x0008))
	{
		__asm__("nop");
	}

	R_RIIC_SendCond(channel, RIIC_TX_MODE_START);

	ret = R_RIIC_Write(channel, buf, (2 + w_byte));

	R_RIIC_SendCond(channel, RIIC_TX_MODE_STOP);
	R_RIIC_DetectStop(channel);

	/* Add delay to ensure IIC device has completed internal writes */
	while (delay--)
	{
		__asm__("nop");
	}

	return (ret);
}

/******************************************************************************
* Function Name: Sample_RIIC_EepRead
* Description  : Reads the data in the EEPROM to the device code specified by
*              : the argument d_code, the device address code specified by the 
*              : argument d_adr, and the memory address specified by the argument
*              : r_adr using the RIIC of the channel specified by the argument
*              : channel. Stores the data for the byte counts specified by the
*              : argument r_byte to the area indicated by the argument r_buffer.
*              : In the sample code, the data is read from the EEPROM by the 
*              : sequential read function and received by the channel 2 RIIC
*              : master receive operation.
* Arguments    : uint32_t channel   : RIIC channel (0 to 3)
*              : uint8_t d_code     : Device code
*              : uint8_t d_adr      : Device address code (3 bits)
*              : uint16_t r_adr     : Memory address
*              : uint32_t r_byte    : Byte counts to be read from EEPROM
*              : uint8_t * r_buffer : Store buffer of the data read from EEPROM
* Return Value : DEVDRV_SUCCESS         : Success of RIIC receive processing
*              : DEVDRV_ERROR           : Failure of RIIC receive processing
*              : DEVDRV_ERROR_RIIC_NACK : Failure of RIIC receive processing 
*              :                        : due to NACK reception
******************************************************************************/
int32_t Sample_RIIC_EepRead(uint32_t channel, uint8_t d_code, uint8_t d_adr, 
                            uint16_t r_adr, uint32_t r_byte, uint8_t * r_buffer)
{
	volatile struct st_riic * priic;

	int32_t   ret;
	uint8_t   pw_buffer[3];
	uint8_t  dummy_data;

	priic = RIIC_GetRegAddr(channel);

	pw_buffer[0] = (uint8_t)(d_code | ((d_adr & 0x7) << 1) | ((r_adr & 0x700) >> 7) | SAMPLE_RIIC_RWCODE_W);
	pw_buffer[1] = (uint8_t)(r_adr & 0x00ff);
	pw_buffer[2] = (uint8_t)(d_code | ((d_adr & 0x7) << 1) | ((r_adr & 0x700) >> 7) | SAMPLE_RIIC_RWCODE_R);

	/* Is the bus free? */
	while (0x00008 == (priic->RIICnCR2.UINT32 & 0x0008))
	{
		__asm__("nop");
	}

	R_RIIC_SendCond(channel, RIIC_TX_MODE_START);

	ret = R_RIIC_Write(channel, pw_buffer, 2);

	if (DEVDRV_SUCCESS == ret)
	{
		R_RIIC_SendCond(channel, RIIC_TX_MODE_RESTART);
		ret = R_RIIC_SingleWrite(channel, pw_buffer[2], RIIC_TEND_WAIT_RECEIVE);

		if (DEVDRV_SUCCESS == ret)
		{
			R_RIIC_Read(channel, r_buffer, r_byte);
		}
		else
		{
			R_RIIC_SendCond(channel, RIIC_TX_MODE_STOP);
			R_RIIC_SingleRead(channel, &dummy_data);
			R_RIIC_DetectStop(channel);
		}
	}

	return (ret);
}

/******************************************************************************
* Function Name: Sample_RIIC_EepAckPolling
* Description  : Waits until the data has been written to the EEPROM indicated
*              : by the device code specified by the argument d_code and the device
*              : address code specified by the argument d_adr with the EEPROM 
*              : acknowledge polling function using the RIIC of the channel
*              : specified by the argument channel.
*              : In the sample code, the processing is repeated until the ACK is 
*              : received from the EEPROM using the channel 2 RIIC master 
*              : transmit operation.
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
*              : uint8_t d_code   : Device code
*              : uint8_t d_adr    : Device address code (3 bits)
* Return Value : none
******************************************************************************/
void Sample_RIIC_EepAckPolling(uint32_t channel, uint8_t d_code, uint8_t d_adr)
{
    int32_t  ret;
    uint16_t w_saddr;

    /* ==== Generate slave address ==== */
    w_saddr = (uint16_t)(d_code | ((d_adr & 0x7) << 1) | SAMPLE_RIIC_RWCODE_W);

    /* ==== Start condition issuance request ==== */
    R_RIIC_SendCond(channel, RIIC_TX_MODE_START);

    /* ==== Wait for ACK reception ==== */
    while (1)
    {
        ret = R_RIIC_WriteSlaveAddr(channel, w_saddr, RIIC_TEND_WAIT_TRANSMIT, 0);

        /* ---- NACK : In the process of writing the data inside the EEPROM ---- */
        if (DEVDRV_ERROR_RIIC_NACK == ret)
        {
            /* Clear NACK flag */
            R_RIIC_ClearNack(channel);

            /* Restart condition issuance request */
            R_RIIC_SendCond(channel, RIIC_TX_MODE_RESTART);
        }
        /* ---- ACK : Not in the process of writing the data inside the EEPROM ---- */
        else
        {
            break;
        }
    }

    /* ==== Stop condition issuance request ==== */
    R_RIIC_SendCond(channel, RIIC_TX_MODE_STOP);
    R_RIIC_DetectStop(channel);
}

/* End of File */

