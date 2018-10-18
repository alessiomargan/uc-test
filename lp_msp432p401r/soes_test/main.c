/******************************************************************************
 * MSP432 Project
 *
 * Description:
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "pins.h"
#include "peripherals.h"

extern uint16_t adc_idx;
extern uint16_t conv_adc[1024][32];

extern void soes_init(void);

volatile bool jumpToBsl = false;

static void print_adc() {
    int i;
    UARTprintf("***************\n");
	for (i=0; i<16;i++) {
		UARTprintf("%d %d\n", i, conv_adc[adc_idx][i] );

	}
}


int main(void)
{
    int i = 0;

    /*
     * see SystemInit ....
     *
     *  #define __HALT_WDT       1
   	 *  #define __SYSTEM_CLOCK   48000000
   	 *  #define __REGULATOR      1
     *
     * */
    MAP_Interrupt_disableMaster();
    /*
     * Enabling the FPU with stacking enabled (for use within ISR)
     * */
    FPU_enableModule();
    FPU_enableLazyStacking();
    /*
     * Config periphs
     * */
    Configure_GPIO();
    Configure_UART();
    Configure_EcatPDI();
    Configure_Timer();
    Configure_ADC_temp();
    /*
     * Init soes
     * */
    soes_init();
    /*
     * */
    MAP_Interrupt_setPriority(INT_PORT5,    (char)(2)<<5);
    MAP_Interrupt_setPriority(INT_T32_INT1, (char)(2)<<5);
    MAP_Interrupt_setPriority(INT_ADC14, 	(char)(1)<<5);
    MAP_Interrupt_enableMaster();

    while(1)
    {
        // Delay
        for(i=100000; i>0; i--);

        if ( jumpToBsl ) {
        	jump_to_bootloader();
        }

        print_adc();


    }
}
