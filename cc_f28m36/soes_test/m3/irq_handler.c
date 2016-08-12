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

void M3_wr_shared() {

    m3_rw_data.v_int16++;
    m3_rw_data.v_int32++;
    m3_rw_data.v_uint64++;
    m3_rw_data.v_float = 0.12345f;

}

/**
 * 
 */
void GPIOGIntHandler(void) {

	GPIOPinRead(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	GPIOPinIntClear(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	ecat_irq_cnt++;

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7);

    ecat_process_pdo();

    //UARTprintf("PDI irq %d\n", ecat_irq_cnt );
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0);
}

/**
 * 
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

    // every cycle
	soes_loop();

    //
    M3_wr_shared();

    // every 1000 cycles
    if ( (timer0_cnt % 1000) == 0 ) {
        // toggle 
        HWREGBITB(&toggle, 0) ^= 1;
        UARTprintf("\r tmr0: %d %d %d", timer0_cnt, toggle, m3_rw_data.v_int32 );
    }

    GPIOPinWrite(LED_1_BASE, LED_1_PIN, toggle << 2 );
}








