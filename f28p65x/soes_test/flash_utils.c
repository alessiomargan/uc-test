/*
 * flash_utils.c
 *
 * Author: amargan
 */


#include "flash_utils.h"
#include "driverlib.h"

void Disable_interrupt()	{ DINT; }
void Enable_interrupt()		{ EINT; }


uint32_t Calc_CRC(uint32_t addr, uint32_t length) {

	uint32_t crc = 0; //HAL_CRC_Calculate(&hcrc, (uint32_t*)addr, length);
	return crc;
}

uint8_t Write_flash(uint32_t flash_addr, void * src, size_t size) {

	return 1;
}
