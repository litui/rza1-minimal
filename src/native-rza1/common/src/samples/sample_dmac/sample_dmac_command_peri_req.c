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
* File Name   : sample_dmac_command_peri_req.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : DMAC sample program - Peripheral module request sample
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

#ifdef DMAC_USE_PERI_REQ

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
#define DMAC_INDEFINE   (255)       /* Macro definition when REQD bit is not used */

/* ==== Request setting information for on-chip peripheral module ==== */
typedef enum dmac_peri_req_reg_type
{
    DMAC_REQ_MID,
    DMAC_REQ_RID,
    DMAC_REQ_AM,
    DMAC_REQ_LVL,
    DMAC_REQ_REQD
} dmac_peri_req_reg_type_t;


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
int32_t DMAC_PeriReq_Init(uint32_t channel, const dmac_transinfo_t * trans_info, uint32_t dmamode,
                                 uint32_t continuation, uint32_t request_factor, uint32_t req_direction);
void    DMAC1_PeriReqInit(const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation, 
                                 uint32_t request_factor, uint32_t req_direction);
void    Sample_DMAC1_Interrupt(uint32_t int_sense);

/* ==== Global variables ==== */
/* On-chip peripheral module request setting table */
static const uint8_t dmac_peri_req_init_table[DMAC_REQ_TOTAL][5] =
{
/*   MID,RID,AM,LVL,REQD                                                                  */
    {   8, 3, 2, 0, DMAC_INDEFINE },    /* OSTM_0 compare match                */
    {   9, 3, 2, 0, DMAC_INDEFINE },    /* OSTM_1 compare match                */
    {  16, 3, 1, 1, DMAC_INDEFINE },    /* MTU2_0 input capture/compare match  */
    {  17, 3, 1, 1, DMAC_INDEFINE },    /* MTU2_1 input capture/compare match  */
    {  18, 3, 1, 1, DMAC_INDEFINE },    /* MTU2_2 input capture/compare match  */
    {  19, 3, 1, 1, DMAC_INDEFINE },    /* MTU2_3 input capture/compare match  */
    {  20, 3, 1, 1, DMAC_INDEFINE },    /* MTU2_4 input capture/compare match  */
    {  24, 1, 2, 1, 1             },    /* SCIF_0 transmit-FIFO-data-empty     */
    {  24, 2, 2, 1, 0             },    /* SCIF_0 receive-FIFO-data-full       */
    {  25, 1, 2, 1, 1             },    /* SCIF_1 transmit-FIFO-data-empty     */
    {  25, 2, 2, 1, 0             },    /* SCIF_1 receive-FIFO-data-full       */
    {  26, 1, 2, 1, 1             },    /* SCIF_2 transmit-FIFO-data-empty     */
    {  26, 2, 2, 1, 0             },    /* SCIF_2 receive-FIFO-data-full       */
    {  27, 1, 2, 1, 1             },    /* SCIF_3 transmit-FIFO-data-empty     */
    {  27, 2, 2, 1, 0             },    /* SCIF_3 receive-FIFO-data-full       */
    {  28, 1, 2, 1, 1             },    /* SCIF_4 transmit-FIFO-data-empty     */
    {  28, 2, 2, 1, 0             },    /* SCIF_4 receive-FIFO-data-full       */
    {  29, 1, 2, 1, 1             },    /* SCIF_5 transmit-FIFO-data-empty     */
    {  29, 2, 2, 1, 0             },    /* SCIF_5 receive-FIFO-data-full       */
    {  30, 1, 2, 1, 1             },    /* SCIF_6 transmit-FIFO-data-empty     */
    {  30, 2, 2, 1, 0             },    /* SCIF_6 receive-FIFO-data-full       */
    {  31, 1, 2, 1, 1             },    /* SCIF_7 transmit-FIFO-data-empty     */
    {  31, 2, 2, 1, 0             },    /* SCIF_7 receive-FIFO-data-full       */
    {  32, 3, 2, 1, 1             },    /* USB_0 channel 0 transmit FIFO empty */
    {  32, 3, 2, 1, 0             },    /* USB_0 channel 0 receive FIFO full   */
    {  33, 3, 2, 1, 1             },    /* USB_0 channel 1 transmit FIFO empty */
    {  33, 3, 2, 1, 0             },    /* USB_0 channel 1 receive FIFO full   */
    {  34, 3, 2, 1, 1             },    /* USB_1 channel 0 transmit FIFO empty */
    {  34, 3, 2, 1, 0             },    /* USB_1 channel 0 receive FIFO full   */
    {  35, 3, 2, 1, 1             },    /* USB_1 channel 1 transmit FIFO empty */
    {  35, 3, 2, 1, 0             },    /* USB_1 channel 1 receive FIFO full   */
    {  36, 3, 1, 1, 0             },    /* ADC conversion end                  */
    {  40, 3, 2, 0, DMAC_INDEFINE },    /* IEBB data interrupt                 */
    {  41, 3, 2, 0, DMAC_INDEFINE },    /* IEBB vector interrupt               */
    {  42, 3, 2, 1, DMAC_INDEFINE },    /* CD-ROM decoder decoding end         */
    {  44, 3, 2, 1, 1             },    /* FLCTL data unit transmit data empty */
    {  44, 3, 2, 1, 0             },    /* FLCTL data unit receive data full   */
    {  48, 1, 2, 1, 1             },    /* SDHI_0 transmission                 */
    {  48, 2, 2, 1, 0             },    /* SDHI_0 reception                    */
    {  49, 1, 2, 1, 1             },    /* SDHI_1 transmission                 */
    {  49, 2, 2, 1, 0             },    /* SDHI_1 reception                    */
    {  50, 1, 2, 1, 1             },    /* MMCIF transmit data empty           */
    {  50, 2, 2, 1, 0             },    /* MMCIF receive data full             */
    {  56, 1, 2, 1, 1             },    /* SSIF_0 transmit data empty          */
    {  56, 2, 2, 1, 0             },    /* SSIF_0 receive data full            */
    {  57, 1, 2, 1, 1             },    /* SSIF_1 transmit data empty          */
    {  57, 2, 2, 1, 0             },    /* SSIF_1 receive data full            */
    {  58, 3, 2, 1, 1             },    /* SSIF_2 transmit data empty          */
    {  58, 3, 2, 1, 0             },    /* SSIF_2 receive data full            */
    {  59, 1, 2, 1, 1             },    /* SSIF_3 transmit data empty          */
    {  59, 2, 2, 1, 0             },    /* SSIF_3 receive data full            */
    {  60, 3, 2, 1, 1             },    /* SSIF_4 transmit data empty          */
    {  60, 3, 2, 1, 0             },    /* SSIF_4 receive data full            */
    {  61, 1, 2, 1, 1             },    /* SSIF_5 transmit data empty          */
    {  61, 2, 2, 1, 0             },    /* SSIF_5 receive data full            */
    {  64, 1, 1, 1, 0             },    /* SCUX FFD0_0 transmission            */
    {  64, 2, 1, 1, 0             },    /* SCUX FFD0_0 reception               */
    {  65, 1, 1, 1, 0             },    /* SCUX FFD0_1 transmission            */
    {  65, 2, 1, 1, 0             },    /* SCUX FFD0_1 reception               */
    {  66, 1, 1, 1, 0             },    /* SCUX FFD0_2 transmission            */
    {  66, 2, 1, 1, 0             },    /* SCUX FFD0_2 reception               */
    {  67, 1, 1, 1, 0             },    /* SCUX FFD0_3 transmission            */
    {  67, 2, 1, 1, 0             },    /* SCUX FFD0_3 reception               */
    {  72, 1, 2, 1, 1             },    /* RSPI_0 transmit data empty          */
    {  72, 2, 2, 1, 0             },    /* RSPI_0 receive data full            */
    {  73, 1, 2, 1, 1             },    /* RSPI_1 transmit data empty          */
    {  73, 2, 2, 1, 0             },    /* RSPI_1 receive data full            */
    {  74, 1, 2, 1, 1             },    /* RSPI_2 transmit data empty          */
    {  74, 2, 2, 1, 0             },    /* RSPI_2 receive data full            */
    {  75, 1, 2, 1, 1             },    /* RSPI_3 transmit data empty          */
    {  75, 2, 2, 1, 0             },    /* RSPI_3 receive data full            */
    {  76, 1, 2, 1, 1             },    /* RSPI_4 transmit data empty          */
    {  76, 2, 2, 1, 0             },    /* RSPI_4 receive data full            */
    {  80, 1, 2, 1, 1             },    /* SPDIF transmission                  */
    {  80, 2, 2, 1, 0             },    /* SPDIF reception                     */
    {  81, 3, 1, 1, 1             },    /* PWM_1 compare match                 */
    {  82, 3, 1, 1, 1             },    /* PWM_2 compare match                 */
    {  83, 3, 2, 1, 1             },    /* MLB channel write                   */
    {  83, 3, 2, 1, 0             },    /* MLB channel read                    */
    {  84, 3, 1, 1, 1             },    /* SDG_0 attenuation end               */
    {  85, 3, 1, 1, 1             },    /* SDG_1 attenuation end               */
    {  86, 3, 1, 1, 1             },    /* SDG_2 attenuation end               */
    {  87, 3, 1, 1, 1             },    /* SDG_3 attenuation end               */
    {  90, 1, 2, 0, 1             },    /* SIMIF_0 transmission                */
    {  90, 2, 2, 0, 0             },    /* SIMIF_0 reception                   */
    {  91, 1, 2, 0, 1             },    /* SIMIF_1 transmission                */
    {  91, 2, 2, 0, 0             },    /* SIMIF_2 reception                   */
    {  96, 1, 2, 0, 1             },    /* I2C_0 transmit data empty           */
    {  96, 2, 2, 0, 0             },    /* I2C_0 receive data full             */
    {  97, 1, 2, 0, 1             },    /* I2C_1 transmit data empty           */
    {  97, 2, 2, 0, 0             },    /* I2C_1 receive data full             */
    {  98, 1, 2, 0, 1             },    /* I2C_2 transmit data empty           */
    {  98, 2, 2, 0, 0             },    /* I2C_2 receive data full             */
    {  99, 1, 2, 0, 1             },    /* I2C_3 transmit data empty           */
    {  99, 2, 2, 0, 0             },    /* I2C_3 receive data full             */
    { 104, 1, 2, 0, 1             },    /* LIN_0 transmission                  */
    { 104, 2, 2, 0, 0             },    /* LIN_0 reception                     */
    { 105, 1, 2, 0, 1             },    /* LIN_1 transmission                  */
    { 105, 2, 2, 0, 0             },    /* LIN_1 reception                     */
    { 108, 1, 2, 1, 1             },    /* PFV_0 output FIFO full              */
    { 108, 2, 2, 1, 0             },    /* PFV_0 input FIFO empty              */
    { 109, 1, 2, 1, 1             },    /* PFV_1 output FIFO full              */
    { 109, 2, 2, 1, 0             },    /* PFV_1 input FIFO empty              */
};


