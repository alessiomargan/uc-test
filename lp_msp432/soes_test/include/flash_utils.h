#ifndef __FLASH_UTILS_H__
#define __FLASH_UTILS_H__

uint32_t calc_CRC(uint32_t addr, uint32_t length);

bool Write_Flash(uint32_t flash_addr, void * src, size_t size);

#endif
