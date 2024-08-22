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
* File Name   : sample_dmac_common.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : DMAC sample program - Common fanctions
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "sample_dmac.h"
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
/* ==== Prototype declaration ==== */
#ifdef DMAC_USE_EXT_REQ
int32_t DMAC0_Open(uint32_t req);
void    DMAC0_Close(uint32_t * remain);
int32_t DMAC0_Load_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count);
int32_t DMAC0_Reload_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count);
#endif /* DMAC_USE_EXT_REQ */

#ifdef DMAC_USE_PERI_REQ
int32_t DMAC1_Open(uint32_t req);
void    DMAC1_Close(uint32_t * remain);
int32_t DMAC1_Load_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count);
int32_t DMAC1_Reload_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count);
#endif /* DMAC_USE_PERI_REQ */

int32_t DMAC3_Open(uint32_t req);
void    DMAC3_Close(uint32_t * remain);
int32_t DMAC3_Load_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count);
int32_t DMAC3_Reload_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count);


/******************************************************************************
* Function Name: DMAC_InitMemory8bit
* Description  : Writes the value specified by the argument data in the 8-bit 
*              : units from the address specified by the argument *addr for 
*              : the times specified by the argument count. 
*              : When the value other than 0 is set for the argument increment, 
*              : increment the argument data every time writing to the memory.
* Arguments    : uint8_t * addr    : Start address of the memory for writing
*              : uint8_t data      : Data to be written
*              : uint32_t coun t   : Writing count (in the 8-bit units)
*              : uint8_t increment : Increment setting for write data
*              :                       0 : Write the data specified by the 
*              :                         : argument data
*              :            Ohter than 0 : Write the data incremented from 
*              :                         : the argument data
* Return Value : None
******************************************************************************/
void DMAC_InitMemory8bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment)
{
    uint32_t offset;

    for (offset = 0; offset < count; offset++)
    {
        if (0 == increment)
        {
            *(addr + offset) = data;
        }
        else
        {
            *(addr + offset) = data++;
        }
    }
}

