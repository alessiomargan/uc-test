#include <soes/utypes.h>
#include <soes/esc.h>
#include <soes/esc_coe.h>
#include <soes/esc_foe.h>
#include <soes/soes.h>

#include "soes_hook.h"
#include "osal.h"

//#define ESC_DEBUG
#ifdef ESC_DEBUG
	#undef DPRINT
    #define DPRINT(...) OSAL_PRINT ("hook: "__VA_ARGS__)
    #define DEBUG_ASSERT(expression)    ASSERT(expression)
#else
    #define DPRINT(...)
    #define DEBUG_ASSERT(expression)
#endif  /* ESC_DEBUG */

esc_cfg_t 	gESC_config = { 0, 0 };
foe_cfg_t 	gFOE_config = { 0, 0, 0 ,0 ,0 };
uint8		foe_buffer[0x400];

extern foe_writefile_cfg_t      gFOE_firmware_files[];

uint16 	flash_cmd;
uint16 	flash_cmd_ack;
uint32 	flash_crc;
uint16  crc_ok;
uint16  et1100_boot_pin;
char 	bl_ver[8] = "msp_1.0";


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
		// !! check objectlist.h for index/subindex params
        case 0x8001:
            {
                /* Handle post-write of parameter values */
                switch ( subindex ) {
                    default:
                        DPRINT("post-write param 0x%04X %d\n", index, subindex);
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

#if 0
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
#endif

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
    DPRINT ("post_state_change_hook 0x%02X 0x%02X\n", *as, *an);

    if ( (*as == BOOT_TO_INIT) && (*an & ESCerror ) == 0 ) {

    	//ESC_ALstatus (ESCinit);
    	ESC_ALerror (ALERR_NONE);

    	DPRINT ("RESET 0x%02X %d\n\n\n\n\n", *as, *an);

    	ResetCtl_initiateHardReset();


    } else {
    //	*an |= ESCerror;
	//	ESC_ALerror (ALERR_INVALIDSTATECHANGE);
    }


}

void setup_esc_configs(void) {

	/* setup pre-post state change hooks */
	gESC_config.pre_state_change_hook  = pre_state_change_hook;
	gESC_config.post_state_change_hook = post_state_change_hook;
	ESC_config ((esc_cfg_t *)&gESC_config);

	/* setup application foe_file structs */
	int file_cnt = 0;
	foe_writefile_cfg_t * tmp_foe_files = gFOE_firmware_files;

	while ( tmp_foe_files->name != 0 ) {
		OSAL_PRINT ("foe_file %s addr 0x%04X\n",
				tmp_foe_files->name,
				tmp_foe_files->dest_start_address);
		tmp_foe_files ++;
		file_cnt ++;
	}

	/** Allocate static in caller func to fit buffer_size */
	gFOE_config.fbuffer = foe_buffer;
	/** Write this to fill-up, ex. 0xFF for "non write" */
	gFOE_config.empty_write = 0xFF;
	/** Buffer size before we flush to destination */
	gFOE_config.buffer_size = sizeof(foe_buffer);
	/** Number of files used in firmware update */
	gFOE_config.n_files = file_cnt;
	/** Pointer to files configured to be used by FoE */
	gFOE_config.files = gFOE_firmware_files;

	FOE_config(&gFOE_config, gFOE_firmware_files);

	DPRINT ("config %d foe_file(s)\n", file_cnt);


}