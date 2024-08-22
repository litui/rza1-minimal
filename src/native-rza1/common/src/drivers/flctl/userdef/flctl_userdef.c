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
* File Name   : flctl_userdef.c
* $Rev: 1516 $
* $Date:: 2015-08-25 15:47:15 +0900#$
* Description : NAND FLASH Memory Controller(FLCTL) device driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "dev_drv.h"                /* Device Driver common header */
#include "devdrv_flctl.h"           /* FLCTL Driver header */
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
* Function Name: Userdef_FLCTL_Init
* Description  : The FCLTL initial setting and setting for interrupts are required
*              : when the FCLTL is used.
* Arguments    : None
* Return Value : None
******************************************************************************/
void Userdef_FLCTL_Init(void)
{
    volatile uint8_t dummy_buf;

    /* ==== Setting of general in/out port==== */
    /* ---- P3_15 : NAF7 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC315_SHIFT,   GPIO_PIBC3_PIBC315);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC315_SHIFT,   GPIO_PBDC3_PBDC315);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM315_SHIFT,       GPIO_PM3_PM315);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC315_SHIFT,     GPIO_PMC3_PMC315);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC315_SHIFT,   GPIO_PIPC3_PIPC315);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC315_SHIFT,   GPIO_PBDC3_PBDC315);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC315_SHIFT,     GPIO_PFC3_PFC315);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE315_SHIFT,   GPIO_PFCE3_PFCE315);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE315_SHIFT, GPIO_PFCAE3_PFCAE315);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC315_SHIFT,   GPIO_PIPC3_PIPC315);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC315_SHIFT,     GPIO_PMC3_PMC315);

    /* ---- P3_14 : NAF6 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC314_SHIFT,   GPIO_PIBC3_PIBC314);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC314_SHIFT,   GPIO_PBDC3_PBDC314);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM314_SHIFT,       GPIO_PM3_PM314);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC314_SHIFT,     GPIO_PMC3_PMC314);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC314_SHIFT,   GPIO_PIPC3_PIPC314);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC314_SHIFT,   GPIO_PBDC3_PBDC314);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC314_SHIFT,     GPIO_PFC3_PFC314);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE314_SHIFT,   GPIO_PFCE3_PFCE314);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE314_SHIFT, GPIO_PFCAE3_PFCAE314);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC314_SHIFT,   GPIO_PIPC3_PIPC314);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC314_SHIFT,     GPIO_PMC3_PMC314);

    /* ---- P3_13 : NAF5 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC313_SHIFT,   GPIO_PIBC3_PIBC313);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC313_SHIFT,   GPIO_PBDC3_PBDC313);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM313_SHIFT,       GPIO_PM3_PM313);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC313_SHIFT,     GPIO_PMC3_PMC313);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC313_SHIFT,   GPIO_PIPC3_PIPC313);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC313_SHIFT,   GPIO_PBDC3_PBDC313);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC313_SHIFT,     GPIO_PFC3_PFC313);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE313_SHIFT,   GPIO_PFCE3_PFCE313);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE313_SHIFT, GPIO_PFCAE3_PFCAE313);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC313_SHIFT,   GPIO_PIPC3_PIPC313);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC313_SHIFT,     GPIO_PMC3_PMC313);

    /* ---- P3_12 : NAF4 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC312_SHIFT,   GPIO_PIBC3_PIBC312);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC312_SHIFT,   GPIO_PBDC3_PBDC312);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM312_SHIFT,       GPIO_PM3_PM312);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC312_SHIFT,     GPIO_PMC3_PMC312);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC312_SHIFT,   GPIO_PIPC3_PIPC312);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC312_SHIFT,   GPIO_PBDC3_PBDC312);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC312_SHIFT,     GPIO_PFC3_PFC312);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE312_SHIFT,   GPIO_PFCE3_PFCE312);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE312_SHIFT, GPIO_PFCAE3_PFCAE312);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC312_SHIFT,   GPIO_PIPC3_PIPC312);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC312_SHIFT,     GPIO_PMC3_PMC312);

    /* ---- P3_11 : NAF3 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC311_SHIFT,   GPIO_PIBC3_PIBC311);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC311_SHIFT,   GPIO_PBDC3_PBDC311);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM311_SHIFT,       GPIO_PM3_PM311);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC311_SHIFT,     GPIO_PMC3_PMC311);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC311_SHIFT,   GPIO_PIPC3_PIPC311);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC311_SHIFT,   GPIO_PBDC3_PBDC311);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC311_SHIFT,     GPIO_PFC3_PFC311);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE311_SHIFT,   GPIO_PFCE3_PFCE311);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE311_SHIFT, GPIO_PFCAE3_PFCAE311);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC311_SHIFT,   GPIO_PIPC3_PIPC311);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC311_SHIFT,     GPIO_PMC3_PMC311);

    /* ---- P3_10 : NAF2 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC310_SHIFT,   GPIO_PIBC3_PIBC310);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC310_SHIFT,   GPIO_PBDC3_PBDC310);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM310_SHIFT,       GPIO_PM3_PM310);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC310_SHIFT,     GPIO_PMC3_PMC310);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC310_SHIFT,   GPIO_PIPC3_PIPC310);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC310_SHIFT,   GPIO_PBDC3_PBDC310);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC310_SHIFT,     GPIO_PFC3_PFC310);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE310_SHIFT,   GPIO_PFCE3_PFCE310);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE310_SHIFT, GPIO_PFCAE3_PFCAE310);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC310_SHIFT,   GPIO_PIPC3_PIPC310);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC310_SHIFT,     GPIO_PMC3_PMC310);

    /* ---- P3_9  : NAF1 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC39_SHIFT,   GPIO_PIBC3_PIBC39);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC39_SHIFT,   GPIO_PBDC3_PBDC39);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM39_SHIFT,       GPIO_PM3_PM39);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC39_SHIFT,     GPIO_PMC3_PMC39);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC39_SHIFT,   GPIO_PIPC3_PIPC39);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC39_SHIFT,   GPIO_PBDC3_PBDC39);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC39_SHIFT,     GPIO_PFC3_PFC39);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE39_SHIFT,   GPIO_PFCE3_PFCE39);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE39_SHIFT, GPIO_PFCAE3_PFCAE39);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC39_SHIFT,   GPIO_PIPC3_PIPC39);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC39_SHIFT,     GPIO_PMC3_PMC39);

    /* ---- P3_8  : NAF0 ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC3,  0, GPIO_PIBC3_PIBC38_SHIFT,   GPIO_PIBC3_PIBC38);
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  0, GPIO_PBDC3_PBDC38_SHIFT,   GPIO_PBDC3_PBDC38);
    RZA_IO_RegWrite_16(&GPIO.PM3,    1, GPIO_PM3_PM38_SHIFT,       GPIO_PM3_PM38);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   0, GPIO_PMC3_PMC38_SHIFT,     GPIO_PMC3_PMC38);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  0, GPIO_PIPC3_PIPC38_SHIFT,   GPIO_PIPC3_PIPC38);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    RZA_IO_RegWrite_16(&GPIO.PBDC3,  1, GPIO_PBDC3_PBDC38_SHIFT,   GPIO_PBDC3_PBDC38);
    RZA_IO_RegWrite_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC38_SHIFT,     GPIO_PFC3_PFC38);
    RZA_IO_RegWrite_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE38_SHIFT,   GPIO_PFCE3_PFCE38);
    RZA_IO_RegWrite_16(&GPIO.PFCAE3, 0, GPIO_PFCAE3_PFCAE38_SHIFT, GPIO_PFCAE3_PFCAE38);
    RZA_IO_RegWrite_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC38_SHIFT,   GPIO_PIPC3_PIPC38);
    RZA_IO_RegWrite_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC38_SHIFT,     GPIO_PMC3_PMC38);

    /* ---- P4_3  : FWE# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC43_SHIFT,   GPIO_PIBC4_PIBC43);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC43_SHIFT,   GPIO_PBDC4_PBDC43);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM43_SHIFT,       GPIO_PM4_PM43);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC43_SHIFT,     GPIO_PMC4_PMC43);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC43_SHIFT,   GPIO_PIPC4_PIPC43);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC43_SHIFT,   GPIO_PBDC4_PBDC43);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC43_SHIFT,     GPIO_PFC4_PFC43);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE43_SHIFT,   GPIO_PFCE4_PFCE43);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 0, GPIO_PFCAE4_PFCAE43_SHIFT, GPIO_PFCAE4_PFCAE43);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC43_SHIFT,   GPIO_PIPC4_PIPC43);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC43_SHIFT,     GPIO_PMC4_PMC43);

    /* ---- P4_2  : FALE ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC42_SHIFT,   GPIO_PIBC4_PIBC42);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC42_SHIFT,   GPIO_PBDC4_PBDC42);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM42_SHIFT,       GPIO_PM4_PM42);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC42_SHIFT,     GPIO_PMC4_PMC42);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC42_SHIFT,   GPIO_PIPC4_PIPC42);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC42_SHIFT,   GPIO_PBDC4_PBDC42);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC42_SHIFT,     GPIO_PFC4_PFC42);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE42_SHIFT,   GPIO_PFCE4_PFCE42);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 0, GPIO_PFCAE4_PFCAE42_SHIFT, GPIO_PFCAE4_PFCAE42);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC42_SHIFT,   GPIO_PIPC4_PIPC42);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC42_SHIFT,     GPIO_PMC4_PMC42);

    /* ---- P4_1  : FCLE ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC41_SHIFT,   GPIO_PIBC4_PIBC41);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC41_SHIFT,   GPIO_PBDC4_PBDC41);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM41_SHIFT,       GPIO_PM4_PM41);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC41_SHIFT,     GPIO_PMC4_PMC41);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC41_SHIFT,   GPIO_PIPC4_PIPC41);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC41_SHIFT,   GPIO_PBDC4_PBDC41);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC41_SHIFT,     GPIO_PFC4_PFC41);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE41_SHIFT,   GPIO_PFCE4_PFCE41);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 0, GPIO_PFCAE4_PFCAE41_SHIFT, GPIO_PFCAE4_PFCAE41);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC41_SHIFT,   GPIO_PIPC4_PIPC41);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC41_SHIFT,     GPIO_PMC4_PMC41);

    /* ---- P4_0  : FRE# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC4,  0, GPIO_PIBC4_PIBC40_SHIFT,   GPIO_PIBC4_PIBC40);
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC40_SHIFT,   GPIO_PBDC4_PBDC40);
    RZA_IO_RegWrite_16(&GPIO.PM4,    1, GPIO_PM4_PM40_SHIFT,       GPIO_PM4_PM40);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   0, GPIO_PMC4_PMC40_SHIFT,     GPIO_PMC4_PMC40);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  0, GPIO_PIPC4_PIPC40_SHIFT,   GPIO_PIPC4_PIPC40);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC4,  0, GPIO_PBDC4_PBDC40_SHIFT,   GPIO_PBDC4_PBDC40);
    RZA_IO_RegWrite_16(&GPIO.PFC4,   0, GPIO_PFC4_PFC40_SHIFT,     GPIO_PFC4_PFC40);
    RZA_IO_RegWrite_16(&GPIO.PFCE4,  1, GPIO_PFCE4_PFCE40_SHIFT,   GPIO_PFCE4_PFCE40);
    RZA_IO_RegWrite_16(&GPIO.PFCAE4, 0, GPIO_PFCAE4_PFCAE40_SHIFT, GPIO_PFCAE4_PFCAE40);
    RZA_IO_RegWrite_16(&GPIO.PIPC4,  1, GPIO_PIPC4_PIPC40_SHIFT,   GPIO_PIPC4_PIPC40);
    RZA_IO_RegWrite_16(&GPIO.PMC4,   1, GPIO_PMC4_PMC40_SHIFT,     GPIO_PMC4_PMC40);

    /* ---- P5_5  : FCE# ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC5,  0, GPIO_PIBC5_PIBC55_SHIFT,   GPIO_PIBC5_PIBC55);
    RZA_IO_RegWrite_16(&GPIO.PBDC5,  0, GPIO_PBDC5_PBDC55_SHIFT,   GPIO_PBDC5_PBDC55);
    RZA_IO_RegWrite_16(&GPIO.PM5,    1, GPIO_PM5_PM55_SHIFT,       GPIO_PM5_PM55);
    RZA_IO_RegWrite_16(&GPIO.PMC5,   0, GPIO_PMC5_PMC55_SHIFT,     GPIO_PMC5_PMC55);
    RZA_IO_RegWrite_16(&GPIO.PIPC5,  0, GPIO_PIPC5_PIPC55_SHIFT,   GPIO_PIPC5_PIPC55);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC5,  0, GPIO_PBDC5_PBDC55_SHIFT,   GPIO_PBDC5_PBDC55);
    RZA_IO_RegWrite_16(&GPIO.PFC5,   0, GPIO_PFC5_PFC55_SHIFT,     GPIO_PFC5_PFC55);
    RZA_IO_RegWrite_16(&GPIO.PFCE5,  1, GPIO_PFCE5_PFCE55_SHIFT,   GPIO_PFCE5_PFCE55);
    RZA_IO_RegWrite_16(&GPIO.PFCAE5, 1, GPIO_PFCAE5_PFCAE55_SHIFT, GPIO_PFCAE5_PFCAE55);
    RZA_IO_RegWrite_16(&GPIO.PIPC5,  1, GPIO_PIPC5_PIPC55_SHIFT,   GPIO_PIPC5_PIPC55);
    RZA_IO_RegWrite_16(&GPIO.PMC5,   1, GPIO_PMC5_PMC55_SHIFT,     GPIO_PMC5_PMC55);

    /* ---- P1_2  : FRB  ---- */
    /* Port initialize */
    RZA_IO_RegWrite_16(&GPIO.PIBC1,  0, GPIO_PIBC1_PIBC12_SHIFT,   GPIO_PIBC1_PIBC12);
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  0, GPIO_PBDC1_PBDC12_SHIFT,   GPIO_PBDC1_PBDC12);
    RZA_IO_RegWrite_16(&GPIO.PM1,    1, GPIO_PM1_PM12_SHIFT,       GPIO_PM1_PM12);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   0, GPIO_PMC1_PMC12_SHIFT,     GPIO_PMC1_PMC12);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  0, GPIO_PIPC1_PIPC12_SHIFT,   GPIO_PIPC1_PIPC12);
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 3rd multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Disable                   */
    RZA_IO_RegWrite_16(&GPIO.PBDC1,  0, GPIO_PBDC1_PBDC12_SHIFT,   GPIO_PBDC1_PBDC12);
    RZA_IO_RegWrite_16(&GPIO.PFC1,   0, GPIO_PFC1_PFC12_SHIFT,     GPIO_PFC1_PFC12);
    RZA_IO_RegWrite_16(&GPIO.PFCE1,  1, GPIO_PFCE1_PFCE12_SHIFT,   GPIO_PFCE1_PFCE12);
    RZA_IO_RegWrite_16(&GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE12_SHIFT, GPIO_PFCAE1_PFCAE12);
    RZA_IO_RegWrite_16(&GPIO.PIPC1,  1, GPIO_PIPC1_PIPC12_SHIFT,   GPIO_PIPC1_PIPC12);
    RZA_IO_RegWrite_16(&GPIO.PMC1,   1, GPIO_PMC1_PMC12_SHIFT,     GPIO_PMC1_PMC12);

    /* ==== release the module standby==== */
    CPG.STBCR7  = 0x24u;        /* DVDEC0, DVDEC1, [1], ETHER, FLCTL, [1], USB0, USB1 */
    dummy_buf   = CPG.STBCR7;   /* (Dummy read)                                       */

    /* ==== Setting of common control register ==== */
    FLCTL.FLCMNCR = 0x00100001ul;      /* negate FCE# */
}


/* End of File */