/******************************************************************************
* Function Name: DMAC_VerifyMemory8bit
* Description  : Compares the area with the size specified by the argument 
*              : count starting with the address specified by the argument 
*              : *addr_1 and the area starting with the address specified by 
*              : the argument *addr_2. Reads it in the8-bit units, and sets 
*              : the compare result for the return value.
* Arguments    : uint8_t * addr_1 : Start address of the memory for comparison
*              : uint8_t * addr_2 : Start address of the memory for comparison
*              : uint32_t count   : Comparison count(in the 8-bit units)
* Return Value :  0 : Verify OK
*              : -1 : Verify Error
******************************************************************************/
int32_t DMAC_VerifyMemory8bit(uint8_t * addr_1, uint8_t * addr_2, uint32_t count)
{
    uint32_t offset;
    int32_t  ret;

    ret = 0;

    for (offset = 0; offset < count; offset++)
    {
        if (*(addr_1 + offset) != *(addr_2 + offset))
        {
            ret = -1;   /* Verify error */
            break;
        }
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC_CheckArg
* Description  : Judges the argument information when DMAC setting function is 
*              : called, and sets the judgment result for the return value.
* Arguments    : uint32_t channel : DMAC channel number
*              : dmac_transinfo_t * trans_info : Setting information for DMAC 
*              :                               : register
*              : uint32_t dmamode : DMA mode
*              : uint32_t continuation : Set continuous transfer to be valid 
*              :                       : after DMA transfer has been completed
* Return Value :  0 : No argument error
*              : -1 : Argument error
******************************************************************************/
int32_t DMAC_CheckArg(uint32_t channel, const dmac_transinfo_t * trans_info, 
                      uint32_t dmamode, uint32_t continuation)
{
    int32_t ret;

    if (channel >= DMAC_CH_TOTAL)
    {
        ret = -1;       /* Argument error */
    }
    else if ((trans_info->src_size  > DMAC_TRANS_SIZE_1024)  ||
             (trans_info->dst_size  > DMAC_TRANS_SIZE_1024)  ||
             (trans_info->saddr_dir > DMAC_TRANS_ADR_FIX) ||
             (trans_info->daddr_dir > DMAC_TRANS_ADR_FIX))
    {
        ret = -1;       /* Argument error */
    }
    else if (dmamode > DMAC_MODE_LINK)
    {
        ret = -1;       /* Argument error */
    }
    else if (continuation > DMAC_SAMPLE_CONTINUATION)
    {
        ret = -1;       /* Argument error */
    }
    else
    {
        ret = 0;        /* No argument error */
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC_Open
* Description  : Enables transfer of the DMA specified by the argument channel.
*              : In this sample program, DMAC channel 3 is enabled to be 
*              : transferred in auto request mode by specifying the 
*              : DMAC_REQ_MODE_SOFT for the argument req.
* Arguments    : uint32_t channel : DMAC channel
*              : uint32_t req     : DMAC request mode
* Return Value :  0 : Succeeded in enabling DMA transfer
*              : -1 : Argument error or failed to enable DMA transfer
******************************************************************************/
int32_t DMAC_Open(uint32_t channel, uint32_t req)
{
    int32_t ret;

    ret = 0;

    /* ==== Argument check ==== */
    if (channel >= DMAC_CH_TOTAL || req > DMAC_REQ_MODE_SOFT)
    {
        return -1;      /* Argument error */
    }

    switch (channel)
    {
#ifdef DMAC_USE_EXT_REQ
        case 0:
            ret = DMAC0_Open(req);
        break;
#endif /* DMAC_USE_EXT_REQ */
#ifdef DMAC_USE_PERI_REQ
        case 1:
            ret = DMAC1_Open(req);
        break;
#endif /* DMAC_USE_PERI_REQ */
        case 3:
            ret = DMAC3_Open(req);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC_Close
* Description  : Aborts transfer of the DMA specified by the argument channel.
*              : DMAC channel 3 transfer is aborted in this sample program.
*              : Returns the remaining transfer byte count at the time of 
*              : DMA transfer abort to the argument *remain.
* Arguments    : uint32_t channel  : DMAC channel
*              : uint32_t * remain : Remaining transfer byte count when 
*              :                   : DMA transfer is aborted.
* Return Value :  0 : Succeeded in DMA transfer abort
*              : -1 : Argument error or failed to enable DMA transfer
******************************************************************************/
int32_t DMAC_Close(uint32_t channel, uint32_t * remain)
{
    /* ==== Argument check ==== */
    if (channel >= DMAC_CH_TOTAL)
    {
        return -1;      /* Argument error */
    }

    switch (channel)
    {
#ifdef DMAC_USE_EXT_REQ
        case 0:
            DMAC0_Close(remain);
        break;
#endif /* DMAC_USE_EXT_REQ */
#ifdef DMAC_USE_PERI_REQ
        case 1:
            DMAC1_Close(remain);
        break;
#endif /* DMAC_USE_PERI_REQ */
        case 3:
            DMAC3_Close(remain);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return 0;
}

/******************************************************************************
* Function Name: DMAC_Load_Set
* Description  : Sets the transfer source address, transfer destination address 
*              : and total transfer byte count respectively specified by the 
*              : argument src_addr, dst_addr, and  count to the DMAC channel 
*              : specified by the argument channel. This function should be
*              : called when DMA transfer of the specified channel is aborted. 
*              : DMAC channel 3 is set in this sample program.
* Arguments    : uint32_t channel  : DMAC channel
*              : uint32_t src_addr : Transfer source address
*              : uint32_t dst_addr : Transfer destination address
*              : uint32_t count    : Total transfer byte count
* Return Value :  0 : Succeeded in DMA setting
*              : -1 : Argument error or DMA specified by the channel is in 
*              :    : operation
******************************************************************************/
int32_t DMAC_Load_Set(uint32_t channel, uint32_t src_addr, uint32_t dst_addr, uint32_t count)
{
    int32_t ret;

    /* ==== Argument check ==== */
    if (channel >= DMAC_CH_TOTAL)
    {
        return -1;      /* Argument error */
    }

    switch (channel)
    {
#ifdef DMAC_USE_EXT_REQ
        case 0:
            ret = DMAC0_Load_Set(src_addr, dst_addr, count);
        break;
#endif /* DMAC_USE_EXT_REQ */
#ifdef DMAC_USE_PERI_REQ
        case 1:
            ret = DMAC1_Load_Set(src_addr, dst_addr, count);
        break;
#endif /* DMAC_USE_PERI_REQ */
        case 3:
            ret = DMAC3_Load_Set(src_addr, dst_addr, count);
        break;
        default:
            /* Do not reach here based on the assumption */
            ret = -1;
        break;
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC_Reload_Set
* Description  : Sets the transfer source address, transfer destination address, 
*              : and total transfer byte count respectively specified by the 
*              : argument src_addr, dst_addr, and count to DMAC channel 
*              : specified by the argument channel as DMA transfer information 
*              : for the next continuous transfer. This function should be 
*              : called when the REN bit becomes "0" at the time of continuous 
*              : transfer while DMA of the specified channel is in operation.
*              : DMAC channel 3 is set in this sample program.
* Arguments    : uint32_t channel  : DMAC channel number
*              : uint32_t src_addr : Transfer source address for reloading
*              : uint32_t dst_addr : Transfer destination address for reloading
*              : uint32_t count    : Total transfer byte count for reloading
* Return Value :  0 : Succeeded in DMA reload setting
*              : -1 : Argument error or DMA transfer specified by the channel 
*              :    : has been completed before reload setting
******************************************************************************/
int32_t DMAC_Reload_Set(uint32_t channel, uint32_t src_addr, uint32_t dst_addr, uint32_t count)
{
    /* NOTE: Use only when executing continuous transfer in register mode. */
    int32_t ret;

    /* ==== Argument check ==== */
    if (channel >= DMAC_CH_TOTAL)
    {
        return -1;      /* Argument error */
    }

    switch (channel)
    {
#ifdef DMAC_USE_EXT_REQ
        case 0:
            ret = DMAC0_Reload_Set(src_addr, dst_addr, count);
        break;
#endif /* DMAC_USE_EXT_REQ */
#ifdef DMAC_USE_PERI_REQ
        case 1:
            ret = DMAC1_Reload_Set(src_addr, dst_addr, count);
        break;
#endif /* DMAC_USE_PERI_REQ */
        case 3:
            ret = DMAC3_Reload_Set(src_addr, dst_addr, count);
        break;
        default:
            /* Do not reach here based on the assumption */
            ret = -1;
        break;
    }

    return ret;
}

#ifdef DMAC_USE_EXT_REQ
/******************************************************************************
* Function Name: DMAC0_Open
* Description  : Enables DMAC channel 0 transfer.
* Arguments    : uint32_t req : DMAC request mode
* Return Value :  0 : Succeeded in enabling DMA transfer
*              : -1 : Failed to enable DMA transfer (due to DMA operation)
******************************************************************************/
int32_t DMAC0_Open(uint32_t req)
{
    int32_t ret;
    volatile uint8_t  dummy;

    /* Transferable? */
    if ((0 == RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_EN_SHIFT,   DMAC0_CHSTAT_n_EN)) &&
        (0 == RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_TACT_SHIFT, DMAC0_CHSTAT_n_TACT)))
    {
        /* Clear Channel Status Register */
        RZA_IO_RegWrite_32(&DMAC0.CHCTRL_n, 1, DMAC0_CHCTRL_n_SWRST_SHIFT, DMAC0_CHCTRL_n_SWRST);
        dummy = RZA_IO_RegRead_32(&DMAC0.CHCTRL_n, DMAC0_CHCTRL_n_SWRST_SHIFT, DMAC0_CHCTRL_n_SWRST);

        /* Enable DMA transfer */
        RZA_IO_RegWrite_32(&DMAC0.CHCTRL_n, 1, DMAC0_CHCTRL_n_SETEN_SHIFT, DMAC0_CHCTRL_n_SETEN);

        /* ---- Request by software ---- */
        if (DMAC_REQ_MODE_SOFT == req)
        {
            /* DMA transfer Request by software */
            RZA_IO_RegWrite_32(&DMAC0.CHCTRL_n, 1, DMAC0_CHCTRL_n_STG_SHIFT, DMAC0_CHCTRL_n_STG);
        }

        ret = 0;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC0_Close
* Description  : Aborts DMAC channel 0 transfer. Returns the remaining transfer 
*              : byte count at the time of DMA transfer abort to the argument 
*              : *remain.
* Arguments    : uint32_t * remain : Remaining transfer byte count when
*              :                   : DMA transfer is aborted
* Return Value :  0 : Succeeded in DMA transfer abort
*              : -1 : Failed to enable DMA transfer
******************************************************************************/
void DMAC0_Close(uint32_t * remain)
{
    uint8_t close_complete;

    close_complete = 0;

    /* ==== Temporarily abort transfer ==== */
    RZA_IO_RegWrite_32(&DMAC0.CHCTRL_n, 1, DMAC0_CHCTRL_n_SETSUS_SHIFT, DMAC0_CHCTRL_n_SETSUS);

    /* Loop until transfer is temporarily aborted */
    while (0 == RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_SUS_SHIFT, DMAC0_CHSTAT_n_SUS))
    {
        if (0 == RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_EN_SHIFT, DMAC0_CHSTAT_n_EN))
        {
            close_complete = 1;
            break;
        }
    }

    if (close_complete == 0)
    {
        /* ==== Abort transfer ==== */
        RZA_IO_RegWrite_32(&DMAC0.CHCTRL_n, 1, DMAC0_CHCTRL_n_CLREN_SHIFT, DMAC0_CHCTRL_n_CLREN);

        while (1 == RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_TACT_SHIFT, DMAC0_CHSTAT_n_TACT))
        {
            /* Loop until transfer is aborted */
        }

        while (1 == RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_EN_SHIFT, DMAC0_CHSTAT_n_EN))
        {
            /* Loop until 0 is se in EN before checking the remaining transfer byte count */
        }
    }

    /* ==== Obtain remaining transfer byte count ==== */
    *remain = DMAC0.CRTB_n;

    /* ==== Clear resource selector ==== */
    RZA_IO_RegWrite_32(&DMAC01.DMARS, 0, DMAC01_DMARS_CH0_MID_SHIFT, DMAC01_DMARS_CH0_MID);
    RZA_IO_RegWrite_32(&DMAC01.DMARS, 0, DMAC01_DMARS_CH0_RID_SHIFT, DMAC01_DMARS_CH0_RID);
}

/******************************************************************************
* Function Name: DMAC0_Load_Set
* Description  : Sets the transfer source address, transfer destination 
*              : address, and total transfer byte count respectively 
*              : specified by the argument src_addr, dst_addr, and count to 
*              : DMAC channel 0 as DMA transfer information. 
*              : Sets the register set selected by the CHCFG_n register 
*              : RSEL bit from the Next0 or Next1 register set. 
*              : This function should be called when DMA transfer of DMAC 
*              : channel 0 is aboted.
* Arguments    : uint32_t src_addr : Transfer source address
*              : uint32_t dst_addr : Transfer destination address
*              : uint32_t count    : Total transfer byte count
* Return Value :  0 : Succeeded in DMA setting
*              : -1 : DMA of DMAC channel 0 is in operation
******************************************************************************/
int32_t DMAC0_Load_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count)
{
    int32_t ret;
    uint8_t reg_set;

    if (0 == RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_EN_SHIFT, DMAC0_CHSTAT_n_EN))
    {
        /* Obtain register set in use */
        reg_set = RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_SR_SHIFT, DMAC0_CHSTAT_n_SR);

        /* Change register set when DMA transfer is completed */
        if (1 == RZA_IO_RegRead_32(&DMAC0.CHCFG_n, DMAC0_CHCFG_n_RSW_SHIFT, DMAC0_CHCFG_n_RSW))
        {
            reg_set ^= 1;
        }

        /* ==== Load ==== */
        if (0 == reg_set)
        {
            /* ---- Next0 Register Set ---- */
            DMAC0.N0SA_n = src_addr;    /* Start address of transfer source      */
            DMAC0.N0DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC0.N0TB_n = count;       /* Total transfer byte count             */
        }
        else
        {
            /* ---- Next1 Register Set ---- */
            DMAC0.N1SA_n = src_addr;    /* Start address of transfer source      */
            DMAC0.N1DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC0.N1TB_n = count;       /* Total transfer byte count             */
        }

        ret = 0;
    }
    else    /* When trying to load during transfer */
    {
        ret = -1;
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC0_Reload_Set
* Description  : Sets the transfer source address, transfer destination 
*              : address, and total transfer byte count respectively specified 
*              : by the argument src_addr, dst_addr, and count to DMAC channel 
*              : 0 as DMA transfer informtion for the next continuous transfer. 
*              : Sets CHCFG_n register REN bit to "1" to enable the continuous 
*              : transfer function.
*              : When the CHCFG_n register RSW bit is set to "1", executes 
*              : reload setting for the register set which is not selected 
*              : by RSEL bit from Next0 or Next1 register set.
*              : When the RSW bit is set to "0", executes reload setting for 
*              : the register set selected by RSEL bit. 
*              : This function should be called when REN bit becomes "0" at 
*              : the time of continuous
*              : transfer while DMA of DMAC channel 0 is in operation.
* Arguments    : uint32_t src_addr : Transfer source address for reloading
*              : uint32_t dst_addr : Transfer destination address for reloading
*              : uint32_t count    : Total transfer byte count for reloading
* Return Value :  0 : Succeeded in DMA reload setting
*              : -1 : DMA of DMAC channel 0 transfer has been completed before 
*              :    : reload setting
******************************************************************************/
int32_t DMAC0_Reload_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count)
{
    int32_t ret;
    uint8_t reg_set;

    if ((1 == RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_EN_SHIFT, DMAC0_CHSTAT_n_EN)) &&
        (0 == RZA_IO_RegRead_32(&DMAC0.CHCFG_n,  DMAC0_CHCFG_n_REN_SHIFT, DMAC0_CHCFG_n_REN)))
    {
        /* Obtain register set in use */
        reg_set = RZA_IO_RegRead_32(&DMAC0.CHSTAT_n, DMAC0_CHSTAT_n_SR_SHIFT, DMAC0_CHSTAT_n_SR);

        /* ==== Reload ==== */
        if (0 == reg_set)
        {
            /* ---- Next1 Register Set ---- */
            DMAC0.N1SA_n = src_addr;    /* Start address of transfer source      */
            DMAC0.N1DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC0.N1TB_n = count;       /* Total transfer byte count             */
        }
        else
        {
            /* ---- Next0 Register Set ---- */
            DMAC0.N0SA_n = src_addr;    /* Start address of transfer source      */
            DMAC0.N0DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC0.N0TB_n = count;       /* Total transfer byte count             */
        }

        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 1, DMAC0_CHCFG_n_REN_SHIFT, DMAC0_CHCFG_n_REN);

        ret = 0;
    }
    else    /* When reloading has been behind */
    {
        ret = -1;
    }

    return ret;
}
#endif /* DMAC_USE_EXT_REQ */

#ifdef DMAC_USE_PERI_REQ
/******************************************************************************
* Function Name: DMAC1_Open
* Description  : Enables DMAC channel 1 transfer.
* Arguments    : uint32_t req : DMAC request mode
* Return Value :  0 : Succeeded in enabling DMA transfer
*              : -1 : Failed to enable DMA transfer (due to DMA operation)
******************************************************************************/
int32_t DMAC1_Open(uint32_t req)
{
    int32_t ret;
    volatile uint8_t  dummy;

    /* Transferable? */
    if ((0 == RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_EN_SHIFT,   DMAC1_CHSTAT_n_EN)) &&
        (0 == RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_TACT_SHIFT, DMAC1_CHSTAT_n_TACT)))
    {
        /* Clear Channel Status Register */
        RZA_IO_RegWrite_32(&DMAC1.CHCTRL_n, 1, DMAC1_CHCTRL_n_SWRST_SHIFT, DMAC1_CHCTRL_n_SWRST);
        dummy = RZA_IO_RegRead_32(&DMAC1.CHCTRL_n, DMAC1_CHCTRL_n_SWRST_SHIFT, DMAC1_CHCTRL_n_SWRST);

        /* Enable DMA transfer */
        RZA_IO_RegWrite_32(&DMAC1.CHCTRL_n, 1, DMAC1_CHCTRL_n_SETEN_SHIFT, DMAC1_CHCTRL_n_SETEN);

        /* ---- Request by software ---- */
        if (DMAC_REQ_MODE_SOFT == req)
        {
            /* DMA transfer Request by software */
            RZA_IO_RegWrite_32(&DMAC1.CHCTRL_n, 1, DMAC1_CHCTRL_n_STG_SHIFT, DMAC1_CHCTRL_n_STG);
        }

        ret = 0;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC1_Close
* Description  : Aborts DMAC channel 1 transfer. Returns the remaining transfer 
*              : byte count at the time of DMA transfer abort to the argument 
*              : *remain.
* Arguments    : uint32_t * remain : Remaining transfer byte count when
*              :                   : DMA transfer is aborted
* Return Value :  0 : Succeeded in DMA transfer abort
*              : -1 : Failed to enable DMA transfer
******************************************************************************/
void DMAC1_Close(uint32_t * remain)
{
    uint8_t close_complete;

    close_complete = 0;

    /* ==== Temporarily abort transfer ==== */
    RZA_IO_RegWrite_32(&DMAC1.CHCTRL_n, 1, DMAC1_CHCTRL_n_SETSUS_SHIFT, DMAC1_CHCTRL_n_SETSUS);

    /* Loop until transfer is temporarily aborted */
    while (0 == RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_SUS_SHIFT, DMAC1_CHSTAT_n_SUS))
    {
        if (0 == RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_EN_SHIFT, DMAC1_CHSTAT_n_EN))
        {
            close_complete = 1;
            break;
        }
    }

    if (close_complete == 0)
    {
        /* ==== Abort transfer ==== */
        RZA_IO_RegWrite_32(&DMAC1.CHCTRL_n, 1, DMAC1_CHCTRL_n_CLREN_SHIFT, DMAC1_CHCTRL_n_CLREN);

        while (1 == RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_TACT_SHIFT, DMAC1_CHSTAT_n_TACT))
        {
            /* Loop until transfer is aborted */
        }

        while (1 == RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_EN_SHIFT, DMAC1_CHSTAT_n_EN))
        {
            /* Loop until 0 is set in EN before checking the remaining transfer byte count */
        }
    }

    /* ==== Obtain remaining transfer byte count ==== */
    *remain = DMAC1.CRTB_n;

    /* ==== Clear resource selector ==== */
    RZA_IO_RegWrite_32(&DMAC01.DMARS, 0, DMAC01_DMARS_CH1_MID_SHIFT, DMAC01_DMARS_CH1_MID);
    RZA_IO_RegWrite_32(&DMAC01.DMARS, 0, DMAC01_DMARS_CH1_RID_SHIFT, DMAC01_DMARS_CH1_RID);
}

