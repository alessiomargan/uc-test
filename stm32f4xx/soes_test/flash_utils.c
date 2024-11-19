/*
 * flash_utils.c
 *
 * Author: amargan
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_crc.h"

#include "string.h"
#include "stdio.h"

#include "flash_utils.h"

extern CRC_HandleTypeDef hcrc;
extern void Error_Handler(void);

void Disable_interrupt()	{ __disable_irq(); }
void Enable_interrupt()		{ __enable_irq(); }


uint32_t Calc_CRC(uint32_t addr, uint32_t length) {

	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)addr, length);
	return crc;
}



static inline int Erase_Flash(FLASH_EraseInitTypeDef * eraseInfo) {

	HAL_StatusTypeDef flashStatus = HAL_OK;
	uint32_t ulBadBlocks = 0;

	HAL_FLASH_Unlock(); //Unlocks the flash memory
	flashStatus = HAL_FLASHEx_Erase(eraseInfo, &ulBadBlocks); //Deletes given sectors */
	HAL_FLASH_Lock();

	return flashStatus;
}

static inline int Erase_Flash_Sector(uint32_t flash_addr) {

	HAL_StatusTypeDef flashStatus = HAL_ERROR;
	FLASH_EraseInitTypeDef eraseInfo;
	eraseInfo.TypeErase = FLASH_TYPEERASE_SECTORS;
	eraseInfo.Banks = FLASH_BANK_1;
	eraseInfo.Sector = UINT32_MAX;
	eraseInfo.NbSectors = 0;			// number of sectors to be erased.
	eraseInfo.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	switch (flash_addr) {
	case FLASH_PARAM_ADDR:
		eraseInfo.Sector = FLASH_PARAM_SECTOR;
		eraseInfo.NbSectors = 1;			// number of sectors to be erased.
		break;
	case FLASH_CALIB_ADDR:
		eraseInfo.Sector = FLASH_CALIB_SECTOR;
		eraseInfo.NbSectors = 1;			// number of sectors to be erased.
		break;
	case FLASH_APP_ADDR:
		eraseInfo.Sector = FLASH_APP_SECTOR;
		eraseInfo.NbSectors = 3;			// number of sectors to be erased.
		break;
	case FLASH_NXT_ADDR:
		eraseInfo.Sector = FLASH_NXT_SECTOR;
		eraseInfo.NbSectors = 3;			// number of sectors to be erased.
		break;
	default:
		break;
	}

	if (eraseInfo.NbSectors != 0 && eraseInfo.Sector != UINT32_MAX) {
		flashStatus = Erase_Flash(&eraseInfo);
	}
	return flashStatus;
}

/*
 * use TYPEPROGRAM_WORD since flash_params_t is 32bit aligned
 */
static inline int Write_Flash_W(uint32_t flash_addr, uint32_t* src, size_t size)
{
	HAL_StatusTypeDef flashStatus = HAL_OK;
	size_t nbytes = size;
	uint32_t addr = flash_addr;
	HAL_FLASH_Unlock();
	while ( nbytes > 0 && flashStatus == HAL_OK) {
		// WORD is 32bit
		flashStatus = HAL_FLASH_Program(TYPEPROGRAM_WORD,addr,*src);
		addr  +=4;
		src   ++; // uint32_t *
		nbytes-=4;
	}
	HAL_FLASH_Lock();
	return flashStatus;
}
/*
 * use TYPEPROGRAM_DOUBLEWORD since we got only this
 */
static inline int Write_Flash_DW(uint32_t flash_addr, uint64_t * src, size_t size)
{
	HAL_StatusTypeDef flashStatus = HAL_OK;
	size_t nbytes = size;
	uint32_t addr = flash_addr;
	HAL_FLASH_Unlock();
	while ( nbytes > 0 && flashStatus == HAL_OK) {
		// DOUBLEWORD is 64bit
		//data = *src;
		flashStatus = HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD,addr,*src);
		addr  +=8;
		src   ++; // uint64_t *
		nbytes-=8;
	}
	HAL_FLASH_Lock();
	return flashStatus;
}


bool Write_flash(uint32_t flash_addr, void * src, size_t size) {

	HAL_StatusTypeDef 		flashStatus = HAL_ERROR;

	if ( Erase_Flash_Sector(flash_addr) == HAL_OK ) {
		flashStatus = Write_Flash_W (flash_addr, src, size);
	}

	return flashStatus == HAL_OK;
}
