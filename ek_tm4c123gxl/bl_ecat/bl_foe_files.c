#include "soes/utypes.h"
#include "soes/esc_foe.h"

foe_writefile_cfg_t      firmware_foe_files[] = {
    {
		.name =					"soes_tm4c.bin",
		.max_data = 			0, // set in setup_esc_configs()
		.dest_start_address =	0, // set in setup_esc_configs()
		.address_offset =		0, // set in setup_esc_configs()
		.filepass =				0xB007,
		.write_function =		0, // set in setup_esc_configs()
		.read_function =		0,
		.on_foe_open = 			0,
		.on_foe_close =			0,
    },
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

