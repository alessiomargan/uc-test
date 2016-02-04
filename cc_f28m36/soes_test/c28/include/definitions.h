#ifndef F28M36x_C28_DEF_H
#define F28M36x_C28_DEF_H

#include "DSP28x_Project.h"
#include "F28M36x_Ipc_drivers.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_PIN_ON		GpioDataRegs.GPCSET.bit.GPIO70 = 1;
#define DEBUG_PIN_OFF		GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1;
#define DEBUG_PIN_TOGGLE	GpioDataRegs.GPCTOGGLE.bit.GPIO70 = 1;

#define LED_0_ON 		GpioDataRegs.GPASET.bit.GPIO31 = 1;
#define LED_0_OFF 		GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
#define LED_0_TOGGLE	GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;

#include "shared_ram.h"

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

extern volatile m3_rw_data_t	m3_ro_data;
extern volatile c28_rw_data_t	c28_rw_data;

extern volatile tIpcController  	g_sIpcController1;
extern volatile tIpcController  	g_sIpcController2;

extern Uint32 *pulMsgRam;
extern volatile Uint16 foe_buffer[128];


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
