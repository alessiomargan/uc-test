#include "globals.h"

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
	.ram.fw_ver = "_(@)(@)_"
};

const flash_sdo_t	dflt_flash_sdo = {
	//
	._signature_ = FLASH_SIGN_DEFAULT,
	.board_id = 747,
	.analog_sample_freq = DFLT_SAMPLE_FREQ,
};

void print_sdo(const flash_sdo_t *s) {

	DPRINT("sdo.flash._signature_=0x%04X\n", s->_signature_);
    DPRINT("sdo.flash.board_id=%d\n", s->board_id);
    DPRINT("sdo.flash.analog_sample_freq=%d\n", s->analog_sample_freq);
}


/************************************************************************************
 * DMA Control Table
 */
#if defined(__TI_COMPILER_VERSION__)
	#pragma DATA_ALIGN(MSP_EXP432P401RLP_DMAControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
	#pragma data_alignment=1024
#elif defined(__GNUC__)
	__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
	__align(1024)
#endif
static DMA_ControlTable MSP_EXP432P401RLP_DMAControlTable[32];


/************************************************************************************
 * ADC14 and temp sensor
 */
uint32_t	cal30;
uint32_t	cal85;
float 		calDifference;
float		tempC;
float		tempF;

uint16_t 	raw_adc[SMPL_NUM][16];
float		conv_adc[16];

