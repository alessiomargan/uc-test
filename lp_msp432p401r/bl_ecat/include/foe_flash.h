#ifndef __FOE_FLASH_H__
#define __FOE_FLASH_H__

#include <stdint.h>

#define FLASH_APP_START	0x20000
#define FLASH_APP_SIZE	0x20000

extern const uint8_t BLDR_Version[8];
extern const uint32_t CRC_App;

uint32_t calc_CRC(uint32_t addr, uint32_t length);

#endif
