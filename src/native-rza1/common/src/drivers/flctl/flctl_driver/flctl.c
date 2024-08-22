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
* File Name   : flctl.c
* $Rev: 1537 $
* $Date:: 2015-09-11 17:21:13 +0900#$
* Description : INTC driver
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_flctl.h"
#include "iodefine.h"
#include "main.h"
#include "rza_io_regrw.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    RESET_MODE,
    ERASE_BLOCK_MODE,
    READ_CMD_ACCESS_MODE,
    WRITE_CMD_ACCESS_MODE,
    READ_ID_MODE
} access_mode_t;


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ====  Prototype function ==== */
static void io_set_flctl( access_mode_t mode, int32_t sctr, int32_t num);
static void flctl_read_sector(int32_t addr, int32_t data_size, uint32_t * data );


/* ==== Global variable ==== */


/*******************************************************************************
* Function Name: R_FLCTL_Init
* Description  : Initialize NAND flash memory controller.
*              : This function initializes the setting necessary to use
*              : NAND flash memory controller.
* Argument     : void
* Return Value : void
* Note         : For boot mode 0, data bus port function is selected.
*              : But, for boot mode 1 to 3, in/out port function is selected.
*******************************************************************************/
int32_t R_FLCTL_Init( void )
{
	Userdef_FLCTL_Init();

	return 0;
}

/*******************************************************************************
* Function Name: R_FLCTL_Reset
* Description  : Issue the reset command
*              : This function issues the reset command, and initialize the NAND
*              : flash memory.
* Argument     : void
* Return Value : 0 and over -   succeeded to reset
*              : -1         -   failed to reset
*******************************************************************************/
int32_t R_FLCTL_Reset( void )
{
    /* ==== Check that there is no transfer ongoing ==== */
    if ( FLCTL.FLTRCR != 0 )
    {
        return -1;
    }

    /* ==== Set FLCTL ==== */
    io_set_flctl( RESET_MODE, 0, 0);

    /* ==== Start command transfer ==== */
    FLCTL.FLTRCR = 0x01;

    /* ==== Wait for complete of transfer ==== */
    while ( RZA_IO_RegRead_8(&FLCTL.FLTRCR, FLCTL_FLTRCR_TREND_SHIFT, FLCTL_FLTRCR_TREND) == 0 )
    {
        /* prohibit stopping by force */
    }

    /* ==== completion transaction ==== */
    FLCTL.FLTRCR  = 0x00;              /* Stop transfer */
    FLCTL.FLCMNCR = 0x00100001ul;      /* chip enable --> negate */

    return 0;
}

/*******************************************************************************
* Function Name: R_FLCTL_Erase
* Description  : Block erase
*              : This function erases the NAND flash memory block which includes
*              : the sectors specified.
*              : Read the status and if failed to erase then returns error.
* Argument     : sctr -   Sector number
*              :          (bit-2 and over:row address, bit1,0:sector number)
* Return Value : 0 and over -   succeeded to erase
*              : under 0    -   failed to erase
*******************************************************************************/
int32_t R_FLCTL_Erase( int32_t sctr )
{
    /* ==== Check that there is no transfer ongoing ==== */
    if ( FLCTL.FLTRCR != 0 )
    {
        return -1;
    }

    /* ==== Set FLCTL ==== */
    io_set_flctl( ERASE_BLOCK_MODE, sctr, 0);

    /* ==== Start command transfer ==== */
    FLCTL.FLTRCR = 0x01;

    /* ==== Wait for complete of transfer ==== */
    while ( RZA_IO_RegRead_8(&FLCTL.FLTRCR, FLCTL_FLTRCR_TREND_SHIFT, FLCTL_FLTRCR_TREND) == 0 )
    {
        /* prohibit stopping by force */
    }

    /* ==== completion transaction ==== */
    FLCTL.FLTRCR  = 0x00;              /* Stop transfer */
    FLCTL.FLCMNCR = 0x00100001ul;      /* chip enable --> negate */

    /* ==== Check the status ==== */
    if ( 1 == RZA_IO_RegRead_32(&FLCTL.FLINTDMACR, FLCTL_FLINTDMACR_STERB_SHIFT, FLCTL_FLINTDMACR_STERB) )
    {
        return -1;                     /* failed to write/erase */
    }

    return 0;
}

