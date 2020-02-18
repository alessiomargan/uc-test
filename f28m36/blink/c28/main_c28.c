#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#include <string.h>

#include <flash_utils.h>

#include "c28/include/definitions.h"
#include "common/include/shared_ram.h"

void Configure_C28_Gpio(void)
{
	EALLOW;
	GpioG1CtrlRegs.GPCDIR.bit.GPIO86 = 1;
	GpioG1CtrlRegs.GPCDIR.bit.GPIO87 = 1;
	EDIS;
	GpioG1CtrlRegs.GPCMUX2.bit.GPIO86 = 0;  // GPIO86 = GPIO			--> DEBUG YELLOW LED
	GpioG1CtrlRegs.GPCMUX2.bit.GPIO87 = 0;  // GPIO87 = GPIO			--> DEBUG ORANGE LED
	YLW_LED_OFF;
	ORG_LED_OFF;
}

void main(void)
{
	unsigned long delay;
	volatile unsigned long ulLoop;

	// Step 1. Initialize System Control:
	// PLL, WatchDog, enable Peripheral Clocks
	// This example function is found in the F28M36x_SysCtrl.c file.
	InitSysCtrl();

	// Step 2. Initialize GPIO:
	// This example function is found in the F28M36x_Gpio.c file and
	// illustrates how to set the GPIO to it's default state.
	InitGpio();

	// Step 3. Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
	DINT;

#ifdef _FLASH
	// .binit section copy table in RAM
	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
	InitFlash();
#endif

	// Initialize the PIE control registers to their default state.
	// The default state is all PIE interrupts disabled and flags
	// are cleared.
	// This function is found in the F28M36x_PieCtrl.c file.
	InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in F28M36x_DefaultIsr.c.
	// This function is found in F28M36x_PieVect.c.
	InitPieVectTable();

	Configure_C28_Gpio();
#ifdef _FLASH
	Configure_flashAPI();
	Test_EraseWrite_flash(0x138000); // FLASH_D
#endif

	// Enable global Interrupts and higher priority real-time debug events:
	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM

	//IpcSync(IPC_FLAG18);
	//	Set CTOM IPC Flag
	CtoMIpcRegs.CTOMIPCSET.bit.IPC18 = 1;

	for(;;) {
		// loop
		for(delay = 0; delay < 2000000; delay++) { }
		YLW_LED_TGL
	}
}





