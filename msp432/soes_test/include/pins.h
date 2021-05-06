#ifdef LAUNCHPAD
//*****************************************************************************
//  LEDs
//  P1.0
//  P2.[0,1,2]
#define PORT_LED_RED	GPIO_PORT_P1
#define PORT_LED_RGB	GPIO_PORT_P2
#define PIN_LED_R		GPIO_PIN0
#define PIN_LED_G		GPIO_PIN1
#define PIN_LED_B		GPIO_PIN2
//
//	GPOut probe
//	P3.6
//  P6.[0,1]
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
//  Analog A0,A1,A6-A12 ==> 9 chs
//  P4.[1,2,3,4,5,6,7]	A12,A11,A10,A9,A8,A7,A6
//	P5.[5,4] 			A0, A1
#define PORT_ADC_P4		GPIO_PORT_P4
#define PORT_ADC_P5		GPIO_PORT_P5
#define PIN_A0			GPIO_PIN5
#define PIN_A1			GPIO_PIN4
#define PIN_A6			GPIO_PIN7
#define PIN_A7			GPIO_PIN6
#define PIN_A8			GPIO_PIN5
#define PIN_A9			GPIO_PIN4
#define PIN_A10			GPIO_PIN3
#define PIN_A11			GPIO_PIN2
#define PIN_A12			GPIO_PIN1
#define AIN_P4_PINS		(PIN_ALL8 & (~GPIO_PIN0))
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
//#define PIN_ECAT_EEL  GPIO_PIN2

#define PORT_ECAT_BOOT  GPIO_PORT_P5
#define PIN_ECAT_BOOT  	GPIO_PIN2

#define INT_ECAT        INT_PORT5

#else
//*****************************************************************************
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
#define PORT_DBG		GPIO_PORT_P1
#define PIN_DBG_1		GPIO_PIN0
#define PIN_DBG_2		GPIO_PIN1
//
//  UART
//  P3.[2,3]
#define EUSCI_UART		EUSCI_A2_BASE
#define PORT_UART		GPIO_PORT_P3
#define PIN_UART_RX		GPIO_PIN2
#define PIN_UART_TX		GPIO_PIN3
//
//	DAC
//  P1.[5,6]
//  P2.[1,3,5]
#define EUSCI_DAC      	EUSCI_A1_BASE
#define PORT_DAC_P1		GPIO_PORT_P1
#define PORT_DAC_P2		GPIO_PORT_P2
#define PIN_DAC_SYNC	GPIO_PIN5
#define PIN_DAC_LDAC	GPIO_PIN6
#define PIN_DAC_CLK		GPIO_PIN1
#define PIN_DAC_DIN		GPIO_PIN3
#define PIN_DAC_CLR		GPIO_PIN5
//
//  Analog 9 chs
//  P4.[5,6,7]			C2(A8), A1(A7), TH_A(A6)
//	P5.[0,1,2,3,4,5]	TH_C,B1,A2,C1,B2,TH_B
#define PORT_ADC_P4		GPIO_PORT_P4
#define PORT_ADC_P5		GPIO_PORT_P5
#define PIN_A0			GPIO_PIN5
#define PIN_A1			GPIO_PIN4
#define PIN_A2			GPIO_PIN3
#define PIN_A3			GPIO_PIN2
#define PIN_A4			GPIO_PIN1
#define PIN_A5			GPIO_PIN0
#define PIN_A6			GPIO_PIN7
#define PIN_A7			GPIO_PIN6
#define PIN_A8			GPIO_PIN5
#define AIN_P5_PINS		(PIN_ALL8 & (~(GPIO_PIN6|GPIO_PIN7)))
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

//*****************************************************************************
#endif


#define LED_PINS		(PIN_LED_R|PIN_LED_G|PIN_LED_B)

#define ECAT_SPI_PINS   (PIN_ECAT_CLK|PIN_ECAT_MOSI|PIN_ECAT_MISO)

