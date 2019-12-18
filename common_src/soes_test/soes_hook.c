#include <string.h>

#include <soes/esc.h>
#include <soes/esc_coe.h>
#include <soes/esc_foe.h>
#include <soes/hal/advr_esc/soes.h>
#include <ecat_options.h>
#include <soes_hook.h>
#include <globals.h>
#include "math.h"

extern const	_objd SDO8002[];
extern const	_objd SDO8003[];

extern float	tempC;

extern void jump_to_bootloader(void);

/* Forward declaration of the default fault handlers. */
void Default_Object_Handler(uint8_t) __attribute__((weak));

extern void Handle_0x7000(uint8_t) __attribute__((weak, alias("Default_Object_Handler")));
extern void Handle_0x8000(uint8_t) __attribute__((weak, alias("Default_Object_Handler")));
extern void Handle_0x8001(uint8_t) __attribute__((weak, alias("Default_Object_Handler")));

void pre_state_change_hook (uint8_t * as, uint8_t * an);
void post_state_change_hook (uint8_t * as, uint8_t * an);
void ESC_App_objecthandler (uint16_t index, uint8_t subindex, uint16_t flags);

/* Setup config hooks */
const esc_cfg_t config =
{
    .user_arg					= "hello_world",
    .use_interrupt				= 1,
    .watchdog_cnt				= 0,
	.set_defaults_hook 			= NULL,
    .pre_state_change_hook 		= pre_state_change_hook,
    .post_state_change_hook 	= post_state_change_hook,
    .application_hook 			= NULL,
    .safeoutput_override 		= NULL,
    .pre_object_download_hook 	= NULL,
    .post_object_download_hook 	= ESC_App_objecthandler,
    .rxpdo_override				= NULL,
    .txpdo_override				= NULL,
    .esc_hw_interrupt_enable	= NULL,
    .esc_hw_interrupt_disable	= NULL,
    .esc_hw_eep_handler			= NULL,
	.esc_check_dc_handler		= NULL,
};


void Default_Object_Handler(uint8_t subindex) {

	DPRINT("%s %d ... not handled\n", __FUNCTION__, subindex);
}

/**
 */
static void on_PREOP(void) {

	memset((void*)&tx_pdo,0,sizeof(tx_pdo));
}

//#pragma CODE_SECTION(handle_aux_pdo_rx,ramFuncSection);
static void handle_aux_pdo_rx(void) {

    const 		_objd * pobjrx = SDO8003;
    uint16_t 	idx_rx = ((rx_pdo.op_idx_aux & 0x7F00)>> 8); 	// 7 bits of write index - XWWWWWW 00000000 --> III = index write
    //UARTprintf("%s %d %d\n", __FUNCTION__, op, idx );
    // check valid idx_rx
    if((idx_rx > pobjrx->value)||(idx_rx == 0)) {
    	// invalid idx_rx
        tx_pdo.op_idx_ack = ((idx_rx | 0x80) << 8);
    } else {
    	// valid idx_rx
		tx_pdo.op_idx_ack = (idx_rx << 8);
    	pobjrx += idx_rx;
    	*(float*)(pobjrx->data) = rx_pdo.aux;
    }
}

//#pragma CODE_SECTION(handle_aux_pdo_tx,ramFuncSection);
static void handle_aux_pdo_tx(void) {

	const 		_objd * pobjtx = SDO8002;
	uint16_t 	idx_tx = (rx_pdo.op_idx_aux & 0x007F);			// 7 bits of read index  - 0000000 XRRRRRRR --> III = index read
	//UARTprintf("%s %d %d\n", __FUNCTION__, op, idx );
	// check valid idx_rd
	if((idx_tx > pobjtx->value)||(idx_tx == 0)) {
		// invalid idx_rd
		tx_pdo.op_idx_ack |= (idx_tx | 0x80);
		tx_pdo.aux = NAN;
	} else {
		// valid idx_rd
		tx_pdo.op_idx_ack |= idx_tx;
		pobjtx += idx_tx;
		tx_pdo.aux = *(float*)(pobjtx->data);
	}
}

/** Mandatory: Hook called from the slave stack SDO Download handler to act on
 * user specified Index and Sub-index.
 *
 * @param[in] index      = index of SDO download request to handle
 * @param[in] sub-index  = sub-index of SDO download request to handle
 */
void ESC_App_objecthandler (uint16_t index, uint8_t subindex, uint16_t flags)
{
	switch ( index ) {
        case 0x7000:
        	Handle_0x7000(subindex);
			break;
         case 0x8000:
 			Handle_0x8000(subindex);
			break;
 		case 0x8001:
 			Handle_0x8001(subindex);
 			break;
         default:
             DPRINT("SDO 0x%04X %d NOT Handled\n", index, subindex);
             break;
    }
}

/** Mandatory: Hook called from the slave stack ESC_stopoutputs to act on state changes
 * forcing us to stop outputs. Here we can set them to a safe state.
 */
void APP_safeoutput (void)
{
    DPRINT ("APP_safeoutput\n");
}

/** Optional: Hook called BEFORE state change for application
 * specific actions for specific state changes. 
 */
void pre_state_change_hook (uint8_t * as, uint8_t * an)
{
    DPRINT ("pre_state_change_hook 0x%02X %d\n", *as, *an);
    if ( (*as == INIT_TO_PREOP) && (*an & ESCerror ) == 0 ) {
    	on_PREOP();
    }
}

/** Optional: Hook called AFTER state change for application
 * specific actions for specific state changes. 
 */
void post_state_change_hook (uint8_t * as, uint8_t * an)
{
    DPRINT ("post_state_change_hook 0x%02X %d\n", *as, *an);
    /* Add specific step change hooks here */
    if ( (*as == INIT_TO_BOOT) && (*an & ESCerror ) == 0 ) {

    	jump_to_bootloader();
    }

}

/**
 * @author amargan (7/4/2014)
 */
void ecat_process_pdo(void) {

	if ( (ESCvar.ALstatus & 0x0f) == ESCop ) {
		RXPDO_update();
		if (rx_pdo.op_idx_aux != 0 ) {
			handle_aux_pdo_rx();
		}
	}

	// set RO aux
	aux_pdo_tx.pos_ref_fb =  0;
	aux_pdo_tx.volt_ref = 0;
	aux_pdo_tx.vout = 0;
	aux_pdo_tx.current = 0;
	aux_pdo_tx.rtt = aux_pdo_rx.ts;

	if (rx_pdo.op_idx_aux != 0 ) {
		handle_aux_pdo_tx();
	}

    tx_pdo.link_pos = rx_pdo.pos_ref;
    tx_pdo.motor_pos = rx_pdo.pos_ref;
    tx_pdo.link_vel = 0;
    tx_pdo.motor_vel = 0;
    tx_pdo.torque = 0;
    //tx_pdo.temperature = (int16_t)(tempC*10);
    tx_pdo.rtt = rx_pdo.ts;

    TXPDO_update();
}


void bootstrap_foe_init(void) {


}