/*******************************************************************************
* Function Name: nd_read_sector
* Description  : Read sector(no ECC error correction)
*              : This function reads the specified sectors of NAND flash memory.
*              : There is no ECC error detection and correction.
*              : Also, control code areas are read without separation.
* Argument     : sctr -   Sector address to read
*              : num  -   Number of sectors to read
*              : data -   Buffer address to store the data
* Return Value : 0 and over -   succeeded to read
*              : under 0    -   failed to read
* Note         : To use FIFO on command access mode, this module control program area
*              : and transfer data area need to be located on the internal RAM.
*******************************************************************************/
int32_t R_FLCTL_Read( int32_t sctr, int32_t num, uint32_t * data)
{
    uint32_t    addr;
    int32_t    total_size;

    /* ==== Check the parameter ==== */
    if ( ((sctr & 0x3) + num) > SECTOR_OF_PAGE )
    {
        return -1;                     /* Access over a page is not allowed */
    }

    /* ==== Check that there is no transfer ongoing ==== */
    if ( FLCTL.FLTRCR != 0 )
    {
        return -1;
    }


   addr = sctr * SECTOR_SZ_DATA_AND_EXT;
   total_size = (num * SECTOR_SZ_DATA_AND_EXT);

   while(total_size > 0 ){

       /* Page Read Commad Issue(128 Byte) */
       if (total_size > 128)
       {
            flctl_read_sector(addr, 128, data);
            addr += 128;
            total_size -= 128;
            data += 128/(sizeof(uint32_t));
       }

       /* Page Read Commad Issue(64 Byte) */
       else
       {
            flctl_read_sector(addr, total_size, data);
            addr += total_size;
            total_size -= total_size;
            data += total_size/(sizeof(uint32_t));
       }
    };


    return 0;
}

/*******************************************************************************
* Function Name: nd_write_sector
* Description  : Write sector(without adding ECC)
*              : This function writes to specified sectors of NAND flash memory.
*              : There is no ECC addition.
*              : Also, control code areas are written without separation.
* Argument     : sctr -   Sector address to write
*              : num  -   Number of sectors to write
*              : data -   Buffer address where the data to write is stored
* Return Value : 0 and over -   succeeded to write
*              : under 0    -   failed to write
* Note         : To use FIFO on command access mode, this module control program area
*              : and transfer data area need to be located on the internal RAM.
*******************************************************************************/
int32_t R_FLCTL_Write( int32_t sctr, int32_t num, uint32_t * data)
{
    int32_t     loop;

    /* ==== Check the parameter ==== */
    if ( ((sctr & 0x3) + num) > SECTOR_OF_PAGE )
    {
        return -1;                     /* Access over a page is not allowed */
    }

    /* ==== Check that there is no transfer ongoing ==== */
    if ( FLCTL.FLTRCR != 0 )
    {
        return -1;
    }

    /* ==== Set FLCTL ==== */
    io_set_flctl( WRITE_CMD_ACCESS_MODE, sctr, num );

    /* ==== Start command transfer ==== */
    FLCTL.FLTRCR = 0x01;

    /* ---- write the data ---- */
    for ( loop = 0; loop < (num * (SECTOR_SZ_DATA_AND_EXT / sizeof(uint32_t))); loop++ )
    {
        while ( RZA_IO_RegRead_32(&FLCTL.FLDTCNTR, FLCTL_FLDTCNTR_DTFLW_SHIFT, FLCTL_FLDTCNTR_DTFLW) < 1 )
        {
            /* wait */
        }
        FLCTL.FLDTFIFO = *data++;
    }

    /* ==== Wait for complete of transfer ==== */
    while ( RZA_IO_RegRead_8(&FLCTL.FLTRCR, FLCTL_FLTRCR_TREND_SHIFT, FLCTL_FLTRCR_TREND) == 0 )
    {
        /* prohibit stopping by force */
    }

    /* ==== completion transaction ==== */
    FLCTL.FLTRCR  = 0x00;              /* Stop transfer */
    FLCTL.FLCMNCR = 0x00100001ul;      /* chip enable --> negate */

    /* ==== Check the error status ==== */
    if ( 1 == RZA_IO_RegRead_32(&FLCTL.FLINTDMACR, FLCTL_FLINTDMACR_STERB_SHIFT, FLCTL_FLINTDMACR_STERB) )
    {
        return -1;                     /* failed to write/erase */
    }

    return 0;
}