/******************************************************************************
* Function Name: DMAC_PeriReq_Init
* Description  : Executes DMAC initial setting in the on-chip peripheral module 
*              : request for DMA transfer request.
*              : In this sample program, the initial setting for DMAC channel 
*              : 1 is executed.
* Arguments    : uint32_t channel : DMAC channel number
*              : dmac_transinfo_t * trans_info : Setting information to DMAC 
*              :                               : register
*              : uint32_t dmamode      : DMA mode (only for DMAC_MODE_REGISTER)
*              : uint32_t continuation : Set continuous transfer to be valid 
*              :                       : after DMA transfer has been completed
*              :         DMAC_SAMPLE_CONTINUATION : Execute continuous transfer
*              :         DMAC_SAMPLE_SINGLE       : Do not execute continuous 
*              :                                  : transfer
*              : uint32_t request_factor : Factor for on-chip peripheral module 
*              :                         : request
*              :         DMAC_REQ_OSTM0TINT : OSTM_0 compare match
*              :         DMAC_REQ_OSTM1TINT : OSTM_1 compare match
*              :         DMAC_REQ_TGI0A     : MTU2_0 input capture/compare match
*              :                 :
*              : uint32_t req_direction : Setting value of CHCFG_n register 
*              :                        : REQD bit
* Return Value :  0 : Succeed in initial setting processing
*              : -1 : Argument error
******************************************************************************/
int32_t DMAC_PeriReq_Init(uint32_t channel, const dmac_transinfo_t * trans_info, uint32_t dmamode, 
                                 uint32_t continuation, uint32_t request_factor, uint32_t req_direction)
{
    int32_t ret;

    /* ==== Argument check ==== */
    ret = DMAC_CheckArg(channel, trans_info, dmamode, continuation);
    if (-1 == ret)
    {
        return -1;        /* Argument error */
    }
    else if ((request_factor >= DMAC_REQ_TOTAL) || (req_direction > DMAC_REQ_DIR_DST))
    {
        return -1;        /* Argument error */
    }
    else
    {
        /* No argument error */
    }

    /* ==== DMAC initialization ==== */
    switch (channel)
    {
        case 1:
            DMAC1_PeriReqInit(trans_info, dmamode, continuation, request_factor, req_direction);
        break;
        default:
            /* Do not reach here based on the assumption */
        break;
    }

    return 0;
}

