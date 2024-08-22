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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : sample_dmac_chext.c
* $Rev: 1517 $
* $Date:: 2015-08-25 18:57:27 +0900#$
* Description : DMAC sample program - CHEXT regster setting
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "sample_dmac.h"
#include "sample_dmac_chext.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


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
* Function Name: DMAC_InitCacheProt
* Description  : Executes the initial setting of the CHEXT_n register of the 
*              : DMAC channel specified by the argument channel. Sets the SCA bits, 
*              : DCA bits, and SPR bits according to the contents specified by 
*              : the argument info respectively. The information of the argument 
*              : info is obtained by calling the Sample_DMAC_GetMemoryType function 
*              : before this function is called.
*              : It is necessary to execute the initial setting of the CHEXT_n 
*              : register before a DMA transfer starts.
* Arguments    : uint32_t channel : DMAC channel (0 to 15)
*              : dmac_cache_prot_info_t * info : Setting information to protection 
*              :                               : unit support and cache support 
*              :                               : for the CHEXT_nregister
* Return Value :  0 : Success of the CHEXT_n register initial setting processing
*              : -1 : Argument error
******************************************************************************/
int32_t DMAC_InitCacheProt(uint32_t channel, dmac_cache_prot_info_t * info)
{
    /* ==== Argument check ==== */
    if (channel >= DMAC_CH_TOTAL)
    {
        return -1;      /* Argument error */
    }
    if ((info->src_prot   > 0x7) || (info->dst_prot   > 0x7) || 
        (info->src_attrib > 0xF) || (info->dst_attrib > 0xF))
    {
        return -1;      /* Argument error */
    }

    /* ==== Set protection and cache attribute to the CHEXT_n register ==== */
    switch (channel)
    {
        case DMAC_CH_0:
            RZA_IO_RegWrite_32(&DMAC0.CHEXT_n, info->src_prot,   DMAC0_CHEXT_n_SPR_SHIFT, DMAC0_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC0.CHEXT_n, info->dst_prot,   DMAC0_CHEXT_n_DPR_SHIFT, DMAC0_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC0.CHEXT_n, info->src_attrib, DMAC0_CHEXT_n_SCA_SHIFT, DMAC0_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC0.CHEXT_n, info->dst_attrib, DMAC0_CHEXT_n_DCA_SHIFT, DMAC0_CHEXT_n_DCA);
        break;
        case DMAC_CH_1:
            RZA_IO_RegWrite_32(&DMAC1.CHEXT_n, info->src_prot,   DMAC1_CHEXT_n_SPR_SHIFT, DMAC1_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC1.CHEXT_n, info->dst_prot,   DMAC1_CHEXT_n_DPR_SHIFT, DMAC1_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC1.CHEXT_n, info->src_attrib, DMAC1_CHEXT_n_SCA_SHIFT, DMAC1_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC1.CHEXT_n, info->dst_attrib, DMAC1_CHEXT_n_DCA_SHIFT, DMAC1_CHEXT_n_DCA);
        break;
        case DMAC_CH_2:
            RZA_IO_RegWrite_32(&DMAC2.CHEXT_n, info->src_prot,   DMAC2_CHEXT_n_SPR_SHIFT, DMAC2_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC2.CHEXT_n, info->dst_prot,   DMAC2_CHEXT_n_DPR_SHIFT, DMAC2_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC2.CHEXT_n, info->src_attrib, DMAC2_CHEXT_n_SCA_SHIFT, DMAC2_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC2.CHEXT_n, info->dst_attrib, DMAC2_CHEXT_n_DCA_SHIFT, DMAC2_CHEXT_n_DCA);
        break;
        case DMAC_CH_3:
            RZA_IO_RegWrite_32(&DMAC3.CHEXT_n, info->src_prot,   DMAC3_CHEXT_n_SPR_SHIFT, DMAC3_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC3.CHEXT_n, info->dst_prot,   DMAC3_CHEXT_n_DPR_SHIFT, DMAC3_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC3.CHEXT_n, info->src_attrib, DMAC3_CHEXT_n_SCA_SHIFT, DMAC3_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC3.CHEXT_n, info->dst_attrib, DMAC3_CHEXT_n_DCA_SHIFT, DMAC3_CHEXT_n_DCA);
        break;
        case DMAC_CH_4:
            RZA_IO_RegWrite_32(&DMAC4.CHEXT_n, info->src_prot,   DMAC4_CHEXT_n_SPR_SHIFT, DMAC4_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC4.CHEXT_n, info->dst_prot,   DMAC4_CHEXT_n_DPR_SHIFT, DMAC4_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC4.CHEXT_n, info->src_attrib, DMAC4_CHEXT_n_SCA_SHIFT, DMAC4_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC4.CHEXT_n, info->dst_attrib, DMAC4_CHEXT_n_DCA_SHIFT, DMAC4_CHEXT_n_DCA);
        break;
        case DMAC_CH_5:
            RZA_IO_RegWrite_32(&DMAC5.CHEXT_n, info->src_prot,   DMAC5_CHEXT_n_SPR_SHIFT, DMAC5_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC5.CHEXT_n, info->dst_prot,   DMAC5_CHEXT_n_DPR_SHIFT, DMAC5_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC5.CHEXT_n, info->src_attrib, DMAC5_CHEXT_n_SCA_SHIFT, DMAC5_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC5.CHEXT_n, info->dst_attrib, DMAC5_CHEXT_n_DCA_SHIFT, DMAC5_CHEXT_n_DCA);
        break;
        case DMAC_CH_6:
            RZA_IO_RegWrite_32(&DMAC6.CHEXT_n, info->src_prot,   DMAC6_CHEXT_n_SPR_SHIFT, DMAC6_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC6.CHEXT_n, info->dst_prot,   DMAC6_CHEXT_n_DPR_SHIFT, DMAC6_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC6.CHEXT_n, info->src_attrib, DMAC6_CHEXT_n_SCA_SHIFT, DMAC6_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC6.CHEXT_n, info->dst_attrib, DMAC6_CHEXT_n_DCA_SHIFT, DMAC6_CHEXT_n_DCA);
        break;
        case DMAC_CH_7:
            RZA_IO_RegWrite_32(&DMAC7.CHEXT_n, info->src_prot,   DMAC7_CHEXT_n_SPR_SHIFT, DMAC7_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC7.CHEXT_n, info->dst_prot,   DMAC7_CHEXT_n_DPR_SHIFT, DMAC7_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC7.CHEXT_n, info->src_attrib, DMAC7_CHEXT_n_SCA_SHIFT, DMAC7_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC7.CHEXT_n, info->dst_attrib, DMAC7_CHEXT_n_DCA_SHIFT, DMAC7_CHEXT_n_DCA);
        break;
        case DMAC_CH_8:
            RZA_IO_RegWrite_32(&DMAC8.CHEXT_n, info->src_prot,   DMAC8_CHEXT_n_SPR_SHIFT, DMAC8_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC8.CHEXT_n, info->dst_prot,   DMAC8_CHEXT_n_DPR_SHIFT, DMAC8_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC8.CHEXT_n, info->src_attrib, DMAC8_CHEXT_n_SCA_SHIFT, DMAC8_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC8.CHEXT_n, info->dst_attrib, DMAC8_CHEXT_n_DCA_SHIFT, DMAC8_CHEXT_n_DCA);
        break;
        case DMAC_CH_9:
            RZA_IO_RegWrite_32(&DMAC9.CHEXT_n, info->src_prot,   DMAC9_CHEXT_n_SPR_SHIFT, DMAC9_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC9.CHEXT_n, info->dst_prot,   DMAC9_CHEXT_n_DPR_SHIFT, DMAC9_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC9.CHEXT_n, info->src_attrib, DMAC9_CHEXT_n_SCA_SHIFT, DMAC9_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC9.CHEXT_n, info->dst_attrib, DMAC9_CHEXT_n_DCA_SHIFT, DMAC9_CHEXT_n_DCA);
        break;
        case DMAC_CH_10:
            RZA_IO_RegWrite_32(&DMAC10.CHEXT_n, info->src_prot,   DMAC10_CHEXT_n_SPR_SHIFT, DMAC10_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC10.CHEXT_n, info->dst_prot,   DMAC10_CHEXT_n_DPR_SHIFT, DMAC10_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC10.CHEXT_n, info->src_attrib, DMAC10_CHEXT_n_SCA_SHIFT, DMAC10_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC10.CHEXT_n, info->dst_attrib, DMAC10_CHEXT_n_DCA_SHIFT, DMAC10_CHEXT_n_DCA);
        break;
        case DMAC_CH_11:
            RZA_IO_RegWrite_32(&DMAC11.CHEXT_n, info->src_prot,   DMAC11_CHEXT_n_SPR_SHIFT, DMAC11_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC11.CHEXT_n, info->dst_prot,   DMAC11_CHEXT_n_DPR_SHIFT, DMAC11_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC11.CHEXT_n, info->src_attrib, DMAC11_CHEXT_n_SCA_SHIFT, DMAC11_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC11.CHEXT_n, info->dst_attrib, DMAC11_CHEXT_n_DCA_SHIFT, DMAC11_CHEXT_n_DCA);
        break;
        case DMAC_CH_12:
            RZA_IO_RegWrite_32(&DMAC12.CHEXT_n, info->src_prot,   DMAC12_CHEXT_n_SPR_SHIFT, DMAC12_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC12.CHEXT_n, info->dst_prot,   DMAC12_CHEXT_n_DPR_SHIFT, DMAC12_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC12.CHEXT_n, info->src_attrib, DMAC12_CHEXT_n_SCA_SHIFT, DMAC12_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC12.CHEXT_n, info->dst_attrib, DMAC12_CHEXT_n_DCA_SHIFT, DMAC12_CHEXT_n_DCA);
        break;
        case DMAC_CH_13:
            RZA_IO_RegWrite_32(&DMAC13.CHEXT_n, info->src_prot,   DMAC13_CHEXT_n_SPR_SHIFT, DMAC13_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC13.CHEXT_n, info->dst_prot,   DMAC13_CHEXT_n_DPR_SHIFT, DMAC13_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC13.CHEXT_n, info->src_attrib, DMAC13_CHEXT_n_SCA_SHIFT, DMAC13_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC13.CHEXT_n, info->dst_attrib, DMAC13_CHEXT_n_DCA_SHIFT, DMAC13_CHEXT_n_DCA);
        break;
        case DMAC_CH_14:
            RZA_IO_RegWrite_32(&DMAC14.CHEXT_n, info->src_prot,   DMAC14_CHEXT_n_SPR_SHIFT, DMAC14_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC14.CHEXT_n, info->dst_prot,   DMAC14_CHEXT_n_DPR_SHIFT, DMAC14_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC14.CHEXT_n, info->src_attrib, DMAC14_CHEXT_n_SCA_SHIFT, DMAC14_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC14.CHEXT_n, info->dst_attrib, DMAC14_CHEXT_n_DCA_SHIFT, DMAC14_CHEXT_n_DCA);
        break;
        case DMAC_CH_15:
            RZA_IO_RegWrite_32(&DMAC15.CHEXT_n, info->src_prot,   DMAC15_CHEXT_n_SPR_SHIFT, DMAC15_CHEXT_n_SPR);
            RZA_IO_RegWrite_32(&DMAC15.CHEXT_n, info->dst_prot,   DMAC15_CHEXT_n_DPR_SHIFT, DMAC15_CHEXT_n_DPR);
            RZA_IO_RegWrite_32(&DMAC15.CHEXT_n, info->src_attrib, DMAC15_CHEXT_n_SCA_SHIFT, DMAC15_CHEXT_n_SCA);
            RZA_IO_RegWrite_32(&DMAC15.CHEXT_n, info->dst_attrib, DMAC15_CHEXT_n_DCA_SHIFT, DMAC15_CHEXT_n_DCA);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return 0;
}

/******************************************************************************
* Function Name: Sample_DMAC_GetMemoryType
* Description  : Obtains the information to be set to the protection unit support 
*              : and the cache support in the area specified by the argument addr. 
*              : The obtained contents are set to the arguments *protection and 
*              : *cache_attrib. In the sample code, the contents of the protection 
*              : unit support and the cache support, shown in the described table 
*              : in the application note, for the area including the address 
*              : specified by the argument addr, are obtained.
* Arguments    : void * addr             : Address
*              : uint32_t * protection   : Protection
*              : uint32_t * cache_attrib : Chache attribute
* Return Value : None
******************************************************************************/
void Sample_DMAC_GetMemoryType(void * addr, uint32_t * protection, uint32_t * cache_attrib)
{
    uint32_t l2c_enable;

    /* NOTE:                                                                */
    /* In this function, the setting information to the protection unit     */
    /* support and the cache support, for the area including the specified  */
    /* address, is obtained according to the information of the default MMU */
    /* translation table used in the sample code.                           */
    /* When the MMU translation table (set by the file ttb_init.s) is       */
    /* customized, it is necessary to modify this function according to     */
    /* the changed contents of the table.                                   */

    l2c_enable = L2C.REG1_CONTROL;

    /* ==== When enable L2 cache ==== */
    if (1 == l2c_enable)
    {
        /* ---- NOR flash memory ---- */
        if ((uint32_t *)addr < (uint32_t *)0x08000000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_L2_CACHEABLE;
        }
        /* ---- SDRAM ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x10000000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_L2_CACHEABLE;
        }
        /* ---- CS4, CS5 space ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x18000000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_STRONGLY;
        }
        /* ---- Serial flash memory ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x20000000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_L2_CACHEABLE;
        }
        /* ---- On-chip large-capacity RAM ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x20A00000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_INTERNAL_AREA;
        }
        /* ---- Others ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x40000000uL)
        {
            *protection   = DMAC_PROT_SECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_INTERNAL_AREA;
        }
        /* ---- NOR flash memory (Mirror space) ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x48000000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_L2_NON_CACHEABLE;
        }
        /* ---- SDRAM (Mirror space) ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x50000000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_L2_NON_CACHEABLE;
        }
        /* ---- CS4, CS5 space (Mirror space) ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x58000000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_STRONGLY;
        }
        /* ---- Serial flash memory (Mirror space) ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x60000000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_L2_NON_CACHEABLE;
        }
        /* ---- On-chip large-capacity RAM (Mirror space) ---- */
        else if ((uint32_t *)addr < (uint32_t *)0x60A00000uL)
        {
            *protection   = DMAC_PROT_NONSECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_INTERNAL_AREA;
        }
        /* ---- Others ---- */
        else
        {
            *protection   = DMAC_PROT_SECURE_NORMAL;
            *cache_attrib = DMAC_CACHE_ATTRIB_INTERNAL_AREA;
        }
    }
    /* ==== When disable L2 cache ==== */
    else
    {
        /* ---- Others ---- */
        if ((((uint32_t *)addr >= (uint32_t *)0x20A00000uL) &&
             ((uint32_t *)addr <  (uint32_t *)0x40000000uL)) ||
             ((uint32_t *)addr >= (uint32_t *)0x60A00000uL))
        {
            *protection = DMAC_PROT_SECURE_NORMAL;
        }
        /* ---- Except others ---- */
        else
        {
            *protection = DMAC_PROT_NONSECURE_NORMAL;
        }

        *cache_attrib = DMAC_CACHE_ATTRIB_L2C_DISABLE;
    }
}


/* End of File */

