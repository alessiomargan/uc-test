#include "foe_flash.h"
#include "soes/utypes.h"
#include "soes/esc_foe.h"

extern uint32 foe_write_flash(foe_writefile_cfg_t * writefile_cfg, uint8 * data);
extern uint32 foe_read_flash(foe_writefile_cfg_t * writefile_cfg, uint8 * data);
extern uint32 on_foe_open_cb ( uint8 op );
extern uint32 on_foe_close_cb( void );

foe_writefile_cfg_t      gFOE_firmware_files[] = {
	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////
    {
    		.name =					"soes_msp432.bin",
			.max_data = 			FLASH_APP_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_APP_START, 	//
			.address_offset =		0,
			.filepass =				0xBEFA,
			.write_function =		foe_write_flash,
			.read_function =		foe_read_flash,
			.on_foe_open = 			on_foe_open_cb,
			.on_foe_close = 		on_foe_close_cb,
    },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

