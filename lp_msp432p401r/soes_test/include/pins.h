//*****************************************************************************
//
//  LEDs
//  P1.0
//  P2.[0,1,2]
//
//	GP out probe
//	P3.6
//  P6.[0,1]

//  Switch
//  P1.[1,4]

//  Analog A0,A1,A6-A12 ==> 9 chs
//  P5.[5,4] P4.[1,7]
//*****************************************************************************


//*****************************************************************************
//
//  SPI pins for PDI EtherCAT interface
//
//*****************************************************************************
#define SPI_ECAT_PORT               GPIO_PORT_P1
#define ECAT_CLK_PIN                GPIO_PIN5
#define ECAT_MOSI_PIN               GPIO_PIN6
#define ECAT_MISO_PIN               GPIO_PIN7
#define ECAT_SPI_PINS               (ECAT_CLK_PIN|ECAT_MOSI_PIN|ECAT_MISO_PIN)

#define ECAT_CS_PORT                GPIO_PORT_P3
#define ECAT_CS_PIN                 GPIO_PIN0

#define ECAT_GPIO_PORT              GPIO_PORT_P5
#define ECAT_IRQ_PIN                GPIO_PIN0
#define ECAT_EEL_PIN                GPIO_PIN2

#define EUSCI_ECAT                  EUSCI_B0_BASE


