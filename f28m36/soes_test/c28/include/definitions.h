#ifndef __F28M36x_C28_DEF_H__
#define __F28M36x_C28_DEF_H__

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

#ifdef _CONTROL_CARD

#define DEBUG_PIN_ON		GpioDataRegs.GPCSET.bit.GPIO70 = 1;
#define DEBUG_PIN_OFF		GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1;
#define DEBUG_PIN_TOGGLE	GpioDataRegs.GPCTOGGLE.bit.GPIO70 = 1;

#define LED_0_ON 			GpioDataRegs.GPASET.bit.GPIO31 = 1;
#define LED_0_OFF 			GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
#define LED_0_TOGGLE		GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;

#else

#define DEBUG_ORG_LED_ON	GpioG1DataRegs.GPESET.bit.GPIO128 = 1;
#define DEBUG_ORG_LED_OFF	GpioG1DataRegs.GPECLEAR.bit.GPIO128 = 1;
#define DEBUG_ORG_LED_TGL	GpioG1DataRegs.GPETOGGLE.bit.GPIO128 = 1;

#endif

extern void cpu_timer0_isr(void);
extern void cpu_timer1_isr(void);
extern void cpu_timer2_isr(void);
extern void epwm1_isr(void);
extern void xint1_isr(void);
extern void MtoC_ipc1_isr(void);
extern void MtoC_ipc2_isr(void);

extern void Configure_Pie_Vector(void);
extern void Configure_C28_Timer(void);
extern void Configure_C28_Gpio(void);
extern void Configure_C28_ePWM(void);
extern void Configure_C28_Ipc(void);
extern void Configure_flashAPI(void);

extern void bootloaderService(Uint32 param);
extern Uint32 bootloaderServiceResult;

extern volatile m3_to_c28_data_t	m3_ro_data;
extern volatile c28_to_m3_data_t	c28_rw_data;

extern volatile tIpcController  	g_sIpcController1;
extern volatile tIpcController  	g_sIpcController2;

extern Uint32 *pulMsgRam;
extern volatile Uint16 foe_buffer[128];


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