/*******************************************************************************
* Function Name: R_FLCTL_Read_Id
* Description  : Issue the read id command
* Argument     : data -   Buffer address to store the id data
* Return Value : 0 and over -   succeeded to command
*              : under 0    -   failed to command
*******************************************************************************/
int32_t R_FLCTL_Read_Id( uint8_t *data)
{
    int32_t     loop;
    uint32_t    read_data;

    /* ==== Check that there is no transfer ongoing ==== */
    if ( FLCTL.FLTRCR != 0 )
    {
        return -1;
    }

    /* ==== Set FLCTL ==== */
    io_set_flctl( READ_ID_MODE, 0, 5 );

    /* ==== Start command transfer ==== */
    FLCTL.FLTRCR = 0x01;

    /* ---- read the data ---- */
    for ( loop = 0; loop < 2; loop++ )
    {
        while ( RZA_IO_RegRead_32(&FLCTL.FLDTCNTR, FLCTL_FLDTCNTR_DTFLW_SHIFT, FLCTL_FLDTCNTR_DTFLW) < 1 )
        {
            /* wait */
        }
        read_data = FLCTL.FLDTFIFO;

        *data++ = (read_data >> 24) & 0xff;
        *data++ = (read_data >> 16) & 0xff;
        *data++ = (read_data >> 8) & 0xff;
        *data++ = read_data & 0xff;
    }

    /* ==== Wait for complete of transfer ==== */
    while ( RZA_IO_RegRead_8(&FLCTL.FLTRCR, FLCTL_FLTRCR_TREND_SHIFT, FLCTL_FLTRCR_TREND) == 0 )
    {
        /* prohibit stopping by force */
    }

    /* ==== completion transaction ==== */
    FLCTL.FLTRCR  = 0x00;              /* Stop transfer */
    FLCTL.FLCMNCR = 0x00100001ul;      /* chip enable --> negate */

    return 0;
}

