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

#include "flashmailbox.h"

#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
   #include "MSP432P401x/BSL432_device_file.h"
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
   #include "MSP432P4111/BSL432_device_file.h"
#else
#endif


#define MAX_FLASH_ERASE_CYCLES 50

uint32_t CheckFlashMailbox(uint32_t flashMailboxCommand)
{
    uint32_t result = BOOT_OVERRIDE_AND_MAILBOX_OPERATIONS_SUCCESS;

    /* Only check for commands different from COMMAND_NONE */
    if(flashMailboxCommand != COMMAND_NONE)
    {
        if(flashMailboxCommand & COMMAND_FACTORY_RESET)
        {
            if(*((uint32_t*)(ACK_FACTORY_RESET)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_BSL_CONFIG)
        {
            if(*((uint32_t*)(ACK_BSL_CONFIG)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_JTAG_SWD_LOCK_SECEN)
        {
            if(*((uint32_t*)(ACK_JTAG_SWD_LOCK_SECEN)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_SEC_ZONE0_EN)
        {
            if(*((uint32_t*)(ACK_SEC_ZONE0_EN)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_SEC_ZONE1_EN)
        {
            if(*((uint32_t*)(ACK_SEC_ZONE1_EN)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_SEC_ZONE2_EN)
        {
            if(*((uint32_t*)(ACK_SEC_ZONE2_EN)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_SEC_ZONE3_EN)
        {
            if(*((uint32_t*)(ACK_SEC_ZONE3_EN)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_SEC_ZONE0_UPDATE)
        {
            if(*((uint32_t*)(ACK_SEC_ZONE0_UPDATE)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_SEC_ZONE1_UPDATE)
        {
            if(*((uint32_t*)(ACK_SEC_ZONE1_UPDATE)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_SEC_ZONE2_UPDATE)
        {
            if(*((uint32_t*)(ACK_SEC_ZONE2_UPDATE)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_SEC_ZONE3_UPDATE)
        {
            if(*((uint32_t*)(ACK_SEC_ZONE3_UPDATE)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
        if(flashMailboxCommand & COMMAND_JTAG_SWD_LOCK_ENC_UPDATE)
        {
            if(*((uint32_t*)(ACK_JTAG_SWD_LOCK_ENC_UPDATE)) != 0xACE)
            {
                result = BOOT_OVERRIDE_OPERATION_FAIL;
            }
        }
    }

    return(result);
}
