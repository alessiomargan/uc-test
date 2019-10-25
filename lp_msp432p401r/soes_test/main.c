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
#include <cc.h>

#include "pins.h"
#include "globals.h"
#include "peripherals.h"
#include "params.h"

// should go in globals.h
// TODO put in globals_priv.h
extern Timer_A_PWMConfig pwmConfig;

volatile bool 	jumpToBsl = false;
uint32_t 		clks[6];

static inline void print_adc() {
    int i;
    DPRINT("***************\n");
	for (i=0; i<6;i++) {
		DPRINT("%d %d\n", i, conv_adc[i] );
	}
}

static inline void print_clks() {
    int i;
    DPRINT("***************\n");
	for (i=0; i<6;i++) {
		DPRINT("%d\n", clks[i] );
	}
}


static void clock_src(void) {

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
    // Halting the Watchdog and disable IRQs
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    // Set the core voltage level to VCORE1
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    // Before we set the DCO, transition the device to use DCDC converter
    MAP_PCM_setPowerState(PCM_AM_DCDC_VCORE1);

    // At 48MHz in VCORE0, MSP432P401R needs 1 wait states
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);

    // Enabling the FPU with stacking enabled (for use within ISR)
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();

    // Set DCO to 48MHz
    MAP_CS_setDCOFrequency(CS_48MHZ);

    /* Initializes Clock System */
	//MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
	MAP_CS_initClockSignal(CS_MCLK,   CS_DCOCLK_SELECT,  CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT,  CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK,  CS_DCOCLK_SELECT,  CS_CLOCK_DIVIDER_2);
	MAP_CS_initClockSignal(CS_ACLK,   CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

	/*
	 *
	 */
    Configure_UART();

    if ( Read_Flash_Params() == PARAMS_CMD_ERROR) {
		//
    	glob_fault.bit.warn_read_flash = 1;
		DPRINT("Read_Flash_Params FAIL\n");
	    if ( Load_Default_Params() == PARAMS_CMD_ERROR) {
	    	// FATAL EROR
	    	while (1) {}
 	    }
	}

    DPRINT("sdo.ram.fw_ver=%s\n", sdo.ram.fw_ver);
    DPRINT("FLASH_SDO\n");
    print_sdo(&flash_sdo);
    DPRINT("DFLT_FLASH_SDO\n");
    print_sdo(&dflt_flash_sdo);
    DPRINT("SDO\n");
    print_sdo(&sdo.flash);

    /*
     * Config periphs
     */
    Configure_GPIO();
    Configure_EcatPDI();
    Configure_DAC();
    Configure_Timer();
    //Configure_ADC_temp();
    Configure_ADC();

    /*
     * Init soes
     */
    soes_init();

    MAP_Interrupt_setPriority(INT_PORT5,    (char)(2)<<5);
    MAP_Interrupt_setPriority(INT_T32_INT1, (char)(2)<<5);
    MAP_Interrupt_setPriority(INT_ADC14, 	(char)(1)<<5);
    MAP_Interrupt_enableMaster();

    while(1)
    {
        //print_adc();

    }
}
