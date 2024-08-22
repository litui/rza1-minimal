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
* File Name : sections.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : 
******************************************************************************/
#include "r_typedefs.h"

/* List of the section of variables with initial value */
typedef struct {
    void    *rom_s;     /* Top address on ROM    */
    void    *rom_e;     /* Bottom address on ROM */
    void    *ram_s;     /* Top address on RAM    */
} DTBL_S;

/* List of the section of variables with zero clear */
typedef struct {
    void    *b_s;       /* Top address    */
    void    *b_e;       /* Bottom address */
} BTBL_S;


#ifndef ON_RAM
extern  short   __vect_start;
extern  short   __vect_end;
extern  short   __vect_load;
#endif

extern  short   __fastcode_start;
extern  short   __fastcode_end;
extern  short   __fastcode_load;

extern  short   __data_start;
extern  short   __data_end;
extern  short   __data_load;

extern  short   __data2_start;
extern  short   __data2_end;
extern  short   __data2_load;

extern  short   __cache_operation_start;
extern  short   __cache_operation_end;
extern  short   __cache_operation_load;

extern  short   __bss_start;
extern  short   __bss_end;

extern  short   __bss2_start;
extern  short   __bss2_end;


#if 0
extern  const DTBL_S    DTBL[];
extern  const BTBL_S    BTBL[];
#endif


const DTBL_S    DTBL[] =
{
#ifndef ON_RAM
    { &__vect_start, &__vect_end, &__vect_load },
#endif
    { &__fastcode_start, &__fastcode_end, &__fastcode_load },
    { &__data_start, &__data_end, &__data_load },
    { &__data2_start, &__data2_end, &__data2_load },
    { &__cache_operation_start, &__cache_operation_end, &__cache_operation_load },
/*  {   _sectop("D"),   _secend("D"),   _sectop("R")    },*/
    {   0,  0,  0   },  /* terminator */
};

const BTBL_S    BTBL[] =
{
    { &__bss_start, &__bss_end },
    { &__bss2_start, &__bss2_end },
/*  {   _sectop("B"),   _secend("B")    },*/
    {   0,  0   },  /* terminator */
};

/* End of File */
