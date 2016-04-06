#include "F021_Concerto_C28x.h"

#include "flash_utils.h"

#include "c28/include/definitions.h"

uint16_t do_jump_to_app = 0;

void jump_to_app(void) {
	DINT;
	asm("	LB 0x137FFE");
}

uint32_t calc_app_crc(void) {

	uint32_t i, k;
	uint32_t crc_calc = 0xF0CACC1A;

	for( i=0,k=0; k < C28_FLASH_APP_SIZE; i++,k=i*2 ) {
		crc_calc ^= *(uint32*)(FLASHE_START+k);
	}

    return crc_calc;
}

Fapi_StatusType write_app_crc(void) {

	uint32_t	crc;
    uint32_t 	ui32FlashAddr = C28_APP_CRC_ADDR;

	crc = calc_app_crc();
	if ( Write_flash(ui32FlashAddr, (uint16_t*)&crc, sizeof(crc)) == Fapi_Status_Success ) {
		c28_rw_data.flash_crc = *(uint32*)C28_APP_CRC_ADDR;
		c28_rw_data.calc_crc = crc;
		return Fapi_Status_Success;
	} else {
		return Fapi_Error_Fail;
	}
}

void bootloaderService(Uint32 param)
{
	Fapi_StatusType	oReturnCheck;

	switch (param) {

	case FN_ERASE_FLASH :
		if ( Erase_flash_sector(FLASHE_START) == Fapi_Status_Success ) {
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

	case FN_APP_CRC :
		if ( write_app_crc() == Fapi_Status_Success ) {
			bootloaderServiceResult = FN_APP_CRC;
		} else {
			bootloaderServiceResult = FN_ERROR;
		}
		break;

	case FN_TEST_FLS :
		oReturnCheck = Test_EraseWrite_flash(0x100000);
		c28_rw_data.test_type_uint32 = oReturnCheck;
		if ( oReturnCheck == Fapi_Status_Success ) {
			bootloaderServiceResult = FN_TEST_FLS;
		} else {
			bootloaderServiceResult = FN_ERROR;
		}
		break;

	case FN_JUMP_APP :
		do_jump_to_app = 0xDEAD;
		bootloaderServiceResult = FN_JUMP_APP;
		break;

	default:
		bootloaderServiceResult = FN_ERROR;
		break;

	}

}
