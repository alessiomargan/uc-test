/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "pins.h"
#include "osal.h"
#include "peripherals.h"
#include "foe_flash.h"
#include "tiva-morser/morse.h"

#include "BSL432_API.h"
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
   #include "MSP432P401x/BSL432_device_file.h"
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
   #include "MSP432P4111/BSL432_device_file.h"
#else
#endif

uint32_t 	calc_crc;


extern void soes_init(void);
extern void soes_loop(void);
extern void ecat_process_pdo(void);
extern void do_morse_led(void);

void do_morse_led(void);

static void jump2app(void) {

	Interrupt_disableMaster(); // disable interrupts
	// redirect the vector table
    // initialize the vector table offset
    SCB->VTOR = FLASH_APP_START;

	// Load the stack pointer from the application's vector table.
	__asm(" ldr r1, [r0, #0]\n"
	      " mov sp, r1\n");
	// Load the initial PC from the application's vector table and branch to
	// the application's entry point.
	__asm(" ldr r0, [r0, #4]\n"
	      " blx r0\n");
}

static uint8_t test_jump2app(void) {

	// poll switch
	uint8_t sw = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);
    bool ret = ( !sw ) | ( calc_crc==CRC_App );
	return (ret);
}


void main(uint32_t bslParams)
{
	uint32_t value;
    int 	i = 0;
    volatile uint32_t	loop_cnt;
    volatile uint8_t	test_jump = 0;

    Configure_UART();
    Configure_Led();
    Configure_EcatPDI();
    //Configure_Timer();
    Configure_Switch();

    calc_crc = calc_CRC(0x20000, 0x20000);
    DPRINT("bldr ver %d.%d\n", BLDR_Version[0],BLDR_Version[1]);
    DPRINT("CRC : calc 0x%04X flash 0x%04X\n", calc_crc, CRC_App);

    ESC_init(0);
    //soes_init();

    //Interrupt_setPriority(INT_T32_INT1, (char)(2)<<5);
    //Interrupt_enableMaster();

    while(1)
    {

        test_jump = test_jump2app();
        if ( test_jump ) {
        	jump2app();
        }

        //soes_loop();
        //value = lan9252_read_32(0x64);
        ESC_read(0x64, &value, 4);

        if ( ! (loop_cnt++ % 100) ) {
			do_morse_led();
        }

        // Delay .. 48 Mhz clock --> 1ms
        for(i=2000; i>0; i--);

    }

}

void do_morse_led(void) {

    static volatile uint32_t  led_status;
    static char * morser_string = "boot";
    static bool sending;
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

    if ( led_status ) {
    	GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    	GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
    } else {
    	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    	GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
    }
}

