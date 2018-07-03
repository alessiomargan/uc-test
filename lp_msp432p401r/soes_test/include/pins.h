//*****************************************************************************
//
//  LEDs
//
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


