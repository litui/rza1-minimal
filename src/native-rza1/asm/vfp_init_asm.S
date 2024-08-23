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
@* File Name   : vfp_init_asm.s
@* $Rev: 1330 $
@* $Date:: 2015-02-17 16:07:56 +0900#$
@* Description : VFP initialize
@*******************************************************************************/


    .EQU    VFPEnable,     0x40000000

@    PRESERVE8
    .section    FPU_INIT, #execinstr
    .arm

    .global  Enable_VFP
    .type    Enable_VFP,%function

Enable_VFP:  @FUNCTION {}
@;
    MRC     p15, 0, r0, c1, c0, 2   @ Read CPACR into r0
    ORR     r0, r0, #(3 << 20)      @ OR in User and Privileged access for CP10
    ORR     r0, r0, #(3 << 22)      @ OR in User and Privileged access for CP11
    BIC     r0, r0, #(3 << 30)      @ Clear ASEDIS/D32DIS if set
    MCR     p15, 0, r0, c1, c0, 2   @ Store new access permissions into CPACR
    ISB                             @ Ensure side-effect of CPACR is visible
@;
    MOV     r0, #VFPEnable
    VMSR    FPEXC, r0
    BX      LR
@;
@    ENDFUNC

    .END
