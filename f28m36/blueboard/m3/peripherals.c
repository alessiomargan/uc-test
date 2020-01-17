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

//#include <soes_hook.h>
//#include <soes/esc.h>

#include <pins.h>
#include <peripherals.h>

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

void jump_to_bootloader(void) {

	UARTprintf("Jump to bootloader\n");
    // Return control to the boot loader.
    Watchdog0Reset();
}

/**
 * This function sets up UART0 to be used for a console to display information
 *
 * @author amargan (7/4/2014)
 */
void Configure_UART(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIOPinConfigure(GPIO_PG1_U2TX);
    GPIOPinConfigure(GPIO_PG0_U2RX);
    GPIOPinTypeUART(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(2);

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
	SysCtlPeripheralEnable(LED_PERIPH);
	GPIOPinTypeGPIOOutput(LED_BASE, LED_PINS_M3);
	GPIOPinWrite(LED_BASE, LED_PINS_M3, 0);

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


