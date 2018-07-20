/**
 * TEXAS INSTRUMENTS TEXT FILE LICENSE
 * 
 * 
 * Copyright (c) 2014 - 2017 Texas Instruments Incorporated
 * 
 * All rights reserved not granted herein.
 * 
 * Limited License.  
 * 
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive license under copyrights and patents it now or hereafter owns or controls to make, have made, use, import, offer to sell and sell ("Utilize") this software subject to the terms herein.  With respect to the foregoing patent license, such license is granted  solely to the extent that any such patent is necessary to Utilize the software alone.  The patent license shall not apply to any combinations which include this software, other than combinations with devices manufactured by or for TI ("TI Devices").  No hardware patent is licensed hereunder.
 * 
 * Redistributions must preserve existing copyright notices and reproduce this license (including the above copyright notice and the disclaimer and (if applicable) source code license limitations below) in the documentation and/or other materials provided with the distribution
 * 
 * Redistribution and use in binary form, without modification, are permitted provided that the following conditions are met:
 * 
 * *	No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any software provided in binary form.
 * 
 * *	any redistribution and use are licensed by TI for use only with TI Devices.
 * 
 * *	Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.
 * 
 * If software source code is provided to you, modification and redistribution of the source code are permitted provided that the following conditions are met:
 * 
 * *	any redistribution and use of the source code, including any resulting derivative works, are licensed by TI for use only with TI Devices.
 * 
 * *	any redistribution and use of any object code compiled from the source code and any resulting derivative works, are licensed by TI for use only with TI Devices.
 * 
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * DISCLAIMER.
 * 
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 

/**
 * \file BSL432_Command_Definition.h
 * \brief Header file for the BSL commands and responses definition
 */

#ifndef BSL432_COMMAND_DEFINITIONS_H_
#define BSL432_COMMAND_DEFINITIONS_H_

/* BSL432 Commands */
#define BSL432_RX_DATA_BLOCK     0x10
#define BSL432_RX_DATA_BLOCK_32  0x20
#define BSL432_RX_PASSWORD       0x21
#define BSL432_ERASE_SECTOR      0x12
#define BSL432_ERASE_SECTOR_32   0x22
#define BSL432_RX_MASS_ERASE     0x15
#define BSL432_REBOOT_RESET      0x25
#define BSL432_RX_CRC_CHECK      0x16
#define BSL432_RX_CRC_CHECK_32   0x26
#define BSL432_RX_LOAD_PC        0x17
#define BSL432_RX_LOAD_PC_32     0x27
#define BSL432_TX_DATA_BLOCK     0x18
#define BSL432_TX_DATA_BLOCK_32  0x28
#define BSL432_TX_BSL_VERSION    0x19
#define BSL432_FACTORY_RESET     0x30
#define BSL432_CHANGE_BAUD_RATE  0x52

/* BSL432 Responses */
#define BSL432_DATA_REPLY        0x3A
#define BSL432_MESSAGE_REPLY     0x3B
#define BSL432_ACK               BSL432_SUCCESSFUL_OPERATION

/* BSL432 Error Codes */
/* API error codes */
#define BSL432_SUCCESSFUL_OPERATION          0x00
#define BSL432_NO_EXCEPTIONS                 0x00
#define BSL432_MEMORY_WRITE_CHECK_FAILED     0x01
#define BSL432_FLASH_FAIL_BIT_SET            0x02
#define BSL432_VOLTAGE_CHANGE_DURING_PROGRAM 0x03
#define BSL432_LOCKED                        0x04
#define BSL432_PASSWORD_ERROR                0x05
#define BSL432_BYTE_WRITE_FORBIDDEN          0x06
/* Command Interpreter error codes */
#define BSL432_UNKNOWN_COMMAND               0x07
#define BSL432_LENGTH_TOO_BIG_FOR_BUFFER     0x08

/**
 * Interprets the command contained in the data buffer and
 * calls the appropriate BSL API function
 */
extern void BSL432_CI_interpretCommand();

#endif /* BSL432_COMMAND_DEFINITIONS_H_ */
