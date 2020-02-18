#ifndef __PINS_H__
#define __PINS_H__

//*****************************************************************************
// GPIO for the user LEDs
//*****************************************************************************
#define LED_PERIPH			SYSCTL_PERIPH_GPIOL
#define LED_BASE        	GPIO_PORTL_BASE
#define LED_RED_PIN         GPIO_PIN_4
#define LED_GRN_PIN         GPIO_PIN_5
#define LED_YLW_PIN         GPIO_PIN_6
#define LED_ORG_PIN       	GPIO_PIN_7
#define LED_PINS_M3			(LED_RED_PIN|LED_GRN_PIN)
#define LED_PINS_C28		(LED_YLW_PIN|LED_ORG_PIN)

//*****************************************************************************
//
//  M3 PDI EtherCAT interface
//
//! -   EPI0S0(D0)    -    PH3        EPI0S1(D1)    -    PH2
//! - 	EPI0S2(D2)    -    PC4        EPI0S3(D3)    -    PC5
//! - 	EPI0S4(D4)    -    PC6        EPI0S5(D5)    -    PC7
//! - 	EPI0S6(D6)    -    PH0        EPI0S7(D7)    -    PH1
//! - 	EPI0S8(A0)    -    PE0        EPI0S9(A1)    -    PE1
//! - 	EPI0S10(A2)   -    PH4        EPI0S11(A3)   -    PH5
//! - 	EPI0S12(A4)   -    PF4
//! - 	EPI0S28(OEn)  -    PJ4        EPI0S29(WEn)  -    PJ5
//! - 	EPI0S30(CEn)  -    PJ6
//*****************************************************************************

#define ECAT_GPIO_PORTBASE		GPIO_PORTK_BASE
#define ECAT_GPIO_SYSCTL_PERIPH SYSCTL_PERIPH_GPIOK

#define ECAT_IRQ                GPIO_PIN_3
#define ECAT_BOOT				GPIO_PIN_2

//*****************************************************************************
//
//  DEBUG PIN & DAC
//
//*****************************************************************************
#define GPIO_DBG_PERIPH		SYSCTL_PERIPH_GPIOP	// DBG_PIN and DAC_08
#define GPIO_DBG_BASE		GPIO_PORTP_BASE
#define GPIO_DBG_PIN		GPIO_PIN_0
#define GPIO_DBG_DAC_CS		GPIO_PIN_4
#define GPIO_DBG_DAC_CLK	GPIO_PIN_5
#define GPIO_DBG_DAC_MOSI	GPIO_PIN_6
#define GPIO_DBG_DAC_LDAC	GPIO_PIN_7


#endif
