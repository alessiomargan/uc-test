#ifndef __SOES_HOOK_H__
#define __SOES_HOOK_H__

#include <stdbool.h>
#include <shared_ram.h>

extern uint16_t gFlash_cmd;
extern uint16_t gFlash_cmd_ack;
extern uint32_t gFlash_crc;
extern uint16_t	m3_crc_ok;
extern uint16_t	c28_crc_ok;
extern uint16_t gET1100_boot_pin;
extern char 	gBL_ver[];

extern m3_bl_rw_data_t		m3_bl_rw_data;
extern c28_bl_rw_data_t	c28_bl_ro_data;

void jump_to_app(void);
bool test_jump_to_app(void);

void setup_esc_configs(void);
void ecat_process_pdo(void);

#endif 