/******************************************************************************
* Function Name: DMAC1_Load_Set
* Description  : Sets the transfer source address, transfer destination 
*              : address, and total transfer byte count respectively 
*              : specified by the argument src_addr, dst_addr, and count to 
*              : DMAC channel 1 as DMA transfer information. 
*              : Sets the register set selected by the CHCFG_n register 
*              : RSEL bit from the Next0 or Next1 register set. 
*              : This function should be called when DMA transfer of DMAC 
*              : channel 1 is aboted.
* Arguments    : uint32_t src_addr : Transfer source address
*              : uint32_t dst_addr : Transfer destination address
*              : uint32_t count    : Total transfer byte count
* Return Value :  0 : Succeeded in DMA setting
*              : -1 : DMA of DMAC channel 1 is in operation
******************************************************************************/
int32_t DMAC1_Load_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count)
{
    int32_t ret;
    uint8_t reg_set;

    if (0 == RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_EN_SHIFT, DMAC1_CHSTAT_n_EN))
    {
        /* Obtain register set in use */
        reg_set = RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_SR_SHIFT, DMAC1_CHSTAT_n_SR);

        /* Change register set when DMA transfer is completed */
        if (1 == RZA_IO_RegRead_32(&DMAC1.CHCFG_n, DMAC1_CHCFG_n_RSW_SHIFT, DMAC1_CHCFG_n_RSW))
        {
            reg_set ^= 1;
        }

        /* ==== Load ==== */
        if (0 == reg_set)
        {
            /* ---- Next0 Register Set ---- */
            DMAC1.N0SA_n = src_addr;    /* Start address of transfer source      */
            DMAC1.N0DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC1.N0TB_n = count;       /* Total transfer byte count             */
        }
        else
        {
            /* ---- Next1 Register Set ---- */
            DMAC1.N1SA_n = src_addr;    /* Start address of transfer source      */
            DMAC1.N1DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC1.N1TB_n = count;       /* Total transfer byte count             */
        }

        ret = 0;
    }
    else    /* When trying to load during transfer */
    {
        ret = -1;
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC1_Reload_Set
* Description  : Sets the transfer source address, transfer destination 
*              : address, and total transfer byte count respectively specified 
*              : by the argument src_addr, dst_addr, and count to DMAC channel 
*              : 1 as DMA transfer informtion for the next continuous transfer. 
*              : Sets CHCFG_n register REN bit to "1" to enable the continuous 
*              : transfer function.
*              : When the CHCFG_n register RSW bit is set to "1", executes 
*              : reload setting for the register set which is not selected 
*              : by RSEL bit from Next0 or Next1 register set.
*              : When the RSW bit is set to "0", executes reload setting for 
*              : the register set selected by RSEL bit. 
*              : This function should be called when REN bit becomes "0" at 
*              : the time of continuous
*              : transfer while DMA of DMAC channel 1 is in operation.
* Arguments    : uint32_t src_addr : Transfer source address for reloading
*              : uint32_t dst_addr : Transfer destination address for reloading
*              : uint32_t count    : Total transfer byte count for reloading
* Return Value :  0 : Succeeded in DMA reload setting
*              : -1 : DMA of DMAC channel 1 transfer has been completed before 
*              :    : reload setting
******************************************************************************/
int32_t DMAC1_Reload_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count)
{
    int32_t ret;
    uint8_t reg_set;

    if ((1 == RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_EN_SHIFT, DMAC1_CHSTAT_n_EN)) &&
        (0 == RZA_IO_RegRead_32(&DMAC1.CHCFG_n,  DMAC1_CHCFG_n_REN_SHIFT, DMAC1_CHCFG_n_REN)))
    {
        /* Obtain register set in use */
        reg_set = RZA_IO_RegRead_32(&DMAC1.CHSTAT_n, DMAC1_CHSTAT_n_SR_SHIFT, DMAC1_CHSTAT_n_SR);

        /* ==== Reload ==== */
        if (0 == reg_set)
        {
            /* ---- Next1 Register Set ---- */
            DMAC1.N1SA_n = src_addr;    /* Start address of transfer source      */
            DMAC1.N1DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC1.N1TB_n = count;       /* Total transfer byte count             */
        }
        else
        {
            /* ---- Next0 Register Set ---- */
            DMAC1.N0SA_n = src_addr;    /* Start address of transfer source      */
            DMAC1.N0DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC1.N0TB_n = count;       /* Total transfer byte count             */
        }

        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 1, DMAC1_CHCFG_n_REN_SHIFT, DMAC1_CHCFG_n_REN);

        ret = 0;
    }
    else    /* When reloading has been behind */
    {
        ret = -1;
    }

    return ret;
}
#endif /* DMAC_USE_PERI_REQ */

