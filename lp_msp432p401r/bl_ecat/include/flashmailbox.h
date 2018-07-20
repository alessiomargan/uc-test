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

#ifndef FLASHMAILBOX_H
#define FLASHMAILBOX_H

#include <stdint.h>

//*****************************************************************************
//
// The following are values that can be returned by the
// ChackFlashMailbox() API indicating the status.
//
//*****************************************************************************
#define BOOT_OVERRIDE_AND_MAILBOX_OPERATIONS_SUCCESS                 0x00
#define BOOT_OVERRIDE_OPERATION_FAIL                                       0x01
#define MAILBOX_ERASE_FAIL                                                 0x02

//*****************************************************************************
//
// The following are addresses used to check acknowledgements from the flash
// mailbox.
//
//*****************************************************************************
#define ACK_FACTORY_RESET                                            0x00200008
#define ACK_BSL_CONFIG                                               0x002001F4
#define ACK_JTAG_SWD_LOCK_SECEN                                      0x00200054
#define ACK_SEC_ZONE0_EN                                             0x002000B4
#define ACK_SEC_ZONE1_EN                                             0x00200114
#define ACK_SEC_ZONE2_EN                                             0x00200174
#define ACK_SEC_ZONE3_EN                                             0x002001D4
#define ACK_SEC_ZONE0_UPDATE                                         0x00200214
#define ACK_SEC_ZONE1_UPDATE                                         0x00200224
#define ACK_SEC_ZONE2_UPDATE                                         0x00200234
#define ACK_SEC_ZONE3_UPDATE                                         0x00200244
#define ACK_JTAG_SWD_LOCK_ENC_UPDATE                                 0x00200204

//*****************************************************************************
//
// Starting address of the flash mailbox
//
//*****************************************************************************
#define FLASH_MAILBOX_START                                          0x00200000

//*****************************************************************************
//
//! \brief Checks the mailbox for ack and erase
//!
//! This function checks the mailbox for an acknowledgement (ack) showing that
//! the flash mailbox command operated successfully. 
//!
//! \param flashMailboxCommand command to check
//!        Valid values are:
//!        - \b COMMAND_FACTORY_RESET
//!        - \b COMMAND_BSL_CONFIG
//!        - \b COMMAND_JTAG_SWD_LOCK_SECEN
//!        - \b COMMAND_SEC_ZONE0_EN
//!        - \b COMMAND_SEC_ZONE1_EN
//!        - \b COMMAND_SEC_ZONE2_EN
//!        - \b COMMAND_SEC_ZONE3_EN
//!        - \b COMMAND_SEC_ZONE0_UPDATE
//!        - \b COMMAND_SEC_ZONE1_UPDATE
//!        - \b COMMAND_SEC_ZONE2_UPDATE
//!        - \b COMMAND_SEC_ZONE3_UPDATE
//!        - \b COMMAND_JTAG_SWD_LOCK_ENC_UPDATE
//!        - \b COMMAND_NONE
//!
//! \return BOOT_OVERRIDE_AND_MAILBOX_OPERATIONS_SUCCESS if an
//!         ack was found and the mailbox was erased. Returns
//!         BOOT_OVERRIDE_OPERATION_FAIL if ack was not found or
//!         MAILBOX_ERASE_FAIL if the mailbox could not be erased
//
//*****************************************************************************
uint32_t CheckFlashMailbox(uint32_t flashMailboxCommand);

#endif
