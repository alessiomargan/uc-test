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

#include "bl_config.h"
#include "peripherals.h"
#include "soes_hook.h"
#include "osal.h"

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

#pragma DATA_SECTION(foe_buffer,"RAM_S1");
uint8		foe_buffer[256];

extern foe_writefile_cfg_t      gFOE_firmware_files[];

uint16 	gFlash_cmd;
uint16 	gFlash_cmd_ack;
uint32 	gFlash_crc;
uint16  gCrc_ok;
uint16  gET1100_boot_pin;
char 	gBL_ver[8] = "cert_1.0";

#pragma DATA_SECTION(gFlash_crc,"FLS_APP_CRC");

void (*jumper)(void);

void jump_to_app(void) {
	//__asm("    .global _c_int00\n"
	//     "    b.w     _c_int00");

	// APPLICATION ENTRY POINT SYMBOL: "_c_int00"  address: 002c5375
	//jumper = (void (*)())0x002c5375;
	jumper = (void (*)())(*(uint32*)APP_START_ADDR);
	jumper();
}

/**
 * return true if :
 * - crc is valid
 * 	 AND
 * - et1000 boot pin is LOW
 */
bool test_jump_to_app(void)
{
	uint32_t app_crc = calc_app_crc();
	//et1100_boot_pin = ESC_GPO_LL() & 0x0004;
	gET1100_boot_pin = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
	gCrc_ok = ( (app_crc == gFlash_crc) && (gFlash_crc != 0xFFFFFFFF) );
	OSAL_PRINT ("gCrc_ok %d : app 0x%X  flash 0x%X\n", gCrc_ok, app_crc, gFlash_crc);
	return ( ! gET1100_boot_pin && gCrc_ok);
}

void handle_sdo_0x8001_01(void)
{
	bool	success = false;

	switch (gFlash_cmd) {
		case FLASH_CMD_ERASE_M3 :
			success = erase_m3_app_flash();
			break;
		case FLASH_CMD_ERASE_C28 :
			success = erase_c28_app_flash();
			break;
		default :
			break;
	}

	if ( ! success ) {
		gFlash_cmd_ack = (CTRL_CMD_ERROR | (gFlash_cmd & 0x00FF));
	} else {
		gFlash_cmd_ack = (CTRL_CMD_DONE  | (gFlash_cmd & 0x00FF));
	}

}

void handle_sdo_0x8001(uint8 subindex)
{
	switch ( subindex ) {
		// 0x8001:01 flash_cmd
		case 1:
			handle_sdo_0x8001_01();
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
    	case 0x8001:
		{
			handle_sdo_0x8001(subindex);
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

	OSAL_PRINT ("config %d foe_file(s)\n", file_cnt);

}




