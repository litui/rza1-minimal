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
@* File Name   : l1_cache_init.s
@* $Rev: 1332 $
@* $Date:: 2015-02-17 17:22:59 +0900#$
@* Description : L1 cache initialize
@*******************************************************************************/


@==================================================================
@ This code provides basic global enable for Cortex-A9 cache.
@ It also enables branch prediction
@ This code must be run from a privileged mode
@==================================================================
    .section INITCA9CACHE, #execinstr

    .global  L1CacheInit
    .func    L1CacheInit
    .type    L1CacheInit,%function

L1CacheInit:

@==================================================================
@ Disable caches
@ Caches are controlled by the System Control Register:
@==================================================================
    @;; I-cache is controlled by bit 12
    @;; D-cache is controlled by bit 2

    MRC  p15, 0, r0, c1, c0, 0          @;; Read CP15 register 1
    AND  r0, r0, #(0x0 << 12)           @;; Disable I Cache
    AND  r0, r0, #(0x0 << 2)            @;; Disable D Cache
    MCR  p15, 0, r0, c1, c0, 0          @;; Write CP15 register 1

@==================================================================
@ Enable caches
@ Caches are controlled by the System Control Register:
@==================================================================
    @;; I-cache is controlled by bit 12
    @;; D-cache is controlled by bit 2

    MRC  p15, 0, r0, c1, c0, 0          @;; Read CP15 register 1
    ORR  r0, r0, #(0x1 << 12)           @;; Enable I Cache
    ORR  r0, r0, #(0x1 << 2)            @;; Enable D Cache
    MCR  p15, 0, r0, c1, c0, 0          @;; Write CP15 register 1

@==================================================================
@ Enable Program Flow Prediction
@
@ Branch prediction is controlled by the System Control Register:
@ Set Bit 11 to enable branch prediction and return  
@==================================================================
    @;; Turning on branch prediction requires a general enable
    @;; CP15, c1. Control Register

    @;; Bit 11 [Z] bit Program flow prediction:
    @;; 0 = Program flow prediction disabled
    @;; 1 = Program flow prediction enabled.

    MRC  p15, 0, r0, c1, c0, 0          @;; Read System Control Register
    ORR  r0, r0, #(0x1 << 11)
    MCR  p15, 0, r0, c1, c0, 0          @;; Write System Control Register

@==================================================================
@ Enable D-side prefetch
@==================================================================
    @;; Bit 2 [DP] Dside prefetch:
    @;; 0 = Dside prefetch disabled
    @;; 1 = Dside prefetch enabled.

    MRC  p15, 0, r0, c1, c0, 1          @;; Read Auxiliary Control Register
    ORR  r0, r0, #(0x1 << 2)            @;; Enable Dside prefetch
    MCR  p15, 0, r0, c1, c0, 1          @;; Write Auxiliary Control Register

    BX        lr

    .endfunc
    .end
