#include "m3/include/definitions.h"
#include "soes/utypes.h"
#include "soes/esc_foe.h"

extern uint32 foe_write_M3_flash(foe_writefile_cfg_t * writefile_cfg, uint8 * data);
extern uint32 foe_write_C28_flash(foe_writefile_cfg_t * writefile_cfg, uint8 * data);
extern void write_M3_app_crc(void);
extern void write_C28_app_crc(void);

foe_writefile_cfg_t      gFOE_firmware_files[] = {
    {
    		.name =					"cent_AC_m3.bin",
			.max_data = 			M3_FLASH_APP_SIZE, 	// sector size ?!?
			.dest_start_address =	M3_FLASH_APP_START, //
			.address_offset =		0,
			.filepass =				0xA550,
			.write_function =		foe_write_M3_flash,
			.on_foe_close = 		write_M3_app_crc,
    },
    {
    		.name =					"params.bin",
    		.max_data = 			0,
			.dest_start_address =	0,
			.address_offset =		0,
			.filepass =				0xDEAD,
			.write_function =		foe_write_M3_flash,
			.on_foe_close = 		0,
    },
    {
    		.name =					"cent_AC_c28.bin",
			.max_data = 			C28_FLASH_APP_SIZE * 2, 	// flash A sector size in bytes
			.dest_start_address =	C28_FLASH_APP_START, //
			.address_offset =		0,
			.filepass =				0xDAD0,
			.write_function =		foe_write_C28_flash,
			.on_foe_close = 		write_C28_app_crc,
    },
    { 0, 0, 0, 0, 0, 0 }
};


