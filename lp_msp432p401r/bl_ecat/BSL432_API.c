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
 * \file BSL432_API.c
 * \brief Source file for the BSL API functions.
 */

/*
 * Changelog
 * 2015-05-29  0004  MG  Updating DriverLib API for v01.05.00.16.
 * 2016-02-16  0005  MG  Adding Factory Reset function.
 * 2016-03-06  0006  MG  All pins set to input direction on LPM entry.
 * 2016-07-25  0007  MG  Removing port reconfigurations from LPM entry.
 *                       Adding defines for new Driver Library API.
 * 2017-11-28  0007  FS  Add different header file for MSP432P4111, MSP432P411V, and MSP432P411Y.
 */

#include "BSL432_API.h"
//#include "BSL432_Peripheral_Interface.h"
#include "BSL432_Command_Definitions.h"
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
   #include "MSP432P401x/BSL432_device_file.h"
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
   #include "MSP432P4111/BSL432_device_file.h"
#endif

#pragma RETAIN(BSL432_VersionAPI)
#pragma DATA_SECTION(BSL432_VersionAPI, ".BSL432_VERSION_API")
const uint8_t BSL432_VersionAPI[2] = {0x00, 0x07};

/**
 * Reflects if the BSL is locked or unlocked.
 */
volatile uint32_t BSL432_LockedStatus = BSL432_STATUS_LOCKED;

/**
 * API initialization routine clears RAM and configures the device.
 *
 * \return None
 */
void BSL432_API_init()
{
    /* Halt watchdog timer and disable interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Clear RAM */
    uint32_t *RAM_clear_Handle;
    for( RAM_clear_Handle = (uint32_t *) BSL432_SECURE_RAM_START;
    	 RAM_clear_Handle < (uint32_t *)(BSL432_SECURE_RAM_START + BSL432_SECURE_RAM_LENGTH);
    	 RAM_clear_Handle++ )
    {
        *RAM_clear_Handle = 0;
    }

    /* Clear LPMx.5 lock */
    PCM->CTL1 = PCM_KEY;
}

/**
 * Configures the timer for the BSL timeout.
 *
 * \return None
 */
void BSL432_API_initTimeout()
{
    BSL432_PI_initTimeout();
}

/**
 * Enters Low Power Mode 4.5 (LPM4.5).
 *
 * \return None
 */
void BSL432_API_enterLPM()
{
    MAP_PCM_enableRudeMode();
    MAP_PCM_shutdownDevice(PCM_LPM45);
    while(1);  // CPU trap, shoud never be reached
}

/**
 * Locks the BSL
 *
 * \return Status of operation
 */
uint32_t BSL432_API_lockBSL()
{
    BSL432_LockedStatus = BSL432_STATUS_LOCKED;
    return BSL432_SUCCESSFUL_OPERATION;
}

/**
 * Compares the data buffer against the BSL password and sets lock state accordingly.
 *
 * \paran data Pointer to the password in the buffer
 *
 * \return Status of operation
 */
uint32_t BSL432_API_unlockBSL(uint8_t *data)
{
    uint8_t passwordComparison = 0;
    uint8_t *interrupts = (uint8_t *)BSL432_INTERRUPT_VECTOR_START;
    uint32_t index;
    for(index = 0; index <= (BSL432_INTERRUPT_VECOTR_END - BSL432_INTERRUPT_VECTOR_START); index++, interrupts++)
    {
        passwordComparison |= *interrupts ^ data[index];
    }
    if(passwordComparison == 0)
    {
        BSL432_LockedStatus = BSL432_STATUS_UNLOCKED;
        return BSL432_SUCCESSFUL_OPERATION;
    }
    else
    {
        BSL432_API_massErase(BSL432_STATUS_UNLOCKED);
        return BSL432_PASSWORD_ERROR;
    }
}

/**
 * Unlocks the Flash memory for writing
 *
 * \return Status of operation
 */