/******************************************************************************
* Function Name: DMAC3_Open
* Description  : Enables DMAC channel 3 transfer.In this sample program, the 
*              : DMAC_REQ_MODE_SOFT is specified for the argument req, and the 
*              : transfer is enabled in auto-request mode.
* Arguments    : uint32_t req : DMAC request mode
* Return Value :  0 : Succeeded in enabling DMA transfer
*              : -1 : Failed to enable DMA transfer (due to DMA operation)
******************************************************************************/
int32_t DMAC3_Open(uint32_t req)
{
    int32_t ret;
    volatile uint8_t  dummy;

    /* Transferable? */
    if ((0 == RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_EN_SHIFT,   DMAC3_CHSTAT_n_EN)) &&
        (0 == RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_TACT_SHIFT, DMAC3_CHSTAT_n_TACT)))
    {
        /* Clear Channel Status Register */
        RZA_IO_RegWrite_32(&DMAC3.CHCTRL_n, 1, DMAC3_CHCTRL_n_SWRST_SHIFT, DMAC3_CHCTRL_n_SWRST);
        dummy = RZA_IO_RegRead_32(&DMAC3.CHCTRL_n, DMAC3_CHCTRL_n_SWRST_SHIFT, DMAC3_CHCTRL_n_SWRST);

        /* Enable DMA transfer */
        RZA_IO_RegWrite_32(&DMAC3.CHCTRL_n, 1, DMAC3_CHCTRL_n_SETEN_SHIFT, DMAC3_CHCTRL_n_SETEN);

        /* ---- Request by software ---- */
        if (DMAC_REQ_MODE_SOFT == req)
        {
            /* DMA transfer Request by software */
            RZA_IO_RegWrite_32(&DMAC3.CHCTRL_n, 1, DMAC3_CHCTRL_n_STG_SHIFT, DMAC3_CHCTRL_n_STG);
        }

        ret = 0;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC3_Close
* Description  : Aborts DMAC channel 3 transfer. Returns the remaining transfer 
*              : byte count at the time of DMA transfer abort to the argument 
*              : *remain.
* Arguments    : uint32_t * remain : Remaining transfer byte count when
*              :                   : DMA transfer is aborted
* Return Value :  0 : Succeeded in DMA transfer abort
*              : -1 : Failed to enable DMA transfer
******************************************************************************/
void DMAC3_Close(uint32_t * remain)
{
    uint8_t close_complete;

    close_complete = 0;

    /* ==== Temporarily abort transfer ==== */
    RZA_IO_RegWrite_32(&DMAC3.CHCTRL_n, 1, DMAC3_CHCTRL_n_SETSUS_SHIFT, DMAC3_CHCTRL_n_SETSUS);

    /* Loop until transfer is temporarily aborted */
    while (0 == RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_SUS_SHIFT, DMAC3_CHSTAT_n_SUS))
    {
        if (0 == RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_EN_SHIFT, DMAC3_CHSTAT_n_EN))
        {
            close_complete = 1;
            break;
        }
    }

    if (close_complete == 0)
    {
        /* ==== Abort transfer ==== */
        RZA_IO_RegWrite_32(&DMAC3.CHCTRL_n, 1, DMAC3_CHCTRL_n_CLREN_SHIFT, DMAC3_CHCTRL_n_CLREN);

        while (1 == RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_TACT_SHIFT, DMAC3_CHSTAT_n_TACT))
        {
            /* Loop until transfer is aborted */
        }

        while (1 == RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_EN_SHIFT, DMAC3_CHSTAT_n_EN))
        {
            /* Loop until 0 is set in EN before checking the remaining transfer byte count */
        }
    }

    /* ==== Obtain remaining transfer byte count ==== */
    *remain = DMAC3.CRTB_n;

    /* ==== Clear resource selector ==== */
    RZA_IO_RegWrite_32(&DMAC23.DMARS, 0, DMAC23_DMARS_CH3_MID_SHIFT, DMAC23_DMARS_CH3_MID);
    RZA_IO_RegWrite_32(&DMAC23.DMARS, 0, DMAC23_DMARS_CH3_RID_SHIFT, DMAC23_DMARS_CH3_RID);
}

