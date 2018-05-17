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
#include <driverlib/watchdog.h>

#include <utils/uartstdio.h>

#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"
#include "soes/utypes.h"
#include "soes/esc.h"
#include <dog_LCD/DogLcd_test.h>

extern void EcatIntHandler(void);
extern void Timer0A_IntHandler(void);
extern void Timer1A_IntHandler(void);
extern void SysTick_IntHandler(void);

extern volatile unsigned long g_ulSysTickCount;
extern volatile unsigned long g_ulLastTick;

void disable_peripheral_irq(void)
{

	IntDisable(INT_TIMER0A);
    //IntDisable(INT_GPIOB);

}


/**
 * This function sets up UART0 to be used for a console to display information
 *
 * @author amargan (7/4/2014)
 */
void Configure_UART(void)
{
#ifdef _CONTROL_CARD
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinConfigure(GPIO_PE4_U0RX);
    GPIOPinConfigure(GPIO_PE5_U0TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTStdioInit(0);
#else
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIOPinConfigure(GPIO_PG1_U2TX);
    GPIOPinConfigure(GPIO_PG0_U2RX);
    GPIOPinTypeUART(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(2);
#endif
    for(int i=0;i<10;i++) {
    	UARTprintf("************************");
    }
	UARTprintf("\n");
    UARTprintf("%s\n",__FUNCTION__);
}

#define TICKS_PER_SECOND 100
#define MS_PER_SYSTICK (1000 / TICKS_PER_SECOND)
void Configure_SysTick( void )
{
	// Configure SysTick for a 100Hz interrupt.
	SysTickPeriodSet(SysCtlClockGet(SYSTEM_CLOCK_SPEED) / TICKS_PER_SECOND);
	SysTickEnable();
	SysTickIntEnable();
	IntRegister(FAULT_SYSTICK, SysTick_IntHandler);
}

unsigned long GetTickms(void)
{
    unsigned long ulRetVal;
    unsigned long ulSaved;

    ulRetVal = g_ulSysTickCount;
    ulSaved = ulRetVal;

    if(ulSaved > g_ulLastTick) {
        ulRetVal = ulSaved - g_ulLastTick;
    } else {
        ulRetVal = g_ulLastTick - ulSaved;
    }
    // This could miss a few milliseconds but the timings here are on a
    // much larger scale.
    g_ulLastTick = ulSaved;
    // Return the number of milliseconds since the last time this was called.
    return(ulRetVal * MS_PER_SYSTICK);
}

/**
 *
 *
 * @author amargan (7/4/2014)
 */
void Configure_EcatPDI (void)
{
	// SSI0 and GPIOD on PORTD
    // peripheralsz must be enabled for use.
	SysCtlPeripheralEnable(ECAT_SSI_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(ECAT_SSI_GPIO_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(ECAT_GPIO_SYSCTL_PERIPH);

	GPIOPinTypeGPIOOutput(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_CS);
    GPIOPinWrite(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_CS, ECAT_SSI_CS);
    // Configure the pin muxing for SSI functions on port
#ifdef _CONTROL_CARD
    GPIOPinConfigure(GPIO_PD2_SSI0CLK);
    GPIOPinConfigure(GPIO_PD1_SSI0RX); // MISO
    GPIOPinConfigure(GPIO_PD0_SSI0TX); // MOSI
#else
    GPIOPinConfigure(GPIO_PR2_SSI3CLK);
    GPIOPinConfigure(GPIO_PR1_SSI3RX); // MISO
    GPIOPinConfigure(GPIO_PR0_SSI3TX); // MOSI
#endif
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_PINS);
    // Configure and enable the SSI port for SPI master mode.
    // Use SSI, system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 8MHz SSI frequency, and 8-bit data.
    SSIConfigSetExpClk(ECAT_SSI_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 7500000, 8);
    // Enable the SSI module.
    SSIEnable(ECAT_SSI_BASE);

    // Configure the SPI INT pin as an input.
    // Configure the SPI EPROM_LOADED pin as an input.
    GPIOPinTypeGPIOInput(ECAT_GPIO_PORTBASE, ECAT_IRQ);
    GPIOPinTypeGPIOInput(ECAT_GPIO_PORTBASE, ECAT_EEPROM_LOADED);
    // Configure the SPI INT pin as interrupt on falling edge.
    GPIOIntTypeSet(ECAT_GPIO_PORTBASE, ECAT_IRQ, GPIO_FALLING_EDGE);

    // ET1100 pin
	GPIOPinTypeGPIOInput(ECAT_GPIO_PORTBASE, ECAT_BOOT);

    GPIOPinIntEnable(ECAT_GPIO_PORTBASE, ECAT_IRQ);
#ifdef _CONTROL_CARD
    IntRegister(INT_GPIOG, EcatIntHandler);
    IntEnable(INT_GPIOG);
#else
    IntRegister(INT_GPIOK, EcatIntHandler);
    IntEnable(INT_GPIOK);
#endif
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
#ifdef _CONTROL_CARD
	GPIOPinConfigure(GPIO_PE2_SSI1CLK);
    GPIOPinConfigure(GPIO_PE0_SSI1TX); // MOSI
#else
	GPIOPinConfigure(GPIO_PL2_SSI1CLK);
    GPIOPinConfigure(GPIO_PL0_SSI1TX); // MOSI
#endif
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(LCD_SSI_GPIO_PORTBASE, LCD_SSI_PINS);

    // Configure and enable the SSI port for SPI master mode.
    // Use SSI, system clock supply, idle clock level high and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    SSIConfigSetExpClk(LCD_SSI_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 5000000, 8);
    // Enable the SSI module.
    SSIEnable(LCD_SSI_BASE);

    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_A0);
    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_CS);
    //GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_RST);
    //GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_VDD);

    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, LCD_CS);
    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, 0);
    //GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, 0);
    //GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, 0);