/*******************************************************************************
* Function Name: io_set_flctl
* Description  : NAND flash memory controller register setting.
*              : This function sets the FLCTL register to specified the mode.
* Argument     : mode -   Access mode to execute
*              : sctr -   Sector number to access
*              : num  -   Number of sectors or data to access
* Return Value : void
*******************************************************************************/
static void io_set_flctl( access_mode_t mode, int32_t sctr, int32_t num )
{
    uint32_t    row1;
    uint32_t    row2;
    uint32_t    sc;
    uint32_t    col;

    switch ( mode )
    {
        /* **** Setting of command access mode (without data) **** */
        case RESET_MODE:            /* Reset command */
        case ERASE_BLOCK_MODE:      /* Block erase command */
            /* ==== Common control register(FLCMNCR) ==== */
            FLCTL.FLCMNCR = 0x00140209ul;      /* Select the large size NAND flash memory */
                                               /* FCLK = P0/2                          */
                                               /* Command access mode                  */
                                               /* Address,data -> 2FCLK cycle output   */
                                               /* Chip negate --> assert               */

            /* ==== Interrupt DMA control register(FLINTDMACR) ==== */
            FLCTL.FLINTDMACR = 0x00000000ul;   /* Disable interrupt                    */
                                               /* Clear the interrupt status           */
            /* ==== Data counter register(FLDTCNTR) ==== */
            FLCTL.FLDTCNTR = 0x00000000ul;     /* No data transfer                     */

            /* ==== Command control register(FLCMDCR)   ==== */
            if ( RESET_MODE == mode )
            {
                RZA_IO_RegWrite_32(&FLCTL.FLCMNCR, 0, FLCTL_FLCMNCR_SNAND_SHIFT, FLCTL_FLCMNCR_SNAND);
                                               /* Do not execute the 2nd command(SNAND=0) */
                FLCTL.FLCMDCR = 0x00010000ul;  /* Execute the 1st command stage        */
                                               /* Do not issue the address             */
                                               /* Do not execute the 2nd command stage */
            }                                  /* Do not read the status               */
            else
            {
                FLCTL.FLCMDCR = 0x013B0000ul;  /* Execute the 1st command stage        */
                                               /* Issue the 3 bytes of address         */
                                               /* Data stage is write direction(uses FLDATAR) */
                                               /* Execute the 2nd command stage        */
            }                                  /* Read the status                      */
            /* ==== Command code register(FLCMCDR) ==== */
            if ( RESET_MODE == mode )
            {
                FLCTL.FLCMCDR = 0x000000FFul;  /* 1st command -> 0xFF                  */
            }
            else
            {
                FLCTL.FLCMCDR = 0x0000D060ul;  /* 1st command -> 0x60, 2nd command -> 0xD0 */
            }
            /* ==== Address register(FLADR) ==== */
            if ( ERASE_BLOCK_MODE == mode )
            {
                FLCTL.FLADR = sctr >> 2;            /* Set the row address into lower 3 bytes   */
            }
        break;

        /* **** Setting of command access mode (with data) **** */
        case WRITE_CMD_ACCESS_MODE: /* Page program command */
            /* ==== Common control register(FLCMNCR) ==== */
            FLCTL.FLCMNCR = 0x00140209ul;      /* Select the large size NAND flash memory */
                                               /* FCLK = P0/2                          */
                                               /* Command access mode                  */
                                               /* Address,data -> 2FCLK cycle output   */
                                               /* Chip negate --> assert               */

            /* ==== Interrupt DMA control register(FLINTDMACR) ==== */
            FLCTL.FLINTDMACR = 0x000C0000ul;   /* Clear the FIFO (Execute before changing FIFO direction)  */
                                               /* Disable interrupt                    */
                                               /* Clear the interrupt status           */
            FLCTL.FLINTDMACR &= 0xFFF3FFFFul;  /* Stop the FIFO clear                  */

            /* ==== Address register2, Address register(FLADR2,FLADR) ==== */
            row1 = (sctr >> 18) & 0x000000FFul;     /* sctr b25 - b18: row address(upper)   */
            row2 = (sctr >>  2) & 0x0000FFFFul;     /*      b17 - b02: row address(middle and lower) */
            sc   = (sctr      ) & 0x00000003ul;     /*      b01 - b00: sector address(0 to 4) */
            col  = sc * SECTOR_SZ_DATA_AND_EXT;
            FLCTL.FLADR2 = row1;
            FLCTL.FLADR  = (row2 << 16) | col;
                                                     
            /* ==== Data counter register(FLDTCNTR) ==== */
            FLCTL.FLDTCNTR = (num * SECTOR_SZ_DATA_AND_EXT) & 0x00000FFFul;
                                                    /* Max 2048 + 64 bytes  */
            
            /* ==== Command control register(FLCMDCR)   ==== */
            FLCTL.FLCMDCR = 0x83330000ul;      /* Execute the 1st command stage                */
                                               /* Issue the 5 bytes of address                 */
                                               /* Data stage is write direction(uses FLDTFIFO) */
                                               /* Execute the 2nd command stage                */
                                               /* Read the status                              */

            /* ==== Command code register(FLCMCDR) ==== */
            FLCTL.FLCMCDR = 0x00001080ul;  /* 1st command -> 0x80, 2nd command -> 0x10 */
        break;

        /* **** Setting of command access mode (with id/status data) **** */
        case READ_ID_MODE:          /* READ ID command */
            /* ==== Common control register(FLCMNCR) ==== */
            FLCTL.FLCMNCR = 0x00100209ul;      /* Do not execute the 2nd command(SNAND=0) */
                                               /* FCLK = P0/2                          */
                                               /* Command access mode                  */
                                               /* Address,data -> 2FCLK cycle output   */
                                               /* Chip negate --> assert               */

            /* ==== Interrupt DMA control register(FLINTDMACR) ==== */
            FLCTL.FLINTDMACR = 0x000C0000ul;   /* Clear the FIFO (Execute before changing FIFO direction)  */
                                               /* Disable interrupt                    */
                                               /* Clear the interrupt status           */
            FLCTL.FLINTDMACR &= 0xFFF3FFFFul;  /* Stop the FIFO clear                  */

            /* ==== Address register(FLADR) ==== */
            FLCTL.FLADR  = 0x00ul;                  /* Set the address into 1 bytes   */

            /* ==== Data counter register(FLDTCNTR) ==== */
            FLCTL.FLDTCNTR = num;

            /* ==== Command control register(FLCMDCR)   ==== */
            FLCTL.FLCMDCR = 0x02110000ul;      /* Execute the 1st command stage        */
                                               /* Issue the 1 bytes of address         */
                                               /* Data stage is read direction(uses FLDTFIFO) */
                                               /* Do not execute the 2nd command stage */

            /* ==== Command code register(FLCMCDR) ==== */
            FLCTL.FLCMCDR = 0x00000090ul;      /* 1st command -> 0x90                  */
        break;

        default:
            /* do nothing */
        break;
    }
}



