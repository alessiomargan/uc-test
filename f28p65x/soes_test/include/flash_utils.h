#ifndef _FLASH_UTILS_H_
#define _FLASH_UTILS_H_

#include <cc.h>

#define FLASH_APP_SECTOR		FLASH_SECTOR_0
#define FLASH_APP_ADDR			0x8000000 // sector 0
#define FLASH_APP_MAX_SIZE		48 // sector 0 1 2

#define FLASH_NXT_SECTOR		FLASH_SECTOR_5
#define FLASH_NXT_ADDR			0x8020000 // sector 0
#define FLASH_NXT_MAX_SIZE		384 // sector 5 6 7


#define FLASH_PARAM_SECTOR		FLASH_SECTOR_3
#define FLASH_PARAM_ADDR		0x800C000 // sector 3
#define FLASH_PARAM_MAX_SIZE	16

#define FLASH_CALIB_SECTOR		FLASH_SECTOR_4
#define FLASH_CALIB_ADDR		0x8010000 // sector 4
#define FLASH_CALIB_MAX_SIZE	64

uint32_t calc_CRC(uint32_t addr, uint32_t length);

uint8_t Write_flash(uint32_t flash_addr, void * src, size_t size);

#endif
