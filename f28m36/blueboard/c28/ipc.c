#include "c28/include/definitions.h"
#include "common/include/shared_ram.h"
#include "flash_utils.h"

#define FLASHA_START	0x13E000
#define FLASHD_START 	0x138000

void IpcService(uint32_t param)
{

	switch (param) {

	default:
		//bootloaderServiceResult = FN_ERROR;
		break;

	}

}
