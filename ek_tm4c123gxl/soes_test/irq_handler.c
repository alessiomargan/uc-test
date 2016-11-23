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
#include <driverlib/pin_map.h>
#include <driverlib/rom.h>
#include <driverlib/ssi.h>
#include <driverlib/timer.h>
#include <driverlib/pwm.h>
#include <driverlib/adc.h>

#include <ethercat.h>
#include <soes/soes.h>

#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"
//#include "pwm_ctrl.h"


volatile long ecat_irq_cnt = 0;
volatile long pwm_irq_cnt = 0;


/**
 * 
 *
 * @author amargan (7/4/2014)
 */
void GPIOBIntHandler(void) {

	GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
	GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);
	ecat_irq_cnt++;

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

    ecat_process_pdo();

    //UARTprintf("PDI irq %d\n", ecat_irq_cnt );
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
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
	soes_loop();


    // every 1000 cycles
    if ( (timer0_cnt % 1000) == 0 ) {
        // toggle 
        HWREGBITB(&toggle, 0) ^= 1;
        //ROM_IntMasterDisable();
        UARTprintf("\r tmr0: %d %d", timer0_cnt, toggle );
        //ROM_IntMasterEnable();
    }

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, toggle << 3 );
}

/**
 * 
 */
void PWM1_1IntHandler(void) {

    static int toggle;
    // Clear the PWM interrupt.  This is done twice since the clear will be
    // ignored by hardware if it occurs on the same cycle as another interrupt
    // event; the second clear takes care of the case wehre the first gets
    // ignored.
    PWMGenIntClear(PWM1_BASE, PWM_GEN_1, PWM_INT_CNT_ZERO);
    PWMGenIntClear(PWM1_BASE, PWM_GEN_1, PWM_INT_CNT_ZERO);

    pwm_irq_cnt++;

    if((PWMPulseWidthGet(PWM1_BASE, PWM_OUT_2) + 10) <=
       ((PWMGenPeriodGet(PWM1_BASE, PWM_GEN_1) * 3) / 4))
    {
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2,
                         PWMPulseWidthGet(PWM1_BASE, PWM_OUT_2) + 64);
    }
    else
    {
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 64);
    }
    //PWMUpdateDutyCycle();
    /*
    if (toggle) {
        LED_PWM_ON();
        toggle = 0;
    } else {
        LED_PWM_OFF();
        toggle = 1;
    }
    */

}

void ADC2IntHandler(void) {

    uint32_t    adc_buff[1];
    float fTemp;

    ADCIntClear(ADC0_BASE, ADC0_SEQ_NR);
    ADCSequenceDataGet(ADC0_BASE, ADC0_SEQ_NR, adc_buff);
    // (adc*vref)/ adc_12bit_resolution
    uint32_t ADCmV = (uint32_t)((adc_buff[0] * 3300) / 4096);
    fTemp = (float)(147500 - 75 * ADCmV) / 1000; //((1475 * 1023) - (2250 * adc_buff[0])) / 10230;

    tx_pdo.temperature = fTemp;

}








