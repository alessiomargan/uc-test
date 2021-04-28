#ifndef __FOE_FLASH_H__
#define __FOE_FLASH_H__

#include <stdint.h>

#define FLASH_SIZE 			0x40000
#define FLASH_APP_START		0x08000
#define FLASH_APP_END		0x3F000
#define FLASH_PAR_START		0x3F000
#define FLASH_PAR_SIZE		0x01000
#define FLASH_APP_SIZE 		FLASH_SIZE - FLASH_PAR_SIZE - FLASH_APP_START

extern const uint8_t BLDR_Version[8];
extern const uint32_t CRC_App;

uint32_t calc_CRC(uint32_t addr, uint32_t length);

#endif
