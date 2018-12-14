#ifndef __PARAMS_H__
#define __PARAMS_H__

/*********************************************************************************************/
// CONTROL COMMAND CODES
/*********************************************************************************************/

#define CTRL_CMD_TEST_DONE		0x0012
#define CTRL_CMD_TEST_ERROR		0x0021

#define CTRL_CMD_RM_OFFSET		0x00CD
#define CTRL_DEBUG_MODE_ON		0x00DB
#define CTRL_DEBUG_MODE_OFF		0x00BD

#define CTRL_CMD_DONE			0x7800
#define CTRL_CMD_WORKING		0xD000
#define CTRL_CMD_ERROR			0xAA00

/*********************************************************************************************/
// FLASH PARAMS COMMAND CODES
/*********************************************************************************************/

#define SAVE_PARAMS_TO_FLASH		0x0012
#define LOAD_PARAMS_FROM_FLASH		0x0034
#define LOAD_DEFAULT_PARAMS			0x0056
#define ERASE_FLASH_PARAMS			0x00F1
#define LOAD_DEFAULT_CAL_MAT		0x00C1

#define PARAMS_CMD_DONE				0x7800
#define PARAMS_CMD_ERROR			0xAA00
#define PARAMS_CMD_FLASH_BLANK		0x8500


uint16_t Load_Default_Params(void);
uint16_t Read_Flash_Params(void);
uint16_t Write_Flash_Params(void);

void Handle_PDO_mailbox(uint8_t);
void Check_updated_parameter(uint8_t);
void Handle_system_commands(void);
void Handle_flash_command(void);
#endif
