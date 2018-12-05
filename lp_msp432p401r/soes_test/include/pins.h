//*****************************************************************************
//
//  LEDs
//  P1.0
//  P2.[0,1,2]
//
//	GP out probe
//	P3.6
//  P6.[0,1]
//
//  Switch
//  P1.[1,4]
//
//  Analog A0,A1,A6-A12 ==> 9 chs
//  P5.[5,4] P4.[1,7]
//
//  ECAT SPI - PDI EtherCAT interface
#define EUSCI_ECAT      EUSCI_B0_BASE
#define PORT_SPI_ECAT	GPIO_PORT_P1
#define PIN_ECAT_CLK    GPIO_PIN5
#define PIN_ECAT_MOSI   GPIO_PIN6
#define PIN_ECAT_MISO   GPIO_PIN7
#define ECAT_SPI_PINS   (PIN_ECAT_CLK|PIN_ECAT_MOSI|PIN_ECAT_MISO)

#define PORT_ECAT_CS    GPIO_PORT_P3
#define PIN_ECAT_CS     GPIO_PIN0

#define PORT_ECAT_GPIO  GPIO_PORT_P5
#define PIN_ECAT_IRQ    GPIO_PIN0
//#define PIN_ECAT_EEL  GPIO_PIN2

#define INT_ECAT        INT_PORT5

