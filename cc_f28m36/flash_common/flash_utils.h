#ifndef __FLASH_UTILS_H__
#define __FLASH_UTILS_H__

#include <stdint.h>

#ifdef _C28X
	//#include "c28/include/definitions.h"
    #include "F021_Concerto_C28x.h"
	#include "F28M36x_Device.h"
	#include "F28M36x_GlobalPrototypes.h"
	#include "F28M36x_Flash.h"
	#define DPRINT(...)
	#define  FLS_BANK_WIDTH	8
#else
	//#include "m3/include/definitions.h"
	#include "F021_Concerto_Cortex.h"
	#include "driverlib/device.h"
	#include "driverlib/flash.h"
	#define  FLS_BANK_WIDTH	16
#endif

#if (CPU_FRQ_150MHZ || DSP28_F28M36Px_150MHZ || DSP28_F28M36Hx_150MHZ)
     #define CPU_FRQ    150
#elif (CPU_FRQ_125MHZ || DSP28_F28M36Px_125MHZ)
     #define CPU_FRQ    125
#elif (CPU_FRQ_100MHZ || DSP28_F28M36Hx_100MHZ)
     #define CPU_FRQ    100
#elif CPU_FRQ_75MHZ
     #define CPU_FRQ    75
#elif CPU_FRQ_60MHZ
     #define CPU_FRQ    60
#else
     #error CPU_FREQ
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
