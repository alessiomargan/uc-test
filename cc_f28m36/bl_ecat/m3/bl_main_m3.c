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
#include "user_flash.h"
#include "bl_ipc.h"
#include "tiva-morser/morse.h"

#define DPRINT(...) UARTprintf("btl: "__VA_ARGS__)
//#define DPRINT(...)


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


m3_bl_rw_data_t		m3_bl_rw_data;
c28_bl_rw_data_t	c28_bl_ro_data;

// map to RAM S0 owned by m3
#pragma DATA_SECTION(m3_bl_rw_data,"RAM_S0");
// map to RAM S4 owned by c28
#pragma DATA_SECTION(c28_bl_ro_data,"RAM_S4");

#pragma CODE_SECTION(do_morse_led,"ramfuncs");
void do_morse_led(void);

static void JumpStackPointerToAddress(uint32 address)
{
	__asm(" mov sp, r0");   // sp is now *address
    __asm(" bx r0" );       // jump to *address
}
static void jump_to_M3_app(void) {

	IntMasterDisable(); // disable interrupts
	HWREG(NVIC_VTABLE) = 0x002C1000; // redirect the vector table
	JumpStackPointerToAddress(0x002C0001); // jump to startup code of application

}

/**
 * return true if :
 * - crc is valid
 * 	 AND
 * - et1000 boot pin is LOW
 */
static bool test_jump_to_M3_app(void)
{
	uint32_t app_crc = calc_app_crc();
	//et1100_boot_pin = ESC_GPO_LL() & 0x0004;
	gET1100_boot_pin = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
	gCrc_ok = ( (app_crc == gFlash_crc) && (gFlash_crc != 0xFFFFFFFF) );
	OSAL_PRINT ("m3Crc_ok %d : calc 0x%X  flash 0x%X\n", gCrc_ok, app_crc, gFlash_crc);
	return ( ! gET1100_boot_pin && gCrc_ok);
}

static bool test_jump_to_C28_app(void)
{
	uint32_t c28_crc_ok;
	gET1100_boot_pin = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
	c28_crc_ok = ( (c28_bl_ro_data.flash_crc == c28_bl_ro_data.calc_crc) &&
				   (c28_bl_ro_data.flash_crc != 0xFFFFFFFF) );
	OSAL_PRINT ("c28Crc_ok %d : calc 0x%X  flash 0x%X\n",
			c28_crc_ok, c28_bl_ro_data.calc_crc, c28_bl_ro_data.flash_crc);
	return ( ! gET1100_boot_pin && c28_crc_ok);
}

//*****************************************************************************
//
//
//
//*****************************************************************************


int main(void)
{
	volatile unsigned long 	ulLoop;
    bool test_M3, test_C28;

    // Disable Protection
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    // Sets up PLL, M3 running at 75MHz and C28 running at 150MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xF) |
                         SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_2 |
                         SYSCTL_XCLKDIV_4);

    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG0);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG1);

#ifdef _FLASH
    // Copy time critical code and Flash setup code to RAM
    // This includes the following functions:  InitFlash();
    // The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
    // symbols are created by the linker. Refer to the device .cmd file.
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    FlashInit();
#endif

    // assign S0 of the shared ram for use by the M3
    RAMMReqSharedMemAccess((S0_ACCESS), SX_M3MASTER);
    // assign S4 of the shared ram for use by the C28
    RAMMReqSharedMemAccess((S4_ACCESS), SX_C28MASTER);

    //
    ConfigureUART();
    ConfigureLed();
    ConfigureEcatPDI();
    Configure_flashAPI();
    Configure_Ipc();

	// Give C28 control of LED_0 Port E pin 7
    GPIOPinConfigureCoreSelect(LED_0_BASE, LED_0_PIN, GPIO_PIN_C_CORE_SELECT);

#ifdef _BOOT_C28
    //  Send boot command to allow the C28 application to begin execution
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);
    //Synchronize the two CPUs.
    IpcSync(IPC_FLAG17);
#endif

    // only for test .. erase flash using SDO
	if ( GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) ) {
		if ( ! erase_M3_app_flash() ) 	{ DPRINT("Fail erase M3 flash\n"); }
#ifdef _BOOT_C28
		// needs c28 running !!!
		if ( ! erase_C28_app_flash() )	{ DPRINT("Fail erase C28 flash\n"); }
#endif
	}

	gFlash_crc = *(uint32_t*)(M3_APP_CRC_ADDR);
	DPRINT("c28 test_types 0x%02X 0x%04X 0x%04X%04X\n",
			c28_bl_ro_data.test_type_uint16,
			c28_bl_ro_data.test_type_uint32,
			(uint32_t)(c28_bl_ro_data.test_type_uint64 >> 32),
			(uint32_t)(c28_bl_ro_data.test_type_uint64) );
	/////////////////////////////////////////////////////////////
	// jump to app if
	// - et1000 GPO3 is LOW
	// - app CRC is valid
	test_M3 = test_jump_to_M3_app();
	test_C28 = test_jump_to_C28_app();

	if ( test_M3 && test_C28 ) {

		// tell c28 to jump to app
		m3_bl_rw_data.boot_stage = 0xB007;

	   	jump_to_M3_app();
	}

	/////////////////////////////////////////////////////////////////
    // Bootloader application
    /////////////////////////////////////////////////////////////////

    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

    // Enable processor interrupts.
    IntMasterEnable();

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

    GPIOPinWrite(LED_1_BASE, LED_1_PIN, led_status << 2 );
    //
    ipc_c28_bits(led_status);
}




