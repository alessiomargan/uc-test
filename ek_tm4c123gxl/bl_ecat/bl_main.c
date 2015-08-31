//*****************************************************************************
//
// bl_main.c - Simple ethercat slave example .
//
//
//*****************************************************************************


#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_sysctl.h>
#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin_map.h>
#include <driverlib/rom.h>
#include <driverlib/ssi.h>
#include <driverlib/uart.h>
#include <driverlib/timer.h>
#include <utils/uartstdio.h>

#include <ethercat.h>
#include <pins.h>

#include "soes/soes.h"

void ConfigureDevice(void);

void MyReinitFunc(void) { ConfigureDevice(); }


//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Enable UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
//
//
//*****************************************************************************
void ConfigureEcatPDI (void) 
{
    // SSI2 and CS use with PortB.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    // CS on PORTB PIN_5
    GPIOPinTypeGPIOOutput(SPI_ECAT_SSI_PORT, SPI_ECAT_CS_PIN);
    GPIOPinWrite(SPI_ECAT_SSI_PORT, SPI_ECAT_CS_PIN, SPI_ECAT_CS_PIN);
    // The SSI2 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    // Configure the pin muxing for SSI2 functions on port
    GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    //ROM_GPIOPinConfigure(GPIO_PB5_SSI2FSS);
    GPIOPinConfigure(GPIO_PB6_SSI2RX); // MISO
    GPIOPinConfigure(GPIO_PB7_SSI2TX); // MOSI
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(SPI_ECAT_SSI_PORT, SPI_ECAT_MOSI_PIN | SPI_ECAT_MISO_PIN | /*GPIO_PIN_5 |*/ SPI_ECAT_CLK_PIN);
    // Configure and enable the SSI port for SPI master mode.
    // Use SSI2, system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 8MHz SSI frequency, and 8-bit data.
    SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 8000000, 8);
    // Enable the SSI2 module.
    SSIEnable(SSI2_BASE);
    // Configure the SPI INT pin as an input.
    // Configure the SPI EPROM_LOADED pin as an input.
    GPIOPinTypeGPIOInput(SPI_ECAT_SSI_PORT, SPI_ECAT_IRQ_PIN);
    GPIOPinTypeGPIOInput(SPI_ECAT_SSI_PORT, SPI_ECAT_EEPROM_LOADED_PIN);
    // Configure the SPI INT pin as interrupt on falling edge.
    GPIOIntTypeSet(SPI_ECAT_SSI_PORT, SPI_ECAT_IRQ_PIN, GPIO_FALLING_EDGE);
    
    // NO ethercat irq
    
    //GPIOIntEnable(SPI_ECAT_SSI_PORT, SPI_ECAT_IRQ_PIN);
    //ROM_IntEnable(INT_GPIOB);


}

void ConfigureLed(void)
{
    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Enable the GPIO pins for the LED (R PF1 - B PF2 - G PF3).
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    int toggle_led = 3;
	while ( toggle_led-- ) {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,  GPIO_PIN_3);
		SysCtlDelay(SysCtlClockGet() / 10 / 3);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,  0);
		SysCtlDelay(SysCtlClockGet() / 10 / 3);
	}

}


//*****************************************************************************
//
//
//
//*****************************************************************************

void ConfigureDevice(void)
{
    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Set up the serial console to use for displaying messages.  This is
    // just for this example program and is not needed for SSI operation.
    ConfigureUART();

    ConfigureLed();
    // Set up ET1100 PDI interface 
    ConfigureEcatPDI();
    // Enable processor interrupts.
    IntMasterEnable();
    //
    soes_init();

}

void Updater(void) {

    UARTprintf("UpdaterECat\n");

    while (1) {

        soes_loop();
        
        
    }

}
