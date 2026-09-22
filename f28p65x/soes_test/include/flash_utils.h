#ifndef _FLASH_UTILS_H_
#define _FLASH_UTILS_H_

#include "flash.h"
#include "FlashTech_F28P65x_C28x.h"
#include "flash_programming_f28p65x.h"

#define FLASH_PARAM_ADDR    FlashBank2StartAddress

// C28 Flash addresses and sizeof() use 16-bit addressable words.
#define DATA_FLASH_SECTOR_SIZE_WORDS   Sector2KB_u16length
#define DATA_FLASH_PROGRAM_SIZE_WORDS  8U
#define DATA_FLASH_ALIGNMENT_WORDS     4U

uint32_t calc_CRC(uint32_t addr, uint32_t length);

Fapi_StatusType Configure_flashAPI(void);
Fapi_StatusType Erase_dataFlashSector(uint32_t address, uint32_t size);
Fapi_StatusType Program_dataFlashSector(const uint16_t *src, uint32_t address, uint32_t size);
bool Write_flash(uint32_t flash_addr, const void *src, uint32_t size);

#endif
