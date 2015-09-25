#include "soes/utypes.h"
#include "soes/esc_foe.h"

   /** Name of file to receive from master */
   const char * name;
   /** Size of file,sizeof data we can recv */
   uint32       max_data;
   /** Where to store the data initially */
   uint32       dest_start_address;
   /** Current address during write of file */
   uint32       address_offset;
   /* FoE password */
   uint32       filepass;
   /* Pointer to application foe write function */
   uint32       (*write_function) (foe_writefile_cfg_t * self, uint8 * data);

foe_writefile_cfg_t      firmware_foe_files[] = {
    {
    		.name =					"soes_tm4c.bin",
			.max_data = 			0, // set in setup_esc_configs()
			.dest_start_address =	0, // set in setup_esc_configs()
			.address_offset =		0, // set in setup_esc_configs()
			.filepass =				0xB007,
			.write_function =		0 // set in setup_esc_configs()
    },
    {
    		.name =					"fake.it",
			.max_data = 			0,
			.dest_start_address =	0,
			.address_offset =		0,
			.filepass =				0xDEAD,
			.write_function =		0
    },
    { 0, 0, 0, 0, 0, 0 }
};

