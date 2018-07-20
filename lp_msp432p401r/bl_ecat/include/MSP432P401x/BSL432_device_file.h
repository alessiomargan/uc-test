/**
 * TEXAS INSTRUMENTS TEXT FILE LICENSE
 * 
 * 
 * Copyright (c) 2014 - 2016 Texas Instruments Incorporated
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
 * \file BSL432_device_file.h
 * \brief The BSL device file contains device specific settings for the BSL.
 */

/*
 * Changelog
 * 2015-06-03  0004  MG  Adding TLV information for MSP432P401R
 * 2017-11-28  0007  FS  Add different header file for MSP432P401M.
 */

#ifndef BSL432_DEVICE_FILE_H_
#define BSL432_DEVICE_FILE_H_

/*
#if defined (__MSP432P401R__)
   #include "msp432p401r.h"
#elif defined (__MSP432P401M__)
   #include "msp432p401m.h"
#else
   #error "Device not supported by this project"
#endif
*/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


#define BSL432_VERSION  {0x00, 0x0A}  // Build ID

#define BSL432_RAM_BSL_PARAMS     0x200000e4
#define BSL432_SECURE_RAM_START   (BSL432_RAM_BSL_PARAMS + 4)  // Do not overwrite the BSL_PARAMS
#define BSL432_SECURE_RAM_LENGTH  (0x20001FC4 - BSL432_SECURE_RAM_START)  // Clear up to the stack pointer

#define BSL432_INTERRUPT_VECTOR_START  0x0
#define BSL432_INTERRUPT_VECOTR_END    0xFF

#define BSL432_RAM_START  0x20000000

#define BSL432_TIMEOUT 32768  /** BSL timeout in ~1/3276 seconds */

/* Flash mailbox */
#define BSL432_FLASHMAILBOXSTART  0x200000
#define MB_ADDR_START  (*((volatile uint32_t *) (BSL432_FLASHMAILBOXSTART + 0x0)))
#define MB_ADDR_CMD    (*((volatile uint32_t *) (BSL432_FLASHMAILBOXSTART + 0x4)))
#define MB_ADDR_FRPW   (*((volatile uint32_t *) (BSL432_FLASHMAILBOXSTART + 0x270)))
#define MB_ADDR_FRACK  (*((volatile uint32_t *) (BSL432_FLASHMAILBOXSTART + 0x280)))
#define MB_ADDR_END    (*((volatile uint32_t *) (BSL432_FLASHMAILBOXSTART + 0x28C)))

#define MB_START_SIG  0x00115ACF6  // Mailbox start signature
#define MB_FACTORY_RESET_PARAMS  0x00010000
#define MB_END_SIG  0x0011E11D  // Mailbox end signature


/* TLV BSL entries */
#define BSL432_TLV_BSL_CONFIGURATION_TAG  0x0F
#define BSL432_TLV_INSTANCE_BSL_PERIPHERAL_INTERFACE_SELECTION  0

/* BSL_PER_IF_SEL */
#define BSL432_BSL_PER_IF_SEL_UART_INST     (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 0) & 0b1111)
#define BSL432_BSL_PER_IF_SEL_UART_MOD      (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 4) & 0b11)
#define BSL432_BSL_PER_IF_SEL_UART_MUX      (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 7) & 0b1)
#define BSL432_BSL_PER_IF_SEL_SPI_MOD       (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 12) & 0b11)
#define BSL432_BSL_PER_IF_SEL_SPI_INST      (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 8) & 0b1111)
#define BSL432_BSL_PER_IF_SEL_SPI_MUX       (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 15) & 0b1)
#define BSL432_BSL_PER_IF_SEL_I2C_INST      (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 16) & 0b1111)
#define BSL432_BSL_PER_IF_SEL_I2C_MOD       (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 20) & 0b11)
#define BSL432_BSL_PER_IF_SEL_I2C_MUX       (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPeripheralInterfaceSelection) >> 23) & 0b1)

/* BSL_PORTCNF_UART */
#define BSL432_BSL_PORTCNF_UART_UART_PORT   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationUART) >> 0) & 0b1111)
#define BSL432_BSL_PORTCNF_UART_BITPOS_RXD  (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationUART) >> 4) & 0b111)
#define BSL432_BSL_PORTCNF_UART_BITPOS_TXD  (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationUART) >> 7) & 0b111)
#define BSL432_BSL_PORTCNF_UART_PSEL0_RXD   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationUART) >> 16) & 0b1)
#define BSL432_BSL_PORTCNF_UART_PSEL0_TXD   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationUART) >> 17) & 0b1)
#define BSL432_BSL_PORTCNF_UART_PSEL1_RXD   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationUART) >> 24) & 0b1)
#define BSL432_BSL_PORTCNF_UART_PSEL1_TXD   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationUART) >> 25) & 0b1)

/* BSL_PORTCNF_SPI */
#define BSL432_BSL_PORTCNF_SPI_SPI_PORT     (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 0) & 0b1111)
#define BSL432_BSL_PORTCNF_SPI_BITPOS_SOMI  (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 4) & 0b111)
#define BSL432_BSL_PORTCNF_SPI_BITPOS_CLK   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 10) & 0b111)
#define BSL432_BSL_PORTCNF_SPI_BITPOS_SIMO  (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 7) & 0b111)
#define BSL432_BSL_PORTCNF_SPI_BITPOS_STE   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 13) & 0b111)
#define BSL432_BSL_PORTCNF_SPI_PSEL0_SOMI   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 16) & 0b1)
#define BSL432_BSL_PORTCNF_SPI_PSEL0_SIMO   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 17) & 0b1)
#define BSL432_BSL_PORTCNF_SPI_PSEL0_CLK    (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 18) & 0b1)
#define BSL432_BSL_PORTCNF_SPI_PSEL0_STE    (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 19) & 0b1)
#define BSL432_BSL_PORTCNF_SPI_PSEL1_SOMI   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 24) & 0b1)
#define BSL432_BSL_PORTCNF_SPI_PSEL1_SIMO   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 25) & 0b1)
#define BSL432_BSL_PORTCNF_SPI_PSEL1_CLK    (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 26) & 0b1)
#define BSL432_BSL_PORTCNF_SPI_PSEL1_STE    (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationSPI) >> 27) & 0b1)

/* BSL_PORTCNF_I2C */
#define BSL432_BSL_PORTCNF_I2C_I2C_PORT     (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationI2C) >> 0) & 0b1111)
#define BSL432_BSL_PORTCNF_I2C_BITPOS_SCL   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationI2C) >> 4) & 0b111)
#define BSL432_BSL_PORTCNF_I2C_BITPOS_SDA   (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationI2C) >> 7) & 0b111)
#define BSL432_BSL_PORTCNF_I2C_PSEL0_SCL    (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationI2C) >> 16) & 0b1)
#define BSL432_BSL_PORTCNF_I2C_PSEL0_SDA    (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationI2C) >> 17) & 0b1)
#define BSL432_BSL_PORTCNF_I2C_PSEL1_SCL    (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationI2C) >> 24) & 0b1)
#define BSL432_BSL_PORTCNF_I2C_PSEL1_SDA    (uint8_t)(((BSL432_TLV_BSL_Configuration->BSLPortInterfaceConfigurationI2C) >> 25) & 0b1)

#endif /* BSL432_DEVICE_FILE_H_ */
