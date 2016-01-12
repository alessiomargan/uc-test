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

extern m3_rw_data_t		m3_rw_data;
extern c28_rw_data_t	c28_ro_data;

/**
 * 
 *
 * @author amargan (7/4/2014)
 */
void GPIOGIntHandler(void) {

	GPIOPinRead(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	GPIOPinIntClear(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	ecat_irq_cnt++;

    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

    ecat_process_pdo();

    //UARTprintf("PDI irq %d\n", ecat_irq_cnt );
    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
}

/**
 * 
 *
 * @author amargan (7/4/2014)
 */
void Timer0AIntHandler(void) {

    static uint32_t timer0_cnt;
    static volatile uint32_t  toggle;

    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    timer0_cnt++;

    read_sensors();

    if ( ! DC_activation() ) {
        ecat_process_pdo();
    }
    // every 10 cycles
    if ( (timer0_cnt % 10) == 0 ) {
        soes_loop();
    }

    m3_rw_data.v_int32++;

    // every 1000 cycles
    if ( (timer0_cnt % 1000) == 0 ) {
        // toggle 
        HWREGBITB(&toggle, 0) ^= 1;
        UARTprintf("\r tmr0: %d %d", timer0_cnt, toggle );
    }

    GPIOPinWrite(LED_1_BASE, LED_1_PIN, toggle << 2 );
}








