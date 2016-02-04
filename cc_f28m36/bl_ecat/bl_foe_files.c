#include "bl_config.h"
#include "soes/utypes.h"
#include "soes/esc_foe.h"

extern uint32 foe_write_flash(foe_writefile_cfg_t * writefile_cfg, uint8 * data);
extern uint32 foe_write_shared_RAM(foe_writefile_cfg_t * writefile_cfg, uint8 * data);
extern void write_app_crc(void);

foe_writefile_cfg_t      gFOE_firmware_files[] = {
    {
    		.name =					"soes_m3.bin",
			.max_data = 			FLASH_APP_SIZE, 	// sector size ?!?
			.dest_start_address =	APP_START_ADDRESS, //
			.address_offset =		0,
			.filepass =				0xDEAD,
			.write_function =		foe_write_flash,
			.on_foe_close = 		write_app_crc,
    },
    {
    		.name =					"params.bin",
    		.max_data = 			0,
			.dest_start_address =	0,
			.address_offset =		0,
			.filepass =				0xDEAD,
			.write_function =		foe_write_flash,
			.on_foe_close = 		0,
    },
    {
    		.name =					"soes_c28.bin",
			.max_data = 			FLASH_APP_SIZE, 	// sector size ?!?
			.dest_start_address =	APP_START_ADDRESS, //
			.address_offset =		0,
			.filepass =				0xDAD0,
			.write_function =		foe_write_shared_RAM,
			.on_foe_close = 		0,
    },
    { 0, 0, 0, 0, 0, 0 }
};


