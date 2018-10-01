#include <soes/esc.h>
#include <soes/esc_coe.h>
#include <soes/esc_foe.h>
#include <soes/hal/advr_esc/soes.h>
#include <config.h>
#include <soes_hook.h>

//#define ESC_DEBUG
#ifdef ESC_DEBUG
	#undef DPRINT
    #define DPRINT(...) printf ("hook: "__VA_ARGS__)
    #define DEBUG_ASSERT(expression)    ASSERT(expression)
#else
    #define DPRINT(...)
    #define DEBUG_ASSERT(expression)
#endif  /* ESC_DEBUG */

esc_cfg_t 	gESC_config = { 0, 0 };
foe_cfg_t 	gFOE_config = { 0, 0, 0 ,0 };
uint8_t		foe_buffer[0x400];

extern foe_writefile_cfg_t      gFOE_firmware_files[];

uint16_t 	flash_cmd;
uint16_t 	flash_cmd_ack;
uint16_t  	crc_ok;
uint16_t  	et1100_boot_pin;

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
}

/** Optional: Hook called AFTER state change for application
 * specific actions for specific state changes. 
 */
void post_state_change_hook (uint8_t * as, uint8_t * an)
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

void bootstrap_foe_init(void) {

	/* setup application foe_file structs */
	int file_cnt = 0;
	foe_writefile_cfg_t * tmp_foe_files = gFOE_firmware_files;

	while ( tmp_foe_files->name != 0 ) {
		DPRINT ("foe_file %s addr 0x%04X\n",
				tmp_foe_files->name,
				tmp_foe_files->dest_start_address);
		tmp_foe_files ++;
		file_cnt ++;
	}

	/** Allocate static in caller func to fit buffer_size */
	gFOE_config.fbuffer = foe_buffer;
	/** Buffer size before we flush to destination */
	gFOE_config.buffer_size = sizeof(foe_buffer);
	/** Number of files used in firmware update */
	gFOE_config.n_files = file_cnt;
	/** Pointer to files configured to be used by FoE */
	gFOE_config.files = gFOE_firmware_files;

	FOE_config(&gFOE_config, gFOE_firmware_files);

	DPRINT ("config %d foe_file(s)\n", file_cnt);


}
