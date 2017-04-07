
//*****************************************************************************
//
//  SPI pins for PDI EtherCAT interface
//
//*****************************************************************************
#define SPI_ECAT_SSI_PORT               GPIO_PORTB_BASE

#define SPI_ECAT_CLK_PIN                GPIO_PIN_4
#define SPI_ECAT_CS_PIN                 GPIO_PIN_5
#define SPI_ECAT_MISO_PIN               GPIO_PIN_6
#define SPI_ECAT_MOSI_PIN               GPIO_PIN_7

#define SPI_ECAT_IRQ_PIN                GPIO_PIN_0
#define SPI_ECAT_EEPROM_LOADED_PIN      GPIO_PIN_1

#define SSI_ECAT_BASE                   SSI2_BASE

//*****************************************************************************
//
//  M3 LCD EA DOGM128-6
//
//*****************************************************************************
#define LCD_SSI_BASE               	SSI1_BASE
#define LCD_SSI_SYSCTL_PERIPH   	SYSCTL_PERIPH_SSI1

#define LCD_SSI_GPIO_PORTBASE		GPIO_PORTD_BASE
#define LCD_SSI_GPIO_SYSCTL_PERIPH	SYSCTL_PERIPH_GPIOE

#define LCD_SSI_CLK               	GPIO_PIN_0
#define LCD_SSI_TX_MOSI           	GPIO_PIN_3

#define LCD_SSI_PINS          		(LCD_SSI_TX_MOSI | LCD_SSI_CLK)

#define LCD_GPIO_PORTBASE           GPIO_PORTD_BASE
#define LCD_GPIO_SYSCTL_PERIPH  	SYSCTL_PERIPH_GPIOD

#define LCD_A0						GPIO_PIN_1
#define LCD_CS						GPIO_PIN_2
#define LCD_RST						GPIO_PIN_6
#define LCD_VDD						GPIO_PIN_7

