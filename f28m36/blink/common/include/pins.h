#ifndef __PINS_H__
#define __PINS_H__

#define CONTROL_CARD	0xCC
#define MOTOR_BOARD		0x01
#define POWER_BOARD		0x03
#define POWER_BOARD_CAN 0x33
#define BLUE_BOARD 		0xBB
#define V3_ACTUATOR     0x11


#if HW_TYPE == CONTROL_CARD

//*****************************************************************************
// GPIO for the user LEDs
//*****************************************************************************
#define LED_PERIPH_M3      	SYSCTL_PERIPH_GPIOE
#define LED_BASE_M3        	GPIO_PORTE_BASE
#define LED_PERIPH_C28      SYSCTL_PERIPH_GPIOF
#define LED_BASE_C28        GPIO_PORTF_BASE

#define LED_RED_PIN          GPIO_PIN_7	// M3
#define LED_ORG_PIN          GPIO_PIN_2	// C28

#define LED_PINS_M3			(LED_RED_PIN)
#define LED_PINS_C28		(LED_ORG_PIN)

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
#define ECAT_BOOT						GPIO_PIN_2

#else

#if HW_TYPE == POWER_BOARD
//*****************************************************************************
// GPIO for the user LEDs
//*****************************************************************************
	#define LED_PERIPH      	SYSCTL_PERIPH_GPIOC
	#define LED_BASE        	GPIO_PORTC_BASE
	#define LED_RED_PIN         GPIO_PIN_7
	#define LED_YLW_PIN         GPIO_PIN_6
	#define LED_GRN_PIN         GPIO_PIN_5
	#define LED_ORG_PIN         GPIO_PIN_4

	#define GPIO_DBG_PERIPH		SYSCTL_PERIPH_GPION	// DBG_PIN
	#define GPIO_DBG_BASE		GPIO_PORTN_BASE
	#define GPIO_DBG_PIN		GPIO_PIN_4

#elif HW_TYPE == POWER_BOARD_CAN
	#define LED_PERIPH_M3      	SYSCTL_PERIPH_GPIOS
	#define LED_BASE_M3        	GPIO_PORTS_BASE
	#define LED_PERIPH_C28      SYSCTL_PERIPH_GPIOS
	#define LED_BASE_C28        GPIO_PORTS_BASE
	#define LED_GRN_PIN         GPIO_PIN_1
	#define LED_YLW_PIN         GPIO_PIN_2
	#define LED_RED_PIN         GPIO_PIN_3
	#define LED_ORG_PIN         GPIO_PIN_4

	#define GPIO_DBG_PERIPH		SYSCTL_PERIPH_GPION	// DBG_PIN
	#define GPIO_DBG_BASE		GPIO_PORTN_BASE
	#define GPIO_DBG_PIN		GPIO_PIN_4

#elif HW_TYPE == BLUE_BOARD
	#define LED_PERIPH_M3		SYSCTL_PERIPH_GPIOL
	#define LED_BASE_M3        	GPIO_PORTL_BASE
	#define LED_RED_PIN         GPIO_PIN_4
	#define LED_GRN_PIN         GPIO_PIN_5
	#define LED_YLW_PIN         GPIO_PIN_6
	#define LED_ORG_PIN       	GPIO_PIN_7

#elif HW_TYPE == MOTOR_BOARD
	#define LED_PERIPH_M3      	SYSCTL_PERIPH_GPIOR	// R6
	#define LED_BASE_M3        	GPIO_PORTR_BASE
	#define LED_PERIPH_C28      SYSCTL_PERIPH_GPIOS	// S0
	#define LED_BASE_C28        GPIO_PORTS_BASE
	#define LED_RED_PIN         GPIO_PIN_5
	#define LED_YLW_PIN         GPIO_PIN_6 	// m3
	#define LED_GRN_PIN         GPIO_PIN_7
	#define LED_ORG_PIN         GPIO_PIN_0

	#define LED_DBG_PERIPH		SYSCTL_PERIPH_GPIOL	// YELLOW TOP LED 1
	#define LED_DBG_BASE      	GPIO_PORTL_BASE
	#define LED_DBG_1_PIN       GPIO_PIN_6
	#define LED_DBG_2_PIN       GPIO_PIN_5

	#define GPIO_DBG_PERIPH		SYSCTL_PERIPH_GPIOP	// DBG_PIN
	#define GPIO_DBG_BASE		GPIO_PORTP_BASE
	#define GPIO_DBG_PIN		GPIO_PIN_0

#elif HW_TYPE == V3_ACTUATOR
	#define HBI_16
	#define LED_PERIPH_M3      	SYSCTL_PERIPH_GPIOC	// RED SIDE LED
	#define LED_PERIPH_C28     	SYSCTL_PERIPH_GPIOG	// RED SIDE LED
	#define LED_BASE_M3        	GPIO_PORTC_BASE
	#define LED_BASE_C28        GPIO_PORTG_BASE
	#define LED_GRN_PIN         GPIO_PIN_0	// 1
	#define LED_YLW_PIN         GPIO_PIN_1	// 2
	#define LED_RED_PIN         GPIO_PIN_6	// 3
	#define LED_ORG_PIN         GPIO_PIN_2	// 4

#else

	#error "Unknown HW_TYPE"
#endif

#define LED_PINS_M3			(LED_YLW_PIN|LED_GRN_PIN)
#define LED_PINS_C28		(LED_RED_PIN|LED_ORG_PIN)

//*****************************************************************************
//
//  M3 PDI EtherCAT interface
//  BlueBoard use EPI0
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


#endif

#endif
