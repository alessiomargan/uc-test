/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <soes/esc.h>
#include <soes/hal/advr_esc/soes.h>
#include <soes_hook.h>

#include "pins.h"
#include "peripherals.h"

volatile uint32_t timer0_cnt = 0;
volatile uint32_t timer1_cnt = 0;
volatile long ecat_irq_cnt = 0;
volatile long pwm_irq_cnt = 0;

extern bool 	jumpToBsl;
extern uint32_t cal30;
extern uint32_t cal85;
extern float 	calDifference;
extern float 	tempC;

extern uint16_t adc_idx;
extern uint16_t conv_adc[1024][32];

extern uint8_t ESC_SYNCactivation(void);

/*
 * let's put irq handlers in RAM
 * */
/*
__attribute__((ramfunc))
void PORT5_IRQHandler(void);
__attribute__((ramfunc))
void PORT1_IRQHandler(void);
__attribute__((ramfunc))
void T32_INT1_IRQHandler(void);
__attribute__((ramfunc))
void ADC14_IRQHandler(void);
__attribute__((ramfunc))
void TA0_N_IRQHandler(void);
*/


/*
 * Ecat PDI irq
 * */
void PORT5_IRQHandler(void) {

    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(ECAT_GPIO_PORT);
    MAP_GPIO_clearInterruptFlag(ECAT_GPIO_PORT, status);

    if(status & ECAT_IRQ_PIN) {
		ecat_irq_cnt++;
		MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
		ecat_process_pdo();
		MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
    }
}

/*
 * GPIO ISR
 * */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if(status & GPIO_PIN1) {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
    } else if(status & GPIO_PIN4) {
    	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        jumpToBsl = true;
    }
}

/*
 * 
 **/
void T32_INT1_IRQHandler(void)
{
	static volatile uint32_t toggle = 0;
	int16_t conRes;

    // Clear the timer interrupt.
	MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);
    timer0_cnt++;

    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN0);

    conRes = ((conv_adc[adc_idx][0] - cal30) * 55);
    tempC = (conRes / calDifference) + 30.0f;

    soes_loop();

    if ( ! ESC_SYNCactivation() ) {
        ecat_process_pdo();
    }

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0);

    // every 1000 cycles
    if ( (timer0_cnt % 100) == 0 ) {
        // Toggle P1.0 output
    	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        //DPRINT("\r tmr_cnt: %d ecat_irq_cnt: %d", timer0_cnt, ecat_irq_cnt );
    }

}

/*
 * This interrupt happens every time a conversion has completed. Since the FPU
 * is enabled in stacking mode, we are able to use the FPU safely to perform
 * efficient floating point arithmetic.
 * */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);


    if(status & ADC_INT16)
    {
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1);
        MAP_ADC14_getMultiSequenceResult(conv_adc[adc_idx]);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN1);
    }


}

void TA0_N_IRQHandler(void) {

	MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_1);
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P6, GPIO_PIN1);

}





