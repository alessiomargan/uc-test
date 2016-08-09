#include <soes/utypes.h>
#include <soes/esc.h>
#include <soes/esc_coe.h>
#include <soes/esc_foe.h>
#include <soes/soes.h>

#include "soes_hook.h"
#include "osal.h"


int par_1;
int par_2;

rx_pdo_t	rx_pdo;
tx_pdo_t    tx_pdo;
sdo_t		sdo = {
		.Min_pos = -0.123,
		.Max_pos =  0.456,
		.fw_ver = "_(@)(@)_",
};

esc_cfg_t 	esc_config = { 0, 0 };


/**
 */
static void jump_to_bootloader(void) {

    // disable periphearals irq ....
	disable_peripheral_irq();

    DPRINT ("Jump to bootloader\n");
    //
    // Return control to the boot loader.  This is a call to the SVC
    // handler in the boot loader.
    //
    (*((void (*)(void))(*(uint32_t *)0x2c)))();

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


/**
 *
 *
 * @author amargan (7/4/2014)
 */
void ecat_process_pdo(void) {

	if ( (ESCvar.ALstatus & 0x0f) == ESCop ) {
	      RXPDO_update();
	}

    tx_pdo.link_pos = rx_pdo.pos_ref;
    tx_pdo.motor_pos = rx_pdo.pos_ref;
    tx_pdo.link_vel = rx_pdo.vel_ref;
    tx_pdo.motor_vel = rx_pdo.vel_ref;
    tx_pdo.torque = rx_pdo.tor_ref;
    //
    tx_pdo.rtt = rx_pdo.ts;
    // get/set:idx_var
    rx_pdo.op_idx_aux;

    TXPDO_update();
}

