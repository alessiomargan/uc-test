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

volatile bool 	jumpToBsl = false;
uint32_t 		clks[6];

static inline void print_adc() {
    int i;
    UARTprintf("***************\n");
	for (i=0; i<16;i++) {
		UARTprintf("%d %d\n", i, conv_adc[adc_idx][i] );
	}
}

static inline void print_clks() {
    int i;
    UARTprintf("***************\n");
	for (i=0; i<6;i++) {
		UARTprintf("%d\n", clks[i] );
	}
}


static void clock_src(void) {

	uint32_t i;

	// set SMCLK = DCO / 2 ==> 24MHz
	//MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);

	clks[0] = CS_getACLK();		// Auxiliary clk
	clks[1] = CS_getMCLK();		// Master clk
	clks[2] = CS_getSMCLK();	// Low-speed subsystem master clk
	clks[3] = CS_getHSMCLK();   // Subsystem master clk
	clks[4] = CS_getBCLK();		// Low-speed backup domain clk
	clks[5] = CS_getDCOFrequency();
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
    clock_src();

    MAP_Interrupt_disableMaster();
    /*
     * Enabling the FPU with stacking enabled (for use within ISR)
     * */
    FPU_enableModule();
    FPU_enableLazyStacking();
    /*
     * Config periphs
     * */
    Configure_UART();
    Configure_GPIO();
    Configure_EcatPDI();
    Configure_Timer();
    Configure_ADC_temp();

    print_clks();

    /* Init soes */
    soes_init();

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
