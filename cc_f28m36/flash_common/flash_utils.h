#ifndef __FLASH_UTILS_H__
#define __FLASH_UTILS_H__

#include <stdint.h>

#ifdef _C28X
	#include "F021_Concerto_C28x.h"
	#include "F28M36x_Device.h"
	#include "F28M36x_GlobalPrototypes.h"
	#include "F28M36x_Flash.h"
	#include "c28/include/definitions.h"
    #define DPRINT(...)
	#define  FLS_BANK_WIDTH	8
#else
	#include "F021_Concerto_Cortex.h"
	#include "driverlib/device.h"
	#include "driverlib/flash.h"
	#include "m3/include/definitions.h"
	#define  FLS_BANK_WIDTH	16
#endif


void Configure_flashAPI(void);

Fapi_StatusType Erase_flash_sector(uint32_t address);

#ifdef _C28X
Fapi_StatusType Write_flash(uint32_t ui32FlashAddr, uint16_t * dataBuffer, uint32_t dataBufferSize);
#else
Fapi_StatusType Write_flash(uint32_t ui32FlashAddr, uint8_t * dataBuffer, uint32_t dataBufferSize);
#endif

Fapi_StatusType Test_EraseWrite_flash(uint32_t sectorAddr);

#endif
