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
/******************************************************************************
* File Name    : sample_dmac.h
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description  : DMAC sample program header
******************************************************************************/
#ifndef SAMPLE_DMAC_H
#define SAMPLE_DMAC_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "command.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct dmac_transinfo
{
    uint32_t src_addr;      /* Transfer source address                */
    uint32_t dst_addr;      /* Transfer destination address           */
    uint32_t count;         /* Transfer byte count                    */
    uint32_t src_size;      /* Transfer source data size              */
    uint32_t dst_size;      /* Transfer destination data size         */
    uint32_t saddr_dir;     /* Transfer source address direction      */
    uint32_t daddr_dir;     /* Transfer destination address direction */
} dmac_transinfo_t;

/******************************************************************************
Macro definitions
******************************************************************************/
#if 0
#define DMAC_USE_EXT_REQ            /* External request sample                  */
#define DMAC_USE_PERI_REQ           /* On-chip peripheral module request sample */
#endif

/* ==== Transfer specification of the sample program ==== */
#define DMAC_SAMPLE_SINGLE          (0)     /* Single transfer                   */
#define DMAC_SAMPLE_CONTINUATION    (1)     /* Continuous transfer (use REN bit) */

/* ==== DMA modes ==== */
#define DMAC_MODE_REGISTER          (0)     /* Register mode */
#define DMAC_MODE_LINK              (1)     /* Link mode     */

/* ==== Transfer requests ==== */
#define DMAC_REQ_MODE_EXT           (0)     /* External request                   */
#define DMAC_REQ_MODE_PERI          (1)     /* On-chip peripheral module request  */
#define DMAC_REQ_MODE_SOFT          (2)     /* Auto-request (request by software) */

/* ==== DMAC transfer sizes ==== */
#define DMAC_TRANS_SIZE_8           (0)     /* 8 bits    */
#define DMAC_TRANS_SIZE_16          (1)     /* 16 bits   */
#define DMAC_TRANS_SIZE_32          (2)     /* 32 bits   */
#define DMAC_TRANS_SIZE_64          (3)     /* 64 bits   */
#define DMAC_TRANS_SIZE_128         (4)     /* 128 bits  */
#define DMAC_TRANS_SIZE_256         (5)     /* 256 bits  */
#define DMAC_TRANS_SIZE_512         (6)     /* 512 bits  */
#define DMAC_TRANS_SIZE_1024        (7)     /* 1024 bits */

/* ==== Address increment for transferring ==== */
#define DMAC_TRANS_ADR_FIX          (1)     /* Fix       */
#define DMAC_TRANS_ADR_INC          (0)     /* Increment */

/* ==== Method for detecting DMA request ==== */
#define DMAC_REQ_DET_FALL           (0)     /* Falling edge detection */
#define DMAC_REQ_DET_RISE           (1)     /* Rising edge detection  */
#define DMAC_REQ_DET_LOW            (2)     /* Low level detection    */
#define DMAC_REQ_DET_HIGH           (3)     /* High level detection   */

/* ==== Request Directions ==== */
#define DMAC_REQ_DIR_SRC            (0)     /* DMAREQ is the source/ DMAACK is active when reading      */
#define DMAC_REQ_DIR_DST            (1)     /* DMAREQ is the destination/ DMAACK is active when writing */

/* ==== Descriptors ==== */
#define DMAC_DESC_HEADER            (0)     /* Header              */
#define DMAC_DESC_SRC_ADDR          (1)     /* Source Address      */
#define DMAC_DESC_DST_ADDR          (2)     /* Destination Address */
#define DMAC_DESC_COUNT             (3)     /* Transaction Byte    */
#define DMAC_DESC_CHCFG             (4)     /* Channel Confg       */
#define DMAC_DESC_CHITVL            (5)     /* Channel Interval    */
#define DMAC_DESC_CHEXT             (6)     /* Channel Extension   */
#define DMAC_DESC_LINK_ADDR         (7)     /* Link Address        */

