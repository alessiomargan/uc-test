#ifndef __PINS_H__
#define __PINS_H__

#ifdef CONTROL_CARD

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

#else

//*****************************************************************************
// GPIO for the user LEDs
//*****************************************************************************
#define LED_RED_PERIPH      SYSCTL_PERIPH_GPIOR	// RED SIDE LED
#define LED_RED_BASE        GPIO_PORTR_BASE
#define LED_RED_PIN         GPIO_PIN_5

#define LED_GRN_PERIPH      SYSCTL_PERIPH_GPIOR	// YELLOW SIDE LED
#define LED_GRN_BASE        GPIO_PORTR_BASE
#define LED_GRN_PIN         GPIO_PIN_7

#define LED_DBG_1_PERIPH	SYSCTL_PERIPH_GPIOL	// YELLOW TOP LED 1
#define LED_DBG_1_BASE      GPIO_PORTL_BASE
#define LED_DBG_1_PIN       GPIO_PIN_6

#define LED_DBG_2_PERIPH	SYSCTL_PERIPH_GPIOL	// YELLOW TOP LED 2:
#define LED_DBG_2_BASE      GPIO_PORTL_BASE
#define LED_DBG_2_PIN       GPIO_PIN_5

#define GPIO_FAN_PERIPH		SYSCTL_PERIPH_GPIOB	// FAN GPIO
#define GPIO_FAN_BASE		GPIO_PORTB_BASE
#define GPIO_FAN_PIN		GPIO_PIN_6

#define GPIO_EXT_LED_PERIPH	SYSCTL_PERIPH_GPIOB	// EXT LED GPIO
#define GPIO_EXT_LED_BASE	GPIO_PORTB_BASE
#define GPIO_EXT_LED_PIN	GPIO_PIN_4

//*****************************************************************************
//
//  M3 PDI EtherCAT interface
//
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

#endif // CONTROL_CARD

#endif
