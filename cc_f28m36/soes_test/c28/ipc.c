#include "F021_Concerto_C28x.h"

#include "definitions.h"
#include "shared_ram.h"
#include "flash_utils.h"

#define FLASHA_START	0x13E000
#define FLASHD_START 	0x138000

void bootloaderService(Uint32 param)
{

	switch (param) {

	case FN_ERASE_FLASH :
		if ( Erase_flash_sector(FLASHA_START) == Fapi_Status_Success ) {
			bootloaderServiceResult = FN_ERASE_FLASH;
		} else {
			bootloaderServiceResult = FN_ERROR;
		}
		break;
	case FN_FOE_BUFF :
		if ( Write_flash(m3_ro_data.foe_flashAddr, m3_ro_data.foe_buffer, 128) == Fapi_Status_Success ) {
			bootloaderServiceResult = FN_FOE_BUFF;
		} else {
			bootloaderServiceResult = FN_ERROR;
		}
		break;
	default:
		bootloaderServiceResult = FN_ERROR;
		break;

	}

}
