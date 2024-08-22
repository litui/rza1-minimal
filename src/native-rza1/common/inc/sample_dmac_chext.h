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
/******************************************************************************
* File Name    : sample_dmac_chext.h
* $Rev: 1349 $
* $Date:: 2015-03-03 18:16:15 +0900#$
* Description  : DMAC sample program header
******************************************************************************/
#ifndef SAMPLE_DMAC_CHEXT_H
#define SAMPLE_DMAC_CHEXT_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct dmac_cache_prot_info
{
    uint32_t src_prot;      /* Source protection           */
    uint32_t dst_prot;      /* Destination protection      */
    uint32_t src_attrib;    /* Source cache attribute      */
    uint32_t dst_attrib;    /* Destination cache attribute */
} dmac_cache_prot_info_t;

/******************************************************************************
Macro definitions
******************************************************************************/
/* ==== Protection Unit Support (Protection Level) ==== */
#define DMAC_PROT_SECURE_NORMAL             (0)     /* Secure access     (Data access, Unprivileged access) */
#define DMAC_PROT_NONSECURE_NORMAL          (2)     /* Non-secure access (Data access, Unprivileged access) */

/* ==== Chache Support (Transaction attribute) ==== */
#define DMAC_CACHE_ATTRIB_L2_CACHEABLE      (0xF)   /* for external bus space (Cacheable normal memory)     */
#define DMAC_CACHE_ATTRIB_L2_NON_CACHEABLE  (0x3)   /* for external bus space (Non-cacheable normal memory) */
#define DMAC_CACHE_ATTRIB_STRONGLY          (0x0)   /* for external bus space (Strongly ordered memory)     */
#define DMAC_CACHE_ATTRIB_DEVICE            (0x1)   /* for external bus space (Device memory)               */
#define DMAC_CACHE_ATTRIB_INTERNAL_AREA     (0x0)   /* for area except external bus space                   */
#define DMAC_CACHE_ATTRIB_L2C_DISABLE       (0x0)   /* when disable L2 cache                                */

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t DMAC_InitCacheProt(uint32_t channel, dmac_cache_prot_info_t * info);
void    Sample_DMAC_GetMemoryType(void * addr, uint32_t * protection, uint32_t * cache_attrib);


#endif  /* SAMPLE_DMAC_CHEXT_H */

/* End of File */
