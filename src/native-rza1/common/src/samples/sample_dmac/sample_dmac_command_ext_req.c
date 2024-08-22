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
* File Name   : sample_dmac_main_ext_req.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : DMAC sample program - External request sample
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_intc.h"            /* INTC Driver Header */
#include "sample_dmac.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

#ifdef DMAC_USE_EXT_REQ

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
int32_t DMAC_ExtReq_Init(const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation, 
                                uint32_t req_detect_mode, uint32_t req_direction);
void    DMAC0_ExtReqInit(const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation, 
                                uint32_t req_detect_mode, uint32_t req_direction);
void    Sample_DMAC0_Interrupt(uint32_t int_sense);


/******************************************************************************
* Function Name: DMAC_ExtReq_Init
* Description  : Executes DMAC initial setting in the external request for DMA 
*              : transfer request.
* Arguments    : dmac_transinfo_t * trans_info : Setting information to DMAC 
*              :                               : register
*              : uint32_t dmamode : DMA mode (only for DMAC_MODE_REGISTER)
*              : uint32_t continuation : Set continuous transfer to be valid 
*              :                       : after DMA transfer has been completed
*              :         DMAC_SAMPLE_CONTINUATION : Execute continuous transfer
*              :         DMAC_SAMPLE_SINGLE       : Do not execute continuous 
*              :                                  : transfer
*              : uint32_t req_detect_mode : Method for detecting DMA request
*              :            DMAC_REQ_DET_FALL : Falling edge detection
*              :            DMAC_REQ_DET_RISE : Rising edge detection
*              :            DMAC_REQ_DET_LOW  : Low level detection
*              :            DMAC_REQ_DET_HIGH : High level detection
*              : uint32_t req_direction : Setting value of CHCFG_n register 
*              :                        : REQD bit
* Return Value :  0 : Succeed in initial setting processing
*              : -1 : Argument error
******************************************************************************/
int32_t DMAC_ExtReq_Init(const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation, 
                                uint32_t req_detect_mode, uint32_t req_direction)
{
    int32_t ret;

    /* ==== Argument check ==== */
    ret = DMAC_CheckArg(0, trans_info, dmamode, continuation);
    if (-1 == ret)
    {
        return -1;        /* Argument error */
    }
    else if ((req_detect_mode > DMAC_REQ_DET_HIGH) || (req_direction > DMAC_REQ_DIR_DST))
    {
        return -1;        /* Argument error */
    }
    else
    {
        /* No argument error */
    }

    /* ==== DMAC initialization ==== */
    DMAC0_ExtReqInit(trans_info, dmamode, continuation, req_detect_mode, req_direction);

    return 0;
}

