#include "globals.h"
#include "params.h"

/************************************************************************************
 * Ethercat PDOs SDOs
 */
rx_pdo_t    	rx_pdo;
tx_pdo_t    	tx_pdo;
aux_pdo_rx_t	aux_pdo_rx;
aux_pdo_tx_t	aux_pdo_tx;
fault_t			glob_fault;

#pragma RETAIN(flash_sdo)
#pragma DATA_SECTION(flash_sdo, ".PAR_APP")
const flash_sdo_t	flash_sdo;

sdo_t	sdo = {
	.ram.fw_ver = "_(B)(B)_"
};

const flash_sdo_t	dflt_flash_sdo = {
	//
	._signature_ = FLASH_SIGN_DEFAULT,
	.board_id = 696,
	.module_params = 0,
};

void print_sdo(const flash_sdo_t *s) {

	DPRINT("sdo.flash._signature_=0x%04X\n", s->_signature_);
    DPRINT("sdo.flash.board_id=%d\n", s->board_id);
    DPRINT("sdo.flash.module_params=%d\n", s->module_params);
}


/************************************************************************************
 * FOE files
 */
uint32_t foe_write_cal_mat( foe_writefile_cfg_t * writefile_cfg, uint8_t * data, size_t length ) {
	return 1;
}
uint32_t foe_read_cal_mat ( foe_writefile_cfg_t * writefile_cfg, uint8_t * data, size_t length ) {
	return 1;
}

foe_cfg_t 	gFOE_config = { 0, 0, 0 ,0 };
uint8_t		foe_buffer[0x400];


foe_writefile_cfg_t      gFOE_firmware_files[] = {
	{
			.name =					"bb_param.bin",
			.max_data = 			FLASH_PARAM_MAX_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_PARAM_SECTOR, 	//
			.address_offset =		0,
			.filepass =				0xA4A4,
//			.write_function =		foe_write_flash,
//			.read_function =		foe_read_flash,
//			.on_foe_open = 			on_foe_open_cb,
//			.on_foe_close = 		on_foe_close_cb,
	},
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
