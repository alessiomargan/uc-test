/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "pins.h"
#include "peripherals.h"
//#include "flashmailbox.h"


#include "BSL432_API.h"
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
   #include "MSP432P401x/BSL432_device_file.h"
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
   #include "MSP432P4111/BSL432_device_file.h"
#else
#endif

const char morse_boot[] = "boot ";

extern void soes_init(void);
extern void soes_loop(void);
extern void ecat_process_pdo(void);
//extern void do_morse_led(void);
extern void play_string(const char *);
extern void play_char_string(const char c);


static void jump2app(void) {

	Interrupt_disableMaster(); // disable interrupts
	// redirect the vector table
    // initialize the vector table offset
    SCB->VTOR = 0x0020000;

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

	return (!sw);
}


void main(uint32_t bslParams)
{
    int 	i = 0;
    char * 	pchar = 0;
    uint32_t	loop_cnt;
    uint8_t		test_jump = 0;

    //if(CheckFlashMailbox(COMMAND_BSL_CONFIG) != BOOT_OVERRIDE_AND_MAILBOX_OPERATIONS_SUCCESS) {
    //
    //}

    Configure_UART();
    Configure_Led();
    Configure_EcatPDI();
    //Configure_Timer();
    Configure_Switch();

    soes_init();

    //Interrupt_setPriority(INT_T32_INT1, (char)(2)<<5);
    //Interrupt_enableMaster();

    while(1)
    {

        test_jump = test_jump2app();
        if ( test_jump ) {
        	jump2app();
        }

#if 0
    	if ( ! *pchar ) {
        	pchar = (char*)morse_boot;
        }
        play_char_string(*pchar);
        pchar++;
#endif
        soes_loop();

        if ( ! (loop_cnt++ % 10) ) {
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN6);
        }

        // Delay .. 48 Mhz clock --> 1ms
        for(i=48000; i>0; i--);

    }

}


