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
* File Name : syscalls.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : 
******************************************************************************/

#include "r_typedefs.h"
#include "sio_char.h"
#include "errno.h"
#include "unused.h"
#include <sys/stat.h>
#include <string.h>

#define STDIN       (0)
#define STDOUT      (1)
#define STDERR      (2)

#define BUFF_SIZE   (256)

static int_t lowsrc_io_buff[BUFF_SIZE];

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr __asm ("sp");

/* prototypes.  */
void    __exit(int);
int     _write(int, char *, int);
int     _read(int, char *, int);
int     _isatty(const int);
void *  _sbrk(const ptrdiff_t);
int     _fstat (int, struct stat *);
int     _close (int);
off_t   _lseek (int, off_t, int);

/********************************************************/
int _read(int file, char * ptr, int len)
{
    static int32_t buff_num = 0;
    static int32_t read_pos = 0;
    int32_t        x;
    int32_t        idx;

    if (file != STDIN)
    {
        return -1;
    }

    if (len < 1)
    {
        x = 0;
    }
    else
    {
        if (0 == buff_num)
        {
            buff_num = SioRead(file, &lowsrc_io_buff[0], BUFF_SIZE);
            read_pos = 0;
        }

        if (buff_num > 0)
        {
            if (buff_num < len)
            {
                x = buff_num;
            }
            else
            {
                x = len;
            }
            for (idx = 0; idx < x; ++idx)
            {
                ptr[idx] = (char)(lowsrc_io_buff[read_pos++]);
                buff_num--;
            }
        }
        else
        {
            x = buff_num;
        }
    }

    if (x < 0)
    {
        return -1;
    }

    return x;
}

/********************************************************/
int _write(int file, char * ptr, int len)
{
    int nChars;

    if ((file != STDOUT) && (file != STDERR))
    {
        return -1;
    }

    for (nChars = len; nChars > 0; nChars--)
    {
        int_t   code = *ptr++;
        if (SioWrite(file, &code, 1) < 0)
        {
            break;
        }
    }


  if (len < 0)
  {
      return -1;
  }
  
  return (len - nChars);
}

/********************************************************/
void __exit(int status)
{
    (void) status;

    while (1)
    {
    }
}

/********************************************************/
int _isatty(const int file)
{
    R_UNUSED(file);
    return true;
}

/********************************************************/
void *_sbrk(const ptrdiff_t incr)
{
    extern char   end __asm ("end"); /* Defined by the linker.  */
    static char * heap_end;
    char *        prev_heap_end;

    if (heap_end == NULL)
        heap_end = & end;

    prev_heap_end = heap_end;

    if (heap_end + incr > stack_ptr) {
        errno = ENOMEM;
        return (void *) -1;
    }

    heap_end += incr;
    return (void *) prev_heap_end;
}

int _fstat (int fd, struct stat * st)
{
    memset (st, 0, sizeof (* st));
    R_UNUSED(fd);
    return 0;
}

int _close (int file) {
    R_UNUSED(file);
    return 0;
}

off_t _lseek (int fd, off_t ptr, int dir)
{
    R_UNUSED(fd);
    R_UNUSED(ptr);
    R_UNUSED(dir);
    return 0;
}

/* End of File */
