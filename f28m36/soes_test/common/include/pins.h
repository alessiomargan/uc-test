#ifndef __PINS_H__
#define __PINS_H__

#define CONTROL_CARD    0xCC
#define MOTOR_BOARD     0x01
#define POWER_BOARD     0x03
#define BLUE_BOARD      0xBB


#if HW_TYPE == CONTROL_CARD

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
#define ECAT_BOOT						GPIO_PIN_3


//*****************************************************************************
//
//  M3 LCD EA DOGM128-6
//
//*****************************************************************************
#define LCD_SSI_BASE               	SSI1_BASE
#define LCD_SSI_SYSCTL_PERIPH   	SYSCTL_PERIPH_SSI1

#define LCD_SSI_GPIO_PORTBASE		GPIO_PORTE_BASE
#define LCD_SSI_GPIO_SYSCTL_PERIPH	SYSCTL_PERIPH_GPIOE

#define LCD_SSI_CLK               	GPIO_PIN_2
#define LCD_SSI_TX_MOSI           	GPIO_PIN_0

#define LCD_SSI_PINS          		(LCD_SSI_TX_MOSI | LCD_SSI_CLK)

#define LCD_GPIO_PORTBASE           GPIO_PORTE_BASE
#define LCD_GPIO_SYSCTL_PERIPH  	SYSCTL_PERIPH_GPIOE

#define LCD_A0						GPIO_PIN_1
#define LCD_CS						GPIO_PIN_3
#define LCD_RST						GPIO_PIN_4
#define LCD_VDD						GPIO_PIN_5

#define LCD_CS_BASE					LCD_SSI_GPIO_PORTBASE
#define LCD_A0_BASE					LCD_GPIO_PORTBASE
#define LCD_RST_BASE				LCD_GPIO_PORTBASE

//*****************************************************************************
//
//  M3 Link Encoder BissC SSI interface
//
//*****************************************************************************
#define LINK_ENC_SSI_BASE               SSI1_BASE
#define LINK_ENC_SSI_SYSCTL_PERIPH   	SYSCTL_PERIPH_SSI1

#define LINK_ENC_SSI_GPIO_PORTBASE      GPIO_PORTE_BASE
#define LINK_ENC_SSI_GPIO_SYSCTL_PERIPH	SYSCTL_PERIPH_GPIOE

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
// #define ADC_EXT_SSI_TX_MOSI            	GPIO_PIN_0

#define ADC_EXT_SSI_PINS            	(ADC_EXT_SSI_RX_MISO | ADC_EXT_SSI_CLK) // | ADC_EXT_SSI_CS)

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
//  M3 LCD EA DOGM128-6
//
//*****************************************************************************
#define LCD_SSI_BASE               	SSI1_BASE
#define LCD_SSI_SYSCTL_PERIPH   	SYSCTL_PERIPH_SSI1

#define LCD_SSI_GPIO_PORTBASE		GPIO_PORTL_BASE
#define LCD_SSI_GPIO_SYSCTL_PERIPH	SYSCTL_PERIPH_GPIOL

#define LCD_SSI_CLK               	GPIO_PIN_2
#define LCD_SSI_TX_MOSI           	GPIO_PIN_0

#define LCD_SSI_PINS          		(LCD_SSI_TX_MOSI | LCD_SSI_CLK)

#define LCD_GPIO_PORTBASE           GPIO_PORTL_BASE
#define LCD_GPIO_SYSCTL_PERIPH  	SYSCTL_PERIPH_GPIOL

#define LCD_A0						GPIO_PIN_1
#define LCD_CS						GPIO_PIN_3
//#define LCD_RST						GPIO_PIN_4
//#define LCD_VDD						GPIO_PIN_5

#define LCD_CS_BASE                 LCD_SSI_GPIO_PORTBASE
#define LCD_A0_BASE                 LCD_GPIO_PORTBASE
#define LCD_RST_BASE                LCD_GPIO_PORTBASE

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

#endif
