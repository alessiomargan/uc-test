#ifndef __GLOBALS_H__
#define __GLOBALS_H__

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <cc.h>

#include <soes/esc_foe.h>
#include <soes_hook.h>
#include <shared_ram.h>

extern m3_to_c28_data_t		m3_rw_data;
extern c28_to_m3_data_t		c28_ro_data;

extern tx_pdo_t 			tx_pdo;
extern rx_pdo_t 			rx_pdo;
extern aux_pdo_rx_t			aux_pdo_rx;
extern aux_pdo_tx_t			aux_pdo_tx;
extern fault_t				glob_fault;
extern sdo_t 				sdo;
extern const flash_sdo_t	dflt_flash_sdo;
extern const flash_sdo_t	flash_sdo;

extern foe_cfg_t 	gFOE_config;
extern uint8_t		foe_buffer[0x400];
extern foe_writefile_cfg_t	gFOE_firmware_files[];

void print_sdo(const flash_sdo_t *);

#endif
