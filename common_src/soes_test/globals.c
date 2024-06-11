#include "globals.h"
#include "flash_utils.h"

/************************************************************************************
 * Ethercat PDOs SDOs
 */
rx_pdo_t    	rx_pdo;
tx_pdo_t    	tx_pdo;
uint8_t *       rxpdo = (uint8_t*)&rx_pdo;
uint8_t *       txpdo = (uint8_t*)&tx_pdo;
aux_pdo_rx_t	aux_pdo_rx;
aux_pdo_tx_t	aux_pdo_tx;
fault_t			glob_fault;

//#pragma RETAIN(flash_sdo)
//#pragma DATA_SECTION(flash_sdo, ".PAR_APP")
__attribute__((section(".params")))
const flash_sdo_t	flash_sdo;

sdo_t	sdo = {
	.ram.fw_ver = "(@)^(@)"
};

const flash_sdo_t	dflt_flash_sdo = {
	//
	._signature_ = FLASH_SIGN_DEFAULT,
	.board_id = 696,
	.analog_sample_freq = DFLT_SAMPLE_FREQ,
	.module_params = 0,
	.blob = {'c','a','z','z','o',' ','d','i',' ','c','a','n','e',0},
	//.blob = {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,
	//         0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF},
};

void print_sdo(const flash_sdo_t *s) {

    int i=0;
	DPRINT("sdo.flash._signature_=0x%04X\n", s->_signature_);
    DPRINT("sdo.flash.board_id=%d\n", s->board_id);
    DPRINT("sdo.flash.analog_sample_freq=%d\n", s->analog_sample_freq);
    DPRINT("sdo.flash.module_params=%d\n", s->module_params);
    DPRINT("sdo.flash.blob=");
    for (i=0;i<sizeof(s->blob);i++) {
        printf("0x%02X ", s->blob[i]);
    }
    printf("\n");
}

/************************************************************************************
 * Calib matrix
 */
//#pragma RETAIN(calib_matrix)
//#pragma DATA_SECTION(calib_matrix, ".CALIB")
__attribute__((section(".calib")))
const volatile float calib_matrix[6][6];

const float	dflt_calib_matrix[6][6] = {
		{ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0, 0.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 },
};


/************************************************************************************
 * ADC14 and temp sensor
 */
uint32_t	cal30;
uint32_t	cal85;
float 		calDifference;
float		tempC;
float		tempF;

uint16_t 	raw_adc[SMPL_NUM][16];
uint16_t	raw_adc_count[16];
float		conv_adc[16];

/************************************************************************************
 * FOE files
 */
uint32_t foe_write_cal_mat( foe_file_cfg_t * writefile_cfg, uint8_t * data, size_t length ) {
	return 1;
}
uint32_t foe_read_cal_mat ( foe_file_cfg_t * writefile_cfg, uint8_t * data, size_t length ) {
	return 1;
}

foe_cfg_t 	gFOE_config = { 0, 0, 0 ,0 };
uint8_t		foe_buffer[0x400];


foe_file_cfg_t      gFOE_firmware_files[] = {
	{
			.name =					"ft6_param.bin",
			.max_data = 			FLASH_PARAM_MAX_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_PARAM_ADDR, 	//
			.address_offset =		0,
			.filepass =				0xA4A4,
//			.write_function =		foe_write_flash,
//			.read_function =		foe_read_flash,
//			.on_foe_open = 			on_foe_open_cb,
//			.on_foe_close = 		on_foe_close_cb,
	},
    {
    		.name =					"cal_mat.bin",
			.max_data = 			FLASH_CALIB_MAX_SIZE, 	// sector size ?!?
			.dest_start_address =	FLASH_CALIB_ADDR, 	//
			.address_offset =		0,
			.filepass =				0xCA71,
			.write_function =		foe_write_cal_mat,
			.read_function =		foe_read_cal_mat,
			.on_foe_open = 			0,
			.on_foe_close = 		0,
    },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
