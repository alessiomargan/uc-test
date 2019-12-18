
#if HW_VER == 0xA

#define LAUNCHPAD

//*****************************************************************************
//  LEDs
//  P1.0
//  P2.[0,1,2]
#define PORT_LED_RED	GPIO_PORT_P1
#define PORT_LED_RBG	GPIO_PORT_P2
#define PIN_LED_R		GPIO_PIN0
#define PIN_LED_G		GPIO_PIN1
#define PIN_LED_B		GPIO_PIN2
//
//  Switch
//  P1.[1,4]
//
//  UART
//  P1.[2,3]
#define EUSCI_UART		EUSCI_A0_BASE
#define PORT_UART		GPIO_PORT_P1
#define PIN_UART_RX		GPIO_PIN2
#define PIN_UART_TX		GPIO_PIN3
//
//  ECAT SPI - PDI EtherCAT interface
#define EUSCI_ECAT      EUSCI_B0_BASE
#define PORT_SPI_ECAT	GPIO_PORT_P1
#define PIN_ECAT_CLK    GPIO_PIN5
#define PIN_ECAT_MOSI   GPIO_PIN6
#define PIN_ECAT_MISO   GPIO_PIN7

#define PORT_ECAT_CS    GPIO_PORT_P3
#define PIN_ECAT_CS     GPIO_PIN0

#define PORT_ECAT_GPIO  GPIO_PORT_P5
#define PIN_ECAT_IRQ    GPIO_PIN0
//#define PIN_ECAT_EEL  	GPIO_PIN2

#define PORT_ECAT_BOOT  GPIO_PORT_P5
#define PIN_ECAT_BOOT  	GPIO_PIN2

#define INT_ECAT        INT_PORT5


//*****************************************************************************
#elif HW_VER == 0xB

//  LEDs
//  P7.6 R
//  P7.4 G
//  P3.0 B
#define PORT_LED_RG		GPIO_PORT_P7
#define PORT_LED_B		GPIO_PORT_P3
#define PIN_LED_R		GPIO_PIN6
#define PIN_LED_G		GPIO_PIN4
#define PIN_LED_B		GPIO_PIN0
//
//	DBG
//	P1.[0,1]
#define PORT_DBG_1		GPIO_PORT_P1
#define PIN_DBG_1		GPIO_PIN0
#define PORT_DBG_2		GPIO_PORT_P1
#define PIN_DBG_2		GPIO_PIN1
//
//  UART
//  P3.[2,3]
#define EUSCI_UART		EUSCI_A2_BASE
#define PORT_UART		GPIO_PORT_P3
#define PIN_UART_RX		GPIO_PIN2
#define PIN_UART_TX		GPIO_PIN3
//
//  ECAT SPI - PDI EtherCAT interface
#define EUSCI_ECAT      EUSCI_B1_BASE
#define PORT_SPI_ECAT   GPIO_PORT_P6
#define PIN_ECAT_CLK    GPIO_PIN3
#define PIN_ECAT_MOSI   GPIO_PIN4
#define PIN_ECAT_MISO   GPIO_PIN5

#define PORT_ECAT_CS    GPIO_PORT_P6
#define PIN_ECAT_CS     GPIO_PIN2

#define PORT_ECAT_GPIO  GPIO_PORT_P6
#define PIN_ECAT_IRQ    GPIO_PIN0
//#define PIN_ECAT_EEL  GPIO_PIN2

#define PORT_ECAT_BOOT  GPIO_PORT_P4
#define PIN_ECAT_BOOT  	GPIO_PIN0

#define INT_ECAT        INT_PORT6

#elif HW_VER == 0xC

//  LEDs
//  P7.6 R
//  P7.4 G
//  P3.0 B
#define PORT_LED_RG		GPIO_PORT_P7
#define PORT_LED_B		GPIO_PORT_P3
#define PIN_LED_R		GPIO_PIN6
#define PIN_LED_G		GPIO_PIN4
#define PIN_LED_B		GPIO_PIN0
//
//	DBG
//	P7.5 P2.7
#define PORT_DBG_1		GPIO_PORT_P7
#define PIN_DBG_1		GPIO_PIN5
#define PORT_DBG_2		GPIO_PORT_P2
#define PIN_DBG_2		GPIO_PIN7
//
//  UART
//  P3.[2,3]
#define EUSCI_UART		EUSCI_A2_BASE
#define PORT_UART		GPIO_PORT_P3
#define PIN_UART_RX		GPIO_PIN2
#define PIN_UART_TX		GPIO_PIN3
//
//  ECAT SPI - PDI EtherCAT interface
#define EUSCI_ECAT      EUSCI_B1_BASE
#define PORT_SPI_ECAT   GPIO_PORT_P6
#define PIN_ECAT_CLK    GPIO_PIN3
#define PIN_ECAT_MOSI   GPIO_PIN4
#define PIN_ECAT_MISO   GPIO_PIN5

#define PORT_ECAT_CS    GPIO_PORT_P6
#define PIN_ECAT_CS     GPIO_PIN2

#define PORT_ECAT_GPIO  GPIO_PORT_P3
#define PIN_ECAT_IRQ    GPIO_PIN1
//#define PIN_ECAT_EEL  GPIO_PIN2

#define PORT_ECAT_BOOT  GPIO_PORT_P7
#define PIN_ECAT_BOOT  	GPIO_PIN3

#define INT_ECAT        INT_PORT3

#else

#error "HW_VER NOT Defined"

#endif


#define LED_PINS		(PIN_LED_R|PIN_LED_G|PIN_LED_B)

#define ECAT_SPI_PINS   (PIN_ECAT_CLK|PIN_ECAT_MOSI|PIN_ECAT_MISO)
