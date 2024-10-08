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
* File Name   : cache.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Cache maintenance operations
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "cache.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

#ifdef __CC_ARM
#pragma arm section code   = "CODE_CACHE_OPERATION"
#pragma arm section rodata = "CONST_CACHE_OPERATION"
#pragma arm section rwdata = "DATA_CACHE_OPERATION"
#pragma arm section zidata = "BSS_CACHE_OPERATION"
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    DRV_SUCCESS = 0,    /*!< No Error  */
    DRV_ERROR   = (-1), /*!< General error  */
} e_err_code_t;

/******************************************************************************
Macro definitions
******************************************************************************/
#define L1CACHE_FLUSH       (0)         /* L1 cache flush                */
#define L1CACHE_WB          (1)         /* L1 cache write back           */
#define L1CACHE_WB_FLUSH    (2)         /* L1 cache write back and flush */
/* The value to set to the L2 cache maintenance operation registers */
/* (reg7_inv_way, reg7_clean_way, reg7_clean_inv_way).              */
#define L2CACHE_8WAY    (0x000000FFuL)  /* All entries(8way) in the L2 cache */

#define CACHE_PRV_ADDR_MAX                (0xFFFFFFFFuLL)
#define CACHE_PRV_ADDR_MASK               (0xFFFFFFFFuLL)

#define CACHE_PRV_CA9_LINE_SIZE           (0x20u)
#define CACHE_PRV_CA9_LINE_MASK           (0xFFFFFFE0u)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern void L1_I_CacheFlushAllAsm(void);
extern void L1_D_CacheOperationAsm(uint32_t ope);
extern void L1_I_CacheEnableAsm(void);
extern void L1_D_CacheEnableAsm(void);
extern void L1_I_CacheDisableAsm(void);
extern void L1_D_CacheDisableAsm(void);
extern void L1_D_CacheFlushLineAsm(uint32_t addr);
extern void L1BtacEnableAsm(void);
extern void L1BtacDisableAsm(void);
extern void L1PrefetchEnableAsm(void);
extern void L1PrefetchDisableAsm(void);

/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name: L1_I_CacheFlushAll
* Description  : Flush all I cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1_I_CacheFlushAll(void)
{
    /* ==== Invalidate all I cache ==== */
    L1_I_CacheFlushAllAsm();
}

/******************************************************************************
* Function Name: L1_D_CacheFlushAll
* Description  : Flush all D cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1_D_CacheFlushAll(void)
{
    /* ==== Invalidate all D cache by set/way ==== */
    L1_D_CacheOperationAsm(L1CACHE_FLUSH);
}

/******************************************************************************
* Function Name: L1_D_CacheWritebackAll
* Description  : Write back all D cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1_D_CacheWritebackAll(void)
{
    /* ==== Clean all D cache by set/way ==== */
    L1_D_CacheOperationAsm(L1CACHE_WB);
}

/******************************************************************************
* Function Name: L1_D_CacheWritebackFlushAll
* Description  : Write back and flush all D cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1_D_CacheWritebackFlushAll(void)
{
    /* ==== Invalidate and clean all D cache by set/way ==== */
    L1_D_CacheOperationAsm(L1CACHE_WB_FLUSH);
}

/******************************************************************************
* Function Name: L1_I_CacheEnable
* Description  : Enable I cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1_I_CacheEnable(void)
{
    L1_I_CacheEnableAsm();
}

/******************************************************************************
* Function Name: L1_D_CacheEnable
* Description  : Enable D cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1_D_CacheEnable(void)
{
    L1_D_CacheEnableAsm();
}

/******************************************************************************
* Function Name: L1_I_CacheDisable
* Description  : Disable I cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1_I_CacheDisable(void)
{
    L1_I_CacheDisableAsm();
}

/******************************************************************************
* Function Name: L1_D_CacheDisable
* Description  : Disable D cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1_D_CacheDisable(void)
{
    L1_D_CacheDisableAsm();
}

/*******************************************************************************
* Function Name: L1_D_CacheFlushLine
* Description  : Apply invalidate operation to the cache lines which is included
*                in the specified address range.
* Arguments    : line_addr -
*                    Starting address of cache operation (virtual address).
*                size -
*                    The byte size from line_addr.
* Return Value : DRV_SUCCESS : successful
*                DRV_ERROR   : address overflow
*******************************************************************************/
int32_t L1_D_CacheFlushLine(void* line_addr, uint32_t size)
{
    e_err_code_t ret   = DRV_SUCCESS;

    /* Casting void type address to uint32_t is valid because there is no loss of information. */
    uint32_t addr     = (((uint32_t)line_addr) & CACHE_PRV_CA9_LINE_MASK);

    /* Casting void type address to uint64_t is valid because there is no loss of information. */
    uint64_t end_addr = ((uint64_t)((uint32_t)line_addr) & CACHE_PRV_ADDR_MASK) + ((uint64_t)size);

    if (CACHE_PRV_ADDR_MAX < end_addr)
    {
        ret = DRV_ERROR;
    }
    else
    {
        for ( ; addr < end_addr; addr += CACHE_PRV_CA9_LINE_SIZE)
        {
        	L1_D_CacheFlushLineAsm(addr);
        }
    }
    return ret;
}

