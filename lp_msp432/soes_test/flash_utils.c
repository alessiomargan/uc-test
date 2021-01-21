/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <soes_hook.h>
#include "globals.h"
#include "flash_utils.h"

uint32_t calc_CRC(uint32_t addr, uint32_t length)
{
    uint32_t crc;
    uint32_t addrIndex;

    MAP_CRC32_setSeed(0xFFFF, CRC32_MODE);  // Initialize CRC

    for(addrIndex = addr; addrIndex < addr + length; addrIndex++)
    {
    	MAP_CRC32_set8BitDataReversed(__data20_read_char(addrIndex), CRC32_MODE);
    }
    crc = MAP_CRC32_getResult(CRC32_MODE);
    return crc;
}

bool Write_flash(uint32_t flash_addr, void * src, size_t size ) {

	bool ret;
	uint32_t sectorNum, bankNum;

	// get flash bank and sector of flash_addr
#if defined(__MCU_HAS_FLCTL_A__)
	MAP_FlashCtl_A_getMemoryInfo(flash_addr, &bankNum, &sectorNum);
	DPRINT("\n%s addr 0x%04X size %d bank %d sector %d\n",
	    	__FUNCTION__, flash_addr, size, bankNum, sectorNum);
    // unprotect bank sector ...
    MAP_FlashCtl_A_unprotectMemory(flash_addr, flash_addr+size);
    if( ! (ret=MAP_FlashCtl_A_eraseSector(flash_addr)) ) {
    	DPRINT("%s FAIL FlashCtl_eraseSector\n", __FUNCTION__);
    	goto failure;
    }
    /* Trying to program the memory. Within this function, the API will
     * automatically try to program the maximum number of tries. */
    if( ! (ret=MAP_FlashCtl_A_programMemory(src, (void*)flash_addr, size)) ) {
    	DPRINT("%s FAIL FlashCtl_programMemory\n", __FUNCTION__);
    	goto failure;
    }
failure:
    // protect bank sector ...
    MAP_FlashCtl_A_protectMemory(flash_addr, flash_addr+size);
#endif
#if defined(__MCU_HAS_FLCTL__)
	MAP_FlashCtl_getMemoryInfo(flash_addr, &bankNum, &sectorNum);
	DPRINT("\n%s addr 0x%04X size %d bank %d sector %d\n",
	    	__FUNCTION__, flash_addr, size, bankNum, sectorNum);
    // unprotect bank sector ...
    MAP_FlashCtl_unprotectSector(0x1<<bankNum, 0x1<<sectorNum);
    if( ! (ret=MAP_FlashCtl_eraseSector(flash_addr)) ) {
    	DPRINT("%s FAIL FlashCtl_eraseSector\n", __FUNCTION__);
    	goto failure;
    }
    /* Trying to program the memory. Within this function, the API will
     * automatically try to program the maximum number of tries. */
    if( ! (ret=MAP_FlashCtl_programMemory(src, (void*)flash_addr, size)) ) {
    	DPRINT("%s FAIL FlashCtl_programMemory\n", __FUNCTION__);
    	goto failure;
    }
failure:
    // protect bank sector ...
    MAP_FlashCtl_protectSector(0x1<<bankNum, 0x1<<sectorNum);
#endif

	return ret;
}
