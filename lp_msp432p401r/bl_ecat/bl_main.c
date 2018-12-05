/******************************************************************************
 * MSP432 Project
 *
 * -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0
 * --define=_DEBUG --define=FOE_READ_SUPPORTED --define=ESC_DEBUG --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x --define=ccs
 * -g --diag_warning=225 --diag_warning=255
 * --include_path="/opt/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.3.LTS/include"
 * --include_path="/opt/ti/simplelink_msp432p4_sdk_2_30_00_14/source"
 * --include_path="/opt/ti/simplelink_msp432p4_sdk_2_30_00_14/source/third_party/CMSIS/Include"
 * --include_path="/home/amargan/work/code/firmware/SOES"
 * --include_path="/home/amargan/work/code/firmware/SOES/soes/include/sys/ccs"
 * --include_path="/home/amargan/work/code/firmware/uc_test/common_src"
 * --include_path="/home/amargan/work/code/firmware/uc_test/lp_msp432p401r/bl_ecat/include"
 *
 * -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0
 * --define=_DEBUG --define=FOE_READ_SUPPORTED --define=ESC_DEBUG --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x --define=ccs
 * -g --diag_warning=225 --diag_warning=255
 * -z -m"bl_msp432.map" --heap_size=1024 --stack_size=1024
 * -i"/opt/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.3.LTS/include"
 * -i"/opt/ti/ccsv7/tools/compiler/ti-cgt-arm_18.1.3.LTS/lib"
 * -i"/opt/ti/simplelink_msp432p4_sdk_2_30_00_14/source"
 * --warn_sections --xml_link_info="bl_msp432_linkInfo.xml" --trampolines=off --rom_model
 *
 */


/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "pins.h"
#include "soes/esc.h"

#include "peripherals.h"
#include "foe_flash.h"

#include "tiva-morser/morse.h"

extern uint32_t	gCalc_crc;
extern uint16_t crc_ok;

extern void soes_init(void);
extern void soes_loop(void);
extern void ecat_process_pdo(void);
extern void do_morse_led(void);

void do_morse_led(void);

void jump2app(void) {

	MAP_Interrupt_disableMaster(); // disable interrupts
	// redirect the vector table
    // initialize the vector table offset
    SCB->VTOR = FLASH_APP_START;

	// Load the stack pointer from the application's vector table.
	__asm(" ldr r1, [r0, #0]\n"
	      " mov sp, r1\n");
	// Load the initial PC from the application's vector table and branch to
	// the application's entry point.
	__asm(" ldr r0, [r0, #4]\n"
	      " blx r0\n");
}

static uint8_t test_jump2app(void) {

	// poll switch ... 0 pressed
	uint8_t sw1 = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);
	bool ret = sw1 && crc_ok;
	//DPRINT("%s : %d %d\n", __FUNCTION__, sw1, sw2);

	return (ret);
}

static void clock_src(void) {

	uint32_t i, clks[6];

	//MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

	clks[0] = MAP_CS_getACLK();		// Auxiliary clk
	clks[1] = MAP_CS_getMCLK();		// Master clk
	clks[2] = MAP_CS_getSMCLK();	// Low-speed subsystem master clk
	clks[3] = MAP_CS_getHSMCLK();   // Subsystem master clk
	clks[4] = MAP_CS_getBCLK();		// Low-speed backup domain clk
	clks[5] = MAP_CS_getDCOFrequency();
	for (i=0; i<6;i++) {
		DPRINT("%d\n", clks[i] );
	}

}

void main(uint32_t bslParams)
{
	int i = 0;
    volatile uint32_t	loop_cnt;
    volatile uint8_t	test_jump = 0;

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
	 * Config periphs
	 */
	Configure_UART();
	clock_src();
    Configure_GPIO();
    Configure_EcatPDI();
    Configure_Switch();

    gCalc_crc = calc_CRC(FLASH_APP_START, FLASH_APP_SIZE);
    crc_ok = (gCalc_crc==CRC_App);
    DPRINT("bldr ver %s\n", BLDR_Version);
    DPRINT("CRC : calc 0x%04X flash 0x%04X\n", gCalc_crc, CRC_App);

    test_jump = test_jump2app();
    if ( test_jump ) {
    	DPRINT(">>>\n");
    	jump2app();
    }

    /*
     * Init soes
     */
    soes_init();

    while(1)
    {
        soes_loop();

        if ( ! (loop_cnt++ % 100) ) {
			do_morse_led();
        }

        // Delay .. 48 Mhz clock --> 1ms
        for(i=2000; i>0; i--);

    }

}

void do_morse_led(void) {

    static volatile uint32_t  led_status;
    static char * morser_string = "boot";
    static bool sending;
    static struct morser m;
    enum OUTPUT res;

    if (sending) {
        res = tick(&m);
        switch(res) {
          case HIGH:
        	  led_status = 1;
			  break;
          case LOW:
        	  led_status = 0;
			  break;
          case END:
        	  led_status = 0;
			  sending = false;
            break;
        }
    } else {
		sending = true;
		init_morser(&m, morser_string);
    }
    /////////////////////////////////////////////////////////////////

    if ( led_status ) {
    	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
    } else {
    	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
    }
}

