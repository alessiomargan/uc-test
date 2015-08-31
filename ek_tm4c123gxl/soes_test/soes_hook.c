#include <soes/utypes.h>
#include <soes/esc.h>
#include <soes/esc_coe.h>
#include <soes/esc_foe.h>
#include <soes/soes.h>

#include "soes_hook.h"
#include "osal.h"


int par_1;
int par_2;

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
    /* Add specific step change hooks here */
    if ( (*as == INIT_TO_BOOT) && (*an & ESCerror ) == 0 ) {

    }

    DPRINT ("post_state_change_hook 0x%02X %d\n", *as, *an);
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

    tx_pdo._sint ++;
    tx_pdo._usint ++;
    tx_pdo._int ++;
    tx_pdo._uint = rx_pdo._type;
    tx_pdo._dint = rx_pdo._value;
    tx_pdo._udint ++;
    tx_pdo._ulint = rx_pdo._ts;

    TXPDO_update();
}

