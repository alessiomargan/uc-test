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

#include <pins.h>
#include <shared_ram.h>
#include <peripherals.h>

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

m3_rw_data_t	m3_rw_data;
c28_rw_data_t	c28_ro_data;

// map to RAM S2
#pragma DATA_SECTION(m3_rw_data,"SHARERAMS2");
// map to RAM S0
#pragma DATA_SECTION(c28_ro_data,"SHARERAMS0");


int main(void)
{
    volatile unsigned long ulLoop;

    // Disable Protection
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Sets up PLL, M3 running at 75MHz and C28 running at 150MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xF) |
                         SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_2 |
                         SYSCTL_XCLKDIV_4);

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

    // assign S0 and S1 of the shared ram for use by the c28
    // Details of how c28 uses these memory sections is defined
    // in the c28 linker file.(28M35H52C1_RAM_lnk.cmd)
    RAMMReqSharedMemAccess((S0_ACCESS | S1_ACCESS), SX_C28MASTER);

#ifdef _STANDALONE
#ifdef _FLASH
    //  Send boot command to allow the C28 application to begin execution
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);
#else
    //  Send boot command to allow the C28 application to begin execution
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_RAM);
#endif
#endif

    // MWare/boards_drivers
    //PinoutSet();

    ConfigureUART();
    ConfigureLed();
    ConfigureEcatPDI();
    ConfigureTimer();

    // Enable Peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    // Give C28 Control of Port C
    GPIOPinConfigureCoreSelect(GPIO_PORTC_BASE, 0xFF,GPIO_PIN_C_CORE_SELECT);
    // Give C28 control of LED_0 Port E pin 7
    GPIOPinConfigureCoreSelect(LED_0_BASE, LED_0_PIN, GPIO_PIN_C_CORE_SELECT);

    // Disable clock supply for the watchdog modules
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG1);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG0);

    //The hardware priority mechanism will only look at the upper N bits of the priority level
    //where N is 3 for the Concerto family
    IntPrioritySet(INT_TIMER0A, 0x5);
    IntPrioritySet(INT_GPIOG,   0x5);

    // Enable processor interrupts.
    IntMasterEnable();

	soes_init();

	// Loop forever while the timers run.
    while (1) {
            // do nothing .. just loop
	}
}




