#ifndef __PINS_H__
#define __PINS_H__

//*****************************************************************************
// GPIO for the user LEDs
//*****************************************************************************
#define LED_0_PERIPH       SYSCTL_PERIPH_GPIOE
#define LED_0_BASE         GPIO_PORTE_BASE
#define LED_0_PIN          GPIO_PIN_7

#define LED_1_PERIPH       SYSCTL_PERIPH_GPIOF
#define LED_1_BASE         GPIO_PORTF_BASE
#define LED_1_PIN          GPIO_PIN_2

//*****************************************************************************
//
//  M3 PDI EtherCAT interface
//
//*****************************************************************************
#define ECAT_SSI_BASE                   SSI0_BASE
#define ECAT_SSI_SYSCTL_PERIPH   		SYSCTL_PERIPH_SSI0

#define ECAT_SSI_GPIO_PORTBASE          GPIO_PORTD_BASE
#define ECAT_SSI_GPIO_SYSCTL_PERIPH  	SYSCTL_PERIPH_GPIOD

#define ECAT_SSI_CS                 	GPIO_PIN_3
#define ECAT_SSI_CLK                	GPIO_PIN_2
#define ECAT_SSI_RX_MISO               	GPIO_PIN_1
#define ECAT_SSI_TX_MOSI               	GPIO_PIN_0

#define ECAT_SSI_PINS            		(ECAT_SSI_RX_MISO | ECAT_SSI_TX_MOSI | ECAT_SSI_CLK)

#define ECAT_GPIO_PORTBASE              GPIO_PORTG_BASE
#define ECAT_GPIO_SYSCTL_PERIPH  		SYSCTL_PERIPH_GPIOG

#define ECAT_IRQ                		GPIO_PIN_0
#define ECAT_EEPROM_LOADED      		GPIO_PIN_1

#endif
