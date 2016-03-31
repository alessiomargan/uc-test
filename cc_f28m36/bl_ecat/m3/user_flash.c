#include <stdbool.h>
#include <stdint.h>

#include "driverlib/device.h"
#include "driverlib/flash.h"

#include "F021_Concerto_Cortex.h"

#include "soes/esc_foe.h"

#include "shared_ram.h"
#include "flash_utils.h"

#include "soes_hook.h"
#include "osal.h"

#define FLASH_DEBUG
#ifdef FLASH_DEBUG
    #define DPRINT(...) OSAL_PRINT ("flash: "__VA_ARGS__)
    #define DEBUG_ASSERT(expression)    ASSERT(expression)
#else
    #define DPRINT(...)
    #define DEBUG_ASSERT(expression)
#endif  /* ESC_DEBUG */

//#define _16KSector_u32length 	0x1000
//#define _64KSector_u32length  0x4000

extern foe_cfg_t 	gFOE_config;
extern m3_bl_rw_data_t	m3_rw_data;
extern c28_bl_rw_data_t	c28_ro_data;



#pragma CODE_SECTION(erase_M3_app_flash,"ramfuncs");
bool erase_M3_app_flash() {

	DPRINT("M3 >> Erase flash 0x%04X and 0x%04X\n", M3_FLASH_APP_START, M3_APP_CRC_ADDR);
	return ( (Erase_flash_sector(M3_FLASH_APP_START) == Fapi_Status_Success) &&
			 (Erase_flash_sector(M3_APP_CRC_ADDR)    == Fapi_Status_Success) );

}

#pragma CODE_SECTION(erase_C28_app_flash,"ramfuncs");
bool erase_C28_app_flash() {

	DPRINT("C28 >> Erase flash 0x%04X\n", C28_FLASH_APP_START);
	return (ipc_c28_service(FN_ERASE_FLASH) == FN_ERASE_FLASH);
}

/*
 * return 0 on success
 */
#pragma CODE_SECTION(foe_write_M3_flash,"ramfuncs");
uint32_t foe_write_M3_flash(foe_writefile_cfg_t * writefile_cfg, uint8_t * data) {

    uint32_t ui32FlashAddr;
    uint32_t bytestowrite = gFOE_config.buffer_size;

    // What address are we about to program to?
	ui32FlashAddr = writefile_cfg->address_offset + writefile_cfg->dest_start_address;
    DPRINT("foe_write_M3 FlashAddr 0x%04X with %d bytes\n", ui32FlashAddr, bytestowrite);

    if ( Write_flash(ui32FlashAddr, data, bytestowrite) != Fapi_Status_Success ) {
		DPRINT("FAIL foe_write_flash FlashAddr 0x%04X with %d bytes\n",
				ui32FlashAddr, bytestowrite);
		return bytestowrite;
	}
	return 0;
}

#pragma CODE_SECTION(foe_write_C28_flash,"ramfuncs");
uint32 foe_write_C28_flash(foe_writefile_cfg_t * writefile_cfg, uint8 * data) {

	uint32_t ui32FlashAddr;
	uint32_t bytestowrite = gFOE_config.buffer_size;
	uint32_t offset_word;

	// set offset_word in word !!!
	offset_word = (writefile_cfg->address_offset)>>1;
	ui32FlashAddr = writefile_cfg->dest_start_address + offset_word;
	DPRINT("foe_write_C28 FlashAddr [0x%04X=0x%04X+0x%04X] with %d bytes %d words\n",
			ui32FlashAddr, writefile_cfg->dest_start_address, offset_word,
			bytestowrite, bytestowrite/2);

	//
	m3_rw_data.foe_flashAddr = ui32FlashAddr;

	if ( ipc_c28_service(FN_FOE_BUFF) != FN_FOE_BUFF ) {
		DPRINT("FAIL foe_write_shared_RAM FlashAddr 0x%04X with %d bytes\n",
				ui32FlashAddr, bytestowrite);
		return bytestowrite;
	}

	return 0;
}

#pragma CODE_SECTION(calc_app_crc,"ramfuncs");
uint32_t calc_app_crc(void) {

	uint32 i, k;
	uint32 crc_calc = 0;

	for(i=0, k=0; k < M3_FLASH_APP_SIZE; i++, k=i*4  ) {
		crc_calc ^= *(uint32*)(M3_FLASH_APP_START+k);
	}
    //crc_calc = 0xF0CACC1A;

	return crc_calc;
}

/**
 * called in FOE_close
 **/
#pragma CODE_SECTION(write_M3_app_crc,"ramfuncs");
void write_M3_app_crc(void) {

	uint32_t	crc;
    uint32_t 	ui32FlashAddr = M3_APP_CRC_ADDR;

	crc = calc_app_crc();
	if ( Write_flash(ui32FlashAddr, (uint8 *)&crc, sizeof(crc)) == Fapi_Status_Success ) {
		gFlash_crc = *(uint32*)M3_APP_CRC_ADDR;
		DPRINT("write_flash appCRC 0x%04X\n", gFlash_crc);
	} else {
		DPRINT("FAIL write_flash appCRC !!!\n");
	}
	return;
}

#pragma CODE_SECTION(write_C28_app_crc,"ramfuncs");
void write_C28_app_crc(void) {

	if ( ipc_c28_service(FN_APP_CRC) == FN_APP_CRC ) {
		DPRINT("ipc_c28_service calc 0x%04X flash 0x%04X\n", c28_ro_data.calc_crc, c28_ro_data.flash_crc);
	} else {
		DPRINT("FAIL write_C28_app_crc\n");
	}

	return;
}

//#pragma CODE_SECTION(test_C28_erase_write_flash,"ramfuncs");
bool test_C28_erase_write_flash(void) {

	return (ipc_c28_service(FN_TEST_FLS) == FN_TEST_FLS );
}
