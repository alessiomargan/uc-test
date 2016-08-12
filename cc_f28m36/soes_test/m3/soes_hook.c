#include <string.h>

#include <soes/utypes.h>
#include <soes/esc.h>
#include <soes/esc_coe.h>
#include <soes/esc_foe.h>
#include <soes/soes.h>

#include "peripherals.h"
#include "soes_hook.h"
#include "osal.h"

#define DPRINT(...)


rx_pdo_t	rx_pdo;
tx_pdo_t    tx_pdo;
aux_pdo_t   aux_pdo;
sdo_t		sdo;

esc_cfg_t 	esc_config = { 0, 0 };


/**
 */
static void jump_to_bootloader(void) {

    DPRINT ("Jump to bootloader\n");

    // Return control to the boot loader.
    Watchdog0Reset();
}

static void on_PREOP(void) {

	memcpy(sdo.fw_ver, fw_ver, sizeof(sdo.fw_ver));
	sdo.PosGainP = 123.456;
	sdo.PosGainI = 456.678;
	sdo.PosGainD = 678.910;

	memset((void*)&tx_pdo,0,sizeof(tx_pdo));
}

/** Mandatory: Hook called from the slave stack SDO Download handler to act on
 * user specified Index and Sub-index.
 *
 * @param[in] index      = index of SDO download request to handle
 * @param[in] sub-index  = sub-index of SDO download request to handle
 */
void ESC_objecthandler (uint16 index, uint8 subindex)
{
    switch ( index ) {
        case 0x1c12:
            {   
                if ( rxpdoitems > 1 ) {
                    rxpdoitems = 1;
                }
                if ( (rxpdomap != 0x1600) && (rxpdomap != 0x1601)
                     && (rxpdomap != 0x0000) ) {
                    rxpdomap = 0x1600;
                }
                RXPDOsize = SM2_sml = sizeRXPDO();
                break;
            }
        case 0x1c13:
            {
                if ( txpdoitems > 1 ) {
                    txpdoitems = 1;
                }
                if ( (txpdomap != 0x1A00) && (txpdomap != 0x1A01)
                     && (rxpdomap != 0x0000) ) {
                    txpdomap = 0x1A00;
                }
                TXPDOsize = SM3_sml = sizeTXPDO();
                break;
            }
        case 0x8001:
            {
                /* Handle post-write of parameter values */
                switch ( subindex ) {
                    default:
                        DPRINT("post-write param %d %d\n", par_1, par_2);
                        break;
                }
                break;
            }
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

/** Mandatory: Write local process data to Sync Manager 3, Master Inputs.
 */
static void TXPDO_update (void)
{
    ESC_write (SM3_sma, &tx_pdo, TXPDOsize, (void *) &ESCvar.ALevent);
}

/** Mandatory: Read Sync Manager 2 to local process data, Master Outputs.
 */
static void RXPDO_update (void)
{
    ESC_read (SM2_sma, &rx_pdo, RXPDOsize, (void *) &ESCvar.ALevent);
}


/** Optional: Hook called BEFORE state change for application
 * specific actions for specific state changes. 
 */
void pre_state_change_hook (uint8 * as, uint8 * an)
{
    DPRINT ("pre_state_change_hook 0x%02X %d\n", *as, *an);
    if ( (*as == INIT_TO_PREOP) && (*an & ESCerror ) == 0 ) {
    	on_PREOP();
    }
}

/** Optional: Hook called AFTER state change for application
 * specific actions for specific state changes. 
 */
void post_state_change_hook (uint8 * as, uint8 * an)
{
    DPRINT ("post_state_change_hook 0x%02X %d\n", *as, *an);
    /* Add specific step change hooks here */
    if ( (*as == INIT_TO_BOOT) && (*an & ESCerror ) == 0 ) {
    	jump_to_bootloader();
    }

}

void setup_esc_configs(void) {

	/* setup pre-post state change hooks */
	esc_config.pre_state_change_hook  = pre_state_change_hook;
	esc_config.post_state_change_hook = post_state_change_hook;
	ESC_config ((esc_cfg_t *)&esc_config);

}


#define AUX_PDO_OP_SET 0xFB
#define AUX_PDO_OP_GET 0xBF
#define AUX_PDO_OP_ACK 0x00
#define AUX_PDO_OP_NAC 0xEE

#define AUX_PDO_EE_INVALID_OP	0xE1
#define AUX_PDO_EE_INVALID_IDX	0xE2
#define AUX_PDO_EE_READONLY 	0xE3
extern const _objd SDO8002[];

void handle_aux_pdo(void) {

	const _objd * pobjd;
	uint8 op  = rx_pdo.op_idx_aux >> 8;
	uint8 idx = rx_pdo.op_idx_aux & 0xFF;

	//UARTprintf("%s %d %d\n", __FUNCTION__, op, idx );

	if ( ! (op == AUX_PDO_OP_SET || op == AUX_PDO_OP_GET) ) {
		tx_pdo.op_idx_ack = (AUX_PDO_OP_NAC << 8) | idx;
		tx_pdo.aux = (float)AUX_PDO_EE_INVALID_OP;
		return;
	}
	pobjd = SDO8002;
	if ( idx < 1 || idx > pobjd->value ) {
		tx_pdo.op_idx_ack = (AUX_PDO_OP_NAC << 8) | idx;
		tx_pdo.aux = (float)AUX_PDO_EE_INVALID_IDX;
		return;
	}
	pobjd = SDO8002+idx;
	if ( op == AUX_PDO_OP_SET && pobjd->access == ATYPE_RO ) {
		tx_pdo.op_idx_ack = (AUX_PDO_OP_NAC << 8) | idx;
		tx_pdo.aux = (float)AUX_PDO_EE_READONLY;
		return;
	}
	// op & idx are valid
	tx_pdo.op_idx_ack = (AUX_PDO_OP_ACK << 8) | idx;
	if ( op == AUX_PDO_OP_SET ) {
		*(float*)(pobjd->data) = rx_pdo.aux;
		tx_pdo.aux = rx_pdo.aux;
	}
	if ( op == AUX_PDO_OP_GET ) {
		tx_pdo.aux = *(float*)(pobjd->data);
	}

}

/**
 *
 */
void ecat_process_pdo(void) {

	if ( (ESCvar.ALstatus & 0x0f) == ESCop ) {
	      RXPDO_update();
	}

	if ( rx_pdo.op_idx_aux != 0 ) {
		handle_aux_pdo();
	}

	aux_pdo.pos_ref_fb = rx_pdo.pos_ref;
	aux_pdo.volt_ref = 0;
	aux_pdo.vout = 0;
	aux_pdo.current = 0;

	tx_pdo.link_pos = rx_pdo.pos_ref;
	tx_pdo.motor_pos = rx_pdo.pos_ref;
	tx_pdo.torque = 0;
	tx_pdo.max_temperature = 21;
	tx_pdo.rtt = rx_pdo.ts;

	TXPDO_update();
}

