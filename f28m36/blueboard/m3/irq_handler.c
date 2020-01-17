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

//#include <soes/esc.h>
//#include <soes/hal/advr_esc/soes.h>
//#include <soes_test/include/soes_hook.h>

#include <pins.h>
#include <shared_ram.h>
#include <peripherals.h>

extern uint8_t ESC_SYNCactivation(void);

volatile uint16_t write_lcd = 0;
volatile long ecat_irq_cnt = 0;
volatile long pwm_irq_cnt = 0;

volatile unsigned long g_ulSysTickCount;
volatile unsigned long g_ulLastTick;

/**
 * 
 */
void EcatIntHandler(void) {

	GPIOPinRead(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	GPIOPinIntClear(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	ecat_irq_cnt++;

    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, GPIO_PIN_7);
    //ecat_process_pdo();
    //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_7, 0);

    //UARTprintf("PDI irq %d\n", ecat_irq_cnt );
}

/**
 * 1 KHz  1 ms
 */
void Timer0A_IntHandler(void) {

    static uint32_t timer0_cnt;
    static volatile uint32_t  toggle;

    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timer0_cnt++;

    // every 1000 cycles
    if ( (timer0_cnt % 1000) == 0 ) {
        // toggle
        HWREGBITB(&toggle, 0) ^= 1;
        //UARTprintf("\r tmr0: %d %d", timer0_cnt, toggle);
    }

    GPIOPinWrite(LED_BASE, LED_RED_PIN, toggle ? LED_RED_PIN : 0 );
}


/**
 * 500 Hz  2 ms
 */
void Timer1A_IntHandler(void) {

    static uint32_t timer1_cnt;
    static volatile uint32_t  toggle;

    // Clear the timer interrupt.
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    timer1_cnt++;

    if ( (timer1_cnt % 1000) == 0 ) {
    	HWREGBITB(&toggle, 0) ^= 1;
    }

    GPIOPinWrite(LED_BASE, LED_GRN_PIN, toggle ? LED_GRN_PIN : 0 );
}


void SysTick_IntHandler(void)
{
    // Update our tick counter.
    g_ulSysTickCount++;
}