#ifdef _LCD
#if 0
    Lcd_init(BOOSTER_OFF);
#else
    lcd_init(BOOSTER_OFF);
#endif
#endif

    UARTprintf("%s\n",__FUNCTION__);
}

void Configure_Link_Enc_BissC (void)
{
	// SSI0
	// peripheralsz must be enabled for use.
	SysCtlPeripheralEnable(LINK_ENC_SSI_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(LINK_ENC_SSI_GPIO_SYSCTL_PERIPH);

    // Configure the pin muxing for SSI functions on port
    GPIOPinConfigure(GPIO_PE2_SSI1CLK);
    GPIOPinConfigure(GPIO_PE1_SSI1RX); // MISO

    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(LINK_ENC_SSI_GPIO_PORTBASE, LINK_ENC_SSI_PINS);

    // Configure and enable the SSI port for SPI master mode.
    // Use SSI, system clock supply, idle clock level high and active low clock in
    // freescale SPI mode, master mode, 5MHz SSI frequency, and 16-bit data.
    SSIConfigSetExpClk(LINK_ENC_SSI_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED), SSI_FRF_MOTO_MODE_2, SSI_MODE_MASTER, 1000000, 16);
    // Enable the SSI module.
    SSIEnable(LINK_ENC_SSI_BASE);

    UARTprintf("%s\n",__FUNCTION__);
}

void Configure_AD7680 (void)
{
	// SSI0
	// peripheralsz must be enabled for use.
	SysCtlPeripheralEnable(LINK_ENC_SSI_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(LINK_ENC_SSI_GPIO_SYSCTL_PERIPH);

	GPIOPinTypeGPIOOutput(LINK_ENC_SSI_GPIO_PORTBASE, GPIO_PIN_3);
	GPIOPinWrite(LINK_ENC_SSI_GPIO_PORTBASE, GPIO_PIN_3, GPIO_PIN_3);

    // Configure the pin muxing for SSI functions on port
    GPIOPinConfigure(GPIO_PE2_SSI1CLK);
    GPIOPinConfigure(GPIO_PE1_SSI1RX); // MISO

    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(LINK_ENC_SSI_GPIO_PORTBASE, LINK_ENC_SSI_PINS);

    // Configure and enable the SSI port for SPI master mode.
    // Use SSI, system clock supply, idle clock level high and active low clock in
    // freescale SPI mode, master mode, 5MHz SSI frequency, and 16-bit data.
    SSIConfigSetExpClk(LINK_ENC_SSI_BASE, SysCtlClockGet(SYSTEM_CLOCK_SPEED), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 1000000, 8);
    // Enable the SSI module.
    SSIEnable(LINK_ENC_SSI_BASE);

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
#ifdef _CONTROL_CARD
    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(LED_1_PERIPH);
    SysCtlPeripheralEnable(LED_0_PERIPH);
    // control card : led1 - PORT F2
    GPIOPinTypeGPIOOutput(LED_1_BASE, LED_1_PIN);
    // control card :
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_7);
    // control card : erase flash : PORT F0
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
#else
    // Enable the GPIO port that is used for the on-board LED.
	SysCtlPeripheralEnable(LED_DBG_1_PERIPH);
	SysCtlPeripheralEnable(LED_DBG_2_PERIPH);
	SysCtlPeripheralEnable(LED_RED_PERIPH);
	SysCtlPeripheralEnable(LED_GRN_PERIPH);

	GPIOPinTypeGPIOOutput(LED_DBG_1_BASE, LED_DBG_1_PIN);
	GPIOPinTypeGPIOOutput(LED_DBG_2_BASE, LED_DBG_2_PIN);
	GPIOPinTypeGPIOOutput(LED_RED_BASE, LED_RED_PIN);
	GPIOPinTypeGPIOOutput(LED_GRN_BASE, LED_GRN_PIN);

	GPIOPinWrite(LED_DBG_1_BASE, LED_DBG_1_PIN,0);
	GPIOPinWrite(LED_DBG_2_BASE, LED_DBG_2_PIN,0);
	GPIOPinWrite(LED_RED_BASE, LED_RED_PIN,0);
	GPIOPinWrite(LED_GRN_BASE, LED_GRN_PIN,0);

#endif

    UARTprintf("%s\n",__FUNCTION__);
}

/**
 *
 *
 * @author amargan (7/4/2014)
 */
void Configure_Timer_0A(void)
{
    // Enable peripheral TIMER0.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Configure the two 32-bit periodic timer.
    TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
    // 1ms period
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet(SYSTEM_CLOCK_SPEED)/1000);
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
    TimerConfigure(TIMER1_BASE, TIMER_CFG_32_BIT_PER);
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet(SYSTEM_CLOCK_SPEED)/500);	// 2 msec timer
    // Setup the interrupts for the timer timeouts.
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    IntRegister(INT_TIMER1A, Timer1A_IntHandler);
    // Enable the timer.
    TimerEnable(TIMER1_BASE, TIMER_A);

    UARTprintf("%s\n",__FUNCTION__);
}

void Watchdog0Reset(void)
{
	WatchdogUnlock(WATCHDOG0_BASE);
	// Set the period of the watchdog timer.
	WatchdogReloadSet(WATCHDOG0_BASE, 0x100);
	// Enable reset generation from the watchdog timer.
	WatchdogResetEnable(WATCHDOG0_BASE);
	// Enable the watchdog timer.
	WatchdogEnable(WATCHDOG0_BASE);
	// Lock subsequent writes to watchdog configuration.
	WatchdogLock(WATCHDOG0_BASE);

	while(1) {}

}