uint8_t BSL432_API_openMemory()
{
    uint8_t exceptions = BSL432_LOCKED;

    if(BSL432_LockedStatus == BSL432_STATUS_UNLOCKED)
    {
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
        /* Unprotecting User Bank 0, Sector 0 to 31 */
        MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK0,
                FLASH_SECTOR0 | FLASH_SECTOR1 |
                FLASH_SECTOR2 | FLASH_SECTOR3 |
                FLASH_SECTOR4 | FLASH_SECTOR5 |
                FLASH_SECTOR6 | FLASH_SECTOR7 |
                FLASH_SECTOR8 | FLASH_SECTOR9 |
                FLASH_SECTOR10 | FLASH_SECTOR11 |
                FLASH_SECTOR12 | FLASH_SECTOR13 |
                FLASH_SECTOR14 | FLASH_SECTOR15 |
                FLASH_SECTOR16 | FLASH_SECTOR17 |
                FLASH_SECTOR18 | FLASH_SECTOR19 |
                FLASH_SECTOR20 | FLASH_SECTOR21 |
                FLASH_SECTOR22 | FLASH_SECTOR23 |
                FLASH_SECTOR24 | FLASH_SECTOR25 |
                FLASH_SECTOR26 | FLASH_SECTOR27 |
                FLASH_SECTOR28 | FLASH_SECTOR29 |
                FLASH_SECTOR30 | FLASH_SECTOR31);
        /* Unprotecting User Bank 1, Sector 0 to 31 */
        MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,
                FLASH_SECTOR0 | FLASH_SECTOR1 |
                FLASH_SECTOR2 | FLASH_SECTOR3 |
                FLASH_SECTOR4 | FLASH_SECTOR5 |
                FLASH_SECTOR6 | FLASH_SECTOR7 |
                FLASH_SECTOR8 | FLASH_SECTOR9 |
                FLASH_SECTOR10 | FLASH_SECTOR11 |
                FLASH_SECTOR12 | FLASH_SECTOR13 |
                FLASH_SECTOR14 | FLASH_SECTOR15 |
                FLASH_SECTOR16 | FLASH_SECTOR17 |
                FLASH_SECTOR18 | FLASH_SECTOR19 |
                FLASH_SECTOR20 | FLASH_SECTOR21 |
                FLASH_SECTOR22 | FLASH_SECTOR23 |
                FLASH_SECTOR24 | FLASH_SECTOR25 |
                FLASH_SECTOR26 | FLASH_SECTOR27 |
                FLASH_SECTOR28 | FLASH_SECTOR29 |
                FLASH_SECTOR30 | FLASH_SECTOR31);
        /* Unprotecting Info Bank 0, Sector 0 and 1 */
        MAP_FlashCtl_unprotectSector(FLASH_INFO_MEMORY_SPACE_BANK0,
                FLASH_SECTOR0 | FLASH_SECTOR1);
        /* Unprotecting Info Bank 1, Sector 0 and 1 */
        MAP_FlashCtl_unprotectSector(FLASH_INFO_MEMORY_SPACE_BANK1,
                FLASH_SECTOR0 | FLASH_SECTOR1);
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
        /* Unprotecting User Bank 0 and 1, Sector 0 to 31 */
        MAP_FlashCtl_A_unprotectMemory(FLASH_BASE, FLASH_BASE + MAP_SysCtl_A_getFlashSize() - 1);
        /* Unprotecting Info Bank 0 and 1, Sector 0 and 1 */
        MAP_FlashCtl_A_unprotectMemory(__INFO_FLASH_A_TECH_START__, __INFO_FLASH_A_TECH_START__ + MAP_SysCtl_A_getInfoFlashSize() - 1);
#else
#error Device not defined.
#endif
        exceptions = BSL432_SUCCESSFUL_OPERATION;
    }
    return exceptions;
}

/**
 * Locks the Flash memory for writing
 *
 * \return Status of operation
 */
