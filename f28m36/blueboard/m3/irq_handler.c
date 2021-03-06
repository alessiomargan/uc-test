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
#include <soes/hal/advr_esc/soes.h>
#include <soes_hook.h>

#include <pins.h>
#include <shared_ram.h>
#include <peripherals.h>

extern uint8_t ESC_SYNCactivation(void);

volatile uint16_t write_lcd = 0;
volatile long ecat_irq_cnt = 0;
volatile long pwm_irq_cnt = 0;

volatile unsigned long g_ulSysTickCount;
volatile unsigned long g_ulLastTick;

/*
 * 
 */
#pragma CODE_SECTION(Ecat_IntHandler,ramFuncSection);
void Ecat_IntHandler(void) {

	GPIOPinRead(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	GPIOPinIntClear(ECAT_GPIO_PORTBASE, ECAT_IRQ);
	ecat_irq_cnt++;
	//setDAC_B(255);
	ecat_process_pdo();
	//setDAC_B(0);
}

/*
 *
 */
#pragma CODE_SECTION(Hall_IntHandler,ramFuncSection);
void Hall_IntHandler(void) {

	m3_rw_data.hall_status = GPIOPinRead(HALL_BASE, HALL_PINS);
	GPIOPinIntClear(HALL_BASE, HALL_PINS);
}

/**fg
 * 1 KHz  1 ms
 */
#pragma CODE_SECTION(Timer0A_IntHandler,ramFuncSection);
void Timer0A_IntHandler(void) {

    static uint32_t timer0_cnt;
    static volatile uint32_t  toggle;

    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timer0_cnt++;

    setDAC_A(255);

    if ( ! ESC_SYNCactivation() ) {
    	ecat_process_pdo();
    }

    // every cycle
   	soes_loop();

   	setDAC_A(0);

    // every 1000 cycles
    if ( (timer0_cnt % 1000) == 0 ) {
        // toggle
        HWREGBITB(&toggle, 0) ^= 1;
        //DPRINT("\r tmr0: %d %d", timer0_cnt, toggle);
        DPRINT("\r %d %d", c28_ro_data.adc_cnt, m3_rw_data.hall_status);
    }

    GPIOPinWrite(LED_BASE, LED_RED_PIN, toggle ? LED_RED_PIN : 0 );
}


/**
 * 500 Hz  2 ms
 */
#pragma CODE_SECTION(Timer1A_IntHandler,ramFuncSection);
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





