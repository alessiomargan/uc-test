//*****************************************************************************
//
// soes_main.c - Simple Open Ethercat Slave example .
//
//
//*****************************************************************************


#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_pwm.h>
#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin_map.h>
#include <driverlib/rom.h>
#include <driverlib/ssi.h>
#include <driverlib/uart.h>
#include <driverlib/timer.h>
#include <driverlib/pwm.h>

#include <utils/uartstdio.h>

#include <ethercat.h>
#include <soes/soes.h>

#include <peripherals.h>


/**
 * 
 * 
 * @author amargan (7/4/2014)
 * 
 */
void main(void)
{
    // Set the clocking to run directly from the crystal.
    // To clock the system from the PLL, use SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
    // and select the appropriate crystal with one of the SYSCTL_XTAL_xxx values
    // PLL is 200 MHz -> 200 / 2.5 = 80 Mhz
    //SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Set up the serial console to use for displaying messages.
    ConfigureUART();
    UARTprintf("\n\nHello, world!\n");
    UARTprintf("%s %s\n",__DATE__ , __TIME__);
    UARTprintf("SysCtlClockGet %d\n", SysCtlClockGet() );
    //
    ConfigureLed();
    // Set up ET1100 PDI interface 
    ConfigureEcatPDI();
    //
    Configure_ADC();
    //
    init_PWM();
    // should be the last configure ...
    ConfigureTimer();

    IntPrioritySet(INT_TIMER0A, 0x20);
    IntPrioritySet(INT_GPIOB,   0x20);

    // Enable processor interrupts.
    IntMasterEnable();

    //
    soes_init();

    while (1) {
        // do nothing .. just loop
    }


}