uint8_t BSL432_API_closeMemory()
{
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
    /* Protecting User Bank 0, Sector 0 to 31 */
    MAP_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK0,
            FLASH_SECTOR0 | FLASH_SECTOR1 |
            FLASH_SECTOR2 | FLASH_SECTOR3 |
            FLASH_SECTOR4 | FLASH_SECTOR5 |
            FLASH_SECTOR6 | FLASH_SECTOR7 |
            FLASH_SECTOR8 | FLASH_SECTOR9 |
            FLASH_SECTOR10 | FLASH_SECTOR11 |
            FLASH_SECTOR12 | FLASH_SECTOR13 |
            FLASH_SECTOR14 | FLASH_SECTOR15 |
            FLASH_SECTOR16 | FLASH_SECTOR17 |
            FLASH_SECTOR18 | FLASH_SECTOR19 |
            FLASH_SECTOR20 | FLASH_SECTOR21 |
            FLASH_SECTOR22 | FLASH_SECTOR23 |
            FLASH_SECTOR24 | FLASH_SECTOR25 |
            FLASH_SECTOR26 | FLASH_SECTOR27 |
            FLASH_SECTOR28 | FLASH_SECTOR29 |
            FLASH_SECTOR30 | FLASH_SECTOR31);
    /* Protecting User Bank 1, Sector 0 to 31 */
    MAP_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,
            FLASH_SECTOR0 | FLASH_SECTOR1 |
            FLASH_SECTOR2 | FLASH_SECTOR3 |
            FLASH_SECTOR4 | FLASH_SECTOR5 |
            FLASH_SECTOR6 | FLASH_SECTOR7 |
            FLASH_SECTOR8 | FLASH_SECTOR9 |
            FLASH_SECTOR10 | FLASH_SECTOR11 |
            FLASH_SECTOR12 | FLASH_SECTOR13 |
            FLASH_SECTOR14 | FLASH_SECTOR15 |
            FLASH_SECTOR16 | FLASH_SECTOR17 |
            FLASH_SECTOR18 | FLASH_SECTOR19 |
            FLASH_SECTOR20 | FLASH_SECTOR21 |
            FLASH_SECTOR22 | FLASH_SECTOR23 |
            FLASH_SECTOR24 | FLASH_SECTOR25 |
            FLASH_SECTOR26 | FLASH_SECTOR27 |
            FLASH_SECTOR28 | FLASH_SECTOR29 |
            FLASH_SECTOR30 | FLASH_SECTOR31);
    /* Protecting Info Bank 0, Sector 0 and 1 */
    MAP_FlashCtl_protectSector(FLASH_INFO_MEMORY_SPACE_BANK0,
            FLASH_SECTOR0 | FLASH_SECTOR1);
    /* Protecting Info Bank 1, Sector 0 and 1 */
    MAP_FlashCtl_protectSector(FLASH_INFO_MEMORY_SPACE_BANK1,
            FLASH_SECTOR0 | FLASH_SECTOR1);
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
    /* Protecting User Bank 0 and 1, Sector 0 to 31 */
    MAP_FlashCtl_A_protectMemory(FLASH_BASE, FLASH_BASE + MAP_SysCtl_A_getFlashSize() - 1);
    /* Protecting Info Bank 0 and 1, Sector 0 and 1 */
    MAP_FlashCtl_A_protectMemory(__INFO_FLASH_A_TECH_START__, __INFO_FLASH_A_TECH_START__ + MAP_SysCtl_A_getInfoFlashSize() - 1);
#else
#error Device not defined.
#endif
    return BSL432_SUCCESSFUL_OPERATION;
}

/**
 * Reads an array of bytes from memory into a supplied array
 *
 * \param addr Address of memory to be read
 * \param length Number of bytes to be read
 * \param data Pointer of memory to write the read data to
 *
 * \return Status of operation
 */
uint8_t BSL432_API_readMemory(uint32_t addr, uint32_t length, uint8_t *data)
{
    int64_t i;

    uint8_t exceptions = BSL432_SUCCESSFUL_OPERATION;

    for(i = addr + length - 1; i >= addr; i--)
    {
        if(BSL432_LockedStatus == BSL432_STATUS_UNLOCKED)
        {
            data[(uint32_t)(i - addr)] = __data20_read_char((uint32_t)i);
        }
        else
        {
            return BSL432_LOCKED;
        }
    }
    return exceptions;
}

/**
 * Returns a CRC check on the memory specified
 *
 * \param addr Start address of the block to be processed by the CRC
 * \param length Number of bytes to be processed by the CRC
 * \param result Pointer to the CRC result
 *
 * \return Status of operation
 */
uint8_t BSL432_API_CRCCheck(uint32_t addr, uint32_t length, uint16_t *result)
{
    MAP_CRC32_setSeed(0xFFFF, CRC16_MODE);  // Initialize CRC

    uint32_t addrIndex;
    for(addrIndex = addr; addrIndex < addr + length; addrIndex++)
    {
        if(BSL432_LockedStatus == BSL432_STATUS_UNLOCKED)
        {
            MAP_CRC32_set8BitDataReversed(__data20_read_char(addrIndex), CRC16_MODE);
        }
        else
        {
            return BSL432_LOCKED;
        }
    }
    *result = MAP_CRC32_getResult(CRC16_MODE);
    return BSL432_SUCCESSFUL_OPERATION;
}

/**
 * Loads the Program Counter with the supplied address
 *
 * \para addr The address to branch to
 *
 * \return Status of operation
 */
uint8_t BSL432_API_callAddress(uint32_t addr)
{
    if(BSL432_LockedStatus == BSL432_STATUS_UNLOCKED)
    {
        ((void (*)())addr)();
        return BSL432_SUCCESSFUL_OPERATION;
    }
    return BSL432_LOCKED;
}

