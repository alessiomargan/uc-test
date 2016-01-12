#ifndef F28M36x_C28_DEF_H
#define F28M36x_C28_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_PIN_ON		GpioDataRegs.GPCSET.bit.GPIO70 = 1;
#define DEBUG_PIN_OFF		GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1;
#define DEBUG_PIN_TOGGLE	GpioDataRegs.GPCTOGGLE.bit.GPIO70 = 1;

#define LED_0_ON 		GpioDataRegs.GPASET.bit.GPIO31 = 1;
#define LED_0_OFF 		GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
#define LED_0_TOGGLE	GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