/******************************************************************************
* Function Name: DMAC1_PeriReqInit
* Description  : Sets the register mode for DMA mode and the on-chip peripheral 
*              : module request for transfer request for DMAC channel 1. 
*              : Executes DMAC initial setting using the DMA information 
*              : specified by the argument *trans_info and the enabled/disabled 
*              : continuous transfer specified by the argument continuation.
*              : Registers DMAC channel 1 interrupt handler function and sets 
*              : the interrupt priority level. Then enables transfer completion 
*              : interrupt.
* Arguments    : dmac_transinfo_t * trans_info : Setting information to DMAC 
*              :                               : register
*              : uint32_t dmamode      : DMA mode (only for DMAC_MODE_REGISTER)
*              : uint32_t continuation : Set continuous transfer to be valid 
*              :                       : after DMA transfer has been completed
*              :         DMAC_SAMPLE_CONTINUATION : Execute continuous transfer
*              :         DMAC_SAMPLE_SINGLE       : Do not execute continuous 
*              :                                  : transfer
*              : uint32_t request_factor : Factor for on-chip peripheral module 
*              :                         : request
*              :         DMAC_REQ_OSTM0TINT : OSTM_0 compare match
*              :         DMAC_REQ_OSTM1TINT : OSTM_1 compare match
*              :         DMAC_REQ_TGI0A     : MTU2_0 input capture/compare match
*              :                 :
*              : uint32_t req_direction : Setting value of CHCFG_n register 
*              :                        : REQD bit
* Return Value : None
******************************************************************************/
void DMAC1_PeriReqInit(const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation, 
                              uint32_t request_factor, uint32_t req_direction)
{
    /* ==== Register mode ==== */
    if (DMAC_MODE_REGISTER == dmamode)
    {
        /* ==== Next0 register set ==== */
        DMAC1.N0SA_n = trans_info->src_addr;    /* Start address of transfer source      */
        DMAC1.N0DA_n = trans_info->dst_addr;    /* Start address of transfer destination */
        DMAC1.N0TB_n = trans_info->count;       /* Total transfer byte count             */

        /* DAD : Transfer destination address counting direction */
        /* SAD : Transfer source address counting direction      */
        /* DDS : Transfer destination transfer size              */
        /* SDS : Transfer source transfer size                   */
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, trans_info->daddr_dir, DMAC1_CHCFG_n_DAD_SHIFT, DMAC1_CHCFG_n_DAD);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, trans_info->saddr_dir, DMAC1_CHCFG_n_SAD_SHIFT, DMAC1_CHCFG_n_SAD);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, trans_info->dst_size,  DMAC1_CHCFG_n_DDS_SHIFT, DMAC1_CHCFG_n_DDS);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, trans_info->src_size,  DMAC1_CHCFG_n_SDS_SHIFT, DMAC1_CHCFG_n_SDS);

        /* DMS  : Register mode                            */
        /* RSEL : Select Next0 register set                */
        /* SBE  : No discharge of buffer data when aborted */
        /* DEM  : No DMA interrupt mask                    */
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 0, DMAC1_CHCFG_n_DMS_SHIFT,  DMAC1_CHCFG_n_DMS);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 0, DMAC1_CHCFG_n_RSEL_SHIFT, DMAC1_CHCFG_n_RSEL);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 0, DMAC1_CHCFG_n_SBE_SHIFT,  DMAC1_CHCFG_n_SBE);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 0, DMAC1_CHCFG_n_DEM_SHIFT,  DMAC1_CHCFG_n_DEM);

        /* ---- Continuous transfer ---- */
        if (DMAC_SAMPLE_CONTINUATION == continuation)
        {
            /* REN : Execute continuous transfer                         */
            /* RSW : Change register set when DMA transfer is completed. */
            RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 1, DMAC1_CHCFG_n_REN_SHIFT, DMAC1_CHCFG_n_REN);
            RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 1, DMAC1_CHCFG_n_RSW_SHIFT, DMAC1_CHCFG_n_RSW);
        }
        /* ---- Single transfer ---- */
        else
        {
            /* REN : Do not execute continuous transfer                         */
            /* RSW : Do not change register set when DMA transfer is completed. */
            RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 0, DMAC1_CHCFG_n_REN_SHIFT, DMAC1_CHCFG_n_REN);
            RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 0, DMAC1_CHCFG_n_RSW_SHIFT, DMAC1_CHCFG_n_RSW);
        }

        /* TM  : Single transfer                          */
        /* SEL : Channel setting                          */
        /* HIEN, LOEN : On-chip peripheral module request */
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 0, DMAC1_CHCFG_n_TM_SHIFT,   DMAC1_CHCFG_n_TM);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 1, DMAC1_CHCFG_n_SEL_SHIFT,  DMAC1_CHCFG_n_SEL);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 1, DMAC1_CHCFG_n_HIEN_SHIFT, DMAC1_CHCFG_n_HIEN);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 0, DMAC1_CHCFG_n_LOEN_SHIFT, DMAC1_CHCFG_n_LOEN);

        /* ---- Set factor by specified on-chip peripheral module request ---- */
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 
                            dmac_peri_req_init_table[request_factor][DMAC_REQ_AM], 
                            DMAC1_CHCFG_n_AM_SHIFT, 
                            DMAC1_CHCFG_n_AM);
        RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 
                            dmac_peri_req_init_table[request_factor][DMAC_REQ_LVL], 
                            DMAC1_CHCFG_n_LVL_SHIFT, 
                            DMAC1_CHCFG_n_LVL);

        if (dmac_peri_req_init_table[request_factor][DMAC_REQ_REQD] != DMAC_INDEFINE)
        {
            RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 
                                dmac_peri_req_init_table[request_factor][DMAC_REQ_REQD], 
                                DMAC1_CHCFG_n_REQD_SHIFT, 
                                DMAC1_CHCFG_n_REQD);
        }
        else
        {
            RZA_IO_RegWrite_32(&DMAC1.CHCFG_n, 
                                req_direction, 
                                DMAC1_CHCFG_n_REQD_SHIFT, 
                                DMAC1_CHCFG_n_REQD);
        }

        RZA_IO_RegWrite_32(&DMAC01.DMARS, 
                            dmac_peri_req_init_table[request_factor][DMAC_REQ_MID], 
                            DMAC01_DMARS_CH1_MID_SHIFT, 
                            DMAC01_DMARS_CH1_MID);
        RZA_IO_RegWrite_32(&DMAC01.DMARS, 
                            dmac_peri_req_init_table[request_factor][DMAC_REQ_RID], 
                            DMAC01_DMARS_CH1_RID_SHIFT, 
                            DMAC01_DMARS_CH1_RID);

        /* PR : Round robin mode */
        RZA_IO_RegWrite_32(&DMAC07.DCTRL_0_7, 1, DMAC07_DCTRL_0_7_PR_SHIFT, DMAC07_DCTRL_0_7_PR);
    }

    /* ==== Link mode ==== */
    else
    {

    }

    /* ==== Register DMAC channel 1 interrupt handler ==== */
    R_INTC_RegistIntFunc(INTC_ID_DMAINT1, Sample_DMAC1_Interrupt);

    /* ==== Set priority of DMAC channel 1 interrupt to 1 ==== */
    R_INTC_SetPriority(INTC_ID_DMAINT1, 1);

    /* ==== Validate DMAC channel 1 interrupt ==== */
    R_INTC_Enable(INTC_ID_DMAINT1);
}

/******************************************************************************
* Function Name: Sample_DMAC1_Interrupt
* Description  : This function is executed when DMAC channel 1 transfer has 
*              : been completed.
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void Sample_DMAC1_Interrupt(uint32_t int_sense)
{
    /* Add processing when using transfer completion interrupt for DMAC channel 1 */
}

#endif /* DMAC_USE_PERI_REQ */


/* End of File */