/*******************************************************************************
* Function Name: flctl_read_sector
* Description  : NAND flash memory controller register setting.
*              : This function sets the FLCTL register to specified the mode.
* Argument     : addr            - NAND Flash Address
*              : data_size       - Read data size
*              : uint32_t * data - Read Buffer Address
* Return Value : void
*******************************************************************************/
static void flctl_read_sector(int32_t addr, int32_t data_size, uint32_t * data )
{
    int32_t     loop;
    uint32_t    addr_1st;
    uint32_t    addr_2nd;
    uint32_t    addr_3rd;
    uint32_t    addr_4th;
    uint32_t    addr_5th;
    uint32_t    colum_addr;
    uint32_t    page_no;
    uint32_t    block_no;

    /* ==== Common control register(FLCMNCR) ==== */
    FLCTL.FLCMNCR = 0x00140209ul;      /* Select the large size NAND flash memory */
                                       /* FCLK = P0/2                             */
                                       /* Command access mode                     */
                                       /* Address,data -> 2FCLK cycle output      */
                                       /* Chip negate --> assert                  */

    /* ==== Interrupt DMA control register(FLINTDMACR) ==== */
    FLCTL.FLINTDMACR = 0x000C0000ul;   /* Clear the FIFO (Execute before changing FIFO direction)  */
                                       /* Disable interrupt                    */
                                       /* Clear the interrupt status           */
    FLCTL.FLINTDMACR &= 0xFFF3FFFFul;  /* Stop the FIFO clear                  */

    /* ==== Address register2, Address register(FLADR2,FLADR) ==== */
    colum_addr = addr % PAGE_SZ_DATA_AND_EXT;
    page_no = addr / PAGE_SZ_DATA_AND_EXT ;
    block_no  = page_no / PAGE_OF_BLOCK;
    addr_1st =  colum_addr        & 0x000000FFul;   /*  A7 -  A0 */
    addr_2nd = (colum_addr >> 8 ) & 0x0000000Ful;   /* A11 -  A8 */

    addr_3rd = ( (block_no & 0x3) << 6) | (page_no & 0x3F); // A19 - A12 (page address)
    addr_4th = (block_no >> 2) & 0x000000FFul;     /* A27 - A20 */
    addr_5th = (block_no >> 10) & 0x000000FFul;    /* A2x - A28 */

    FLCTL.FLADR2 = addr_5th;
    FLCTL.FLADR  = (addr_4th << 24) | (addr_3rd << 16) | (addr_2nd << 8) | addr_1st;


    FLCTL.FLDTCNTR = data_size;  /* READ : max. 128 byte */

    /* ==== Command control register(FLCMDCR)   ==== */
    FLCTL.FLCMDCR = 0x82130000ul;  /* Execute the 1st command stage               */
                                   /* Issue the 5 bytes of address                */
                                   /* Data stage is read direction(uses FLDTFIFO) */
                                   /* Execute the 2nd command stage               */

    /* ==== Command code register(FLCMCDR) ==== */
    FLCTL.FLCMCDR = 0x00003000ul;  /* 1st command -> 0x00, 2nd command -> 0x30 */


    /* ==== Start command transfer ==== */
    FLCTL.FLTRCR = 0x01;

    /* ---- read the data ---- */
    for ( loop = 0; loop < (data_size / sizeof(uint32_t)); loop++ )
    {
        while ( RZA_IO_RegRead_32(&FLCTL.FLDTCNTR, FLCTL_FLDTCNTR_DTFLW_SHIFT, FLCTL_FLDTCNTR_DTFLW) < 1 )
        {
            /* wait */
        }
        *data++ = FLCTL.FLDTFIFO;
    }

    /* ==== Wait for complete of transfer ==== */
    while ( RZA_IO_RegRead_8(&FLCTL.FLTRCR, FLCTL_FLTRCR_TREND_SHIFT, FLCTL_FLTRCR_TREND) == 0 )
    {
        /* prohibit stopping by force */
    }

    /* ==== completion transaction ==== */
    FLCTL.FLTRCR  = 0x00;              /* Stop transfer */
    FLCTL.FLCMNCR = 0x00100001ul;      /* chip enable --> negate */

}

/* End of File */
