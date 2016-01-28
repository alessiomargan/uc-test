#ifndef __BL_CONFIG_H__
#define __BL_CONFIG_H__

#define CTRL_CMD_ERASE			0x00EE
#define CTRL_CMD_DONE			0x7800
#define CTRL_CMD_ERROR			0xAA00

#define SectorE_start        0x002C0000
#define SectorE_end          0x002E0000

#define APP_START_ADDR 	0x002DFFFC
#define APP_CRC_ADDRESS 	0x00218000

#define APP_START_ADDRESS 	SectorE_start
#define FLASH_APP_SIZE		(SectorE_end-SectorE_start)

#endif