/******************************************************************************
* Function Name: DMAC0_ExtReqInit
* Description  : Sets the register mode for DMA mode and the external request 
*              : for transfer request for DMAC channel 0. Executes DMAC initial 
*              : setting using the DMA information specified by the argument 
*              : *trans_info, and the enabled/disabled continuous transfer 
*              : specified by the argument continuation. 
*              : Registers DMAC channel 0 interrupt hander function and sets 
*              : the interrupt priority level. Then enables transfer completion 
*              : interrupt.
* Arguments    : dmac_transinfo_t * trans_info : Setting information to DMAC 
*              :                               : register
*              : uint32_t dmamode : DMA mode (only for DMAC_MODE_REGISTER)
*              : uint32_t continuation : Set continuous transfer to be valid 
*              :                       : after DMA transfer has been completed
*              :         DMAC_SAMPLE_CONTINUATION : Execute continuous transfer
*              :         DMAC_SAMPLE_SINGLE       : Do not execute continuous 
*              :                                  : transfer
*              : uint32_t req_detect_mode : Method for detecting DMA request
*              :            DMAC_REQ_DET_FALL : Falling edge detection
*              :            DMAC_REQ_DET_RISE : Rising edge detection
*              :            DMAC_REQ_DET_LOW  : Low level detection
*              :            DMAC_REQ_DET_HIGH : High level detection
*              : uint32_t req_direction : Setting value of CHCFG_n register 
*              :                        : REQD bit
* Return Value : None
******************************************************************************/
void DMAC0_ExtReqInit(const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation, 
                             uint32_t req_detect_mode, uint32_t req_direction)
{
    /* ==== Register mode ==== */
    if (DMAC_MODE_REGISTER == dmamode)
    {
        /* ==== Next0 register set ==== */
        DMAC0.N0SA_n = trans_info->src_addr;    /* Start address of transfer source      */
        DMAC0.N0DA_n = trans_info->dst_addr;    /* Start address of transfer destination */
        DMAC0.N0TB_n = trans_info->count;       /* Total transfer byte count             */

        /* DAD : Transfer destination address counting direction */
        /* SAD : Transfer source address counting direction      */
        /* DDS : Transfer destination transfer size              */
        /* SDS : Transfer source transfer size                   */
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, trans_info->daddr_dir, DMAC0_CHCFG_n_DAD_SHIFT, DMAC0_CHCFG_n_DAD);
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, trans_info->saddr_dir, DMAC0_CHCFG_n_SAD_SHIFT, DMAC0_CHCFG_n_SAD);
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, trans_info->dst_size,  DMAC0_CHCFG_n_DDS_SHIFT, DMAC0_CHCFG_n_DDS);
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, trans_info->src_size,  DMAC0_CHCFG_n_SDS_SHIFT, DMAC0_CHCFG_n_SDS);

        /* DMS  : Register mode                            */
        /* RSEL : Select Next0 register set                */
        /* SBE  : No discharge of buffer data when aborted */
        /* DEM  : No DMA interrupt mask                    */
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_DMS_SHIFT,  DMAC0_CHCFG_n_DMS);
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_RSEL_SHIFT, DMAC0_CHCFG_n_RSEL);
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_SBE_SHIFT,  DMAC0_CHCFG_n_SBE);
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_DEM_SHIFT,  DMAC0_CHCFG_n_DEM);

        /* ---- Continuous transfer ---- */
        if (DMAC_SAMPLE_CONTINUATION == continuation)
        {
            /* REN : Execute continuous transfer                        */
            /* RSW : Change register set when DMA transfer is completed */
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 1, DMAC0_CHCFG_n_REN_SHIFT, DMAC0_CHCFG_n_REN);
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 1, DMAC0_CHCFG_n_RSW_SHIFT, DMAC0_CHCFG_n_RSW);
        }
        /* ---- Single transfer ---- */
        else
        {
            /* REN : Do not execute continuous transfer                        */
            /* RSW : Do not change register set when DMA transfer is completed */
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_REN_SHIFT, DMAC0_CHCFG_n_REN);
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_RSW_SHIFT, DMAC0_CHCFG_n_RSW);
        }

        /* Single transfer */
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_TM_SHIFT,  DMAC0_CHCFG_n_TM);

        if ((DMAC_REQ_DET_FALL == req_detect_mode) || (DMAC_REQ_DET_RISE == req_detect_mode))
        {
            /* Edge detection */
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_LVL_SHIFT,  DMAC0_CHCFG_n_LVL);
        }
        else
        {
            /* Level sense */
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 1, DMAC0_CHCFG_n_LVL_SHIFT,  DMAC0_CHCFG_n_LVL);
        }

        if ((DMAC_REQ_DET_RISE == req_detect_mode) || (DMAC_REQ_DET_HIGH == req_detect_mode))
        {
            /* ---- Rising edge detection or High level detection ---- */
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 1, DMAC0_CHCFG_n_HIEN_SHIFT, DMAC0_CHCFG_n_HIEN);
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_LOEN_SHIFT, DMAC0_CHCFG_n_LOEN);
        }
        else
        {
            /* ---- Falling edge detection or Low level detection ---- */
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 1, DMAC0_CHCFG_n_LOEN_SHIFT, DMAC0_CHCFG_n_LOEN);
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_HIEN_SHIFT, DMAC0_CHCFG_n_HIEN);
        }

        if (DMAC_REQ_DIR_SRC == req_direction)
        {
            /* DMAREQ is the source / DMAACK is active when reading */
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_REQD_SHIFT, DMAC0_CHCFG_n_REQD);
        }
        else
        {
            /* DMAREQ is the destination / DMAACK is active when writing */
            RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 1, DMAC0_CHCFG_n_REQD_SHIFT, DMAC0_CHCFG_n_REQD);
        }

        /* AM  : Level mode      */
        /* SEL : Channel setting */
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 1, DMAC0_CHCFG_n_AM_SHIFT,  DMAC0_CHCFG_n_AM);
        RZA_IO_RegWrite_32(&DMAC0.CHCFG_n, 0, DMAC0_CHCFG_n_SEL_SHIFT, DMAC0_CHCFG_n_SEL);

        /* MID, RID : External request */
        RZA_IO_RegWrite_32(&DMAC01.DMARS, 0, DMAC01_DMARS_CH0_MID_SHIFT, DMAC01_DMARS_CH0_MID);
        RZA_IO_RegWrite_32(&DMAC01.DMARS, 3, DMAC01_DMARS_CH0_RID_SHIFT, DMAC01_DMARS_CH0_RID);

        /* PR : Round robin mode */
        RZA_IO_RegWrite_32(&DMAC07.DCTRL_0_7, 1, DMAC07_DCTRL_0_7_PR_SHIFT, DMAC07_DCTRL_0_7_PR);
    }

    /* ==== Link mode ==== */
    else
    {

    }

    /* ==== Register DMAC channel 0 interrupt handler ==== */
    R_INTC_RegistIntFunc(INTC_ID_DMAINT0, Sample_DMAC0_Interrupt);

    /* ==== Set priority of DMAC channel 0 interrupt to 1 ==== */
    R_INTC_SetPriority(INTC_ID_DMAINT0, 1);

    /* ==== Validate DMAC channel 0 interrupt ==== */
    R_INTC_Enable(INTC_ID_DMAINT0);
}

/******************************************************************************
* Function Name: Sample_DMAC0_Interrupt
* Description  : This function is executed when DMAC channel 0 transfer has 
*              : been completed.
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void Sample_DMAC0_Interrupt(uint32_t int_sense)
{
    /* Add processing when using transfer completion interrupt for DMAC channel 0 */
}

#endif /* DMAC_USE_EXT_REQ */


/* End of File */