/******************************************************************************
* Function Name: DMAC3_Load_Set
* Description  : Sets the transfer source address, transfer destination 
*              : address, and total transfer byte count respectively 
*              : specified by the argument src_addr, dst_addr, and count to 
*              : DMAC channel 3 as DMA transfer information. 
*              : Sets the register set selected by the CHCFG_n register 
*              : RSEL bit from the Next0 or Next1 register set. 
*              : This function should be called when DMA transfer of DMAC 
*              : channel 3 is aboted.
* Arguments    : uint32_t src_addr : Transfer source address
*              : uint32_t dst_addr : Transfer destination address
*              : uint32_t count    : Total transfer byte count
* Return Value :  0 : Succeeded in DMA setting
*              : -1 : DMA of DMAC channel 3 is in operation
******************************************************************************/
int32_t DMAC3_Load_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count)
{
    int32_t ret;
    uint8_t reg_set;

    if (0 == RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_EN_SHIFT, DMAC3_CHSTAT_n_EN))
    {
        /* Obtain register set in use */
        reg_set = RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_SR_SHIFT, DMAC3_CHSTAT_n_SR);

        /* Change register set when DMA transfer is completed */
        if (1 == RZA_IO_RegRead_32(&DMAC3.CHCFG_n, DMAC3_CHCFG_n_RSW_SHIFT, DMAC3_CHCFG_n_RSW))
        {
            reg_set ^= 1;
        }

        /* ==== Load ==== */
        if (0 == reg_set)
        {
            /* ---- Next0 Register Set ---- */
            DMAC3.N0SA_n = src_addr;    /* Start address of transfer source      */
            DMAC3.N0DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC3.N0TB_n = count;       /* Total transfer byte count             */
        }
        else
        {
            /* ---- Next1 Register Set ---- */
            DMAC3.N1SA_n = src_addr;    /* Start address of transfer source      */
            DMAC3.N1DA_n = dst_addr;    /* Start address of transfer destination */
            DMAC3.N1TB_n = count;       /* Total transfer byte count             */
        }

        ret = 0;
    }
    else    /* When trying to load during transfer */
    {
        ret = -1;
    }

    return ret;
}

