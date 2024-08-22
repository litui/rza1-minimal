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
* Copyright (C) 2014 - 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : sample_flctl_command.c
* $Rev: 1516 $
* $Date:: 2015-08-25 15:47:15 +0900#$
* Description : RZ/A1H NAND Sample Program
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "r_typedefs.h"
#include "devdrv_flctl.h"
#include "command.h"
#include "sample_flctl.h"

/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

    /* For one page            */
    /* SECTOR_SZ_DATA_ONLY=512 */
    /* SECTOR_SZ_EXT_ONLY =16  */
    /* SECTOR_OF_PAGE=4        */
    /* PAGE_OF_BLOCK=64        */
    /* (512+16)*(64*4)         */
#ifdef __CC_ARM
    #pragma arm section zidata = "BSS_NANDNC"
    static uint32_t test_nand_rw_buff[ (SECTOR_SZ_DATA_AND_EXT * SECTOR_OF_PAGE) / sizeof(uint32_t) ];
    #pragma arm section zidata
#endif

#ifdef __ICCARM__
    static uint32_t test_nand_rw_buff[ (SECTOR_SZ_DATA_AND_EXT * SECTOR_OF_PAGE) / sizeof(uint32_t) ] @ "BSS_NANDNC";
#endif

#ifdef __GNUC__
    static uint32_t test_nand_rw_buff[ (SECTOR_SZ_DATA_AND_EXT * SECTOR_OF_PAGE) / sizeof(uint32_t) ] __attribute__ ((section ("BSS_NANDNC")));

#endif





/******************************************************************************
Private global variables and functions
******************************************************************************/


/* ==== function prototype ==== */
static void nand_print_put_dmp(uint8_t * Ptr, uint16_t Size, uint16_t Spc, uint16_t Wid);
static void nand_print_put_dmp2(uint8_t * Ptr, uint32_t Size, uint16_t Spc, uint16_t Wid);
static void nand_make_buffer_data(uint8_t * Ptr, uint32_t Size, uint16_t Wid, uint32_t start_value);

/***************************************************************************** 
* Function Name: Sample_FLCTL_Init
* Description  : Initialize the NAND Driver FLCTL IP
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : 0 -   Succeed to execute
******************************************************************************/
int32_t Sample_FLCTL_Init(int32_t argc, char_t ** argv)
{
    /* ---- Initialize FLCTL ---- */
    printf("initialize FLCTL Module\n");
    R_FLCTL_Init();

    /* ---- Issue the reset command to NAND flash memory ---- */
    printf("issue RESET Command to NAND\n");
    R_FLCTL_Reset();

    return 0;
}

/***************************************************************************** 
* Function Name: Sample_FLCTL_Reset
* Description  : Issue the RESET Command to NAND
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : 0 -   Succeed to execute
******************************************************************************/
int32_t Sample_FLCTL_Reset(int32_t argc, char_t ** argv)
{
    /* ---- Issue the reset command to NAND flash memory ---- */
    printf("issue RESET Command to NAND\n");
    R_FLCTL_Reset();

    return 0;
}

/*******************************************************************************
* Function Name: Sample_FLCTL_Erase
* Description  : Issue the ERASE Command to NAND
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : 0 -   Succeed to execute
*              : 1 -   Error occurred
*******************************************************************************/
int32_t Sample_FLCTL_Erase(int32_t argc, char_t ** argv)
{
    int32_t     err;
    int32_t     psn;

    printf("Please input sector number:");
    scanf("%d", &psn);
    printf("\n");

    err = R_FLCTL_Erase ( psn );
    if ( err != 0 )
    {
        printf("error:erase error sector=%d\n", psn);
        return 1;
    }

    printf("success:erase success sector=%d\n", psn);
    return 0;
}

