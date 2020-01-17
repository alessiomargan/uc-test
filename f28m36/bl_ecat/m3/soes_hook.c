#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


#include <soes/utypes.h>
#include <soes/esc.h>
#include <soes/esc_coe.h>
#include <soes/esc_foe.h>
#include <soes/soes.h>

#include "m3/include/definitions.h"

#include "peripherals.h"
#include "user_flash.h"
#include "soes_hook.h"
#include "osal.h"
#include "shared_ram.h"

//#define ESC_DEBUG
#ifdef ESC_DEBUG
    #define DPRINT(...) OSAL_PRINT ("soes: "__VA_ARGS__)
    #define DEBUG_ASSERT(expression)    ASSERT(expression)
#else
    #define DPRINT(...)
    #define DEBUG_ASSERT(expression)
#endif  /* ESC_DEBUG */


esc_cfg_t 	gESC_config = { 0, 0 };
foe_cfg_t 	gFOE_config = { 0, 0, 0, 0, 0 };

extern m3_bl_rw_data_t	m3_bl_rw_data;
extern c28_bl_rw_data_t	c28_bl_ro_data;

extern foe_writefile_cfg_t      gFOE_firmware_files[];

uint16 	gFlash_cmd;
uint16 	gFlash_cmd_ack;
uint32 	gFlash_crc;
uint16  m3_crc_ok;
uint16  c28_crc_ok;
uint16  gET1100_boot_pin;
char 	gBL_ver[16] = "m3_F28M3x_1.0";

#pragma DATA_SECTION(gFlash_crc,"FLS_APP_CRC");


void handle_sdo_0x8000_01(void)
{
	bool	success = false;

	switch (gFlash_cmd) {
		case FLASH_CMD_ERASE_M3 :
			success = erase_M3_app_flash();
			break;
		case FLASH_CMD_ERASE_C28 :
			success = erase_C28_app_flash();
			break;
		case FLASH_TEST_ERASE_C28 :
			success = test_C28_erase_write_flash();
		default :
			break;
	}

	if ( ! success ) {
		gFlash_cmd_ack = (CTRL_CMD_ERROR | (gFlash_cmd & 0x00FF));
	} else {
		gFlash_cmd_ack = (CTRL_CMD_DONE  | (gFlash_cmd & 0x00FF));
	}

}

void handle_sdo_0x8000(uint8 subindex)
{
	switch ( subindex ) {
		// 0x8001:01 flash_cmd
		case 1:
			handle_sdo_0x8000_01();
			break;
		default:
			break;
	}

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
    	case 0x8000:
		{
			handle_sdo_0x8000(subindex);
			break;
		}
        default:
        	DPRINT("post-write param 0x%04X %d\n", index, subindex);
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

    if ( (*as == BOOT_TO_INIT) && (*an & ESCerror ) == 0 ) {

    	//ESC_ALstatus (ESCinit);
    	ESC_ALerror (ALERR_NONE);

    	Watchdog0Reset();

    } else {
    //	*an |= ESCerror;
	//	ESC_ALerror (ALERR_INVALIDSTATECHANGE);
    }

}

void setup_esc_configs(void)
{
	/* setup pre-post state change hooks */
	gESC_config.pre_state_change_hook  = pre_state_change_hook;
	gESC_config.post_state_change_hook = post_state_change_hook;
	ESC_config ((esc_cfg_t *)&gESC_config);

	/* setup application foe_file structs */
	int file_cnt = 0;
	foe_writefile_cfg_t * tmp_foe_files = gFOE_firmware_files;

	while ( tmp_foe_files->name != 0 ) {
		OSAL_PRINT ("foe_file %s\n", tmp_foe_files->name);
		tmp_foe_files ++;
		file_cnt ++;
	}

	/** Allocate static in caller func to fit buffer_size */
	gFOE_config.fbuffer = m3_bl_rw_data.foe_buffer;
	/** Write this to fill-up, ex. 0xFF for "non write" */
	gFOE_config.empty_write = 0xFF;
	/** Buffer size before we flush to destination */
	gFOE_config.buffer_size = sizeof(m3_bl_rw_data.foe_buffer);
	/** Number of files used in firmware update */
	gFOE_config.n_files = file_cnt;
	/** Pointer to files configured to be used by FoE */
	gFOE_config.files = gFOE_firmware_files;

	FOE_config(&gFOE_config, gFOE_firmware_files);

	OSAL_PRINT ("config %d foe_file(s)\n", file_cnt);

}




