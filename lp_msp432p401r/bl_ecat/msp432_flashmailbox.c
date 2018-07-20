/******************************************************************************
*
* Copyright (C) 2012 - 2018 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*  Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
   #include "MSP432P401x/BSL432_device_file.h"
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
   #include "MSP432P4111/BSL432_device_file.h"
#else
#endif


#if defined (__TI_ARM__)   /* TI CGT Compiler */
#pragma DATA_SECTION(FlashMailBox, ".flashMailbox")
#pragma RETAIN(FlashMailBox)
const volatile uint32_t FlashMailBox[] =

#elif defined (__ICCARM__) /* IAR Compiler */
__root const uint32_t FlashMailBox [] @ 0x00200000 =

#elif defined (__CC_ARM)   /* ARM Compiler */
const volatile uint32_t FlashMailBox [] __attribute__((at(0x00200000))) =

#elif defined (__GNUC__)   /* GCC Compiler */
const volatile uint32_t FlashMailBox [] __attribute__((section(".flashMailbox"))) =
#endif
{
/*------General Configuration Group--------------------*/
    0x0115ACF6,                                                         // MailBox start,
    /* General Parameter Command:
     * Acceptable values are one or a combination of the
     * COMMAND_xxxx definitions or COMMAND_NONE */
    COMMAND_BSL_CONFIG,                                                 // General Parameter Command

    /* !Do not change, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // Factory Reset ACK
    0xFFFFFFFF,                                                         // Reserved
/*---------JTAG & SWD Lock Group------------------------*/

    /* JTAG and SWD Lock Enable
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* JTAG SWD Lock AES Initialization Vector [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* JTAG SWD Lock AES CBC Security Keys [0-7] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* Password to be appended to the firmware update payload */
    0xFFFFFFFF,                                                         // FC_SEC_PARAMS_AES_UNENC_PWD_0
    0xFFFFFFFF,                                                         // FC_SEC_PARAMS_AES_UNENC_PWD_1
    0xFFFFFFFF,                                                         // FC_SEC_PARAMS_AES_UNENC_PWD_2
    0xFFFFFFFF,                                                         // FC_SEC_PARAMS_AES_UNENC_PWD_3

    /* !Do not change, read-back/reserved addresses for JTAG SWD lock */
    0xFFFFFFFF,                                                         // JTAG/SWD Lock operation ACK
    0xFFFFFFFF,                                                         // Reserved
    0xFFFFFFFF,                                                         // Reserved

/*-------IP Protection Zone 0 Group-------------------------*/

    /* IP Protection Zone 0 Enable
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* Start Address <0x0-0x00400000:0x00001000> */
    0xFFFFFFFF,

    /* Length <0x0-0x00400000:0x00001000> */
    0xFFFFFFFF,

    /* IP Protection Zone 0 - AES Initialization Vector [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 0 - AES CBC Security Keys [0-7] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 0 - Password to append to firmware update payload [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 0 - Encrypted update mode
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* IP Protection Zone 0 - Enable data access within protected zone
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* !Do not change, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // IP Protection Zone 0 - ACK
    0xFFFFFFFF,                                                         // Reserved
    0xFFFFFFFF,                                                         // Reserved

/*-------IP Protection Zone 1 Group-------------------------*/

    /* IP Protection Zone 1 Enable
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* Start Address <0x0-0x00400000:0x00001000> */
    0xFFFFFFFF,

    /* Length <0x0-0x00400000:0x00001000> */
    0xFFFFFFFF,

    /* IP Protection Zone 1 - AES Initialization Vector [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 1 - AES CBC Security Keys [0-7] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 1 - Password to append to firmware update payload [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 1 - Encrypted update mode
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* IP Protection Zone 1 - Enable data access within protected zone
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* !Do not change, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // IP Protection Zone 1 - ACK
    0xFFFFFFFF,                                                         // Reserved
    0xFFFFFFFF,                                                         // Reserved

/*-------IP Protection Zone 2 Group-------------------------*/

    /* IP Protection Zone 2 Enable
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* Start Address <0x0-0x00400000:0x00001000> */
    0xFFFFFFFF,

    /* Length <0x0-0x00400000:0x00001000> */
    0xFFFFFFFF,

    /* IP Protection Zone 2 - AES Initialization Vector [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 2 - AES CBC Security Keys [0-7] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 2 - Password to append to firmware update payload [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 2 - Encrypted update mode
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* IP Protection Zone 2 - Enable data access within protected zone
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* !Do not change, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // IP Protection Zone 2 - ACK
    0xFFFFFFFF,                                                         // Reserved
    0xFFFFFFFF,                                                         // Reserved

/*-------IP Protection Zone 3 Group-------------------------*/

    /* IP Protection Zone 3 Enable
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* Start Address <0x0-0x00400000:0x00001000> */
    0xFFFFFFFF,

    /* Length <0x0-0x00400000:0x00001000> */
    0xFFFFFFFF,

    /* IP Protection Zone 3 - AES Initialization Vector [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 3 - AES CBC Security Keys [0-7] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 3 - Password to append to firmware update payload [0-3] */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* IP Protection Zone 3 - Encrypted update mode
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* IP Protection Zone 3 - Enable data access within protected zone
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0xFFFFFFFF,

    /* !Do not change, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // IP Protection Zone 3 - ACK
    0xFFFFFFFF,                                                         // Reserved
    0xFFFFFFFF,                                                         // Reserved

/*-------BSL Configuration Group-------------------------*/

    /* BSL Enable
     * 0xFFFFFFFF: Disable
     * 0x00000000: Enable */
    0x00000000,

    /* BSL Starting address, default pointing to TI BSL at 0x00202000 */
    BSL_API_TABLE_ADDR,

    /* BSL Parameter
     * Use HW Invoke on Port 3, Pin 0, on HIGH
     * Use UART
     */
    BSL_CONFIG_HW_INVOKE_PORT3 | BSL_CONFIG_HW_INVOKE_PIN0 |
    BSL_CONFIG_HW_INVOKE | BSL_CONFIG_HW_INVOKE_PIN_HIGH |
    BSL_CONFIG_INTERFACE_UART,

    /* !Do not modify, reserved values */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,                                                         // BSL ACK

/*----------[JTAG/SWD] Encrypted Firmware Update Group----*/
    /* Encrypted Update configuration */
    0xFFFFFFFF,                                                         // Encrypted Update Start Address
    0xFFFFFFFF,                                                         // Encrypted Update Length
    0xFFFFFFFF,                                                         // Destination address after decryption

    /* !Do not modify, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // Encrypted Update ACK
    0xFFFFFFFF,                                                         // Reserved

/*-----IP Protection Zone 0 Firmware Update Group -------*/
    /* IP Protection Zone 0 Update configuration */
    0xFFFFFFFF,                                                         // IPP Zone 0 Start Address
    0xFFFFFFFF,                                                         // IPP Zone 0 Length

    /* !Do not modify, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // IPP Zone 0 Update ACK
    0xFFFFFFFF,                                                         // Reserved

/*-----IP Protection Zone 1 Firmware Update Group -------*/
    /* IP Protection Zone 1 Update configuration */
    0xFFFFFFFF,                                                         // IPP Zone 1 Start Address
    0xFFFFFFFF,                                                         // IPP Zone 1 Length

    /* !Do not modify, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // IPP Zone 1 Update ACK
    0xFFFFFFFF,                                                         // Reserved

/*-----IP Protection Zone 2 Firmware Update Group -------*/
    /* IP Protection Zone 2 Update configuration */
    0xFFFFFFFF,                                                         // IPP Zone 2 Start Address
    0xFFFFFFFF,                                                         // IPP Zone 2 Length

    /* !Do not modify, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // IPP Zone 2 Update ACK
    0xFFFFFFFF,                                                         // Reserved

/*-----IP Protection Zone 3 Firmware Update Group -------*/
    /* IP Protection Zone 3 Update configuration */
    0xFFFFFFFF,                                                         // IPP Zone 3 Start Address
    0xFFFFFFFF,                                                         // IPP Zone 3 Length

    /* !Do not modify, read-back/reserved addresses */
    0xFFFFFFFF,                                                         // IPP Zone 3 Update ACK
    0xFFFFFFFF,                                                         // Reserved
    0xFFFFFFFF,                                                         // Reserved
    
/*-------Factory Reset Parameters Group-------------------------*/
    /* Factory Reset Enable */
    0xFFFFFFFF,

    /* Factory Reset Password Enable */
    0xFFFFFFFF,
    /* Factory Reset Password */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* !Do not modify, reserved values */
    0xFFFFFFFF,
    0xFFFFFFFF,


/*-------Factory Reset Group-------------------------*/
    /* Factory Reset Password */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* !Do not modify, reserved values */
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,

    /* Flash MailBox End */
    0x0011E11D
};

