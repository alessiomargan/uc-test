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

extern bool jumpToBsl;
extern uint8_t ESC_SYNCactivation(void);

/*
 * Ecat PDI irq
 */
void PORT5_IRQHandler(void) {

    uint32_t status;

    status = GPIO_getEnabledInterruptStatus(ECAT_GPIO_PORT);
    GPIO_clearInterruptFlag(ECAT_GPIO_PORT, status);

	ecat_irq_cnt++;

    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
    ecat_process_pdo();
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
}


/*
 *  GPIO ISR
 */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN1)
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
    }
    /* Toggling the output on the LED */
    if(status & GPIO_PIN4)
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        jumpToBsl = true;
    }
}


/**
 * 
 */
void T32_INT1_IRQHandler(void)
{
	static volatile uint32_t toggle = 0;
    // Clear the timer interrupt.
	Timer32_clearInterruptFlag(TIMER32_0_BASE);
    timer0_cnt++;

    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);

    soes_loop();

    if ( ! ESC_SYNCactivation() ) {
        ecat_process_pdo();
    }

    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);

    // every 1000 cycles
    if ( (timer0_cnt % 100) == 0 ) {
        // Toggle P1.0 output
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        DPRINT("\r tmr_cnt: %d ecat_irq_cnt: %d", timer0_cnt, ecat_irq_cnt );
    }

}










