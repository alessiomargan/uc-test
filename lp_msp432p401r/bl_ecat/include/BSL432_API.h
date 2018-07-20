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
 * \file BSL432_API.h
 * \brief Header file for the BSL API functions.
 */

#ifndef BSL432_API_H_
#define BSL432_API_H_

#include <stdint.h>

#define BSL432_STATUS_LOCKED   0x00
#define BSL432_STATUS_UNLOCKED 0xA5A5

/**
 * API initialization routine clears RAM and configures the device.
 */
extern void BSL432_API_init();

/**
 * Configures the timer for the BSL timeout.
 */
extern void BSL432_API_initTimeout();

/**
 * Enters Low Power Mode 4.5 (LPM4.5).
 */
extern void BSL432_API_enterLPM();

/**
 * Locks the BSL
 */
extern uint32_t BSL432_API_lockBSL();

/**
 * Compares the data buffer against the BSL password and sets lock state accordingly.
 */
extern uint32_t BSL432_API_unlockBSL(uint8_t *data);

/**
 * Unlocks the Flash memory for writing
 */
extern uint8_t BSL432_API_openMemory();

/**
 * Locks the Flash memory for writing
 */
extern uint8_t BSL432_API_closeMemory();

/**
 * Reads an array of bytes from memory into a supplied array
 */
extern uint8_t BSL432_API_readMemory(uint32_t addr, uint32_t length, uint8_t *data);

/**
 * Returns a CRC check on the memory specified
 */
extern uint8_t BSL432_API_CRCCheck(uint32_t addr, uint32_t length, uint16_t *result);

/**
 * Loads the Program Counter with the supplied address
 */
extern uint8_t BSL432_API_callAddress(uint32_t addr);

/**
 * Writes a byte array starting at a given address
 */
extern uint8_t BSL432_API_writeMemory(uint32_t startAddress, uint32_t size, uint8_t *data);

/**
 * Erases all code flash in the MSP432 except for Information Memory
 * and areas defined as regional secure zone.
 */
extern uint8_t BSL432_API_massErase(uint32_t locked);

/**
 * Triggers a REBOOT_RESET that causes the device to re-initialize ifself and causes bootcode to execute again.
 */
extern void BSL432_API_rebootReset();

/**
 * Erases the sector of the given address.
 */
extern uint8_t BSL432_API_eraseSector(uint32_t addr);

/**
 * Executes Factory Reset via flash mailbox.
 */
extern uint8_t BSL432_API_factoryReset(uint8_t *data);

#endif /* BSL432_API_H_ */