/******************************************************************************
* Function Name: L1BtacEnable
* Description  : Enable branch prediction.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1BtacEnable(void)
{
    L1BtacEnableAsm();
}

/******************************************************************************
* Function Name: L1BtacDisable
* Description  : Disable branch prediction.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1BtacDisable(void)
{
    L1BtacDisableAsm();
}

/******************************************************************************
* Function Name: L1PrefetchEnable
* Description  : Enable Dside prefetch.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1PrefetchEnable(void)
{
    L1PrefetchEnableAsm();
}

/******************************************************************************
* Function Name: L1PrefetchDisable
* Description  : Disable Dside prefetch.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L1PrefetchDisable(void)
{
    L1PrefetchDisableAsm();
}

/******************************************************************************
* Function Name: L2CacheFlushAll
* Description  : Flush all cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L2CacheFlushAll(void)
{
    /* ==== Invalidate all cache by Way ==== */
    /* Adjusting to sequence documented here: https://developer.arm.com/documentation/ddi0246/h/programmers-model/about-this-programmers-model/initialization-sequence?lang=en */
    L2C.REG7_INV_WAY = (0xffffuL);

    /* Wait until Way bits[7:0] is cleard */
    while ((L2C.REG7_INV_WAY & (0xffffuL)) != 0x00000000uL)
    {
        /* Wait completion */
    }
}

/******************************************************************************
* Function Name: L2CacheEnable
* Description  : Enable L2 cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L2CacheEnable(void)
{
    L2C.REG2_INT_CLEAR      = 0x000001FFuL;    /* Clear the reg2_int_raw_status register */
    L2C.REG9_D_LOCKDOWN0    = 0x00000000uL;

    L2C.REG1_CONTROL = 0x00000001uL;        /* Enable L2 cache */
}

/******************************************************************************
* Function Name: L2CacheDisable
* Description  : Disable L2 cache.
* Arguments    : none
* Return Value : none
******************************************************************************/
void L2CacheDisable(void)
{
    L2C.REG1_CONTROL = 0x00000000uL;        /* Disable L2 cache */
}

/* Borrowed and adjusted routine from Blackmagic Debug project */
void InvalidateAllCaches(void)
{
    uint32_t cache_geometry;
    __asm volatile ("MRC p15, 1, %0, c0, c0, 1" : "=r"(cache_geometry));
    const uint8_t coherence_level = (cache_geometry & 0x07000000U) >> 24U;

    for (uint8_t cache_level = 0; cache_level < coherence_level; ++cache_level) {
        const uint8_t cache_type = (cache_geometry >> (cache_level * 3U)) & 0x07U;

		if ((cache_type & 0x02U) != 0x02U)
			continue;

        /* CSSELR */
        __asm ("MCR p15, 2, %0, c0, c0, 0" :: "r"(cache_level << 1U));

        uint32_t cache_size;
        /* CCSIDR */
        __asm volatile ("MRC p15, 1, %0, c0, c0, 0" : "=r"(cache_size));

        const uint8_t cache_set_shift = (cache_size & 7U) + 4U;
		/* Extract the cache associativity (number of ways) */
		const uint16_t cache_ways = ((cache_size >> 3U) & 0x3ffU) + 1U;
		/* Extract the number of cache sets */
		const uint16_t cache_sets = ((cache_size >> 13U) & 0x7fffU) + 1U;
		/* Calculate how much to shift the cache way number by */
		const uint8_t cache_ways_shift = 32U - ((uint8_t)((sizeof(uint32_t) * 8U) - (uint8_t)__builtin_clz(cache_ways)));
		/* For each set in the cache */
		for (uint16_t cache_set = 0U; cache_set < cache_sets; ++cache_set) {
			/* For each way in the cache */
			for (uint16_t cache_way = 0U; cache_way < cache_ways; ++cache_way) {
                /* DCCISW */
                __asm ("MCR p15, 0, %0, c7, c14, 2" :: "r"(
                    (cache_way << cache_ways_shift) | (cache_set << cache_set_shift) | (cache_level << 1U)
                ));
            }
        }
    }

    /* ICIALLU */
    __asm ("MCR p15, 0, %0, c7, c5, 0" :: "r"(0));
}


/* End of File */

