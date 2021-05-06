
//*****************************************************************************
//
//  SPI pins for PDI EtherCAT interface
//
//*****************************************************************************
#define ECAT_SSI_BASE                   SSI2_BASE
#define ECAT_SSI_SYSCTL_PERIPH          SYSCTL_PERIPH_SSI2

#define ECAT_SSI_GPIO_PORTBASE          GPIO_PORTB_BASE
#define ECAT_SSI_GPIO_SYSCTL_PERIPH     SYSCTL_PERIPH_GPIOB

#define ECAT_SSI_CLK                    GPIO_PIN_4
#define ECAT_SSI_CS                     GPIO_PIN_5
#define ECAT_SSI_RX_MISO                GPIO_PIN_6
#define ECAT_SSI_TX_MOSI                GPIO_PIN_7

#define ECAT_GPIO_PORTBASE              GPIO_PORTB_BASE
#define ECAT_GPIO_SYSCTL_PERIPH         SYSCTL_PERIPH_GPIOB

#define ECAT_IRQ                        GPIO_PIN_0
#define ECAT_EEPROM_LOADED              GPIO_PIN_1

#define ECAT_SSI_PINS                   (ECAT_SSI_RX_MISO | ECAT_SSI_TX_MOSI | ECAT_SSI_CLK)
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

