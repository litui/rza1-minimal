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
* File Name   : port_init.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Initialize peripheral function sample - PORT initialize
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "port_init.h"
#include "iodefine.h"
#include "rza_io_regrw.h"

#ifdef __CC_ARM
#pragma arm section code   = "CODE_RESET"
#pragma arm section rodata = "CONST_RESET"
#pragma arm section rwdata = "DATA_RESET"
#pragma arm section zidata = "BSS_RESET"
#endif

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
* Function Name: PORT_Init
* Description  : Executes initial setting for the PORT.
*              : In this sample code, the PORT initial setting is executed to 
*              : connect the SDRAM to the CS2 and the CS3 spaces.
* Arguments    : none
* Return Value : none
******************************************************************************/
void PORT_Init(void)
{
    /* ==== BSC settings ==== */

    /* ---- P7_2 : RAS# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC72_SHIFT,   GPIO_PIBC7_PIBC72);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC72_SHIFT,   GPIO_PBDC7_PBDC72);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM72_SHIFT,       GPIO_PM7_PM72);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC72_SHIFT,     GPIO_PMC7_PMC72);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC72_SHIFT,   GPIO_PIPC7_PIPC72);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC72_SHIFT,   GPIO_PBDC7_PBDC72);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC72_SHIFT,     GPIO_PFC7_PFC72);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE72_SHIFT,   GPIO_PFCE7_PFCE72);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE72_SHIFT, GPIO_PFCAE7_PFCAE72);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC72_SHIFT,   GPIO_PIPC7_PIPC72);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC72_SHIFT,     GPIO_PMC7_PMC72);

    /* ---- P7_3 : CAS# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC73_SHIFT,   GPIO_PIBC7_PIBC73);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC73_SHIFT,   GPIO_PBDC7_PBDC73);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM73_SHIFT,       GPIO_PM7_PM73);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC73_SHIFT,     GPIO_PMC7_PMC73);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC73_SHIFT,   GPIO_PIPC7_PIPC73);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC73_SHIFT,   GPIO_PBDC7_PBDC73);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC73_SHIFT,     GPIO_PFC7_PFC73);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE73_SHIFT,   GPIO_PFCE7_PFCE73);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE73_SHIFT, GPIO_PFCAE7_PFCAE73);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC73_SHIFT,   GPIO_PIPC7_PIPC73);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC73_SHIFT,     GPIO_PMC7_PMC73);

    /* ---- P7_4 : CKE ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC74_SHIFT,   GPIO_PIBC7_PIBC74);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC74_SHIFT,   GPIO_PBDC7_PBDC74);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM74_SHIFT,       GPIO_PM7_PM74);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC74_SHIFT,     GPIO_PMC7_PMC74);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC74_SHIFT,   GPIO_PIPC7_PIPC74);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC74_SHIFT,   GPIO_PBDC7_PBDC74);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC74_SHIFT,     GPIO_PFC7_PFC74);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE74_SHIFT,   GPIO_PFCE7_PFCE74);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE74_SHIFT, GPIO_PFCAE7_PFCAE74);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC74_SHIFT,   GPIO_PIPC7_PIPC74);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC74_SHIFT,     GPIO_PMC7_PMC74);

    /* ---- P7_5 : RD/WR# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC75_SHIFT,   GPIO_PIBC7_PIBC75);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC75_SHIFT,   GPIO_PBDC7_PBDC75);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM75_SHIFT,       GPIO_PM7_PM75);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC75_SHIFT,     GPIO_PMC7_PMC75);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC75_SHIFT,   GPIO_PIPC7_PIPC75);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC75_SHIFT,   GPIO_PBDC7_PBDC75);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC75_SHIFT,     GPIO_PFC7_PFC75);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE75_SHIFT,   GPIO_PFCE7_PFCE75);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE75_SHIFT, GPIO_PFCAE7_PFCAE75);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC75_SHIFT,   GPIO_PIPC7_PIPC75);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC75_SHIFT,     GPIO_PMC7_PMC75);

    /* ---- P7_7 : DQMLU ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC77_SHIFT,   GPIO_PIBC7_PIBC77);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC77_SHIFT,   GPIO_PBDC7_PBDC77);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM77_SHIFT,       GPIO_PM7_PM77);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC77_SHIFT,     GPIO_PMC7_PMC77);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC77_SHIFT,   GPIO_PIPC7_PIPC77);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC77_SHIFT,   GPIO_PBDC7_PBDC77);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC77_SHIFT,     GPIO_PFC7_PFC77);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE77_SHIFT,   GPIO_PFCE7_PFCE77);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE77_SHIFT, GPIO_PFCAE7_PFCAE77);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC77_SHIFT,   GPIO_PIPC7_PIPC77);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC77_SHIFT,     GPIO_PMC7_PMC77);

    /* ---- P5_8 : CS2# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC5,  0, GPIO_PIBC5_PIBC58_SHIFT,   GPIO_PIBC5_PIBC58);
    RZA_IO_RegWrite_16(&GPIO.PBDC5,  0, GPIO_PBDC5_PBDC58_SHIFT,   GPIO_PBDC5_PBDC58);
    RZA_IO_RegWrite_16(&GPIO.PM5,    1, GPIO_PM5_PM58_SHIFT,       GPIO_PM5_PM58);
    RZA_IO_RegWrite_16(&GPIO.PMC5,   0, GPIO_PMC5_PMC58_SHIFT,     GPIO_PMC5_PMC58);
    RZA_IO_RegWrite_16(&GPIO.PIPC5,  0, GPIO_PIPC5_PIPC58_SHIFT,   GPIO_PIPC5_PIPC58);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 6th multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC5,  0, GPIO_PBDC5_PBDC58_SHIFT,   GPIO_PBDC5_PBDC58);
    RZA_IO_RegWrite_16(&GPIO.PFC5,   1, GPIO_PFC5_PFC58_SHIFT,     GPIO_PFC5_PFC58);
    RZA_IO_RegWrite_16(&GPIO.PFCE5,  0, GPIO_PFCE5_PFCE58_SHIFT,   GPIO_PFCE5_PFCE58);
    RZA_IO_RegWrite_16(&GPIO.PFCAE5, 1, GPIO_PFCAE5_PFCAE58_SHIFT, GPIO_PFCAE5_PFCAE58);
    RZA_IO_RegWrite_16(&GPIO.PIPC5,  1, GPIO_PIPC5_PIPC58_SHIFT,   GPIO_PIPC5_PIPC58);
    RZA_IO_RegWrite_16(&GPIO.PMC5,   1, GPIO_PMC5_PMC58_SHIFT,     GPIO_PMC5_PMC58);

    /* ---- P7_1 : CS3# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC7,  0, GPIO_PIBC7_PIBC71_SHIFT,   GPIO_PIBC7_PIBC71);
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC71_SHIFT,   GPIO_PBDC7_PBDC71);
    RZA_IO_RegWrite_16(&GPIO.PM7,    1, GPIO_PM7_PM71_SHIFT,       GPIO_PM7_PM71);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   0, GPIO_PMC7_PMC71_SHIFT,     GPIO_PMC7_PMC71);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  0, GPIO_PIPC7_PIPC71_SHIFT,   GPIO_PIPC7_PIPC71);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC7,  0, GPIO_PBDC7_PBDC71_SHIFT,   GPIO_PBDC7_PBDC71);
    RZA_IO_RegWrite_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC71_SHIFT,     GPIO_PFC7_PFC71);
    RZA_IO_RegWrite_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE71_SHIFT,   GPIO_PFCE7_PFCE71);
    RZA_IO_RegWrite_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE71_SHIFT, GPIO_PFCAE7_PFCAE71);
    RZA_IO_RegWrite_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC71_SHIFT,   GPIO_PIPC7_PIPC71);
    RZA_IO_RegWrite_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC71_SHIFT,     GPIO_PMC7_PMC71);
}

/* End of File */

