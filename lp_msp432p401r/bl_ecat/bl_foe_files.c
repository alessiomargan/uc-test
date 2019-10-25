#include "foe_flash.h"
#include <soes/esc_foe.h>

extern uint32_t foe_write_flash( foe_writefile_cfg_t * writefile_cfg, uint8_t * data, size_t length );
extern uint32_t foe_read_flash ( foe_writefile_cfg_t * writefile_cfg, uint8_t * data, size_t length );
extern uint32_t on_foe_open_cb ( uint8_t op );
extern uint32_t on_foe_close_cb( void );

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
    {
    		.name =					"soes_param.bin",
			.max_data = 			FLASH_PAR_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_PAR_START, 	//
			.address_offset =		0,
			.filepass =				0,
			.write_function =		foe_write_flash,
			.read_function =		foe_read_flash,
			.on_foe_open = 			on_foe_open_cb,
			.on_foe_close = 		on_foe_close_cb,
    },
	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////
    {
    		.name =					"ft6_msp432.bin",
			.max_data = 			FLASH_APP_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_APP_START, 	//
			.address_offset =		0,
			.filepass =				0xBEFA,
			.write_function =		foe_write_flash,
			.read_function =		foe_read_flash,
			.on_foe_open = 			on_foe_open_cb,
			.on_foe_close = 		on_foe_close_cb,
    },
    {
    		.name =					"ft6_param.bin",
			.max_data = 			FLASH_PAR_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_PAR_START, 	//
			.address_offset =		0,
			.filepass =				0,
			.write_function =		foe_write_flash,
			.read_function =		foe_read_flash,
			.on_foe_open = 			on_foe_open_cb,
			.on_foe_close = 		on_foe_close_cb,
    },
	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////
    {
    		.name =					"ain_msp432.bin",
			.max_data = 			FLASH_APP_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_APP_START, 	//
			.address_offset =		0,
			.filepass =				0xA1A1,
			.write_function =		foe_write_flash,
			.read_function =		foe_read_flash,
			.on_foe_open = 			on_foe_open_cb,
			.on_foe_close = 		on_foe_close_cb,
    },
    {
    		.name =					"ain_param.bin",
			.max_data = 			FLASH_PAR_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_PAR_START, 	//
			.address_offset =		0,
			.filepass =				0,
			.write_function =		foe_write_flash,
			.read_function =		foe_read_flash,
			.on_foe_open = 			on_foe_open_cb,
			.on_foe_close = 		on_foe_close_cb,
    },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

