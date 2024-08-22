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
* File Name   : bsc_userdef.c
* $Rev: 1330 $
* $Date:: 2015-02-17 16:07:56 +0900#$
* Description : Common driver (User define function)
*******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "devdrv_bsc.h"         /* Common Driver Header */
#include "iodefine.h"

#ifdef __CC_ARM
#pragma arm section code   = "CODE_BASIC_SETUP"
#pragma arm section rodata = "CONST_BASIC_SETUP"
#pragma arm section rwdata = "DATA_BASIC_SETUP"
#pragma arm section zidata = "BSS_BASIC_SETUP"
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
/* The address when writing in a SDRAM mode register */
#define SDRAM_MODE_CS2    (*(volatile uint16_t *)(0x3FFFD040))
#define SDRAM_MODE_CS3    (*(volatile uint16_t *)(0x3FFFE040))

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
* Function Name: Userdef_BSC_CS0Init
* Description  : This is the user-defined function called by the R_BSC_Init 
*              : function. The setting for initialization of the BSC in the CS0 
*              : space is required. In this sample code, the setting to use 
*              : the NOR flash memory in the CS0 space is executed. Sets the BSC 
*              : to connect the Spansion NOR flash memory S29GL512S10T to the 
*              : CS0 space with 16-bit bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS0Init(void)
{
    /* ---- CS0BCR settings ---- */
    BSC.CS0BCR = 0x10000C00uL;
                                    /* Idle Cycles between Write-read Cycles    */
                                    /*  and Write-write Cycles : 1 idle cycle   */
                                    /* Data Bus Size: 16-bit                    */

    /* ---- CS0WCR settings ----  */
    BSC.CS0WCR = 0x00000B40uL;
                                    /* Number of Delay Cycles from Address,     */
                                    /*  CS0# Assertion to RD#,WEn Assertion     */
                                    /*  : 1.5 cycles                            */
                                    /* Number of Access Wait Cycles: 6 cycles   */
                                    /* Delay Cycles from RD,WEn# negation to    */
                                    /*  Address,CSn# negation: 0.5 cycles       */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS1Init
* Description  : This is the user-defined function called by the R_BSC_Init 
*              : function. The setting for initialization of the BSC in the CS1 
*              : space is required. In this sample code, the setting to use 
*              : the NOR flash memory in the CS1 space is executed. Sets the BSC 
*              : to connect the Spansion NOR flash memory S29GL512S10T to the 
*              : CS1 space with 16-bit bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS1Init(void)
{
    /* ---- CS1BCR settings ---- */
    BSC.CS1BCR = 0x10000C00uL;
                                    /* Idle Cycles between Write-read Cycles    */
                                    /*  and Write-write Cycles : 1 idle cycle   */
                                    /* Data Bus Size: 16-bit                    */

    /* ---- CS1WCR settings ----  */
    BSC.CS1WCR = 0x00000B40uL;
                                    /* Number of Delay Cycles from Address,     */
                                    /*  CS1# Assertion to RD#,WEn Assertion     */
                                    /*  : 1.5 cycles                            */
                                    /* Number of Access Wait Cycles: 6 cycles   */
                                    /* Delay Cycles from RD,WEn# negation to    */
                                    /*  Address,CSn# negation: 0.5 cycles       */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS2Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the BSC in the CS2
*              : space is required. In this sample code, the setting to use
*              : the SDRAM in the CS2 space is executed. Sets the BSC to 
*              : connect the ISSI IS42S16320B-75 to the CS2 space with 16-bit
*              : bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS2Init(void)
{
    /* ==== CS2BCR settings ==== */
    BSC.CS2BCR = 0x00004C00uL;
                                /* Idle Cycles between Write-read Cycles  */
                                /* and Write-write Cycles : 0 idle cycles */
                                /* Memory type :SDRAM                     */
                                /* Data Bus Size : 16-bit                 */

    /* ==== CS2WCR settings ==== */
    BSC.CS2WCR = 0x00000480uL;
                                /* CAS latency for Area 2 : 2 cycles */


    /* ==== Written in SDRAM Mode Register ==== */
    SDRAM_MODE_CS2 = 0;
                                /* The writing data is arbitrary            */
                                /* SDRAM mode register setting CS2 space    */
                                /* Burst read (burst length 1)./Burst write */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS3Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the BSC in the CS3
*              : space is required. In this sample code, the setting to use
*              : the SDRAM in the CS3 space is executed. Sets the BSC to 
*              : connect the ISSI IS42S16320B-75 to the CS3 space with 16-bit
*              : bus width.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS3Init(void)
{
    volatile int32_t cnt;

    /* NOTE: Secure wait time according to SDRAM specification. */
    cnt = 100;
    while (cnt-- > 0)
    {
        /* wait */
    }

    /* ==== CS3BCR settings ==== */
    BSC.CS3BCR = 0x00004C00uL;
                                /* Idle Cycles between Write-read Cycles  */
                                /* and Write-write Cycles : 0 idle cycles */
                                /* Memory type :SDRAM                     */
                                /* Data Bus Size : 16-bit                 */

    /* ==== CS3WCR settings ==== */
    BSC.CS3WCR = 0x00002492uL;
                                /* Precharge completion wait cycles: 1 cycle     */
                                /* Wait cycles between ACTV command              */
                                /* and READ(A)/WRITE(A) command : 1 cycles       */
                                /* CAS latency for Area 3 : 2 cycles             */
                                /* Auto-precharge startup wait cycles : 2 cycles */
                                /* Idle cycles from REF command/self-refresh     */
                                /* Release to ACTV/REF/MRS command : 5 cycles    */

    /* ==== SDCR settings ==== */
    BSC.SDCR = 0x00120812uL;
                                /* Row address for Area 2 : 13-bit    */
                                /* Column Address for Area 2 : 10-bit */
                                /* Refresh Control :Refresh           */
                                /* RMODE :Auto-refresh is performed   */
                                /* BACTV :Auto-precharge mode         */
                                /* Row address for Area 3 : 13-bit    */
                                /* Column Address for Area 3 : 10-bit */

    /* ==== RTCOR settings ==== */
    BSC.RTCOR = 0xA55A0020uL;
                                /* 7.813usec /240nsec            */
                                /*   = 32(0x20)cycles per refresh */

    /* ==== RTCSR settings ==== */
    BSC.RTCSR = 0xA55A0010uL;
                                /* Initialization sequence start */
                                /* Clock select CKIO-phy/16      */
                                /* Refresh count :Once           */

    /* ==== Written in SDRAM Mode Register ==== */
    SDRAM_MODE_CS3 = 0x0000u;
                                /* The writing data is arbitrary            */
                                /* SDRAM mode register setting CS3 space    */
                                /* Burst read (burst length 1)./Burst write */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS4Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function. The setting for initialization of the CS4 space is 
*              : required. 
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS4Init(void)
{
    /* Add processing when using CS4 space */
}

/******************************************************************************
* Function Name: Userdef_BSC_CS5Init
* Description  : This is the user-defined function called by the R_BSC_Init
*              : function The setting for initialization of the CS5 space is 
*              : required.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Userdef_BSC_CS5Init(void)
{
    /* Add processing when using CS5 space */
}


/* End of File */