/*******************************************************************************
* Function Name: Sample_FLCTL_Read
* Description  : Issue the READ Command to NAND
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : 0 -   Succeed to execute
*              : 1 -   Error occurred
*******************************************************************************/
int32_t Sample_FLCTL_Read(int32_t argc, char_t ** argv)
{
    int32_t     err;
    int32_t     psn;
    int32_t     cnt;
    uint32_t    bytes;

    printf("Please input sector number:");
    scanf("%d", &psn);
    printf("\n");

    printf("Please input sector count:");
    scanf("%d", &cnt);
    printf("\n");
    if ( cnt > SECTOR_OF_PAGE )
    {
        printf("error: sector count is over than buffer size!!\n");
        return 1;
    }

    if ( cnt <= 0 )
    {
        printf("error: sector count is zero or less!!\n");
        return 1;
    }

    if ( ((psn & 0x3) + cnt) > SECTOR_OF_PAGE )
    {
        printf("error: sector number or sector count is overlay nand page!!\n");
        return 1;
    }

    err = R_FLCTL_Read( psn, cnt, test_nand_rw_buff );
    if ( err != 0 )
    {
        printf("error:read error\n");
        return 1;
    }

    bytes = (cnt * SECTOR_SZ_DATA_AND_EXT);
    nand_print_put_dmp2((uint8_t *)&test_nand_rw_buff[0], bytes, 4, 16);

    return 0;
}

/*******************************************************************************
* Function Name: Sample_FLCTL_Write
* Description  : Issue the WRITE Command to NAND
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : 0 -   Succeed to execute
*              : 1 -   Error occurred
*******************************************************************************/
int32_t Sample_FLCTL_Write(int32_t argc, char_t ** argv)
{
    int32_t     loop;
    int32_t     err;
    int32_t     psn;
    int32_t     cnt;
    uint32_t    bytes;
    uint32_t    bytes_temp;
    int32_t     start_value;
    uint32_t    offset;

    printf("Please input sector number:");
    scanf("%d", &psn);
    printf("\n");

    printf("Please input sector count:");
    scanf("%d", &cnt);
    printf("\n");
    if ( cnt > SECTOR_OF_PAGE )
    {
        printf("error: sector count is over than buffer size!!\n");
        return 1;
    }

    if ( cnt <= 0 )
    {
        printf("error: sector count is zero or less!!\n");
        return 1;
    }

    if ( ((psn & 0x3) + cnt) > SECTOR_OF_PAGE )
    {
        printf("error: sector number or sector count is overlay nand page!!\n");
        return 1;
    }

    printf("Please input Start Value to write sector:");
    scanf("%d", &start_value);
    printf("\n");

    bytes = (cnt * SECTOR_SZ_DATA_AND_EXT);

    loop        = 0;
    bytes_temp  = bytes;
    do
    {
        offset = (loop * SECTOR_SZ_DATA_AND_EXT) / sizeof(uint32_t);
        nand_make_buffer_data((uint8_t *)&test_nand_rw_buff[offset], SECTOR_SZ_DATA_AND_EXT, 8, start_value);
        bytes_temp -= SECTOR_SZ_DATA_AND_EXT;
        if ( bytes_temp <= 0 )
        {
            break;
        }
        start_value++;
        loop++;

    } while (1);

    err = R_FLCTL_Write( psn, cnt, test_nand_rw_buff );
    if ( err != 0 )
    {
        printf("error:write error\n");
        return 1;
    }

    return 0;
}

/*******************************************************************************
* Function Name: Sample_FLCTL_Info
* Description  : Issue the READ ID Command to NAND
* Argument     : argc -   arg count
*              : argv -   arg value
* Return Value : 0 -   Succeed to execute
*              : 1 -   Error occurred
*******************************************************************************/
int32_t Sample_FLCTL_Info(int32_t argc, char_t **argv)
{
    int32_t     err;
    uint8_t     read_buff[16];

    /* issue READ ID command */
    memset(read_buff, 0, 16);
    err = R_FLCTL_Read_Id ( read_buff );
    if ( err != 0 )
    {
        printf("error:read_id error\n");
        return 1;
    }
    else
    {
        printf("NAND Read ID information.\n");
        printf("  1st code : %02x\n", read_buff[0]);
        printf("  2nd code : %02x\n", read_buff[1]);
        printf("  3rd code : %02x\n", read_buff[2]);
        printf("  4th code : %02x\n", read_buff[3]);
        printf("  5th code : %02x\n", read_buff[4]);
    }

    return 0;
}

