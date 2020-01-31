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
// GPIO for power ON/OFF sequence
//*****************************************************************************
#define GPIO_PWR_IN_PERIPH      SYSCTL_PERIPH_GPIOQ	// POWER ON STATE INPUT
#define GPIO_PWR_IN_BASE        GPIO_PORTQ_BASE
#define GPIO_PWR_IN_PIN         GPIO_PIN_2

#define GPIO_FF_CLK_PERIPH      SYSCTL_PERIPH_GPIOQ	// FLIP FLOP CLOCK
#define GPIO_FF_CLK_BASE        GPIO_PORTQ_BASE
#define GPIO_FF_CLK_PIN         GPIO_PIN_3

#define GPIO_FF_RES_PERIPH      SYSCTL_PERIPH_GPIOQ	// FLIP FLOP RESET
#define GPIO_FF_RES_BASE        GPIO_PORTQ_BASE
#define GPIO_FF_RES_PIN         GPIO_PIN_4

#define GPIO_I_FAULT_RES_PERIPH SYSCTL_PERIPH_GPIOM	// I FAULT RESET
#define GPIO_I_FAULT_RES_BASE   GPIO_PORTM_BASE
#define GPIO_I_FAULT_RES_PIN    GPIO_PIN_0

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
#define ECAT_SSI_BASE                   SSI3_BASE
#define ECAT_SSI_SYSCTL_PERIPH   		SYSCTL_PERIPH_SSI3

#define ECAT_SSI_GPIO_PORTBASE          GPIO_PORTR_BASE
#define ECAT_SSI_GPIO_SYSCTL_PERIPH  	SYSCTL_PERIPH_GPIOR

#define ECAT_SSI_CS                 	GPIO_PIN_3
#define ECAT_SSI_CLK                	GPIO_PIN_2
#define ECAT_SSI_RX_MISO               	GPIO_PIN_1
#define ECAT_SSI_TX_MOSI               	GPIO_PIN_0

#define ECAT_SSI_PINS            		(ECAT_SSI_RX_MISO | ECAT_SSI_TX_MOSI | ECAT_SSI_CLK)

#define ECAT_GPIO_PORTBASE              GPIO_PORTK_BASE
#define ECAT_GPIO_SYSCTL_PERIPH  		SYSCTL_PERIPH_GPIOK

#define ECAT_IRQ                		GPIO_PIN_3
#define ECAT_EEPROM_LOADED      		GPIO_PIN_0
#define ECAT_BOOT						GPIO_PIN_2

//*****************************************************************************
//
//  M3 Link Encoder BissC SSI interface
//
//*****************************************************************************
#define LINK_ENC_SSI_BASE               SSI0_BASE
#define LINK_ENC_SSI_SYSCTL_PERIPH   	SYSCTL_PERIPH_SSI0

#define LINK_ENC_SSI_GPIO_PORTBASE      GPIO_PORTD_BASE
#define LINK_ENC_SSI_GPIO_SYSCTL_PERIPH	SYSCTL_PERIPH_GPIOD

#define LINK_ENC_SSI_CLK                GPIO_PIN_2
#define LINK_ENC_SSI_RX_MISO            GPIO_PIN_1

#define LINK_ENC_SSI_PINS          		(LINK_ENC_SSI_RX_MISO | LINK_ENC_SSI_CLK)

//*****************************************************************************
//
//  M3 Deflection Encoder BissC SSI interface
//
//*****************************************************************************
#define DEFL_ENC_SSI_BASE               SSI1_BASE
#define DEFL_ENC_SSI_SYSCTL_PERIPH   	SYSCTL_PERIPH_SSI1

#define DEFL_ENC_SSI_GPIO_PORTBASE      GPIO_PORTL_BASE
#define DEFL_ENC_SSI_GPIO_SYSCTL_PERIPH	SYSCTL_PERIPH_GPIOL

#define DEFL_ENC_SSI_CLK                GPIO_PIN_2
#define DEFL_ENC_SSI_RX_MISO            GPIO_PIN_1

#define DEFL_ENC_SSI_PINS          		(DEFL_ENC_SSI_RX_MISO | DEFL_ENC_SSI_CLK)


//*****************************************************************************
//
//  M3 External ADC AD7680 SSI interface
//
//*****************************************************************************
#define ADC_EXT_SSI_BASE                SSI2_BASE
#define ADC_EXT_SSI_SYSCTL_PERIPH   	SYSCTL_PERIPH_SSI2

#define ADC_EXT_SSI_GPIO_PORTBASE       GPIO_PORTB_BASE
#define ADC_EXT_SSI_GPIO_SYSCTL_PERIPH 	SYSCTL_PERIPH_GPIOB

#define ADC_EXT_SSI_CS	       			GPIO_PIN_3
#define ADC_EXT_SSI_CLK                	GPIO_PIN_2
#define ADC_EXT_SSI_RX_MISO            	GPIO_PIN_1
#define ADC_EXT_SSI_TX_MOSI            	GPIO_PIN_0

#define ADC_EXT_SSI_PINS            	(ADC_EXT_SSI_TX


#endif
