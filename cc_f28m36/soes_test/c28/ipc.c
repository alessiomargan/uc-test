#include "definitions.h"
#include "shared_ram.h"

#include "F021_Concerto_C28x.h"

#define FLASHA_START	0x13E000

void bootloaderService(Uint32 param)
{

	switch (param) {

	case FN_ERASE_FLASH :
		if ( erase_flash_sector(FLASHA_START) == Fapi_Status_Success ) {
			bootloaderServiceResult = FN_ERASE_FLASH;
		} else {
			bootloaderServiceResult = FN_ERROR;
		}
		break;
	case FN_FOE_BUFF :

		bootloaderServiceResult = FN_FOE_BUFF;
		break;
	default:
		bootloaderServiceResult = FN_ERROR;
		break;

	}

}
