#ifndef __FLASH_UTILS_H__
#define __FLASH_UTILS_H__

uint32_t calc_CRC(uint32_t addr, uint32_t length);

bool Write_Flash(void * dest, uint32_t flash_addr, size_t size);

#endif
