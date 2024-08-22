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
* File Name   : sample_dmac_command_auto_req.c
* $Rev: 1349 $
* $Date:: 2015-03-03 18:16:15 +0900#$
* Description : DMAC sample program - Auto request sample
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_intc.h"            /* INTC Driver Header */
#include "sample_dmac.h"
#include "sample_dmac_chext.h"
#include "cache.h"
#include "command.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define DMAC_FLG_ON          (1)
#define DMAC_FLG_OFF         (0)
#define DMAC_BUFF_SIZE_256   (256)       /* Transfer buffer size :  256 bytes */
#define DMAC_BUFF_SIZE_1024  (1024)      /* Transfer buffer size : 1024 bytes */
#define DMAC_BUFF_SIZE_4096  (4096)      /* Transfer buffer size : 4096 bytes */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* ==== Prototype declaration ==== */
int32_t DMAC_AutoReq_Init(uint32_t channel, const dmac_transinfo_t * trans_info, 
                                 uint32_t dmamode, uint32_t continuation);
void    DMAC3_AutoReqInit(const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation);
void    Sample_DMAC3_Interrupt(uint32_t int_sense);

/* ==== Global variables ==== */
/* DMAC transfer completion interrupt notification flag */
static volatile uint8_t  dmac_autoreq_trans_flg[DMAC_CH_TOTAL];

/* Transfer source buffer : allocated to L1 cache enabled area in large-capacity on-chip RAM space */
#ifdef __CC_ARM
#pragma arm section zidata = "BSS_DMAC_SAMPLE_INTERNAL_RAM"
static uint8_t dmac_src_data_internalram_1[DMAC_BUFF_SIZE_4096];
static uint8_t dmac_src_data_internalram_2[DMAC_BUFF_SIZE_1024];
static uint8_t dmac_src_data_internalram_3[DMAC_BUFF_SIZE_256];
#pragma arm section zidata
#endif
#ifdef __ICCARM__
static uint8_t dmac_src_data_internalram_1[DMAC_BUFF_SIZE_4096] @ "BSS_DMAC_SAMPLE_INTERNAL_RAM";
static uint8_t dmac_src_data_internalram_2[DMAC_BUFF_SIZE_1024] @ "BSS_DMAC_SAMPLE_INTERNAL_RAM";
static uint8_t dmac_src_data_internalram_3[DMAC_BUFF_SIZE_256]  @ "BSS_DMAC_SAMPLE_INTERNAL_RAM";
#endif
#ifdef __GNUC__
static uint8_t dmac_src_data_internalram_1[DMAC_BUFF_SIZE_4096] __attribute__ ((section ("BSS_DMAC_SAMPLE_INTERNAL_RAM")));
static uint8_t dmac_src_data_internalram_2[DMAC_BUFF_SIZE_1024] __attribute__ ((section ("BSS_DMAC_SAMPLE_INTERNAL_RAM")));
static uint8_t dmac_src_data_internalram_3[DMAC_BUFF_SIZE_256]  __attribute__ ((section ("BSS_DMAC_SAMPLE_INTERNAL_RAM")));
#endif

/* Transfer destination buffer : allocated to L1 cache and L2 cache enabled area in CS2 SDRAM space */
#ifdef __CC_ARM
#pragma arm section zidata = "BSS_DMAC_SAMPLE_SDRAM"
static uint8_t dmac_dst_data_sdram_1[DMAC_BUFF_SIZE_4096];
static uint8_t dmac_dst_data_sdram_2[DMAC_BUFF_SIZE_1024];
static uint8_t dmac_dst_data_sdram_3[DMAC_BUFF_SIZE_256];
#pragma arm section zidata
#endif
#ifdef __ICCARM__
static uint8_t dmac_dst_data_sdram_1[DMAC_BUFF_SIZE_4096] @ "BSS_DMAC_SAMPLE_SDRAM";
static uint8_t dmac_dst_data_sdram_2[DMAC_BUFF_SIZE_1024] @ "BSS_DMAC_SAMPLE_SDRAM";
static uint8_t dmac_dst_data_sdram_3[DMAC_BUFF_SIZE_256]  @ "BSS_DMAC_SAMPLE_SDRAM";
#endif
#ifdef __GNUC__
static uint8_t dmac_dst_data_sdram_1[DMAC_BUFF_SIZE_4096] __attribute__ ((section ("BSS_DMAC_SAMPLE_SDRAM")));
static uint8_t dmac_dst_data_sdram_2[DMAC_BUFF_SIZE_1024] __attribute__ ((section ("BSS_DMAC_SAMPLE_SDRAM")));
static uint8_t dmac_dst_data_sdram_3[DMAC_BUFF_SIZE_256]  __attribute__ ((section ("BSS_DMAC_SAMPLE_SDRAM")));
#endif


