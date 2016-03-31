#ifndef __F28M36x_M3_DEF_H__
#define __F28M36x_M3_DEF_H__

#include "common/include/shared_ram.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (CPU_FRQ_150MHZ || DSP28_F28M36Px_150MHZ || DSP28_F28M36Hx_150MHZ)
	#define CPU_FRQ	150
#elif (CPU_FRQ_125MHZ || DSP28_F28M36Px_125MHZ)
	#define CPU_FRQ	125
#elif (CPU_FRQ_100MHZ || DSP28_F28M36Hx_100MHZ)
	#define CPU_FRQ	100
#elif CPU_FRQ_75MHZ
	#define CPU_FRQ	75
#elif CPU_FRQ_60MHZ
	#define CPU_FRQ	60
#else
	#error CPU_FREQ
#endif

#define FLASH_CMD_ERASE_M3		0x00E1
#define FLASH_CMD_ERASE_C28		0x00E2
#define FLASH_TEST_ERASE_M3		0x0071
#define FLASH_TEST_ERASE_C28	0x0072
#define CTRL_CMD_DONE			0x7800
#define CTRL_CMD_ERROR			0xAA00

#define SectorE_start        	0x002C0000
#define SectorE_end          	0x002E0000

#define M3_ENTRY_POINT_ADDR 	0x002DFFFC
#define M3_APP_CRC_ADDR 		0x00218000

#define M3_FLASH_APP_START 		SectorE_start
#define M3_FLASH_APP_SIZE		(SectorE_end-SectorE_start)

#define C28_FLASH_APP_START 	0x130000 //0x13E000
#define C28_FLASH_APP_SIZE		0x008000 //0x001F80

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
