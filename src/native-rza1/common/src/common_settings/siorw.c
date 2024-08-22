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
* File Name   : siorw.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Serial I/O character R/W
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "sio_char.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* File descriptor */
#define STDIN           (0)
#define STDOUT          (1)
#define STDERR          (2)

#define SIORW_SUCCESS   (0)
#define SIORW_ERROR     (-1)
#define SIORW_FLAG_OFF  (0)
#define SIORW_FLAG_ON   (1)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name: SioWrite
* Description  : The character strings specified with buffer is output for n
*              : bytes from serial port. The output is determined by file number fileno.
*              : The effective outputs in this version are STDOUT and STDERR, and
*              : it is output to the same serial port.
*              : The linefeed code '\n'(LF) is converted in '\r'(CR)+'\n'(LF) to output.
* Arguments    : int32_t  file_no  ; I : File number to be the target of writing
*              : int_t  * buffer   ; O : Pointer to the area in which writing data is stored
*              : uint32_t writing_b; I : Writing bytes
* Return Value : >=0 : Number of transmitting characters
*              : -1  : File number error
******************************************************************************/
int32_t SioWrite(int32_t file_no, const int_t * buffer, uint32_t writing_b)
{
    uint32_t offset;

    if ((STDOUT == file_no) || (STDERR == file_no))
    {
        for (offset = 0; offset < writing_b; offset++)
        {
            /* Writing in buffer converting linefeed code */
            if ('\n' == *(buffer + offset))
            {
                if (0 == offset)
                {
                    IoPutchar('\r');
                }
                else
                {
                    if (*(buffer + offset - 1) != '\r')
                    {
                        IoPutchar('\r');
                    }
                }
                IoPutchar('\n');
            }
            else
            {
                IoPutchar(*(buffer + offset));
            }
        }
        return (int32_t)offset;
    }

    return SIORW_ERROR;     /* File number error */
}

/******************************************************************************
* Function Name: SioRead
* Description  : The character strings specified with buffer is input for
*              : n bytes from serial port.The input is determined by file number fileno.
*              : The effective input in this version is STDIN.
* Arguments    : int32_t  file_no  ; I : File number to be the target of reading
*              : int_t  * buffer   ; O : Pointer to the area in which reading data is stored
*              : uint32_t reading_b; I : Reading bytes
* Return Value : >0 : Number of receiving characters
*              : -1 : File number, receiving data error
******************************************************************************/
int32_t SioRead(int32_t file_no, int_t * buffer, uint32_t reading_b)
{
    int_t          char_mem;
    int_t          SP_char;
    uint32_t       offset;
    static int32_t sjis_flg = SIORW_FLAG_OFF;

    if (STDIN == file_no)
    {
        for (offset = 0; offset < reading_b; )
        {
            /* Reading receiving data */
            char_mem = IoGetchar();

            if (-1 == char_mem)   /* -1 is returned when it is receiving data error */
            {
                return SIORW_ERROR;
            }
            
            if (SIORW_FLAG_ON == sjis_flg)
            {
                sjis_flg = SIORW_FLAG_OFF;
                SioWrite(STDOUT, &char_mem, 1);
                *(buffer + offset) = char_mem;
                offset++;
            }
            else if ((0x20 <= char_mem) && (char_mem <= 0x7E))
            {
                /* Data possible to display */
                SioWrite(STDOUT, &char_mem, 1);
                *(buffer + offset) = char_mem;
                offset++;
            }
            else if ('\b' == char_mem)      /* BS process */
            {
                if (offset > 0)
                {
                    SP_char = 0x20;
                    SioWrite(STDOUT, &char_mem, 1);
                    SioWrite(STDOUT, &SP_char, 1);
                    SioWrite(STDOUT, &char_mem, 1);
                    offset--;
                }
            }
            else if ('\r' == char_mem)      /* CR process */
            {
                *(buffer + offset) = '\n';
                SioWrite(STDOUT, buffer + offset, 1);
                offset++;
                break;
            }
            /* Japanese SJIS ? */
            else if (((char_mem >= 0x80) && (char_mem < 0xA0)) || ((char_mem >= 0xE0) && (char_mem < 0xFE)))
            {
                /* Data possible to display */
                SioWrite(STDOUT, &char_mem, 1);
                *(buffer + offset) = char_mem;
                offset++;
                sjis_flg = SIORW_FLAG_ON;
            }
            /* Character cord out of the support */
            else
            {
                /* Do Nothing */
            }
        }
        return (int32_t)offset;
    }

    return SIORW_ERROR;     /* File number error */
}


/* End of File */

