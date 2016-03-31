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
#include "inc/hw_ram.h"
#include "inc/hw_ipc.h"
#include "driverlib/debug.h"
#include "driverlib/flash.h"
#include "driverlib/ipc.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/ram.h"
#include "driverlib/ipc.h"

#include "utils/uartstdio.h"

#include <ethercat.h>
#include <soes/soes.h>

#include "pins.h"
#include "shared_ram.h"
#include "peripherals.h"
#include "soes_hook.h"
#include "flash_utils.h"
#include "tiva-morser/morse.h"

#define DPRINT(...) UARTprintf("btl: "__VA_ARGS__)


#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern unsigned long RamfuncsLoadStart;
extern unsigned long RamfuncsRunStart;
extern unsigned long RamfuncsLoadSize;
#endif


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

// map to RAM S0
#pragma DATA_SECTION(m3_rw_data,"RAM_S0");
volatile m3_bl_rw_data_t	m3_rw_data;
// map to RAM S4
#pragma DATA_SECTION(c28_ro_data,"RAM_S4");
volatile c28_bl_rw_data_t	c28_ro_data;


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
    //
    ipc_c28_bits(led_status);
}

int main(void)
{
    volatile unsigned long 	ulLoop;
    bool test_M3, test_C28;

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

    // assign S0 and S1 of the shared ram for use by the M3
    RAMMReqSharedMemAccess((S0_ACCESS | S1_ACCESS), SX_M3MASTER);
    // assign S4 and S5 of the shared ram for use by the C28
    RAMMReqSharedMemAccess((S4_ACCESS | S5_ACCESS), SX_C28MASTER);

    // Initialize S0 S1 RAM
    HWREG(RAM_CONFIG_BASE + RAM_O_MSXRTESTINIT1) |= 0x5;
    while((HWREG(RAM_CONFIG_BASE + RAM_O_MSXRINITDONE1)&0x5) != 0x5)  {  }
    // MTOC_MSG_RAM Test and Initialization
    HWREG(RAM_CONFIG_BASE + RAM_O_MTOCCRTESTINIT1) |= 0x1;
    while((HWREG(RAM_CONFIG_BASE + RAM_O_MTOCRINITDONE)&0x1) != 0x1)  {  }

    //
    ConfigureUART();
    ConfigureLed();
    ConfigureEcatPDI();
    Configure_flashAPI();
    Configure_Ipc();

	// Give C28 control of LED_0 Port E pin 7
    GPIOPinConfigureCoreSelect(LED_0_BASE, LED_0_PIN, GPIO_PIN_C_CORE_SELECT);

#ifdef _STANDALONE
    //  Send boot command to allow the C28 application to begin execution
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);
    // Spin here until C28 bootloader is ready
    while (!IPCCtoMFlagBusy(IPC_FLAG17));
    IPCCtoMFlagAcknowledge(IPC_FLAG17);
#endif

    // only for test .. erase flash using SDO
	if ( GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) ) {
		//if ( ! erase_M3_app_flash() ) 	{ DPRINT("Fail erase M3 flash\n"); }
#ifdef _STANDALONE
		// needs c28 running !!!
		if ( ! erase_C28_app_flash() )	{ DPRINT("Fail erase C28 flash\n"); }
#endif
	}

	gFlash_crc = *(uint32_t*)(M3_APP_CRC_ADDR);
	DPRINT("app main 0x%X\n", *(uint32_t*)(M3_ENTRY_POINT_ADDR));
	DPRINT("c28 test_types 0x%02X 0x%04X 0x%04X%04X\n",
			c28_ro_data.test_type_uint16,
			c28_ro_data.test_type_uint32,
			(uint32_t)(c28_ro_data.test_type_uint64 >> 32),
			(uint32_t)(c28_ro_data.test_type_uint64) );
	/////////////////////////////////////////////////////////////
	// jump to app if
	// - et1000 GPO3 is LOW
	// - app CRC is valid
	test_M3 = test_jump_to_M3_app();
	test_C28 = test_jump_to_C28_app();

	if ( test_M3 && test_C28 ) {

#ifdef _STANDALONE
	   	jump_to_C28_app();
#endif
	   	jump_to_M3_app();
	   	// ..........
	}

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG1);

    // Enable processor interrupts.
    IntMasterEnable();

	/////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////

    // ecat initialization
	soes_init();

	while (1) {

		ulLoop ++;

		soes_loop();

		if ( ! (ulLoop % 100) ) {
			do_morse_led();
		}

		SysCtlDelay(SysCtlClockGet(SYSTEM_CLOCK_SPEED) / 10000);

	}
}




