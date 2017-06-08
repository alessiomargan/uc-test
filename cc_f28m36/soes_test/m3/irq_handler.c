#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_sysctl.h>
#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/ssi.h>
#include <driverlib/timer.h>

#include <ethercat.h>
#include <soes/soes.h>

#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"
#include "shared_ram.h"


volatile long ecat_irq_cnt = 0;
volatile long pwm_irq_cnt = 0;



/**
 * 
 */
void EcatIntHandler(void) {

	GPIOPinRead(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	GPIOPinIntClear(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	ecat_irq_cnt++;

    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7);
    ecat_process_pdo();
    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0);

    //UARTprintf("PDI irq %d\n", ecat_irq_cnt );
}

/**
 * 
 */
void Timer0A_IntHandler(void) {

    static uint32_t timer0_cnt;
    static volatile uint32_t  toggle;

    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timer0_cnt++;

    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);

	if ( ! DC_activation() ) {
        ecat_process_pdo();
    }

    // every cycle
	soes_loop();

    // every 1000 cycles
    if ( (timer0_cnt % 1000) == 0 ) {
        // toggle
        HWREGBITB(&toggle, 0) ^= 1;
        UARTprintf("\r tmr0: %d %d", timer0_cnt, toggle);
    }

#ifdef _CONTROL_CARD
    GPIOPinWrite(LED_1_BASE, LED_1_PIN, toggle ? LED_1_PIN : 0 );
#else
    GPIOPinWrite(LED_GRN_BASE, LED_GRN_PIN, toggle ? LED_GRN_PIN : 0 );
#endif

    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);

}


void Timer1A_IntHandler(void) {

    static uint32_t timer1_cnt;
    static volatile uint32_t  toggle;

    // Clear the timer interrupt.
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    timer1_cnt++;

    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);

    //Process_ADC_EXT_AD7680();
    //Process_Link_encoder_read();

    if ( (timer1_cnt % 500) == 0 ) {
    	//lcd_test_sprint();
    	//Flush();
    }

    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);

}







