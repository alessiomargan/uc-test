#ifndef F28M36x_C28_DEF_H
#define F28M36x_C28_DEF_H

#include "DSP28x_Project.h"
#include "F28M36x_Ipc_drivers.h"

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

#define DEBUG_PIN_ON		GpioDataRegs.GPCSET.bit.GPIO70 = 1;
#define DEBUG_PIN_OFF		GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1;
#define DEBUG_PIN_TOGGLE	GpioDataRegs.GPCTOGGLE.bit.GPIO70 = 1;

#define LED_0_ON 		GpioDataRegs.GPASET.bit.GPIO31 = 1;
#define LED_0_OFF 		GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
#define LED_0_TOGGLE	GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;

#define FLASHA_START		0x13E000
#define FLASHE_START 		0x130000
#define FLASHE_END 			0x138000
#define C28_FLASH_APP_SIZE  0x008000
#define C28_APP_CRC_ADDR	0x106000


extern void MtoC_ipc1_isr(void);
extern void MtoC_ipc2_isr(void);

extern void Configure_Pie_Vector(void);
extern void Configure_C28_Gpio(void);
extern void Configure_C28_Ipc(void);
extern void Configure_flashAPI(void);

extern uint32_t calc_app_crc(void);

extern void bootloaderService(Uint32 param);
extern Uint32 bootloaderServiceResult;
extern Uint32 bootM3bits;

extern volatile m3_bl_rw_data_t		m3_ro_data;
extern volatile c28_bl_rw_data_t	c28_rw_data;

extern volatile tIpcController  	g_sIpcController1;
extern volatile tIpcController  	g_sIpcController2;

extern Uint32 *pulMsgRam;
extern volatile Uint16 foe_buffer[128];


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
