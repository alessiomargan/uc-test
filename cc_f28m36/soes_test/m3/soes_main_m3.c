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
#include "inc/hw_ram.h"
#include "inc/hw_ipc.h"
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
#include <flash_utils.h>
#ifdef _LCD
#include <dog_LCD/DogLcd_test.h>
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

const char fw_ver[8] = "xXx/\\xXx";

m3_to_c28_data_t	m3_rw_data;
c28_to_m3_data_t	c28_ro_data;

// map to RAM S1
#pragma DATA_SECTION(m3_rw_data,"RAM_S1");
// map to RAM S5
#pragma DATA_SECTION(c28_ro_data,"RAM_S5");

int main(void)
{
    volatile unsigned long ulLoop;

    // Disable Protection
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Sets up PLL, M3 running at 75MHz and C28 running at 150MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xF) |
                         SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_2 |
                         SYSCTL_XCLKDIV_4);
/*
    // Sets up PLL, M3 running at 125MHz and C28 running at 125MHz
    SysCtlClockConfigSet(SYSCTL_USE_PLL | (SYSCTL_SPLLIMULT_M & 0xC) | (SYSCTL_SPLLFMULT_M & 0x2) |
                       	 SYSCTL_SYSDIV_1 | SYSCTL_M3SSDIV_1 |
                         SYSCTL_XCLKDIV_4);
*/
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG0);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_WDOG1);

#ifdef _FLASH
    // .binit copy table do the job ....
    // Copy time critical code and Flash setup code to RAM

    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    FlashInit();
    Test_EraseWrite_flash(0x002F0000); // FLASH_B
#endif

    // assign S1 of the shared ram for use by the M3
    RAMMReqSharedMemAccess((S1_ACCESS), SX_M3MASTER);
    // assign S5 of the shared ram for use by the C28
    RAMMReqSharedMemAccess((S5_ACCESS), SX_C28MASTER);

    //
    Configure_UART();
    Configure_Led();
#ifdef _LCD
    Configure_LCD();
#endif
    Configure_EcatPDI();
    // ecat timer
	Configure_Timer_0A();
	// sensor & lcd timer
	Configure_Timer_1A();

	//Configure_Link_Enc_BissC();
    //Configure_AD7680();

#ifdef _CONTROL_CARD
    // Enable C28 Peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // ePWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH); //
    // Give C28 Control of Port C
	GPIOPinConfigureCoreSelect(GPIO_PORTA_BASE, 0xFF,GPIO_PIN_C_CORE_SELECT);
	GPIOPinConfigureCoreSelect(GPIO_PORTC_BASE, 0xFF,GPIO_PIN_C_CORE_SELECT);
	GPIOPinConfigureCoreSelect(GPIO_PORTH_BASE, 0xFF,GPIO_PIN_C_CORE_SELECT);
	// Give C28 control of LED_0 Port E pin 7
    GPIOPinConfigureCoreSelect(LED_0_BASE, LED_0_PIN, GPIO_PIN_C_CORE_SELECT);
#else
    // Enable C28 Peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // ePWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC); //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH); //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOR); //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOS); //
     // Give C28 Control of Port A C H
 	GPIOPinConfigureCoreSelect(GPIO_PORTA_BASE, 0xFF,GPIO_PIN_C_CORE_SELECT);
 	GPIOPinConfigureCoreSelect(GPIO_PORTC_BASE, 0xFF,GPIO_PIN_C_CORE_SELECT);
 	GPIOPinConfigureCoreSelect(GPIO_PORTH_BASE, 0xFF,GPIO_PIN_C_CORE_SELECT);
	GPIOPinConfigureCoreSelect(GPIO_PORTR_BASE, GPIO_PIN_6,GPIO_PIN_C_CORE_SELECT); // DBG LED YLW
	GPIOPinConfigureCoreSelect(GPIO_PORTS_BASE, GPIO_PIN_0,GPIO_PIN_C_CORE_SELECT); // DBG LED ORG
#endif


#ifdef _BOOT_C28
    //  Send boot command to allow the C28 application to begin execution
    IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);
    //IpcSync(IPC_FLAG18);
    //	Wait for CTOM IPC Flag
    while((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & IPC_FLAG18) == 0) {}
#endif

    //The hardware priority mechanism will only look at the upper N bits of the priority level
    //where N is 3 for the Concerto family
    IntPrioritySet(INT_TIMER0A, (char)(3)<<5); // middle prio soes loop
    IntPrioritySet(INT_TIMER1A, (char)(2)<<5); // higher prio sensor
#ifdef _CONTROL_CARD
    IntPrioritySet(INT_GPIOG,   (char)(3)<<5); // middle prio pdi ecat irq
#else
    IntPrioritySet(INT_GPIOK,   (char)(3)<<5); // middle prio pdi ecat irq
#endif

    // Enable processor interrupts.
    IntMasterEnable();

    /////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////

	// ecat initialization
	soes_init();

#ifdef _LCD
	lcd_test_2d();
	lcd_test_char();
	lcd_test_string();
	Flush();
#endif
	// Loop forever while the timers run.
	ulLoop = 0;
    while (1) {

#ifdef _LCD
    	lcd_test_sprint();
    	Flush();
#endif
    	// 500 msec ?!?
        SysCtlDelay( (SysCtlClockGet(SYSTEM_CLOCK_SPEED) / 1000) * 500 );
        //SysCtlDelay( 1000 );

	}
}