/**
 * Writes a byte array starting at a given address
 *
 * \param startAddress The start address of the memory to write to
 * \param size The number of bytes to write
 * \param data Pointer to the data that shall be written to the destination
 *
 * \return Status of operation
 */
uint8_t BSL432_API_writeMemory(uint32_t startAddress, uint32_t size, uint8_t *data)
{
    uint8_t exceptions = BSL432_SUCCESSFUL_OPERATION;
    if(BSL432_LockedStatus == BSL432_STATUS_UNLOCKED)
    {
        if(startAddress >= BSL432_RAM_START)  // Write to RAM and peripherals
        {
            // Copy data to RAM
            uint8_t *ram_pointer;
            ram_pointer = (uint8_t *)startAddress;
            while(size--)
            {
                *ram_pointer++ = *data++;
            }
        }
        else  // Write to FLASH
        {
            /* Perpetually trying to program the sector with the given data */
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
            while (!MAP_FlashCtl_programMemory(data, (void *)startAddress, size));
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
            while (!MAP_FlashCtl_A_programMemory(data, (void *)startAddress, size));
#else
#error Device not defined.
#endif
        }
    }
    else
    {
        exceptions = BSL432_LOCKED;
    }
    return exceptions;
}

/**
 * Erases all code flash in the MSP432 except for Information Memory
 * and areas defined as regional secure zone.
 *
 * \param locked selects if unlocked BSL is required to execute mass erase.
 *
 * \return Status of operation
 */
uint8_t BSL432_API_massErase(uint32_t locked)
{
    uint8_t exceptions = BSL432_SUCCESSFUL_OPERATION;
    if((BSL432_LockedStatus == BSL432_STATUS_UNLOCKED) || (locked == BSL432_STATUS_UNLOCKED))
    {
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
        /* Protecting Info Bank 0, Sector 0 and 1 */
        MAP_FlashCtl_protectSector(FLASH_INFO_MEMORY_SPACE_BANK0,
                FLASH_SECTOR0 | FLASH_SECTOR1);
        /* Protecting Info Bank 1, Sector 0 and 1 */
        MAP_FlashCtl_protectSector(FLASH_INFO_MEMORY_SPACE_BANK1,
                FLASH_SECTOR0 | FLASH_SECTOR1);
        /* Unprotecting User Bank 0, Sector 0 to 31  */
        MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK0,
                FLASH_SECTOR0 | FLASH_SECTOR1 |
                FLASH_SECTOR2 | FLASH_SECTOR3 |
                FLASH_SECTOR4 | FLASH_SECTOR5 |
                FLASH_SECTOR6 | FLASH_SECTOR7 |
                FLASH_SECTOR8 | FLASH_SECTOR9 |
                FLASH_SECTOR10 | FLASH_SECTOR11 |
                FLASH_SECTOR12 | FLASH_SECTOR13 |
                FLASH_SECTOR14 | FLASH_SECTOR15 |
                FLASH_SECTOR16 | FLASH_SECTOR17 |
                FLASH_SECTOR18 | FLASH_SECTOR19 |
                FLASH_SECTOR20 | FLASH_SECTOR21 |
                FLASH_SECTOR22 | FLASH_SECTOR23 |
                FLASH_SECTOR24 | FLASH_SECTOR25 |
                FLASH_SECTOR26 | FLASH_SECTOR27 |
                FLASH_SECTOR28 | FLASH_SECTOR29 |
                FLASH_SECTOR30 | FLASH_SECTOR31);
        /* Unprotecting User Bank 1, Sector 0 to 31  */
        MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,
                FLASH_SECTOR0 | FLASH_SECTOR1 |
                FLASH_SECTOR2 | FLASH_SECTOR3 |
                FLASH_SECTOR4 | FLASH_SECTOR5 |
                FLASH_SECTOR6 | FLASH_SECTOR7 |
                FLASH_SECTOR8 | FLASH_SECTOR9 |
                FLASH_SECTOR10 | FLASH_SECTOR11 |
                FLASH_SECTOR12 | FLASH_SECTOR13 |
                FLASH_SECTOR14 | FLASH_SECTOR15 |
                FLASH_SECTOR16 | FLASH_SECTOR17 |
                FLASH_SECTOR18 | FLASH_SECTOR19 |
                FLASH_SECTOR20 | FLASH_SECTOR21 |
                FLASH_SECTOR22 | FLASH_SECTOR23 |
                FLASH_SECTOR24 | FLASH_SECTOR25 |
                FLASH_SECTOR26 | FLASH_SECTOR27 |
                FLASH_SECTOR28 | FLASH_SECTOR29 |
                FLASH_SECTOR30 | FLASH_SECTOR31);
        MAP_FlashCtl_performMassErase();
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
        /* Protecting Info Bank 0 and 1, Sector 0 and 1 */
        MAP_FlashCtl_A_protectMemory(__INFO_FLASH_A_TECH_START__, __INFO_FLASH_A_TECH_START__ + MAP_SysCtl_A_getInfoFlashSize() - 1);
        /* Unprotecting User Bank 0 and 1, Sector 0 to 31  */
        MAP_FlashCtl_A_unprotectMemory(FLASH_BASE, FLASH_BASE + MAP_SysCtl_A_getFlashSize() - 1);
        MAP_FlashCtl_A_performMassErase();
