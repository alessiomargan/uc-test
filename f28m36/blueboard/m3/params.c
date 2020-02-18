#include "globals.h"
#include "flash_utils.h"
#include "params.h"

#include <soes_hook.h>

extern void Disable_interrupt();
extern void Enable_interrupt();


uint16_t Load_Default_Params(void) {

	memcpy((void*)&sdo.flash, (void*)&dflt_flash_sdo, sizeof(dflt_flash_sdo));
	if ( sdo.flash._signature_ != FLASH_SIGN_DEFAULT ) {
		return PARAMS_CMD_ERROR;
	}
	return PARAMS_CMD_DONE;

}

uint16_t Read_Flash_Params(void) {

	memcpy((void*)&sdo.flash, (void*)FLASH_PARAM_SECTOR, sizeof(sdo.flash));
	if ( sdo.flash._signature_ != FLASH_SIGN_VALID ) {
		return PARAMS_CMD_ERROR;
	}
	return PARAMS_CMD_DONE;

}

uint16_t Write_Flash_Params(void) {

	bool ret;
	sdo.flash._signature_ = FLASH_SIGN_VALID;
	ret = Write_Flash((uint32_t)&flash_sdo, (void*)&sdo.flash, sizeof(flash_sdo_t));
	return (ret ? PARAMS_CMD_DONE : PARAMS_CMD_ERROR);
}

/*
 * Handle_PDO_mailbox
 */
void Handle_0x7000(uint8_t subidx) {

	DPRINT("%s %d\n", __FUNCTION__, subidx);

//	if(ESCvar.ALstatus == ESCop) {
//		return;
//	}

	switch(subidx) {

	default:
		break;
	}
}

/*
 * Check_updated_parameter
 */
void Handle_0x8000(uint8_t subidx) {

	DPRINT("%s %d\n", __FUNCTION__, subidx);

	switch(subidx) {

	default:
		break;
	}

}


static uint16_t Test_done(void) { return CTRL_CMD_DONE; }
static uint16_t Test_nedo(void) { return CTRL_CMD_ERROR; }
static uint16_t Remove_offset(void) { return CTRL_CMD_DONE; }

static void Handle_system_commands(void) {

	DPRINT("%s 0x%04X\n", __FUNCTION__, sdo.ram.ctrl_status_cmd);

	switch(sdo.ram.ctrl_status_cmd) {

	case CTRL_CMD_RM_OFFSET :
		sdo.ram.ctrl_status_cmd_ack = (Remove_offset() | CTRL_CMD_RM_OFFSET);
		break;

	case CTRL_CMD_TEST_DONE :
		sdo.ram.ctrl_status_cmd_ack = (Test_done() | CTRL_CMD_TEST_DONE);
		break;
	case CTRL_CMD_TEST_ERROR :
		sdo.ram.ctrl_status_cmd_ack = (Test_nedo() | CTRL_CMD_TEST_ERROR);
		break;
	default:
		sdo.ram.ctrl_status_cmd_ack = (CTRL_CMD_ERROR | sdo.ram.ctrl_status_cmd);
		break;
	}
}

static void Handle_flash_command(void)	{

	DPRINT("%s 0x%04X\n", __FUNCTION__, sdo.ram.flash_params_cmd);

	switch(sdo.ram.flash_params_cmd){

		case SAVE_PARAMS_TO_FLASH :
			Disable_interrupt();
			sdo.ram.flash_params_cmd_ack = (Write_Flash_Params() | SAVE_PARAMS_TO_FLASH);
			Enable_interrupt();
			break;

		case LOAD_PARAMS_FROM_FLASH:
			sdo.ram.flash_params_cmd_ack = (Read_Flash_Params() | LOAD_PARAMS_FROM_FLASH);
			break;

		case LOAD_DEFAULT_PARAMS :
			sdo.ram.flash_params_cmd_ack = (Load_Default_Params() | LOAD_DEFAULT_PARAMS);
			break;

		case ERASE_FLASH_PARAMS:
			sdo.ram.flash_params_cmd_ack = (PARAMS_CMD_ERROR | ERASE_FLASH_PARAMS);
			break;

		default:
			sdo.ram.flash_params_cmd_ack = (PARAMS_CMD_ERROR | sdo.ram.flash_params_cmd);
			break;
	}

	print_sdo(&sdo.flash);
}


void Handle_0x8001(uint8_t subidx) {

	switch(subidx) {
		// Receive system cmd : 0x8001:2 --> sdo.ram_par.ctrl_status_cmd
		case 2:
			Handle_system_commands();
			break;
		// Receive flash param cmd : 0x8001:4 --> sdo.ram_par.ctrl_param_flash_cmd
		case 4:
			Handle_flash_command();
			break;
		default:
			break;
	}

}
