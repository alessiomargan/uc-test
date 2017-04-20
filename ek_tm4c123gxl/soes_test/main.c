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

extern volatile uint32_t timer1_cnt;
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
    // PLL is 200 MHz -> 200 / 2.5 = 80 Mhz;
    //SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Set up the serial console to use for displaying messages.
    Configure_UART();
    UARTprintf("\n\nHello, world!\n");
    UARTprintf("%s %s\n",__DATE__ , __TIME__);
    UARTprintf("SysCtlClockGet %d\n", SysCtlClockGet() );
    //
    Configure_Led();
    //Configure_EcatPDI();
    Configure_LCD();
    Configure_ADC();
    // should be the last configure ...
    //Configure_Timer_0A();
    Configure_Timer_1A();

    IntPrioritySet(INT_TIMER1A, 0x30);
    //IntPrioritySet(INT_TIMER0A, 0x20);
    //IntPrioritySet(INT_GPIOB,   0x20);

    // Enable processor interrupts.
    IntMasterEnable();

    //
    //soes_init();
	lcd_self_2d();
    lcd_self_char();
    lcd_self_string();
	Flush();

    while (1) {

    	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3 );
    	SysCtlDelay( 2000000 );

        //lcd_self_sprint();
    	//Flush();

    	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0 );
		SysCtlDelay( 2000000 );
	}


}
