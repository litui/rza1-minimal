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
* File Name   : flctl_iobitmask.h
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : SCIF register define header
*******************************************************************************/
#ifndef FLCTL_IOBITMASK_H
#define FLCTL_IOBITMASK_H


/* ==== Mask values for IO registers ==== */
#define FLCTL_FLCMNCR_CE                        (0x00000008u)
#define FLCTL_FLCMNCR_NANDWF                    (0x00000200u)
#define FLCTL_FLCMNCR_ACM                       (0x00000c00u)
#define FLCTL_FLCMNCR_QTSEL                     (0x00020000u)
#define FLCTL_FLCMNCR_SNAND                     (0x00040000u)
#define FLCTL_FLCMNCR_BUSYON                    (0x00200000u)

#define FLCTL_FLCMDCR_SCTCNTL                   (0x0000ffffu)
#define FLCTL_FLCMDCR_DOCMD1                    (0x00010000u)
#define FLCTL_FLCMDCR_DOCMD2                    (0x00020000u)
#define FLCTL_FLCMDCR_ADRCNT                    (0x000c0000u)
#define FLCTL_FLCMDCR_DOADR                     (0x00100000u)
#define FLCTL_FLCMDCR_SELRW                     (0x00200000u)
#define FLCTL_FLCMDCR_DOSR                      (0x01000000u)
#define FLCTL_FLCMDCR_CDSRC                     (0x02000000u)
#define FLCTL_FLCMDCR_ADRMD                     (0x04000000u)
#define FLCTL_FLCMDCR_SCTCNTH                   (0x78000000u)
#define FLCTL_FLCMDCR_ADRCNT2                   (0x80000000u)

#define FLCTL_FLDTCNTR_DTCNT                    (0x00000fffu)
#define FLCTL_FLDTCNTR_DTFLW                    (0x00ff0000u)
#define FLCTL_FLDTCNTR_ECFLW                    (0xff000000u)

#define FLCTL_FLINTDMACR_TRINTE0                (0x00000001u)
#define FLCTL_FLINTDMACR_TRINTE1                (0x00000002u)
#define FLCTL_FLINTDMACR_TEINTE                 (0x00000004u)
#define FLCTL_FLINTDMACR_RBERINTE               (0x00000008u)
#define FLCTL_FLINTDMACR_STERINTE               (0x00000010u)
#define FLCTL_FLINTDMACR_TRREQF0                (0x00000020u)
#define FLCTL_FLINTDMACR_TRREQF1                (0x00000040u)
#define FLCTL_FLINTDMACR_BTOERB                 (0x00000080u)
#define FLCTL_FLINTDMACR_STERB                  (0x00000100u)
#define FLCTL_FLINTDMACR_DREQ0EN                (0x00010000u)
#define FLCTL_FLINTDMACR_DREQ1EN                (0x00020000u)
#define FLCTL_FLINTDMACR_AC0CLR                 (0x00040000u)
#define FLCTL_FLINTDMACR_AC1CLR                 (0x00080000u)
#define FLCTL_FLINTDMACR_FIFOTRG                (0x00300000u)

#define FLCTL_FLBSYCNT_RBTIMCNT                 (0x000fffffu)
#define FLCTL_FLBSYCNT_STAT                     (0xff000000u)

#define FLCTL_FLTRCR_TRSTRT                     (0x01u)
#define FLCTL_FLTRCR_TREND                      (0x02u)
#define FLCTL_FLTRCR_TRSTAT                     (0x04u)

#define FLCTL_FLHOLDCR_HOLDEN                   (0x00000001u)



/* ==== Shift values for IO registers ==== */
#define FLCTL_FLCMNCR_CE_SHIFT                  (3u)
#define FLCTL_FLCMNCR_NANDWF_SHIFT              (9u)
#define FLCTL_FLCMNCR_ACM_SHIFT                 (10u)
#define FLCTL_FLCMNCR_QTSEL_SHIFT               (17u)
#define FLCTL_FLCMNCR_SNAND_SHIFT               (18u)
#define FLCTL_FLCMNCR_BUSYON_SHIFT              (21u)

#define FLCTL_FLCMDCR_SCTCNTL_SHIFT             (0u)
#define FLCTL_FLCMDCR_DOCMD1_SHIFT              (16u)
#define FLCTL_FLCMDCR_DOCMD2_SHIFT              (17u)
#define FLCTL_FLCMDCR_ADRCNT_SHIFT              (18u)
#define FLCTL_FLCMDCR_DOADR_SHIFT               (20u)
#define FLCTL_FLCMDCR_SELRW_SHIFT               (21u)
#define FLCTL_FLCMDCR_DOSR_SHIFT                (24u)
#define FLCTL_FLCMDCR_CDSRC_SHIFT               (25u)
#define FLCTL_FLCMDCR_ADRMD_SHIFT               (26u)
#define FLCTL_FLCMDCR_SCTCNTH_SHIFT             (27u)
#define FLCTL_FLCMDCR_ADRCNT2_SHIFT             (31u)

#define FLCTL_FLDTCNTR_DTCNT_SHIFT              (0u)
#define FLCTL_FLDTCNTR_DTFLW_SHIFT              (16u)
#define FLCTL_FLDTCNTR_ECFLW_SHFT               (24u)

#define FLCTL_FLINTDMACR_TRINTE0_SHIFT          (0u)
#define FLCTL_FLINTDMACR_TRINTE1_SHIFT          (1u)
#define FLCTL_FLINTDMACR_TEINTE_SHIFT           (2u)
#define FLCTL_FLINTDMACR_RBERINTE_SHIFT         (3u)
#define FLCTL_FLINTDMACR_STERINTE_SHIFT         (4u)
#define FLCTL_FLINTDMACR_TRREQF0_SHIFT          (5u)
#define FLCTL_FLINTDMACR_TRREQF1_SHIFT          (6u)
#define FLCTL_FLINTDMACR_BTOERB_SHIFT           (7u)
#define FLCTL_FLINTDMACR_STERB_SHIFT            (8u)
#define FLCTL_FLINTDMACR_DREQ0EN_SHIFT          (16u)
#define FLCTL_FLINTDMACR_DREQ1EN_SHIFT          (17u)
#define FLCTL_FLINTDMACR_AC0CLR_SHIFT           (18u)
#define FLCTL_FLINTDMACR_AC1CLR_SHIFT           (19u)
#define FLCTL_FLINTDMACR_FIFOTRG_SHIFT          (20u)

#define FLCTL_FLBSYCNT_RBTIMCNT_SHIFT           (0u)
#define FLCTL_FLBSYCNT_STAT_SHIFT               (24u)

#define FLCTL_FLTRCR_TRSTRT_SHIFT               (0u)
#define FLCTL_FLTRCR_TREND_SHIFT                (1u)
#define FLCTL_FLTRCR_TRSTAT_SHIFT               (2u)

#define FLCTL_FLHOLDCR_HOLDEN_SHIFT             (0u)


#endif /* FLCTL_IOBITMASK_H */

/* End of File */
