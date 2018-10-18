#include <string.h>

#include <soes/esc.h>
#include <soes/esc_coe.h>
#include <soes/esc_foe.h>
#include <soes/hal/advr_esc/soes.h>
#include <config.h>
#include <soes_hook.h>
#include "math.h"

//#define ESC_DEBUG
#ifdef ESC_DEBUG
	#undef DPRINT
    #define DPRINT(...) printf ("hook: "__VA_ARGS__)
    #define DEBUG_ASSERT(expression)    ASSERT(expression)
#else
    #define DPRINT(...)
    #define DEBUG_ASSERT(expression)
#endif  /* ESC_DEBUG */

extern const 	_objd SDO8002[];
extern const 	_objd SDO8003[];
extern float	tempC;

rx_pdo_t    	rx_pdo;
tx_pdo_t    	tx_pdo;
aux_pdo_rx_t	aux_pdo_rx;
aux_pdo_tx_t	aux_pdo_tx;

sdo_t	sdo = {
	.board_id = 696,
	.fw_ver = "_(@)(@)_",
};

extern void jump_to_bootloader(void);


/**
 */
static void on_PREOP(void) {

	memset((void*)&tx_pdo,0,sizeof(tx_pdo));
}

/** Mandatory: Write local process data to Sync Manager 3, Master Inputs.
 */
static void TXPDO_update (void)
{
    ESC_write (SM3_sma, &tx_pdo, ESCvar.TXPDOsize);
}

/** Mandatory: Read Sync Manager 2 to local process data, Master Outputs.
 */
static void RXPDO_update (void)
{
    ESC_read (SM2_sma, &rx_pdo, ESCvar.RXPDOsize);
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


/** Function to pre-qualify the incoming SDO download.
 *
 * @param[in] index      = index of SDO download request to check
 * @param[in] sub-index  = sub-index of SDO download request to check
 * @return 1 if the SDO Download is correct. 0 If not correct.
 */
int ESC_pre_objecthandler (uint16_t index, uint8_t subindex)
{
	if ((index == 0x1c12) && (subindex > 0) && (rxpdoitems != 0))
	{
		SDO_abort (index, subindex, ABORT_READONLY);
		return 0;
	}
	if ((index == 0x1c13) && (subindex > 0) && (txpdoitems != 0))
	{
		SDO_abort (index, subindex, ABORT_READONLY);
		return 0;
	}
	return 1;
}


/** Mandatory: Hook called from the slave stack SDO Download handler to act on
 * user specified Index and Sub-index.
 *
 * @param[in] index      = index of SDO download request to handle
 * @param[in] sub-index  = sub-index of SDO download request to handle
 */
void ESC_objecthandler (uint16_t index, uint8_t subindex)
{
	switch ( index ) {
    	case 0x1c12:
			if ( rxpdoitems > 1 ) {
				rxpdoitems = 1;
			}
			if ( (rxpdomap != 0x1600) && (rxpdomap != 0x1601)
				 && (rxpdomap != 0x0000) ) {
				rxpdomap = 0x1600;
			}
			ESCvar.RXPDOsize = ESCvar.ESC_SM2_sml = sizeOfPDO(RX_PDO_OBJIDX);
			break;
        case 0x1c13:
			if ( txpdoitems > 1 ) {
				txpdoitems = 1;
			}
			if ( (txpdomap != 0x1A00) && (txpdomap != 0x1A01)
				 && (rxpdomap != 0x0000) ) {
				txpdomap = 0x1A00;
			}
			ESCvar.TXPDOsize = ESCvar.ESC_SM3_sml = sizeOfPDO(TX_PDO_OBJIDX);
			break;
        case 0x8001:
			/* Handle post-write of parameter values */
			switch ( subindex ) {
				default:
					DPRINT("post-write param %d %d\n", index, subindex);
					break;
			}
			break;
        default:
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
    tx_pdo.temperature = (int16_t)(tempC*10);
    tx_pdo.rtt = rx_pdo.ts;

    TXPDO_update();
}


void bootstrap_foe_init(void) {


}

