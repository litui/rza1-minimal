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
@* File Name   : reset_handler.s
@* $Rev: 1330 $
@* $Date:: 2015-02-17 16:07:56 +0900#$
@* Description : Reset handler
@*******************************************************************************/


@ Standard definitions of mode bits and interrupt (I & F) flags in PSRs
    .EQU    USR_MODE,   0x10
    .EQU    FIQ_MODE,   0x11
    .EQU    IRQ_MODE,   0x12
    .EQU    SVC_MODE,   0x13
    .EQU    ABT_MODE,   0x17
    .EQU    UND_MODE,   0x1b
    .EQU    SYS_MODE,   0x1f
    .EQU    Thum_bit,   0x20            @ CPSR/SPSR Thumb bit


@==================================================================
@ Entry point for the Reset handler
@==================================================================
@    PRESERVE8
    .section    RESET_HANDLER, #execinstr
    .arm

@    IMPORT  Image$$ARM_LIB_STACK$$ZI$$Limit     @ Linker symbol from scatter file
@    IMPORT  Image$$IRQ_STACK$$ZI$$Limit         @ Linker symbol from scatter file
@    IMPORT  Image$$FIQ_STACK$$ZI$$Limit         @ Linker symbol from scatter file
@    IMPORT  Image$$SVC_STACK$$ZI$$Limit         @ Linker symbol from scatter file
@    IMPORT  Image$$ABT_STACK$$ZI$$Limit         @ Linker symbol from scatter file

@    IMPORT  Peripheral_BasicInit
@    IMPORT  init_TTB
@    IMPORT  INITSCT
@    IMPORT  Enable_VFP
@    IMPORT  main
@    IMPORT  IRQ_FIQ_HANDLER

    .global  reset_handler
    .global  undefined_handler
    .global  svc_handler
    .global  prefetch_handler
    .global  abort_handler
    .global  reserved_handler
    .global  irq_handler
    .global  fiq_handler

@******************************************************************************
@ Function Name : reset_handler
@ Description   : This function is the assembler function executed after reset
@               : cancellation. After initial setting for the stack pointer or 
@               : the MMU and reset cancellation, executes initial setting for 
@               : the minimum required peripheral functions. Calls the __main
@               : of the standard library function to execute the main function.
@******************************************************************************
    .global start
    .type   start,%function
start:
reset_handler:   @FUNCTION {}

@==================================================================
@ Disable cache and MMU in case it was left enabled from an earlier run
@ This does not need to be done from a cold reset 
@==================================================================
    MRC  p15, 0, r0, c1, c0, 0      @;; Read CP15 System Control register (SCTLR)
    BIC  r0, r0, #(0x1 << 12)       @;; Clear I bit 12 to disable I Cache
    BIC  r0, r0, #(0x1 <<  2)       @;; Clear C bit  2 to disable D Cache
    BIC  r0, r0, #0x1               @;; Clear M bit  0 to disable MMU
    @ Added the following two lines to hopefully solve the HIVECS issue
    BIC  r0, r0, #(0x1 << 11)       @;; Clear Z bit 11 to disable branch prediction 
    BIC  r0, r0, #(0x1 << 13)       @;; Clear V bit 13 to disable hivecs 
    MCR  p15, 0, r0, c1, c0, 0      @;; Write value back to CP15 System Control register

@==================================================================
@ Setting up Stack Area
@==================================================================
                                    @;; SVC Mode(Default)
    LDR  sp, =Image$$SVC_STACK$$ZI$$Limit

    CPS  #IRQ_MODE                  @;; IRQ Mode
    LDR  sp, =Image$$IRQ_STACK$$ZI$$Limit

    CPS  #FIQ_MODE                  @;; FIQ Mode
    LDR  sp, =Image$$FIQ_STACK$$ZI$$Limit

    CPS  #ABT_MODE                  @;; ABT Mode
    LDR  sp, =Image$$ABT_STACK$$ZI$$Limit

    CPS  #SYS_MODE                  @;; SYS Mode

@; System mode Stack pointer is set up ARM_LIB_STACK in the __main()->__entry()
    LDR  sp, =Image$$ARM_LIB_STACK$$ZI$$Limit

@==================================================================
@ Hardware initialize
@ Initialize CPG, BSC for CS0 and CS1, and enable On-Chip Data-Retention RAM
@==================================================================
    LDR  r12,=Peripheral_BasicInit  @;; Save this in register for possible long jump
    BLX  r12                        @;; Hardware Initialize

@==================================================================
@ TLB maintenance, Invalidate Data and Instruction TLBs
@==================================================================
    MOV  r0,#0
    MCR  p15, 0, r0, c8, c7, 0      @;; Cortex-A9 I-TLB and D-TLB invalidation (TLBIALL)

@===================================================================
@ Invalidate instruction cache, also flushes BTAC
@===================================================================
    MOV  r0, #0                     @;; SBZ
    MCR  p15, 0, r0, c7, c5, 0      @;; ICIALLU - Invalidate entire I Cache, and flushes branch target cache

@==================================================================
@ Cache Invalidation code for Cortex-A9
@==================================================================
    @;; Invalidate L1 Instruction Cache
    MRC  p15, 1, r0, c0, c0, 1      @;; Read Cache Level ID Register (CLIDR)
    TST  r0, #0x3                   @;; Harvard Cache?
    MOV  r0, #0
    MCRNE   p15, 0, r0, c7, c5, 0   @;; Invalidate Instruction Cache

    @;; Invalidate Data/Unified Caches
    MRC  p15, 1, r0, c0, c0, 1      @;; Read CLIDR
    ANDS r3, r0, #0x07000000        @;; Extract coherency level
    MOV  r3, r3, LSR #23            @;; Total cache levels << 1
    BEQ  Finished                   @;; If 0, no need to clean

    MOV  r10, #0                    @;; R10 holds current cache level << 1