#else
#error Device not defined.
#endif
        BSL432_API_closeMemory();
    }
    else
    {
        exceptions = BSL432_LOCKED;
    }
    return exceptions;
}

/**
 * Triggers a REBOOT_RESET that causes the device to re-initialize ifself and causes bootcode to execute again.
 *
 * \return None
 */
void BSL432_API_rebootReset()
{
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
    MAP_SysCtl_rebootDevice();
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
    MAP_SysCtl_A_rebootDevice();
#else
#error Device not defined.
#endif
}

/**
 * Erases the sector of the given address.
 *
 * \param address address inside the sector to erase
 *
 * \return Status of operation
 */
uint8_t BSL432_API_eraseSector(uint32_t addr)
{
    uint8_t exceptions = BSL432_LOCKED;
    if(BSL432_LockedStatus == BSL432_STATUS_UNLOCKED)
    {
        exceptions = BSL432_SUCCESSFUL_OPERATION;
        exceptions = BSL432_API_openMemory();
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
        MAP_FlashCtl_eraseSector(addr);
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
        MAP_FlashCtl_A_eraseSector(addr);
#else
#error Device not defined.
#endif
        exceptions = BSL432_API_closeMemory();
    }
    else
    {
        exceptions = BSL432_LOCKED;
    }
    return exceptions;
}

/**
 * Executes a Factory Reset via flash mailbox.
 * The device will always restart and is not returning.
 *
 * \return None
 */
uint8_t BSL432_API_factoryReset(uint8_t *data)
{

    const uint32_t MB_START = MB_START_SIG;  // Mailbox start signature
    const uint32_t MB_CMD = MB_FACTORY_RESET_PARAMS;
    const uint32_t MB_END = MB_END_SIG;

    uint8_t exceptions = BSL432_SUCCESSFUL_OPERATION;
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
    MAP_FlashCtl_unprotectSector(FLASH_INFO_MEMORY_SPACE_BANK0, FLASH_SECTOR0);
    MAP_FlashCtl_eraseSector(BSL432_FLASHMAILBOXSTART);
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
    MAP_FlashCtl_A_unprotectMemory(__INFO_FLASH_A_TECH_START__, __INFO_FLASH_A_TECH_START__ + FLASH_A_SECTOR_SIZE - 1);
    MAP_FlashCtl_A_eraseSector(BSL432_FLASHMAILBOXSTART);
#else
#error Device not defined.
#endif

#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
    MAP_FlashCtl_programMemory((void *)&MB_START, (void *)&MB_ADDR_START, 4);
    MAP_FlashCtl_programMemory((void *)&MB_CMD, (void *)&MB_ADDR_CMD, 4);
    MAP_FlashCtl_programMemory(data, (void *)&MB_ADDR_FRPW, 16);
    MAP_FlashCtl_programMemory((void *)&MB_END, (void *)&MB_ADDR_END, 4);
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
    MAP_FlashCtl_A_programMemory((void *)&MB_START, (void *)&MB_ADDR_START, 4);
    MAP_FlashCtl_A_programMemory((void *)&MB_CMD, (void *)&MB_ADDR_CMD, 4);
    MAP_FlashCtl_A_programMemory(data, (void *)&MB_ADDR_FRPW, 16);
    MAP_FlashCtl_A_programMemory((void *)&MB_END, (void *)&MB_ADDR_END, 4);
#else
#error Device not defined.
#endif

#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
    MAP_SysCtl_rebootDevice();  // Reboot to execute the flash mailbox command
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
    MAP_SysCtl_A_rebootDevice();  // Reboot to execute the flash mailbox command
#else
#error Device not defined.
#endif

    // The device will reset and this function never returns regardless if FR was successful or failing.
    return exceptions;
}
