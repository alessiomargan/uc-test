#include "foe_flash.h"
#include <soes/esc_foe.h>

foe_writefile_cfg_t      gFOE_firmware_files[] = {
    // launchpad
	{
		.name =					"soes_tm4c.bin",
		.max_data = 			FLASH_APP_SIZE,
		.dest_start_address =	FLASH_APP_START,
		.address_offset =		0, // set in setup_esc_configs()
		.filepass =				0xB007,
		.write_function =		0, // set in setup_esc_configs()
		.read_function =		0,
		.on_foe_open = 			0,
		.on_foe_close =			0,
    },
	// hyq hub_board
    {
		.name =					"hub_board.bin",
		.max_data = 			FLASH_APP_SIZE,
		.dest_start_address =	FLASH_APP_START,
		.address_offset =		0,
		.filepass =				0xB001,
		.write_function =		0,
		.read_function =		0,
		.on_foe_open = 			0,
		.on_foe_close =			0,
    },
	// hyq valve_board
    {
		.name =					"valve_board.bin",
		.max_data = 			FLASH_APP_SIZE,
		.dest_start_address =	FLASH_APP_START,
		.address_offset =		0,
		.filepass =				0xB002,
		.write_function =		0,
		.read_function =		0,
		.on_foe_open = 			0,
		.on_foe_close =			0,
    },
	//
	{
		.name =					"fake.it",
		.max_data = 			0,
		.dest_start_address =	0,
		.address_offset =		0,
		.filepass =				0xDEAD,
		.write_function =		0,
		.read_function =		0,
		.on_foe_open = 			0,
		.on_foe_close =			0,
    },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

