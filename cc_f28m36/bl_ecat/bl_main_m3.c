//###########################################################################
// FILE:   blinky.c
// TITLE:  blinky example.
//###########################################################################
// $TI Release: F28M36x Support Library v207 $
// $Release Date: Mon Sep 21 16:44:39 CDT 2015 $
// $Copyright: Copyright (C) 2012-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################
#include <string.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/flash.h"
#include "driverlib/ipc.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/ram.h"

#include "utils/uartstdio.h"

#include <ethercat.h>
#include <soes/soes.h>

#include "pins.h"
#include "bl_config.h"
#include "shared_ram.h"
#include "peripherals.h"
#include "soes_hook.h"
#include "tiva-morser/morse.h"

#define DPRINT(...) UARTprintf("btl: "__VA_ARGS__)


#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern unsigned long RamfuncsLoadStart;
extern unsigned long RamfuncsRunStart;
extern unsigned long RamfuncsLoadSize;
#endif

extern void Configure_flashAPI(void);

//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}

#endif

//*****************************************************************************
//
//
//
//*****************************************************************************

m3_rw_data_t	m3_rw_data;
c28_rw_data_t	c28_ro_data;

// map to RAM S2
#pragma DATA_SECTION(m3_rw_data,"SHARERAMS2");
// map to RAM S0
#pragma DATA_SECTION(c28_ro_data,"SHARERAMS0");

struct morser m;
enum OUTPUT res;
bool sending;
char * morser_string = "boot";

#pragma CODE_SECTION(do_morse_led,"ramfuncs");
void do_morse_led(void) {

    static volatile uint32_t  led_status;

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
	   //if ( (timer0_cnt % 10) == 0 ) {
			// toggle
			sending = true;
			init_morser(&m, morser_string);
			//UARTprintf("init morser\n");
		//}
    }
    /////////////////////////////////////////////////////////////////

    GPIOPinWrite(LED_1_BASE, LED_1_PIN, led_status << 2 );

}

int main(void)
{
    volatile unsigned long ulLoop;

    // Disable Protection
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Sets up PLL, M3 running at 75MHz and C28 running at 150MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xF) |
                         SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_2 |
                         SYSCTL_XCLKDIV_4);

    // Copy time critical code and Flash setup code to RAM
    // This includes the following functions:  InitFlash();
    // The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
    // symbols are created by the linker. Refer to the device .cmd file.
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    FlashInit();

    // assign S0 and S1 of the shared ram for use by the c28
    // Details of how c28 uses these memory sections is defined
    // in the c28 linker file.(28M35H52C1_RAM_lnk.cmd)
    RAMMReqSharedMemAccess((S0_ACCESS | S1_ACCESS), SX_C28MASTER);

#ifdef _STANDALONE
    //  Send boot command to allow the C28 application to begin execution
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);
#endif

    //
    ConfigureUART();
    ConfigureLed();
    ConfigureEcatPDI();
    Configure_flashAPI();

	// Give C28 control of LED_0 Port E pin 7
    //GPIOPinConfigureCoreSelect(LED_0_BASE, LED_0_PIN, GPIO_PIN_C_CORE_SELECT);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG1);

    // Enable processor interrupts.
    IntMasterEnable();

    // ecat initialization
	soes_init();

	// only for test .. erase flash using SDO
	if ( GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) ) {
		if ( ! erase_prog_flash() ) {
			DPRINT("Fail erase flash\n");
		}
	}

	gFlash_crc = *(uint32_t*)(APP_CRC_ADDRESS);
	DPRINT("app _c_int00 0x%X\n", *(uint32_t*)(APP_START_ADDR));
	// jump to app if
	// - et1000 GPO3 is LOW
	// - app CRC is valid
	if ( test_jump_to_app() ) {
	   	jump_to_app();
	}

	while (1) {

		ulLoop ++;

		soes_loop();

		if ( ! (ulLoop % 100) ) {
			do_morse_led();
		}

		SysCtlDelay(SysCtlClockGet(SYSTEM_CLOCK_SPEED) / 10000);

	}
}




