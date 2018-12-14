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
#include "globals.h"
#include "peripherals.h"

static volatile uint32_t timer0_cnt = 0;
static volatile long ecat_irq_cnt = 0;

extern uint8_t ESC_SYNCactivation(void);

/*
 * let's put irq handlers in RAM
 * */

__attribute__((ramfunc))
void PORT5_IRQHandler(void);
__attribute__((ramfunc))
void PORT1_IRQHandler(void);
__attribute__((ramfunc))
void T32_INT1_IRQHandler(void);
__attribute__((ramfunc))
void ADC14_IRQHandler(void);

/*
 * TODO calc exec time
 *
 */

inline void avg_samples(int chs) {

	int ch, sm;
	uint32_t temp;

	for ( ch=0; ch<chs; ch++ ) {
		temp = 0;
#pragma UNROLL(SMPL_NUM)
		for ( sm=0; sm<SMPL_NUM; sm++ ) {
			temp += raw_adc[sm][ch];
		}
		// divide by 8 using shift ==>  >>3
		conv_adc[ch] = (temp>>3)*VTICK;
	}

}


/*
 * Ecat PDI irq
 * */
void PORT5_IRQHandler(void) {

    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(PORT_ECAT_GPIO);
    MAP_GPIO_clearInterruptFlag(PORT_ECAT_GPIO, status);

    if(status & PIN_ECAT_IRQ) {
		ecat_irq_cnt++;
		//MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
		P3->OUT |= BIT6;
		ecat_process_pdo();
		//MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
		P3->OUT &= ~BIT6;
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
    	jump_to_bootloader();
    }
}

/*
 * 
 **/
void T32_INT1_IRQHandler(void)
{
	static volatile uint32_t toggle = 0;
	//int16_t conRes;

    // Clear the timer interrupt.
	MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);
    timer0_cnt++;

    //MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN0);
    P6->OUT |= BIT0;

    //conRes = ((conv_adc[adc_sample_idx][0] - cal30) * 55);
    //tempC = (conRes / calDifference) + 30.0f;

	MAP_Interrupt_disableMaster();
	avg_samples(9);
	MAP_Interrupt_enableMaster();

    soes_loop();

    if ( ! ESC_SYNCactivation() ) {
        ecat_process_pdo();
    }

    //MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0);
    P6->OUT &= ~BIT0;

    // every 1000 cycles
    if ( (timer0_cnt % 1000) == 0 ) {
        // Toggle P1.0 output
    	//MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    	P1->OUT ^= BIT0;
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
	static uint16_t	adc_sample_idx;
	uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if(status & ADC_INT8)
    //if(status & ADC_INT16)
    {
    	MAP_ADC14_disableConversion();
    	//BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ENC_OFS) = 0;
    	P6->OUT |= BIT1;
		MAP_ADC14_getMultiSequenceResult(raw_adc[adc_sample_idx++]);
        P6->OUT &= ~BIT1;
        MAP_ADC14_enableConversion();
        //BITBAND_PERI(ADC14->CTL0, ADC14_CTL0_ENC_OFS) = 1;

    	if ( adc_sample_idx >= SMPL_NUM ) {
    		adc_sample_idx = 0;
		}

    }

}






