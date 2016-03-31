/**
 * 
 */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_nvic.h>
#include <inc/hw_types.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/ssi.h>
#include <driverlib/timer.h>
#include <driverlib/uart.h>
#include "driverlib/watchdog.h"

#include <utils/uartstdio.h>

#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"

#include "soes/utypes.h"
#include "soes/esc.h"


#define DPRINT(...) UARTprintf("btl: "__VA_ARGS__)

/**
 * This function sets up UART0 to be used for a console to display information
 *
 * @author amargan (7/4/2014)
 */
void ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinConfigure(GPIO_PE4_U0RX);
    GPIOPinConfigure(GPIO_PE5_U0TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTStdioInit(0);
    DPRINT("************************\n");
    DPRINT("%s\n",__FUNCTION__);
}

/**
 *
 *
 * @author amargan (7/4/2014)
 */
void ConfigureEcatPDI (void)
{
	// SSI0 and GPIOD on PORTD
    // peripheralsz must be enabled for use.
	SysCtlPeripheralEnable(ECAT_SSI_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(ECAT_SSI_GPIO_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(ECAT_GPIO_SYSCTL_PERIPH);

	GPIOPinTypeGPIOOutput(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_CS);
    GPIOPinWrite(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_CS, ECAT_SSI_CS);
    // Configure the pin muxing for SSI functions on port
    GPIOPinConfigure(GPIO_PD2_SSI0CLK);
    //ROM_GPIOPinConfigure(GPIO_PB5_SSI2FSS);
    GPIOPinConfigure(GPIO_PD1_SSI0RX); // MISO
    GPIOPinConfigure(GPIO_PD0_SSI0TX); // MOSI
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_PINS);
    // Configure and enable the SSI port for SPI master mode.
    // Use SSI, system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 8MHz SSI frequency, and 8-bit data.
    SSIConfigSetExpClk(ECAT_SSI_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 8000000, 8);
    // Enable the SSI module.
    SSIEnable(ECAT_SSI_BASE);

    // Configure the SPI INT pin as an input.
    // Configure the SPI EPROM_LOADED pin as an input.
    GPIOPinTypeGPIOInput(ECAT_GPIO_PORTBASE, ECAT_IRQ);
    GPIOPinTypeGPIOInput(ECAT_GPIO_PORTBASE, ECAT_EEPROM_LOADED);
    // Configure the SPI INT pin as interrupt on falling edge.
    GPIOIntTypeSet(ECAT_GPIO_PORTBASE, ECAT_IRQ, GPIO_FALLING_EDGE);

    // NO IRQ
    //GPIOPinIntEnable(ECAT_GPIO_PORTBASE, ECAT_IRQ);
    //IntRegister(INT_GPIOG, GPIOGIntHandler);
    //IntEnable(INT_GPIOG);

    DPRINT("%s\n",__FUNCTION__);

}

/**
 *
 *
 * @author amargan (7/4/2014)
 */
void ConfigureLed(void)
{
    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(LED_1_PERIPH);
    SysCtlPeripheralEnable(LED_0_PERIPH);

    // control card led1
    GPIOPinTypeGPIOOutput(LED_1_BASE, LED_1_PIN);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7);
    //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);

    DPRINT("%s\n",__FUNCTION__);
}


void Watchdog0Reset(void)
{
	WatchdogUnlock(WATCHDOG0_BASE);
	// Set the period of the watchdog timer.
	WatchdogReloadSet(WATCHDOG0_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED));
	// Enable reset generation from the watchdog timer.
	WatchdogResetEnable(WATCHDOG0_BASE);
	// Enable the watchdog timer.
	WatchdogEnable(WATCHDOG0_BASE);
	// Lock subsequent writes to watchdog configuration.
	WatchdogLock(WATCHDOG0_BASE);

}
