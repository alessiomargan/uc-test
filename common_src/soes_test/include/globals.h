#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#ifdef __MSP432P401R__
/* msp432 DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#endif

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <cc.h>

#include <soes/esc_foe.h>
#include <soes_hook.h>

#define FLASH_SIGN_VALID		0xCAFE
#define FLASH_SIGN_DEFAULT		0xDEFA

#define FLASH_PARAM_SECTOR		0x12000
#define FLASH_PARAM_MAX_SIZE	0x1000
#define FLASH_CALIB_SECTOR		0x13000
#define FLASH_CALIB_MAX_SIZE	0x1000

#define SMCLK_FREQUENCY		CS_24MHZ
//#define SAMPLE_FREQUENCY	5000		//  5kHz
#define DFLT_SAMPLE_FREQ	10000		// 10kHz
//#define SAMPLE_FREQUENCY	20000		// 20kHz
#define VTICK				0.0001525f 	// 2.5V / 2^14
#define SMPL_NUM			8

extern tx_pdo_t 			tx_pdo;
extern rx_pdo_t 			rx_pdo;
extern aux_pdo_rx_t			aux_pdo_rx;
extern aux_pdo_tx_t			aux_pdo_tx;
extern fault_t				glob_fault;
extern sdo_t 				sdo;
extern const flash_sdo_t	dflt_flash_sdo;
extern const flash_sdo_t	flash_sdo;

extern uint32_t cal30;
extern uint32_t cal85;
extern float 	calDifference;
extern float 	tempC;

extern uint16_t raw_adc[SMPL_NUM][16];
extern uint16_t raw_adc_count[16];
extern float conv_adc[16];

#ifdef __MSP432P401R__
extern Timer_A_PWMConfig pwmConfig;
#endif
extern foe_cfg_t 	gFOE_config;
extern uint8_t		foe_buffer[0x400];
extern foe_writefile_cfg_t	gFOE_firmware_files[];

void print_sdo(const flash_sdo_t *);

#endif
