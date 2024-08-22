@/*******************************************************************************
@* DISCLAIMER
@* This software is supplied by Renesas Electronics Corporation and is only
@* intended for use with Renesas products. No other uses are authorized. This
@* software is owned by Renesas Electronics Corporation and is protected under
@* all applicable laws, including copyright laws.
@* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
@* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
@* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
@* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
@* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
@* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
@* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
@* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
@* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
@* Renesas reserves the right, without notice, to make changes to this software
@* and to discontinue the availability of this software. By using this software,
@* you agree to the additional terms and conditions found by accessing the
@* following link:
@* http://www.renesas.com/disclaimer
@* Copyright (C) 2012 - 2015 Renesas Electronics Corporation. All rights reserved.
@*******************************************************************************/
@/*******************************************************************************
@* File Name   : vector_mirrortable.s
@* $Rev: 1330 $
@* $Date:: 2015-02-17 16:07:56 +0900#$
@* Description : Vector mirrortable
@*******************************************************************************/


@==================================================================
@ Entry point for the Reset handler
@==================================================================
@    PRESERVE8
    .section    VECTOR_MIRROR_TABLE, #execinstr
    .arm

@   EXPORT  vector_table

@    IMPORT  reset_handler
@    IMPORT  undefined_handler
@    IMPORT  svc_handler
@    IMPORT  prefetch_handler
@    IMPORT  abort_handler
@    IMPORT  reserved_handler
@    IMPORT  irq_handler
@    IMPORT  fiq_handler

@   ENTRY

@   EXPORT Start

@Start

vector_table2:
    LDR pc, =reset_handler          @ 0x0000_0000 : Reset exception
    LDR pc, =undefined_handler      @ 0x0000_0004 : Undefined instructions exception
    LDR pc, =svc_handler            @ 0x0000_0008 : Software interrupts exceptions
    LDR pc, =prefetch_handler       @ 0x0000_000c : Prefetch abort exception
    LDR pc, =abort_handler          @ 0x0000_0010 : Data abort exception
    LDR pc, =reserved_handler       @ 0x0000_0014 : Reserved
    LDR pc, =irq_handler            @ 0x0000_0018 : IRQ exception
    LDR pc, =fiq_handler            @ 0x0000_001c : FIQ exception

Literals:
    .LTORG

    .END