/******************************************************************************
* Function Name: DMAC3_Reload_Set
* Description  : Sets the transfer source address, transfer destination 
*              : address, and total transfer byte count respectively specified 
*              : by the argument src_addr, dst_addr, and count to DMAC channel 
*              : 3 as DMA transfer informtion for the next continuous transfer. 
*              : Sets CHCFG_n register REN bit to "1" to enable the continuous 
*              : transfer function.
*              : When the CHCFG_n register RSW bit is set to "1", executes 
*              : reload setting for the register set which is not selected 
*              : by RSEL bit from Next0 or Next1 register set.
*              : When the RSW bit is set to "0", executes reload setting for 
*              : the register set selected by RSEL bit. 
*              : This function should be called when REN bit becomes "0" at 
*              : the time of continuous
*              : transfer while DMA of DMAC channel 3 is in operation.
* Arguments    : uint32_t src_addr : Transfer source address for reloading
*              : uint32_t dst_addr : Transfer destination address for reloading
*              : uint32_t count    : Total transfer byte count for reloading
* Return Value :  0 : Succeeded in DMA reload setting
*              : -1 : DMA of DMAC channel 3 transfer has been completed before 
*              :    : reload setting
******************************************************************************/
int32_t DMAC3_Reload_Set(uint32_t src_addr, uint32_t dst_addr, uint32_t count)
{
    int32_t ret;
    uint8_t reg_set;

    if ((1 == RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_EN_SHIFT, DMAC3_CHSTAT_n_EN)) &&
        (0 == RZA_IO_RegRead_32(&DMAC3.CHCFG_n,  DMAC3_CHCFG_n_REN_SHIFT, DMAC3_CHCFG_n_REN)))
    {
        /* Obtain register set in use */
        reg_set = RZA_IO_RegRead_32(&DMAC3.CHSTAT_n, DMAC3_CHSTAT_n_SR_SHIFT, DMAC3_CHSTAT_n_SR);

        /* ==== Reload ==== */
        if (0 == reg_set)
        {
            /* ---- Next1 Register Set ---- */
            DMAC3.N1SA_n = src_addr;     /* Start address of transfer source      */
            DMAC3.N1DA_n = dst_addr;     /* Start address of transfer destination */
            DMAC3.N1TB_n = count;        /* Total transfer byte count             */
        }
        else
        {
            /* ---- Next0 Register Set ---- */
            DMAC3.N0SA_n = src_addr;     /* Start address of transfer source      */
            DMAC3.N0DA_n = dst_addr;     /* Start address of transfer destination */
            DMAC3.N0TB_n = count;        /* Total transfer byte count             */
        }

        RZA_IO_RegWrite_32(&DMAC3.CHCFG_n, 1, DMAC3_CHCFG_n_REN_SHIFT, DMAC3_CHCFG_n_REN);

        ret = 0;
    }
    else    /* When reloading has been behind */
    {
        printf("error: not in time\n");
        ret = -1;
    }

    return ret;
}


/* End of File */

