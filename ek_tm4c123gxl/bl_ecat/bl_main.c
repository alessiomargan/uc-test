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

#include <pins.h>

#include <cc.h>
#include <soes/esc.h>
#include <soes/hal/advr_esc/soes.h>

#include <tiva-morser/morse.h>

void ConfigureDevice(void);

void MyReinitFunc(void) { ConfigureDevice(); }

extern esc_cfg_t config;

void jump2app(void) {

}

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

    DPRINT("%s\n",__FUNCTION__);

}

//*****************************************************************************
//
//
//
//*****************************************************************************
void ConfigureEcatPDI (void) 
{
    // enable SSI and GPIO periph
    SysCtlPeripheralEnable(ECAT_SSI_SYSCTL_PERIPH);
    SysCtlPeripheralEnable(ECAT_SSI_GPIO_SYSCTL_PERIPH);
    SysCtlPeripheralEnable(ECAT_GPIO_SYSCTL_PERIPH);

    GPIOPinTypeGPIOOutput(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_CS);
    GPIOPinWrite(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_CS, ECAT_SSI_CS);

    // Configure the pin muxing for SSI2 functions on port
#if HW_TYPE == LAUNCHPAD
    GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    GPIOPinConfigure(GPIO_PB6_SSI2RX); // MISO
    GPIOPinConfigure(GPIO_PB7_SSI2TX); // MOSI
#elif HW_TYPE == VV_IO
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA4_SSI0RX); // MISO
    GPIOPinConfigure(GPIO_PA5_SSI0TX); // MOSI
#else
	#error "Unknown HW_TYPE"
#endif
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(ECAT_SSI_GPIO_PORTBASE, ECAT_SSI_PINS);
    // Configure and enable the SSI port for SPI master mode.
    // Use SSI, system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 8MHz SSI frequency, and 8-bit data.
    SSIConfigSetExpClk(ECAT_SSI_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_3, SSI_MODE_MASTER, 7500000, 8);
    // Enable the SSI module.
    SSIEnable(ECAT_SSI_BASE);

    // Configure the SPI INT pin as an input.
    // Configure the SPI EPROM_LOADED pin as an input.
    GPIOPinTypeGPIOInput(ECAT_GPIO_PORTBASE, ECAT_IRQ);
    GPIOPinTypeGPIOInput(ECAT_GPIO_PORTBASE, ECAT_EEPROM_LOADED);
    // Configure the SPI INT pin as interrupt on falling edge.
    GPIOIntTypeSet(ECAT_GPIO_PORTBASE, ECAT_IRQ, GPIO_FALLING_EDGE);
    // Ecat pin
    //GPIOPinTypeGPIOInput(ECAT_GPIO_PORTBASE, ECAT_BOOT);
    
    // NO ethercat irq
    
    //GPIOIntEnable(SPI_ECAT_SSI_PORT, SPI_ECAT_IRQ_PIN);
    //ROM_IntEnable(INT_GPIOB);

    DPRINT("%s\n",__FUNCTION__);

}

void ConfigureLed(void)
{
    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(LED_PERIPH);
    // Enable the GPIO pins for the LED (R PF1 - B PF2 - G PF3).
    GPIOPinTypeGPIOOutput(LED_BASE, LED_PIN);
    //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    DPRINT("%s\n",__FUNCTION__);

}


void do_morse_led(void) {

	static volatile uint32_t led_status;
	static char *morser_string = "boot";
	static bool sending = false;
	static struct morser m;
	enum OUTPUT res;

    if (sending) {
        res = tick(&m);
        switch(res) {
          case HIGH:
        	  led_status = 1;
			  break;
          case LOW:
        	  led_status = 0;
			  break;
          case END:
        	  led_status = 0;
			  sending = false;
            break;
        }
    } else {
		sending = true;
		init_morser(&m, morser_string);
    }
    /////////////////////////////////////////////////////////////////

    GPIOPinWrite(LED_BASE, LED_PIN, led_status ? LED_PIN : 0 );

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
    //
    //ConfigureTimer();

    // Enable processor interrupts.
    IntMasterEnable();
    //
    soes_init(&config);

}

void Updater(void) {

	static uint32_t loop_cnt;

	DPRINT("UpdaterECat\n");

    while (1) {

    	loop_cnt ++;

        soes_loop();

        if ( ! (loop_cnt % 100) ) {
        	do_morse_led();
        }

        SysCtlDelay(SysCtlClockGet() / 10000);
        
    }

}

/*
void main(void) {

	ConfigureDevice();

	while ( 1 ) {

		Updater();

	}
}
*/
