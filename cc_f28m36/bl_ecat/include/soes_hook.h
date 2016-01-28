#ifndef __SOES_HOOK_H__
#define __SOES_HOOK_H__

#include <stdbool.h>

extern uint16_t gFlash_cmd;
extern uint16_t gFlash_cmd_ack;
extern uint32_t gFlash_crc;
extern uint16_t	gCrc_ok;
extern uint16_t gET1100_boot_pin;
extern char 	gBL_ver[];

void jump_to_app(void);
bool test_jump_to_app(void);

void setup_esc_configs(void);
void ecat_process_pdo(void);

#endif 
