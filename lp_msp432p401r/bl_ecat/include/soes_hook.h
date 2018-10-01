#ifndef __SOES_HOOK_H__
#define __SOES_HOOK_H__


// esc_config hook
void pre_state_change_hook (uint8_t * as, uint8_t * an);
void post_state_change_hook (uint8_t * as, uint8_t * an);
int ESC_pre_objecthandler (uint16_t index, uint8_t subindex);
void ESC_objecthandler (uint16_t index, uint8_t subindex);

void ecat_process_pdo(void);
void bootstrap_foe_init(void);

#endif 
