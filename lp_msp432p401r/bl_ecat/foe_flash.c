#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
   #include "MSP432P401x/BSL432_device_file.h"
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
   #include "MSP432P4111/BSL432_device_file.h"
#endif

#include "foe_flash.h"
#include <soes/esc_foe.h>

/*
 * bool FlashCtl_eraseSector ( uint32_t addr )
 *
 * Due to the hardware limitations of the flash controller, this function cannot erase a memory
 * adress in the same flash bank that it is executing from. If using the ROM version of this API
 * (by using the (ROM_ or MAP_ prefixes) this is a don’t care, however if this API resides in
 * flash then special care needs to be taken to ensure no code execution or reads happen in
 * the flash bank being programmed while this API is being executed.
 */

#define ALL_FLASH_SECTORS (                                             \
	FLASH_SECTOR0  | FLASH_SECTOR1  | FLASH_SECTOR2  | FLASH_SECTOR3  | \
    FLASH_SECTOR4  | FLASH_SECTOR5  | FLASH_SECTOR6  | FLASH_SECTOR7  | \
    FLASH_SECTOR8  | FLASH_SECTOR9  | FLASH_SECTOR10 | FLASH_SECTOR11 |	\
    FLASH_SECTOR12 | FLASH_SECTOR13 | FLASH_SECTOR14 | FLASH_SECTOR15 |	\
    FLASH_SECTOR16 | FLASH_SECTOR17 | FLASH_SECTOR18 | FLASH_SECTOR19 |	\
    FLASH_SECTOR20 | FLASH_SECTOR21 | FLASH_SECTOR22 | FLASH_SECTOR23 |	\
    FLASH_SECTOR24 | FLASH_SECTOR25 | FLASH_SECTOR26 | FLASH_SECTOR27 |	\
    FLASH_SECTOR28 | FLASH_SECTOR29 | FLASH_SECTOR30 | FLASH_SECTOR31 )

#define APP_FLASH_SECTORS (                                             \
	FLASH_SECTOR0  | FLASH_SECTOR1  | FLASH_SECTOR2  | FLASH_SECTOR3  | \
    FLASH_SECTOR4  | FLASH_SECTOR5  | FLASH_SECTOR6  | FLASH_SECTOR7  | \
    FLASH_SECTOR8  | FLASH_SECTOR9  | FLASH_SECTOR10 | FLASH_SECTOR11 |	\
    FLASH_SECTOR12 | FLASH_SECTOR13 | FLASH_SECTOR14 | FLASH_SECTOR15 )

extern foe_cfg_t 	gFOE_config;

#pragma RETAIN(BLDR_Version)
#pragma DATA_SECTION(BLDR_Version, ".BLDR_VERSION")
const uint8_t BLDR_Version[8] = "msp_1.1";

#pragma RETAIN(CRC_App)
#pragma DATA_SECTION(CRC_App, ".CRC_APP")
const uint32_t CRC_App;

uint32_t 	gCalc_crc;
uint16_t  	crc_ok;

uint32_t calc_CRC(uint32_t addr, uint32_t length)
{
    uint32_t crc;
	MAP_CRC32_setSeed(0xFFFF, CRC32_MODE);  // Initialize CRC

    uint32_t addrIndex;
    for(addrIndex = addr; addrIndex < addr + length; addrIndex++)
    {
    	MAP_CRC32_set8BitDataReversed(__data20_read_char(addrIndex), CRC32_MODE);
    }
    crc = MAP_CRC32_getResult(CRC32_MODE);
    return crc;
}


uint32_t foe_write_flash(foe_writefile_cfg_t * writefile_cfg, uint8_t * data, size_t length) {

	uint32_t bytestowrite = gFOE_config.buffer_size;
	uint32_t ui32FlashAddr = writefile_cfg->address_offset + writefile_cfg->dest_start_address;
    DPRINT("%s FlashAddr 0x%04X with %d bytes\n",
    		__FUNCTION__, ui32FlashAddr, bytestowrite);

    bool ret = MAP_FlashCtl_programMemory((void*)data, (void*)ui32FlashAddr, bytestowrite);

    // 0 on success
	return (!ret);
}

uint32_t foe_read_flash(foe_writefile_cfg_t * writefile_cfg, uint8_t * data, size_t length) {

	int i;
    uint32_t ui32FlashAddr;
    uint32_t bytestoread = gFOE_config.buffer_size;

    // What address are we about to read from ?
	ui32FlashAddr = writefile_cfg->address_offset + writefile_cfg->dest_start_address;
    DPRINT("%s FlashAddr 0x%04X with %d bytes\n",
    		__FUNCTION__, ui32FlashAddr, bytestoread);

	for( i=0; i < bytestoread; i++ ) {
		(*(data+i)) = (*((volatile uint8_t*)(ui32FlashAddr+i)));
	}
	return bytestoread;

}

uint32_t on_foe_open_cb ( uint8_t op ) {

	bool ret;
	// 1. Erase application memory section
	// Performs a mass erase on all unprotected flash sectors.
	// Protected sectors are ignored.
	// Unprotecting User Bank 1, Sectors 0 to 31 */
	MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, APP_FLASH_SECTORS);
	ret = MAP_FlashCtl_performMassErase();
    DPRINT("\n%s Erase application memory section %d\n", __FUNCTION__, ret);
	// 0 on success
	return (!ret);
}

uint32_t on_foe_close_cb( void ) {

	// write crc
	uint32_t sectorNum, bankNum;
	bool ret;

	gCalc_crc = calc_CRC(FLASH_APP_START, FLASH_APP_SIZE);
	// get flash bank and sector of crc address ....
	MAP_FlashCtl_getMemoryInfo((uint32_t)&CRC_App, &bankNum, &sectorNum);
    DPRINT("\n%s crc 0x%04X addr 0x%04X bank %d sector %d\n",
    		__FUNCTION__, gCalc_crc, &CRC_App, bankNum, sectorNum);

	// unprotect bank and sector ...
    MAP_FlashCtl_unprotectSector(0x1<<bankNum, 0x1<<sectorNum);
	// erase & program
	MAP_FlashCtl_eraseSector((uint32_t)&CRC_App);
	ret = MAP_FlashCtl_programMemory((void*)&gCalc_crc, (void*)&CRC_App, sizeof(gCalc_crc));
	// protect
	MAP_FlashCtl_protectSector(0x1<<bankNum, 0x1<<sectorNum);
	MAP_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, APP_FLASH_SECTORS);

	crc_ok = (gCalc_crc==CRC_App) ? 1 : 0;

	// 0 on success
	return (!ret);
}