/******************************************************************************
* Function Name: Sample_DMAC_AutoReq
* Description  : Transfers the large-capacity on-chip RAM data to SDRAM in CS2 
*              : space using DMAC channel 3 in the register mode for DMA mode 
*              : and in the auto-request for transfer request. Sets the next DMA 
*              : transfer information before the DMA transfer is completed so 
*              : that DMA transfer can be executed continuously after transfer 
*              : completion by activating DMA transfer only once. Total of three 
*              : transfers are executed.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal
*              : char_t ** argv : Pointer to the command input from the terminal
* Return Value : COMMAND_SUCCESS : Succeeded in DMAC sample code processing
*              : COMMAND_ERROR   : Failed in DMAC sample code processing
******************************************************************************/
int32_t Sample_DMAC_AutoReq(int32_t argc, char_t ** argv)
{
    dmac_transinfo_t       buff;
    dmac_cache_prot_info_t info;
    int32_t  ret;
    uint32_t continuation;
    uint32_t dmamode;
    uint32_t channel;
    uint32_t src_addr;
    uint32_t dst_addr;
    uint32_t count;

    printf("\n");
    printf("<DMA transfer information>\n");
    printf("1st transfer\n");
    printf(" - Source Address      : H'%08X\n", (uint32_t)dmac_src_data_internalram_1);
    printf(" - Destination Address : H'%08X\n", (uint32_t)dmac_dst_data_sdram_1);
    printf(" - Transaction Byte    : %d\n", DMAC_BUFF_SIZE_4096);
    printf("2nd transfer\n");
    printf(" - Source Address      : H'%08X\n", (uint32_t)dmac_src_data_internalram_2);
    printf(" - Destination Address : H'%08X\n", (uint32_t)dmac_dst_data_sdram_2);
    printf(" - Transaction Byte    : %d\n", DMAC_BUFF_SIZE_1024);
    printf("3rd transfer\n");
    printf(" - Source Address      : H'%08X\n", (uint32_t)dmac_src_data_internalram_3);
    printf(" - Destination Address : H'%08X\n", (uint32_t)dmac_dst_data_sdram_3);
    printf(" - Transaction Byte    : %d\n", DMAC_BUFF_SIZE_256);
    printf("\n");

    /* ==== Memory initialization ==== */
    /* ---- Transfer source : L1 cache disabled area in large-capacity on-chip RAM space ---- */
    DMAC_InitMemory8bit(dmac_src_data_internalram_1, 0x5A, DMAC_BUFF_SIZE_4096, 0);
    DMAC_InitMemory8bit(dmac_src_data_internalram_2, 0xA5, DMAC_BUFF_SIZE_1024, 0);
    DMAC_InitMemory8bit(dmac_src_data_internalram_3, 0x00, DMAC_BUFF_SIZE_256,  1);
    /* ---- Transfer destination : L1 cache disabled area in CS2 SDRAM space ---- */
    DMAC_InitMemory8bit(dmac_dst_data_sdram_1, 0x00, DMAC_BUFF_SIZE_4096, 0);
    DMAC_InitMemory8bit(dmac_dst_data_sdram_2, 0x00, DMAC_BUFF_SIZE_1024, 0);
    DMAC_InitMemory8bit(dmac_dst_data_sdram_3, 0x00, DMAC_BUFF_SIZE_256,  0);
    /* ---- Variables ---- */
    /* Initialization of DMA transfer completion notification flag */
    dmac_autoreq_trans_flg[DMAC_CH_3]   = DMAC_FLG_OFF;

    /* Write back and flush all L1 data cache */
    L1_D_CacheWritebackFlushAll();

    /* ==== DMAC initialization ==== */
    continuation   = DMAC_SAMPLE_CONTINUATION;  /* Continuous transfer (use REN bit) */
    dmamode        = DMAC_MODE_REGISTER;        /* Register mode                     */
    channel        = DMAC_CH_3;                 /* Use DMAC channel 3                */
    buff.src_addr  = (uint32_t)dmac_src_data_internalram_1; /* Start address of transfer source      */
    buff.dst_addr  = (uint32_t)dmac_dst_data_sdram_1;       /* Start address of transfer destination */
    buff.count     = DMAC_BUFF_SIZE_4096;       /* Total transfer byte count                       */
    buff.src_size  = DMAC_TRANS_SIZE_8;         /* Transfer source transfer size                   */
    buff.dst_size  = DMAC_TRANS_SIZE_8;         /* Transfer destination transfer size              */
    buff.saddr_dir = DMAC_TRANS_ADR_INC;        /* Transfer source address counting direction      */
    buff.daddr_dir = DMAC_TRANS_ADR_INC;        /* Transfer destination address counting direction */
    Sample_DMAC_GetMemoryType(dmac_src_data_internalram_1, &(info.src_prot), &(info.src_attrib));
    Sample_DMAC_GetMemoryType(dmac_dst_data_sdram_1, &(info.dst_prot), &(info.dst_attrib));

    ret = DMAC_AutoReq_Init(channel, &buff, dmamode, continuation);
    DMAC_InitCacheProt(channel, &info);         /* Initialize CHEXT_3 register */
    if (-1 == ret)
    {
        printf("error: DMA transfer initialize error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Set for the next DMA transfer (2nd DMA transfer) information ==== */
    src_addr = (uint32_t)dmac_src_data_internalram_2;
    dst_addr = (uint32_t)dmac_dst_data_sdram_2;
    count    = DMAC_BUFF_SIZE_1024;

    ret = DMAC_Load_Set(channel, src_addr, dst_addr, count);
    if (-1 == ret)
    {
        printf("error: DMA transfer load error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Start DMAC transfer ==== */
    ret = DMAC_Open(channel, DMAC_REQ_MODE_SOFT);
    if (-1 == ret)
    {
        printf("error: DMA transfer open error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Wait for the 1st DMA transfer completion ==== */
    while (dmac_autoreq_trans_flg[DMAC_CH_3] == DMAC_FLG_OFF)
    {
        /* Wait */
    }
    /* Initialization of DMA transfer completion notification flag */
    dmac_autoreq_trans_flg[DMAC_CH_3] = DMAC_FLG_OFF;

    /* ==== Set for the next DMA transfer (3rd DMA transfer) information ==== */
    src_addr = (uint32_t)dmac_src_data_internalram_3;
    dst_addr = (uint32_t)dmac_dst_data_sdram_3;
    count    = DMAC_BUFF_SIZE_256;

    ret = DMAC_Reload_Set(channel, src_addr, dst_addr, count);
    if (-1 == ret)
    {
        printf("error: DMA transfer reload error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Wait for the 2nd DMA transfer completion ==== */
    while (dmac_autoreq_trans_flg[DMAC_CH_3] == DMAC_FLG_OFF)
    {
        /* Wait */
    }
    /* Initialization of DMA transfer completion notification flag */
    dmac_autoreq_trans_flg[DMAC_CH_3] = DMAC_FLG_OFF;

    /* ==== Wait for the 3rd DMA transfer completion ==== */
    while (dmac_autoreq_trans_flg[DMAC_CH_3] == DMAC_FLG_OFF)
    {
        /* Wait */
    }
    /* Initialization of DMA transfer completion notification flag */
    dmac_autoreq_trans_flg[DMAC_CH_3] = DMAC_FLG_OFF;

    printf("DMA transfer complete.\n");
    printf("\n");

    /* Flush all L1 data cache */
    /* Measure of the problem of cashing data before DMA transmission */
    /* because of the transfer buffer read by prefetch                */
    L1_D_CacheFlushLine(dmac_dst_data_sdram_1, sizeof(dmac_dst_data_sdram_1));
    L1_D_CacheFlushLine(dmac_dst_data_sdram_2, sizeof(dmac_dst_data_sdram_2));
    L1_D_CacheFlushLine(dmac_dst_data_sdram_3, sizeof(dmac_dst_data_sdram_3));

    /* ==== Verify ==== */
    ret = DMAC_VerifyMemory8bit(dmac_src_data_internalram_1, dmac_dst_data_sdram_1, DMAC_BUFF_SIZE_4096);
    if (-1 == ret)
    {
        printf("error: 1st transfer verify error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }
    ret = DMAC_VerifyMemory8bit(dmac_src_data_internalram_2, dmac_dst_data_sdram_2, DMAC_BUFF_SIZE_1024);
    if (-1 == ret)
    {
        printf("error: 2nd transfer verify error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }
    ret = DMAC_VerifyMemory8bit(dmac_src_data_internalram_3, dmac_dst_data_sdram_3, DMAC_BUFF_SIZE_256);
    if (-1 == ret)
    {
        printf("error: 3rd transfer verify error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    printf("Verify complete.\n");
    printf("\n");

    return COMMAND_SUCCESS;
}

/******************************************************************************
* Function Name: Sample_DMAC_AutoReqSingle
* Description  : Transfers the large-capacity on-chip RAM data to the SDRAM in 
*              : CS2 space using the auto-request for transfer request in the 
*              : register mode for DMA mode. Sets and enables the next DMA 
*              : transfer when DMA transfer is completed. Activates DMA for 
*              : each transfer so that total of three transfers can be executed. 
*              : 4096 bytes, 1024 bytes, and 256 bytes of data is transferred 
*              : in order.
* Arguments    : int32_t   argc : The number of command arguments input from 
*              :                : the terminal
*              : char_t ** argv : Pointer to the command input from the terminal
* Return Value : COMMAND_SUCCESS : Succeeded in DMAC sample code processing
*              : COMMAND_ERROR   : Failed in DMAC sample code processing
******************************************************************************/
int32_t Sample_DMAC_AutoReqSingle(int32_t argc, char_t ** argv)
{
    dmac_transinfo_t       buff;
    dmac_cache_prot_info_t info;
    int32_t  ret;
    uint32_t continuation;
    uint32_t dmamode;
    uint32_t channel;
    uint32_t src_addr;
    uint32_t dst_addr;
    uint32_t count;

    printf("\n");
    printf("<DMA transfer information>\n");
    printf("1st transfer\n");
    printf(" - Source Address      : H'%08X\n", (uint32_t)dmac_src_data_internalram_1);
    printf(" - Destination Address : H'%08X\n", (uint32_t)dmac_dst_data_sdram_1);
    printf(" - Transaction Byte    : %d\n", DMAC_BUFF_SIZE_4096);
    printf("2nd transfer\n");
    printf(" - Source Address      : H'%08X\n", (uint32_t)dmac_src_data_internalram_2);
    printf(" - Destination Address : H'%08X\n", (uint32_t)dmac_dst_data_sdram_2);
    printf(" - Transaction Byte    : %d\n", DMAC_BUFF_SIZE_1024);
    printf("3rd transfer\n");
    printf(" - Source Address      : H'%08X\n", (uint32_t)dmac_src_data_internalram_3);
    printf(" - Destination Address : H'%08X\n", (uint32_t)dmac_dst_data_sdram_3);
    printf(" - Transaction Byte    : %d\n", DMAC_BUFF_SIZE_256);
    printf("\n");

    /* ==== Memory initialization ==== */
    /* ---- Transfer source : L1 cache disabled area in large-capacity on-chip RAM space ---- */
    DMAC_InitMemory8bit(dmac_src_data_internalram_1, 0x5A, DMAC_BUFF_SIZE_4096, 0);
    DMAC_InitMemory8bit(dmac_src_data_internalram_2, 0xA5, DMAC_BUFF_SIZE_1024, 0);
    DMAC_InitMemory8bit(dmac_src_data_internalram_3, 0x00, DMAC_BUFF_SIZE_256,  1);
    /* ---- Transfer destination : L1 cache disabled area in CS2 SDRAM space ---- */
    DMAC_InitMemory8bit(dmac_dst_data_sdram_1, 0x00, DMAC_BUFF_SIZE_4096, 0);
    DMAC_InitMemory8bit(dmac_dst_data_sdram_2, 0x00, DMAC_BUFF_SIZE_1024, 0);
    DMAC_InitMemory8bit(dmac_dst_data_sdram_3, 0x00, DMAC_BUFF_SIZE_256,  0);
    /* ---- Variables ---- */
    /* Initialization of DMA transfer completion notification flag */
    dmac_autoreq_trans_flg[DMAC_CH_3]   = DMAC_FLG_OFF;

    /* Write back and flush all L1 data cache */
    L1_D_CacheWritebackFlushAll();

    /* ==== DMAC initialization ==== */
    continuation   = DMAC_SAMPLE_SINGLE;        /* Single transfer    */
    dmamode        = DMAC_MODE_REGISTER;        /* Register mode      */
    channel        = DMAC_CH_3;                 /* Use DMAC channel 3 */
    buff.src_addr  = (uint32_t)dmac_src_data_internalram_1; /* Start address of transfer source      */
    buff.dst_addr  = (uint32_t)dmac_dst_data_sdram_1;       /* Start address of transfer destination */
    buff.count     = DMAC_BUFF_SIZE_4096;       /* Total transfer byte count                       */
    buff.src_size  = DMAC_TRANS_SIZE_8;         /* Transfer source transfer size                   */
    buff.dst_size  = DMAC_TRANS_SIZE_8;         /* Transfer destination transfer size              */
    buff.saddr_dir = DMAC_TRANS_ADR_INC;        /* Transfer source address counting direction      */
    buff.daddr_dir = DMAC_TRANS_ADR_INC;        /* Transfer destination address counting direction */
    Sample_DMAC_GetMemoryType(dmac_src_data_internalram_1, &(info.src_prot), &(info.src_attrib));
    Sample_DMAC_GetMemoryType(dmac_dst_data_sdram_1, &(info.dst_prot), &(info.dst_attrib));

    ret = DMAC_AutoReq_Init(channel, &buff, dmamode, continuation);
    DMAC_InitCacheProt(channel, &info);         /* Initialize CHEXT_3 register */
    if (-1 == ret)
    {
        printf("error: DMA transfer initialize error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Start the 1st DMA transfer ==== */
    ret = DMAC_Open(channel, DMAC_REQ_MODE_SOFT);
    if (-1 == ret)
    {
        printf("error: DMA transfer open error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Wait for the 1st DMA transfer completion ==== */
    while (dmac_autoreq_trans_flg[DMAC_CH_3] == DMAC_FLG_OFF)
    {
        /* Wait */
    }
    /* Initialization of DMA transfer completion notification flag */
    dmac_autoreq_trans_flg[DMAC_CH_3] = DMAC_FLG_OFF;

    /* ==== Set the next DMA transfer (2nd DMA transfer) information ==== */
    src_addr = (uint32_t)dmac_src_data_internalram_2;
    dst_addr = (uint32_t)dmac_dst_data_sdram_2;
    count    = DMAC_BUFF_SIZE_1024;

    ret = DMAC_Load_Set(channel, src_addr, dst_addr, count);
    if (-1 == ret)
    {
        printf("error: DMA transfer load error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Start the 2nd DMA transfer ==== */
    ret = DMAC_Open(channel, DMAC_REQ_MODE_SOFT);
    if (-1 == ret)
    {
        printf("error: DMA transfer open error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Wait for the 2nd DMA transfer completion ==== */
    while (dmac_autoreq_trans_flg[DMAC_CH_3] == DMAC_FLG_OFF)
    {
        /* Wait */
    }
    /* Initialization of DMA transfer completion notification flag */
    dmac_autoreq_trans_flg[DMAC_CH_3] = DMAC_FLG_OFF;

    /* ==== Set the next DMA transfer (3rd DMA transfer) information ==== */
    src_addr = (uint32_t)dmac_src_data_internalram_3;
    dst_addr = (uint32_t)dmac_dst_data_sdram_3;
    count    = DMAC_BUFF_SIZE_256;

    ret = DMAC_Load_Set(channel, src_addr, dst_addr, count);
    if (-1 == ret)
    {
        printf("error: DMA transfer load error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Start the 3rd DMA transfer ==== */
    ret = DMAC_Open(channel, DMAC_REQ_MODE_SOFT);
    if (-1 == ret)
    {
        printf("error: DMA transfer open error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    /* ==== Wait for the 3rd DMA transfer completion ==== */
    while (dmac_autoreq_trans_flg[DMAC_CH_3] == DMAC_FLG_OFF)
    {
        /* Wait */
    }
    /* Initialization of DMA transfer completion notification flag */
    dmac_autoreq_trans_flg[DMAC_CH_3] = DMAC_FLG_OFF;

    printf("DMA transfer complete.\n");
    printf("\n");

    /* Flush all L1 data cache */
    /* Measure of the problem of cashing data before DMA transmission */
    /* because of the transfer buffer read by prefetch                */
    L1_D_CacheFlushLine(dmac_dst_data_sdram_1, sizeof(dmac_dst_data_sdram_1));
    L1_D_CacheFlushLine(dmac_dst_data_sdram_2, sizeof(dmac_dst_data_sdram_2));
    L1_D_CacheFlushLine(dmac_dst_data_sdram_3, sizeof(dmac_dst_data_sdram_3));
    /* ==== Verify ==== */
    ret = DMAC_VerifyMemory8bit(dmac_src_data_internalram_1, dmac_dst_data_sdram_1, DMAC_BUFF_SIZE_4096);
    if (-1 == ret)
    {
        printf("error: 1st transfer verify error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }
    ret = DMAC_VerifyMemory8bit(dmac_src_data_internalram_2, dmac_dst_data_sdram_2, DMAC_BUFF_SIZE_1024);
    if (-1 == ret)
    {
        printf("error: 2nd transfer verify error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }
    ret = DMAC_VerifyMemory8bit(dmac_src_data_internalram_3, dmac_dst_data_sdram_3, DMAC_BUFF_SIZE_256);
    if (-1 == ret)
    {
        printf("error: 3rd transfer verify error.\n");
        printf("\n");
        return COMMAND_ERROR;
    }

    printf("Verify complete.\n");
    printf("\n");

    return COMMAND_SUCCESS;
}

/******************************************************************************
* Function Name: DMAC_AutoReq_Init
* Description  : Executes DMAC initial setting in auto-request for DMA transfer 
*              : request. The initial setting for DMAC channel 3 is executed 
*              : in this sample program.
* Arguments    : uint32_t channel      : DMAC channel number
*              : dmac_transinfo_t * trans_info : Setting information to DMAC 
*              :                               : register
*              : uint32_t dmamode      : DMA mode (only for DMAC_MODE_REGISTER)
*              : uint32_t continuation : Set continuous transfer to be valid 
*              :                       : after DMA transfer has been completed
*              :         DMAC_SAMPLE_CONTINUATION : Execute continuous transfer
*              :         DMAC_SAMPLE_SINGLE       : Do not execute continuous 
*              :                                  : transfer
* Return Value :  0 : Succeeded in initial setting processing
*              : -1 : Argument error
******************************************************************************/
int32_t DMAC_AutoReq_Init(uint32_t channel, const dmac_transinfo_t * trans_info, 
                                 uint32_t dmamode, uint32_t continuation)
{
    int32_t ret;

    /* ==== Argument check ==== */
    ret = DMAC_CheckArg(channel, trans_info, dmamode, continuation);
    if (-1 == ret)
    {
        return -1;        /* Argument error */
    }

    /* ==== DMAC initialization ==== */
    switch (channel)
    {
        case 3:
            DMAC3_AutoReqInit(trans_info, dmamode, continuation);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return 0;
}

/******************************************************************************
* Function Name: DMAC3_AutoReqInit
* Description  : Sets DMAC channel 3 to the register mode for DMA mode, and 
*              : the auto-request for transfer request. Executes DMA initial 
*              : setting using the DMA information specified the argument 
*              : *trans_info and the enabled/disabled continuous transfer 
*              : specified by the argument continuation. 
*              : Registers DMAC channel 3 interrupt handler function, and 
*              : sets the interrupt priority level. Then enables transfer 
*              : completion interrupt.
* Arguments    : dmac_transinfo_t * trans_info : Setting information to DMAC 
*              :                               : register
*              : uint32_t dmamode : DMA mode (only for DMAC_MODE_REGISTER)
*              : uint32_t continuation : Set continuous transfer to be valid 
*              :                       : after DMA transfer has been completed
*              :         DMAC_SAMPLE_CONTINUATION : Execute continuous transfer
*              :         DMAC_SAMPLE_SINGLE       : Do not execute continuous 
*              :                                  : transfer
* Return Value : None
******************************************************************************/
void DMAC3_AutoReqInit(const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation)
{
    /* ==== Register mode ==== */
    if (DMAC_MODE_REGISTER == dmamode)
    {
        /* ==== Next0 register set ==== */
        DMAC3.N0SA_n = trans_info->src_addr;    /* Start address of transfer source      */
        DMAC3.N0DA_n = trans_info->dst_addr;    /* Start address of transfer destination */
        DMAC3.N0TB_n = trans_info->count;       /* Total transfer byte count             */

        /* DAD : Transfer destination address counting direction */
        /* SAD : Transfer source address counting direction      */
        /* DDS : Transfer destination transfer size              */
        /* SDS : Transfer source transfer size                   */
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, trans_info->daddr_dir, DMAC3_CHCFG_n_DAD_SHIFT, DMAC3_CHCFG_n_DAD);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, trans_info->saddr_dir, DMAC3_CHCFG_n_SAD_SHIFT, DMAC3_CHCFG_n_SAD);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, trans_info->dst_size,  DMAC3_CHCFG_n_DDS_SHIFT, DMAC3_CHCFG_n_DDS);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, trans_info->src_size,  DMAC3_CHCFG_n_SDS_SHIFT, DMAC3_CHCFG_n_SDS);

        /* DMS  : Register mode                            */
        /* RSEL : Select Next0 register set                */
        /* SBE  : No discharge of buffer data when aborted */
        /* DEM  : No DMA interrupt mask                    */
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_DMS_SHIFT,  DMAC3_CHCFG_n_DMS);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_RSEL_SHIFT, DMAC3_CHCFG_n_RSEL);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_SBE_SHIFT,  DMAC3_CHCFG_n_SBE);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_DEM_SHIFT,  DMAC3_CHCFG_n_DEM);

        /* ---- Continuous transfer ---- */
        if (DMAC_SAMPLE_CONTINUATION == continuation)
        {
            /* REN : Execute continuous transfer                        */
            /* RSW : Change register set when DMA transfer is completed */
            RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 1, DMAC3_CHCFG_n_REN_SHIFT, DMAC3_CHCFG_n_REN);
            RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 1, DMAC3_CHCFG_n_RSW_SHIFT, DMAC3_CHCFG_n_RSW);
        }
        /* ---- Single transfer ---- */
        else
        {
            /* REN : Do not execute continuous transfer                        */
            /* RSW : Do not change register set when DMA transfer is completed */
            RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_REN_SHIFT, DMAC3_CHCFG_n_REN);
            RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_RSW_SHIFT, DMAC3_CHCFG_n_RSW);
        }

        /* TM  : Block transfer        */
        /* AM  : DMA activation by STG */
        /* SEL : Channel setting       */
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 1, DMAC3_CHCFG_n_TM_SHIFT,  DMAC3_CHCFG_n_TM);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 4, DMAC3_CHCFG_n_AM_SHIFT,  DMAC3_CHCFG_n_AM);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 3, DMAC3_CHCFG_n_SEL_SHIFT, DMAC3_CHCFG_n_SEL);

        /* LVL, HIEN, LOWN, REQD : Initial value */
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_LVL_SHIFT,  DMAC3_CHCFG_n_LVL);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_HIEN_SHIFT, DMAC3_CHCFG_n_HIEN);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_LOEN_SHIFT, DMAC3_CHCFG_n_LOEN);
        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 0, DMAC3_CHCFG_n_REQD_SHIFT, DMAC3_CHCFG_n_REQD);

        /* MID, RID : Initial value */
        RZA_IO_RegWrite_32(&DMAC23.DMARS, 0, DMAC23_DMARS_CH3_MID_SHIFT, DMAC23_DMARS_CH3_MID);
        RZA_IO_RegWrite_32(&DMAC23.DMARS, 0, DMAC23_DMARS_CH3_RID_SHIFT, DMAC23_DMARS_CH3_RID);

        /* PR : Round robin mode */
        RZA_IO_RegWrite_32(&DMAC07.DCTRL_0_7, 1, DMAC07_DCTRL_0_7_PR_SHIFT, DMAC07_DCTRL_0_7_PR);
    }

    /* ==== Link mode ==== */
    else
    {

    }

    /* ==== Register DMAC channel 3 interrupt handler ==== */
    R_INTC_RegistIntFunc(INTC_ID_DMAINT3, Sample_DMAC3_Interrupt);

    /* ==== Set priority of DMAC channel 3 interrupt to 1 ==== */
    R_INTC_SetPriority(INTC_ID_DMAINT3, 1);

    /* ==== Validate DMAC channel 3 interrupt ==== */
    R_INTC_Enable(INTC_ID_DMAINT3);
}

/******************************************************************************
* Function Name: Sample_DMAC3_Interrupt
* Description  : This is the interrupt handler that runs when the DMAC channel 
*              : 3 transfer end interrupt is accepted. It performs the processing 
*              : to notify that the DMAC channel 3 transfer end interrupt has 
*              : generated.
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void Sample_DMAC3_Interrupt(uint32_t int_sense)
{
    /* ==== DMA transfer completion ==== */
    dmac_autoreq_trans_flg[DMAC_CH_3] = DMAC_FLG_ON;
}


/* End of File */