/* ==== Channels ===== */
#define DMAC_CH_TOTAL               (16)
typedef enum dmac_ch
{
    DMAC_CH_0,
    DMAC_CH_1,
    DMAC_CH_2,
    DMAC_CH_3,
    DMAC_CH_4,
    DMAC_CH_5,
    DMAC_CH_6,
    DMAC_CH_7,
    DMAC_CH_8,
    DMAC_CH_9,
    DMAC_CH_10,
    DMAC_CH_11,
    DMAC_CH_12,
    DMAC_CH_13,
    DMAC_CH_14,
    DMAC_CH_15,
} dmac_ch_t;

/* ==== On-chip peripheral module requests ===== */
typedef enum dmac_request_factor
{
    DMAC_REQ_OSTM0TINT,         /* OSTM_0 compare match                */
    DMAC_REQ_OSTM1TINT,         /* OSTM_1 compare match                */
    DMAC_REQ_TGIA_0,            /* MTU2_0 input capture/compare match  */
    DMAC_REQ_TGIA_1,            /* MTU2_1 input capture/compare match  */
    DMAC_REQ_TGIA_2,            /* MTU2_2 input capture/compare match  */
    DMAC_REQ_TGIA_3,            /* MTU2_3 input capture/compare match  */
    DMAC_REQ_TGIA_4,            /* MTU2_4 input capture/compare match  */
    DMAC_REQ_TXI0,              /* SCIF_0 transmit-FIFO-data-empty     */
    DMAC_REQ_RXI0,              /* SCIF_0 receive-FIFO-data-full       */
    DMAC_REQ_TXI1,              /* SCIF_1 transmit-FIFO-data-empty     */
    DMAC_REQ_RXI1,              /* SCIF_1 receive-FIFO-data-full       */
    DMAC_REQ_TXI2,              /* SCIF_2 transmit-FIFO-data-empty     */
    DMAC_REQ_RXI2,              /* SCIF_2 receive-FIFO-data-full       */
    DMAC_REQ_TXI3,              /* SCIF_3 transmit-FIFO-data-empty     */
    DMAC_REQ_RXI3,              /* SCIF_3 receive-FIFO-data-full       */
    DMAC_REQ_TXI4,              /* SCIF_4 transmit-FIFO-data-empty     */
    DMAC_REQ_RXI4,              /* SCIF_4 receive-FIFO-data-full       */
    DMAC_REQ_TXI5,              /* SCIF_5 transmit-FIFO-data-empty     */
    DMAC_REQ_RXI5,              /* SCIF_5 receive-FIFO-data-full       */
    DMAC_REQ_TXI6,              /* SCIF_6 transmit-FIFO-data-empty     */
    DMAC_REQ_RXI6,              /* SCIF_6 receive-FIFO-data-full       */
    DMAC_REQ_TXI7,              /* SCIF_7 transmit-FIFO-data-empty     */
    DMAC_REQ_RXI7,              /* SCIF_7 receive-FIFO-data-full       */
    DMAC_REQ_USB0_DMA0_TX,      /* USB_0 channel 0 transmit FIFO empty */
    DMAC_REQ_USB0_DMA0_RX,      /* USB_0 channel 0 receive FIFO full   */
    DMAC_REQ_USB0_DMA1_TX,      /* USB_0 channel 1 transmit FIFO empty */
    DMAC_REQ_USB0_DMA1_RX,      /* USB_0 channel 1 receive FIFO full   */
    DMAC_REQ_USB1_DMA0_TX,      /* USB_1 channel 0 transmit FIFO empty */
    DMAC_REQ_USB1_DMA0_RX,      /* USB_1 channel 0 receive FIFO full   */
    DMAC_REQ_USB1_DMA1_TX,      /* USB_1 channel 1 transmit FIFO empty */
    DMAC_REQ_USB1_DMA1_RX,      /* USB_1 channel 1 receive FIFO full   */
    DMAC_REQ_ADI,               /* ADC conversion end                  */
    DMAC_REQ_IEBBTD,            /* IEBB data interrupt                 */
    DMAC_REQ_IEBBTV,            /* IEBB vector interrupt               */
    DMAC_REQ_IREADY,            /* CD-ROM decoder decoding end         */
    DMAC_REQ_FLCTL_0_TX,        /* FLCTL data unit transmit data empty */
    DMAC_REQ_FLCTL_0_RX,        /* FLCTL data unit receive data full   */
    DMAC_REQ_SDHI_0_TX,         /* SDHI_0 transmission                 */
    DMAC_REQ_SDHI_0_RX,         /* SDHI_0 reception                    */
    DMAC_REQ_SDHI_1_TX,         /* SDHI_1 transmission                 */
    DMAC_REQ_SDHI_1_RX,         /* SDHI_1 reception                    */
    DMAC_REQ_MMC_TX,            /* MMCIF transmit data empty           */
    DMAC_REQ_MMC_RX,            /* MMCIF receive data full             */
    DMAC_REQ_SSITXI0,           /* SSIF_0 transmit data empty          */
    DMAC_REQ_SSIRXI0,           /* SSIF_0 receive data full            */
    DMAC_REQ_SSITXI1,           /* SSIF_1 transmit data empty          */
    DMAC_REQ_SSIRXI1,           /* SSIF_1 receive data full            */
    DMAC_REQ_SSITXI2,           /* SSIF_2 transmit data empty          */
    DMAC_REQ_SSIRXI2,           /* SSIF_2 receive data full            */
    DMAC_REQ_SSITXI3,           /* SSIF_3 transmit data empty          */
    DMAC_REQ_SSIRXI3,           /* SSIF_3 receive data full            */
    DMAC_REQ_SSITXI4,           /* SSIF_4 transmit data empty          */
    DMAC_REQ_SSIRXI4,           /* SSIF_4 receive data full            */
    DMAC_REQ_SSITXI5,           /* SSIF_5 transmit data empty          */
    DMAC_REQ_SSIRXI5,           /* SSIF_5 receive data full            */
    DMAC_REQ_SCUTXI0,           /* SCUX FFD0_0 transmission            */
    DMAC_REQ_SCURXI0,           /* SCUX FFD0_0 reception               */
    DMAC_REQ_SCUTXI1,           /* SCUX FFD0_1 transmission            */
    DMAC_REQ_SCURXI1,           /* SCUX FFD0_1 reception               */
    DMAC_REQ_SCUTXI2,           /* SCUX FFD0_2 transmission            */
    DMAC_REQ_SCURXI2,           /* SCUX FFD0_2 reception               */
    DMAC_REQ_SCUTXI3,           /* SCUX FFD0_3 transmission            */
    DMAC_REQ_SCURXI3,           /* SCUX FFD0_3 reception               */
    DMAC_REQ_SPTI0,             /* RSPI_0 transmit data empty          */
    DMAC_REQ_SPRI0,             /* RSPI_0 receive data full            */
    DMAC_REQ_SPTI1,             /* RSPI_1 transmit data empty          */
    DMAC_REQ_SPRI1,             /* RSPI_1 receive data full            */
    DMAC_REQ_SPTI2,             /* RSPI_2 transmit data empty          */
    DMAC_REQ_SPRI2,             /* RSPI_2 receive data full            */
    DMAC_REQ_SPTI3,             /* RSPI_3 transmit data empty          */
    DMAC_REQ_SPRI3,             /* RSPI_3 receive data full            */
    DMAC_REQ_SPTI4,             /* RSPI_4 transmit data empty          */
    DMAC_REQ_SPRI4,             /* RSPI_4 receive data full            */
    DMAC_REQ_SPDIFTXI,          /* SPDIF transmission                  */
    DMAC_REQ_SPDIFRXI,          /* SPDIF reception                     */
    DMAC_REQ_CMI1,              /* PWM_1 compare match                 */
    DMAC_REQ_CMI2,              /* PWM_2 compare match                 */
    DMAC_REQ_MLB_CINT_TX,       /* MLB channel write                   */
    DMAC_REQ_MLB_CINT_RX,       /* MLB channel read                    */
    DMAC_REQ_SGDEI0,            /* SDG_0 attenuation end               */
    DMAC_REQ_SGDEI1,            /* SDG_1 attenuation end               */
    DMAC_REQ_SGDEI2,            /* SDG_2 attenuation end               */
    DMAC_REQ_SGDEI3,            /* SDG_3 attenuation end               */
    DMAC_REQ_SCI_TXI0,          /* SIMIF_0 transmission                */
    DMAC_REQ_SCI_RXI0,          /* SIMIF_0 reception                   */
    DMAC_REQ_SCI_TXI1,          /* SIMIF_1 transmission                */
    DMAC_REQ_SCI_RXI1,          /* SIMIF_2 reception                   */
    DMAC_REQ_TI0,               /* I2C_0 transmit data empty           */
    DMAC_REQ_RI0,               /* I2C_0 receive data full             */
    DMAC_REQ_TI1,               /* I2C_1 transmit data empty           */
    DMAC_REQ_RI1,               /* I2C_1 receive data full             */
    DMAC_REQ_TI2,               /* I2C_2 transmit data empty           */
    DMAC_REQ_RI2,               /* I2C_2 receive data full             */
    DMAC_REQ_TI3,               /* I2C_3 transmit data empty           */
    DMAC_REQ_RI3,               /* I2C_3 receive data full             */
    DMAC_REQ_LIN0_TX,           /* LIN_0 transmission                  */
    DMAC_REQ_LIN0_RX,           /* LIN_0 reception                     */
    DMAC_REQ_LIN1_TX,           /* LIN_1 transmission                  */
    DMAC_REQ_LIN1_RX,           /* LIN_1 reception                     */
    DMAC_REQ_IFEI0_TX,          /* PFV_0 output FIFO full              */
    DMAC_REQ_IFEI0_RX,          /* PFV_0 input FIFO empty              */
    DMAC_REQ_IFEI1_TX,          /* PFV_1 output FIFO full              */
    DMAC_REQ_IFEI1_RX,          /* PFV_1 input FIFO empty              */
    DMAC_REQ_TOTAL
} dmac_request_factor_t;

