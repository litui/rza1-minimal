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
@* File Name   : initsect.s
@* $Rev: 1330 $
@* $Date:: 2015-02-17 16:07:56 +0900#$
@* Description : copy and fill the variables
@*******************************************************************************/

        .EQU    num, 20     @ set number of words to be copied

        .section    CODE_RESET
        .global     INITSCT

        @@ This program need the stack area (64bytes?)
INITSCT:
        push    {lr}
        BL      _COPYVAR        @ copy the variables
        BL      _ZEROVAR        @ fill the variables with zero
        pop     {pc}            @ return from subroutine

@@@ 
@@@ Copy the vaiables
@@@ This program gets from the table of coping area "DTBL[]"
_COPYVAR:
        ldr     r4, =DTBL           @ top of table

_CVAR01:
        ldr     r0, [r4], #4        @ r0 = top of source block
        ldr     r1, [r4], #4        @ r1 = end of source block
        sub     r2, r1, r0          @ r1 = bytes to copy
        ldr     r1, [r4], #4        @ r1 = top of destination block

        cmp     r0, #0              @ check terminator
        bxeq    lr                  @ return

        movs    r3,r2, LSR #(3+2)   @ Number of eight word multiples
        beq     _CVAR03             @ Fewer than eight words to move?

        push    {r4-r11}            @ Save some working registers

_CVAR02:
        ldm     r0!, {r4-r11}       @ Load 8 words from the source
        stm     r1!, {r4-r11}       @ and put them at the destination
        subs    r3, r3, #1          @ Decrement the counter
        bne     _CVAR02             @ ... copy more

        pop     {r4-r11}            @ Don't require these now - restore
                                    @ originals

_CVAR03:
        ands    r2, r2, #0x1f       @ Number of bytes to copy
        beq     _CVAR05             @ No byte left to copy?

_CVAR04:
        ldrb    r3, [r0], #1        @ Load a word from the source and
        strb    r3, [r1], #1        @ store it to the destination
        subs    r2, r2, #1          @ Decrement the counter
        bne     _CVAR04             @ ... copy more

_CVAR05:
        b       _CVAR01             @ copy the next block


@@@ 
@@@ Fill the vaiables
@@@ This program gets from the table of coping area "BTBL[]"
_ZEROVAR:
        ldr     r4, =BTBL           @ top of table

_ZVAR01:
        ldr     r1, [r4], #4        @ r1 = top of block
        ldr     r2, [r4], #4        @ r2 = end of block
        sub     r2, r2, r1          @ r2 = number of bytes to fill zero

        cmp     r1, #0              @ check terminator
        bxeq    lr                  @ return

        mov     r0, #0              @ fill value

        movs    r3,r2, LSR #(3+2)   @ Number of eight word multiples
        beq     _ZVAR03             @ Fewer than eight words to clean?

        push    {r4-r11}            @ Save some working registers

        mov     r4, r0              @ fill zero
        mov     r5, r0              @
        mov     r6, r0              @
        mov     r7, r0              @
        mov     r8, r0              @
        mov     r9, r0              @
        mov     r10, r0             @
        mov     r11, r0             @

_ZVAR02:
        stm     r1!, {r4-r11}       @ fill the block
        subs    r3, r3, #1          @ Decrement the counter
        bne     _ZVAR02             @ ... fill more

        pop     {r4-r11}            @ Don't require these now - restore
                                    @ originals
    
_ZVAR03:
        ands    r2, r2, #0x1f       @ Number of bytes to copy
        beq     _ZVAR05             @ No words left to copy?

_ZVAR04:
        strb    r0, [r1], #1        @ Fill the block
        subs    r2, r2, #1          @ Decrement the counter
        bne     _ZVAR04             @ ... fill more

_ZVAR05:
        b       _ZVAR01             @ fill the next block
        

        .END

