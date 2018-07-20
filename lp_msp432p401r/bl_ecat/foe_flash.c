#include <ti/devices/msp432p4xx/driverlib/flash.h>

#include "soes/utypes.h"
#include "soes/esc_foe.h"

#define APP_FLASH_SECTORS (                                             \
	FLASH_SECTOR0  | FLASH_SECTOR1  | FLASH_SECTOR2  | FLASH_SECTOR3  | \
    FLASH_SECTOR4  | FLASH_SECTOR5  | FLASH_SECTOR6  | FLASH_SECTOR7  | \
    FLASH_SECTOR8  | FLASH_SECTOR9  | FLASH_SECTOR10 | FLASH_SECTOR11 |	\
    FLASH_SECTOR12 | FLASH_SECTOR13 | FLASH_SECTOR14 | FLASH_SECTOR15 |	\
    FLASH_SECTOR16 | FLASH_SECTOR17 | FLASH_SECTOR18 | FLASH_SECTOR19 |	\
    FLASH_SECTOR20 | FLASH_SECTOR21 | FLASH_SECTOR22 | FLASH_SECTOR23 |	\
    FLASH_SECTOR24 | FLASH_SECTOR25 | FLASH_SECTOR26 | FLASH_SECTOR27 |	\
    FLASH_SECTOR28 | FLASH_SECTOR29 | FLASH_SECTOR30 | FLASH_SECTOR31 )

uint32_t foe_write_flash(foe_writefile_cfg_t * writefile_cfg, uint8_t * data) {

	return 0;
}

uint32_t foe_read_flash(foe_writefile_cfg_t * writefile_cfg, uint8_t * data) {

	return 0;
}

uint32_t on_foe_open_cb ( uint8_t op ) {

	// 1. Erase application memory section
	// Performs a mass erase on all unprotected flash sectors.
	// Protected sectors are ignored.
	// Unprotecting User Bank 1, Sectors 0 to 31 */
	FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, APP_FLASH_SECTORS);
	if( ! FlashCtl_performMassErase() ) {
		while(1);
	}

	return 0;
}

uint32_t on_foe_close_cb( void ) {

	// write crc

	return 0;
}
