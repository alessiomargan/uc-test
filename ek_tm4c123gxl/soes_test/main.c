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
#include <grlib/grlib.h>

#include <utils/uartstdio.h>

#include <cc.h>
#include <soes/esc.h>
#include <soes/hal/advr_esc/soes.h>

#include "pins.h"
#include "globals.h"
#include "peripherals.h"
#include "params.h"


#ifdef USE_LCD
	#ifdef C_LCD
		extern void lcd_test_2d( void );
		extern void lcd_test_char( void );
		extern void lcd_test_string( void );
	#else
		#include "cpp_dog_LCD/c_lcd_iface.h"
		#include "cpp_dog_LCD/pics/hellombed.h"
		#include "cpp_dog_LCD/pics/logo_BLH.h"
		#include "cpp_dog_LCD/fonts/font_6x8.h"
		#include "cpp_dog_LCD/fonts/font_8x8.h"
		#include "cpp_dog_LCD/fonts/font_8x16.h"
		#include "cpp_dog_LCD/fonts/font_16x32nums.h"
	#endif
#endif


extern esc_cfg_t config;

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
    sdo.flash.board_id = 969;
    //
    Configure_Led();
    Configure_EcatPDI();
#ifdef USE_LCD
    Configure_LCD();
#endif

    Configure_ADC();
    // should be the last configure ...
    Configure_Timer_0A(); // ecat
    Configure_Timer_1A();

    IntPrioritySet(INT_TIMER1A, 0x30);
    IntPrioritySet(INT_TIMER0A, 0x20);
    IntPrioritySet(INT_GPIOB,   0x20);

    //
    soes_init(&config);
#ifdef USE_LCD
#ifdef C_LCD
    lcd_test_2d ();
    lcd_test_char();
    lcd_test_string();
	Flush();
#else
	lcd_clear_screen();
	//lcd_send_bitmap(pic_hellombed);
	lcd_send_pic(0, 0, ea_logo);
	lcd_string(60, 0, font_16x32num, "3");
	lcd_string(0, 4, font_8x16, "Ciao bello ?");
	lcd_string(0, 6, font_8x8, "Hello world !!");
#endif
#endif

    // Enable processor interrupts.
    IntMasterEnable();


    while (1) {

    	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3 );
    	//SysCtlDelay( 2000000 );
        //lcd_test_string();
    	//Flush();
    	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0 );
		//SysCtlDelay( 2000000 );
	}


}
