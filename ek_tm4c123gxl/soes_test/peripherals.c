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
#include <driverlib/pin_map.h>
#include <driverlib/rom.h>
#include <driverlib/ssi.h>
#include <driverlib/timer.h>
#include <driverlib/adc.h>
#include <driverlib/qei.h>
#include <driverlib/i2c.h>
#include <driverlib/uart.h>

#include <utils/uartstdio.h>

#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"

#include "soes/utypes.h"
#include "soes/esc.h"


void disable_peripheral_irq(void)
{
    IntDisable(INT_PWM1_1);

    //ROM_IntMasterDisable();
}


/**
 * This function sets up UART0 to be used for a console to display information
 *
 * @author amargan (7/4/2014)
 */
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

/**
 *
 *
 * @author amargan (7/4/2014)
 */
void ConfigureEcatPDI (void)
{
	// SSI2 and GPIOB on PORTB
    // peripheralsz must be enabled for use.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	// CS on PORTB
    GPIOPinTypeGPIOOutput(SPI_ECAT_SSI_PORT, SPI_ECAT_CS_PIN);
    GPIOPinWrite(SPI_ECAT_SSI_PORT, SPI_ECAT_CS_PIN, SPI_ECAT_CS_PIN);
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
    SSIConfigSetExpClk(SSI_ECAT_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 8000000, 8);
    // Enable the SSI2 module.
    SSIEnable(SSI_ECAT_BASE);
    // Configure the SPI INT pin as an input.
    // Configure the SPI EPROM_LOADED pin as an input.
    GPIOPinTypeGPIOInput(SPI_ECAT_SSI_PORT, SPI_ECAT_IRQ_PIN);
    GPIOPinTypeGPIOInput(SPI_ECAT_SSI_PORT, SPI_ECAT_EEPROM_LOADED_PIN);
    // Configure the SPI INT pin as interrupt on falling edge.
    GPIOIntTypeSet(SPI_ECAT_SSI_PORT, SPI_ECAT_IRQ_PIN, GPIO_FALLING_EDGE);

    GPIOIntEnable(SPI_ECAT_SSI_PORT, SPI_ECAT_IRQ_PIN);
    IntEnable(INT_GPIOB);

    UARTprintf("%s\n",__FUNCTION__);

}

/**
 *
 *
 * @author amargan (7/4/2014)
 */
void ConfigureLed(void)
{
    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Enable the GPIO pins for the LED (R PF1 - B PF2 - G PF3).
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    UARTprintf("%s\n",__FUNCTION__);
}

/**
 *
 *
 * @author amargan (7/4/2014)
 */
void ConfigureTimer(void)
{
    // Enable peripheral TIMER0.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Configure the two 32-bit periodic timer.
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // 1ms period
    TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet()/1000);
    // Setup the interrupts for the timer timeouts.
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Enable the timer.
    TimerEnable(TIMER0_BASE, TIMER_A);

    UARTprintf("%s\n",__FUNCTION__);
}


void Configure_ADC(void)
{
	// The ADC0 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    // Enable sample sequence 2 with a processor signal trigger, max priority 0
    ADCSequenceConfigure(ADC0_BASE, ADC0_SEQ_NR, ADC_TRIGGER_PROCESSOR, 0);
    // Program the sequence.
    // Sample the temperature sensor (ADC_CTL_TS)
    // configure the interrupt flag (ADC_CTL_IE) to be set when the sample is done.
    // Tell the ADC logic that this is the last conversion on sequence 3 (ADC_CTL_END)
    ADCSequenceStepConfigure(ADC0_BASE, ADC0_SEQ_NR, 0, ADC_CTL_END | ADC_CTL_IE | ADC_CTL_TS); // Temperature
    // Renable the ADC sequence code.
    ADCSequenceEnable(ADC0_BASE, ADC0_SEQ_NR);
    ADCIntClear(ADC0_BASE, ADC0_SEQ_NR);
    ADCIntEnable(ADC0_BASE, ADC0_SEQ_NR);
    IntEnable(INT_ADC0SS2);

    UARTprintf("%s\n",__FUNCTION__);

}

void trigger_internal_ADC(void)
{
    // Retrigger the ADC sequence.
    ADCProcessorTrigger(ADC0_BASE, ADC0_SEQ_NR);
}


void read_sensors(void) {

	trigger_internal_ADC();
}