/***************************************************************************** 
* Function Name: nand_print_put_dmp
* Description  : Outputs normally the information according to the value specified
*              : by the argument.
* Argument     : Ptr  -   Data Pointer
*              : Size -   Data Size
*              : Spc  -   Line Head Space
*              : Wid  -   Line Data Count
* Return Value : none
******************************************************************************/
static void nand_print_put_dmp(uint8_t * Ptr, uint16_t Size, uint16_t Spc, uint16_t Wid)
{
    uint16_t    i;
    uint16_t    j;
    uint16_t    k;

    if ( Wid != 0 )
    {
        for ( i = 0; i < Size;  )
        {
            for ( k = 0; k < Spc; ++k )
            {
                printf( "  " );
            }

            for ( j = 0; (i < Size) && (j < Wid); ++j, ++i, ++Ptr )
            {
                printf( "%02x", *Ptr );
                printf( "  " );
            }
            if ( i < Size )
            {
                printf( "\n" );
            }
        }
    }
    printf( "\n" );
}

/***************************************************************************** 
* Function Name: nand_print_put_dmp2
* Description  : Outputs normally the information according to the value specified 
*              : by the argument.
* Argument     : Ptr  -   Data Pointer
*              : Size -   Data Size
*              : Spc  -   Line Head Space
*              : Wid  -   Line Data Count
* Return Value : none
******************************************************************************/
static void nand_print_put_dmp2(uint8_t * Ptr, uint32_t Size, uint16_t Spc, uint16_t Wid)
{
    uint32_t        dump;

    do
    {
        /* print DATA AREA */
        if ( Size > SECTOR_SZ_DATA_ONLY )
        {
            dump = SECTOR_SZ_DATA_ONLY;
        }
        else
        {
            dump = (uint16_t)Size;
        }

        nand_print_put_dmp( Ptr, (uint16_t)dump, Spc, Wid );
        printf( "\n" );

        Ptr  += dump;
        Size -= dump;

        /* print EXT AREA */
        if ( Size > SECTOR_SZ_EXT_ONLY )
        {
            dump = SECTOR_SZ_EXT_ONLY;
        }
        else
        {
            dump = (uint16_t)Size;
        }

        nand_print_put_dmp( Ptr, (uint16_t)dump, Spc, Wid );
        printf( "\n" );

        Ptr  += dump;
        Size -= dump;

    } while ( Size > 0 );
}

/***************************************************************************** 
* Function Name: nand_make_buffer_data
* Description  : Fill the memory area with incremental data.
* Argument     : Ptr  -   Data Address
*              : Size -   Data Size
*              : Wid  -   Bit width of one data.(8, 16 or 32)
*              : start_value - initial value of filled data.
* Return Value : none
******************************************************************************/
static void nand_make_buffer_data(uint8_t * Ptr, uint32_t Size, uint16_t Wid, uint32_t start_value)
{
    int             loop;
    uint8_t         value_char;
    uint16_t        value_short;
    uint32_t        value_long;
    uint8_t       * ptr_char;
    uint32_t      * ptr_long;
    uint16_t      * ptr_short;

    if ( 8 == Wid )
    {
        value_char = (uint8_t)start_value;
        ptr_char = (uint8_t *)Ptr;
        for ( loop = 0; loop < Size; loop++ )
        {
            *ptr_char = value_char;
            ptr_char++;
            value_char++;
        }
    }
    else if ( 16 == Wid )
    {
        value_short = (uint16_t)start_value;
        ptr_short = (uint16_t *)Ptr;
        for ( loop = 0; loop < Size; (loop = loop + 2) )
        {
            *ptr_short = value_short;
            ptr_short++;
            value_short++;
        }
    }
    else
    {
        value_long = (uint32_t)start_value;
        ptr_long = (uint32_t *)Ptr;
        for ( loop = 0; loop < Size; (loop = loop + 4) )
        {
            *ptr_long = value_long;
            ptr_long++;
            value_long++;
        }
    }
}


/* End of File */