Loop1:
    ADD  r2, r10, r10, LSR #1       @;; R2 holds cache "Set" position 
    MOV  r1, r0, LSR r2             @;; Bottom 3 bits are the Cache-type for this level
    AND  r1, r1, #7                 @;; Isolate those lower 3 bits
    CMP  r1, #2
    BLT  Skip                       @;; No cache or only instruction cache at this level

    MCR  p15, 2, r10, c0, c0, 0     @;; Write the Cache Size selection register (CSSELR)
    ISB                             @;; ISB to sync the change to the CacheSizeID reg
    MRC  p15, 1, r1, c0, c0, 0      @;; Reads current Cache Size ID register (CCSIDR)
    AND  r2, r1, #7                 @;; Extract the line length field
    ADD  r2, r2, #4                 @;; Add 4 for the line length offset (log2 16 bytes)
    LDR  r4, =0x3FF
    ANDS r4, r4, r1, LSR #3         @;; R4 is the max number on the way size (right aligned)
    CLZ  r5, r4                     @;; R5 is the bit position of the way size increment
    LDR  r7, =0x7FFF
    ANDS r7, r7, r1, LSR #13        @;; R7 is the max number of the index size (right aligned)
Loop2:
    MOV  r9, r4                     @;; R9 working copy of the max way size (right aligned)

Loop3:
    ORR  r11, r10, r9, LSL r5       @;; Factor in the Way number and cache number into R11
    ORR  r11, r11, r7, LSL r2       @;; Factor in the Set number
    MCR  p15, 0, r11, c7, c6, 2     @;; Invalidate by Set/Way (DCISW)
    SUBS r9, r9, #1                 @;; Decrement the Way number
    BGE  Loop3
    SUBS r7, r7, #1                 @;; Decrement the Set number
    BGE  Loop2
Skip:
    ADD  r10, r10, #2               @;; increment the cache number
    CMP  r3, r10
    BGT  Loop1

Finished:

@==================================================================
@ TTB initialize
@==================================================================
    BL  init_TTB                    @;; Initialize TTB

@===================================================================
@ Setup domain control register - Enable all domains to client mode
@===================================================================
    MRC  p15, 0, r0, c3, c0, 0      @;; Read Domain Access Control Register (DACR)
    LDR  r0, =0x55555555            @;; Initialize every domain entry to b01 (client)
    MCR  p15, 0, r0, c3, c0, 0      @;; Write Domain Access Control Register

@            IF {TARGET_FEATURE_NEON} || {TARGET_FPU_VFP}
#if defined(TARGET_FEATURE_NEON) || defined(TARGET_FPU_VFP)
@==================================================================
@ Enable access to NEON/VFP by enabling access to Coprocessors 10 and 11.
@ Enables Full Access i.e. in both privileged and non privileged modes
@==================================================================
    MRC  p15, 0, r0, c1, c0, 2      @;; Read Coprocessor Access Control Register (CPACR)
    ORR  r0, r0, #(0xF << 20)       @;; Enable access to CP 10 & 11
    MCR  p15, 0, r0, c1, c0, 2      @;; Write Coprocessor Access Control Register (CPACR)
    ISB

@=================================================================
@ Switch on the VFP and NEON hardware
@=================================================================
    MOV  r0, #0x40000000
    VMSR FPEXC, r0                  @;; Write FPEXC register, EN bit set

#endif
@            ENDIF

@===================================================================
@ Enable MMU
@ Leaving the caches disabled until after scatter loading(__main).
@===================================================================
    MRC  p15, 0, r0, c1, c0, 0      @;; Read CP15 System Control register (SCTLR)
    BIC  r0, r0, #(0x1 << 12)       @;; Clear I bit 12 to disable I Cache
    BIC  r0, r0, #(0x1 <<  2)       @;; Clear C bit  2 to disable D Cache
    BIC  r0, r0, #0x2               @;; Clear A bit  1 to disable strict alignment fault checking
    ORR  r0, r0, #0x1               @;; Set M bit 0 to enable MMU before scatter loading
    MCR  p15, 0, r0, c1, c0, 0      @;; Write CP15 System Control register

@===================================================================
@ copy and clear the sections 
@===================================================================
    LDR  r12,=INITSCT               @;; Save this in register for possible long jump
    BLX  r12                        @;; Call to copy and to clear the sections

@===================================================================
@ Enable the vfp
@===================================================================
    LDR  r12,=Enable_VFP            @;; Save this in register for possible long jump
    BLX  r12                        @;; Call to enable the vfp

@===================================================================
@ Call to __main
@===================================================================
    LDR  r12,=main                  @;; Save this in register for possible long jump
    BLX  r12                        @;; Call to __main C library entry point

@===================================================================
@ loop foreaver
@===================================================================
terminate:
    B    terminate                  @;; Loop forever

@    ENDFUNC

Literals2:
    .LTORG


@==================================================================
@ Other Handler
@==================================================================
undefined_handler:
    B   undefined_handler           @;; Looping in this address

svc_handler:
    B   svc_handler                 @;; Looping in this address

prefetch_handler:
    B   prefetch_handler            @;; Looping in this address

abort_handler:
    B   abort_handler               @;; Looping in this address

reserved_handler:
    B   reserved_handler            @;; Looping in this address

    .END
