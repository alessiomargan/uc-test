#ifndef __BL_CONFIG_H__
#define __BL_CONFIG_H__

#define FLASH_CMD_ERASE_M3		0x00E1
#define FLASH_CMD_ERASE_C28		0x00E2
#define CTRL_CMD_DONE			0x7800
#define CTRL_CMD_ERROR			0xAA00

#define SectorE_start        	0x002C0000
#define SectorE_end          	0x002E0000

#define M3_ENTRY_POINT_ADDR 	0x002DFFFC
#define M3_APP_CRC_ADDR 		0x00218000

#define M3_FLASH_APP_START 		SectorE_start
#define M3_FLASH_APP_SIZE		(SectorE_end-SectorE_start)

#define C28_FLASH_APP_START 	0x138000 //0x13E000
#define C28_FLASH_APP_SIZE		0x002000 //0x001F80

#endif