/******************************************************************************
Variable Externs
******************************************************************************/


/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t Sample_DMAC_Main(int32_t argc, char_t ** argv);
int32_t Sample_DMAC_Help(int32_t argc, char_t ** argv);
int32_t Sample_DMAC_Exit(int32_t argc, char_t ** argv);

int32_t Sample_DMAC_AutoReq(int32_t argc, char_t ** argv);
int32_t Sample_DMAC_AutoReqSingle(int32_t argc, char_t ** argv);

int32_t DMAC_Open(uint32_t channel, uint32_t req);
int32_t DMAC_Close(uint32_t channel, uint32_t * remain);
int32_t DMAC_Load_Set(uint32_t channel, uint32_t src_addr, uint32_t dst_addr, uint32_t count);
int32_t DMAC_Reload_Set(uint32_t channel, uint32_t src_addr, uint32_t dst_addr, uint32_t count);

int32_t DMAC_CheckArg(uint32_t channel, const dmac_transinfo_t * trans_info, uint32_t dmamode, uint32_t continuation);
void    DMAC_InitMemory8bit(uint8_t * addr, uint8_t data, uint32_t count, uint8_t increment);
int32_t DMAC_VerifyMemory8bit(uint8_t * addr_1, uint8_t * addr_2, uint32_t count);

const command_list_t * Sample_DMAC_GetCmdList(void);


#endif  /* SAMPLE_DMAC_H */

/* End of File */
