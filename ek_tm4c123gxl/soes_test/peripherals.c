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

#include "osal.h"
#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"

#include "soes/utypes.h"
#include "soes/esc.h"


void disable_peripheral_irq(void)
{

	IntDisable(INT_PWM1_1);
    IntDisable(INT_ADC0SS2);
    IntDisable(INT_TIMER0A);
    IntDisable(INT_GPIOB);

}


/**
 * This function sets up UART0 to be used for a console to display information
 *
 * @author amargan (7/4/2014)
 */
void Configure_UART(void)
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
 */
void Configure_EcatPDI (void)
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
    IntRegister(INT_GPIOB, GPIOB_IntHandler);
	IntEnable(INT_GPIOB);

	UARTprintf("%s\n",__FUNCTION__);

}

void Configure_LCD (void)
{
	// SSI1
	// peripherals must be enabled for use.
	SysCtlPeripheralEnable(LCD_SSI_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(LCD_SSI_GPIO_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(LCD_GPIO_SYSCTL_PERIPH);

    // Configure the pin muxing for SSI functions on port
    GPIOPinConfigure(GPIO_PD0_SSI1CLK);
    GPIOPinConfigure(GPIO_PD3_SSI1TX); // MOSI

    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(LCD_SSI_GPIO_PORTBASE, LCD_SSI_PINS);

    // Configure and enable the SSI port for SPI master mode.
    // Use SSI, system clock supply, idle clock level high and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    SSIConfigSetExpClk(LCD_SSI_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 5000000, 8);
    // Enable the SSI module.
    SSIEnable(LCD_SSI_BASE);

    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_A0);
    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_CS);
    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_RST);
    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_VDD);

    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, 0);
    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, LCD_CS);
    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, 0);
    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, 0);

    Lcd_init();

    UARTprintf("%s\n",__FUNCTION__);
}

/**
 *
 *
 * @author amargan (7/4/2014)
 */
void Configure_Led(void)
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
 */
void Configure_Timer_0A(void)
{
    // Enable peripheral TIMER0.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Configure the two 32-bit periodic timer.
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // 1ms period
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/1000);
    // Setup the interrupts for the timer timeouts.
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntRegister(INT_TIMER0A, Timer0A_IntHandler);
    // Enable the timer.
    TimerEnable(TIMER0_BASE, TIMER_A);

    UARTprintf("%s\n",__FUNCTION__);
}
void Configure_Timer_1A(void)
{
    // Enable peripheral TIMER1.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    // Configure the two 32-bit periodic timer.
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    // 2 msec timer
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/500);
    // Setup the interrupts for the timer timeouts.
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    IntRegister(INT_TIMER1A, Timer1A_IntHandler);
    // Enable the timer.
    TimerEnable(TIMER1_BASE, TIMER_A);

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
    IntRegister(INT_ADC0SS2, ADC2_IntHandler);
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
