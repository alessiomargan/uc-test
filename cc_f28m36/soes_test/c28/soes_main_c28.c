//###########################################################################
// FILE:   blinky_c28.c
// TITLE:  Blinky Example for F28M36x.
//
// This Dual Core Blinky Example demonstrates how to run and
// implement a standalone application on both cores.  Due to an errata in
// the bootROM this example may not run correctly with the debugger connected.
// To run the example, program both cores with their respective project and
// then disconnect the debugger.  Set SW3 switch 1 to the down position
// (disconnect TRSTn) as well as setting all 4 switches on SW1 to the down
// position.  Cycle power and both LEDs should begin to blink.
//
//###########################################################################
// $TI Release: F28M36x Support Library v207 $
// $Release Date: Mon Sep 21 16:44:39 CDT 2015 $
// $Copyright: Copyright (C) 2012-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include <string.h>


#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
#endif

extern void Configure_C28_Timer();

void main(void)
{

   unsigned long delay;
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F28M36x_SysCtrl.c file.
    InitSysCtrl();

// Step 2. Initialize GPIO:
// This example function is found in the F28M36x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
    InitGpio(); // Skipped for this example

    EALLOW;
    LED_0_DIR_REG = 1;
    GpioCtrlRegs.GPCDIR.bit.GPIO70 = 1;
    EDIS;
    LED_0_DAT_REG = 1;// turn off LED

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
    DINT;

#ifdef _FLASH
// Copy time critical code and Flash setup code to RAM
// This includes the following functions: InitFlash();
// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
// symbols are created by the linker. Refer to the device .cmd file.
   memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

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

    Configure_C28_Timer();

// Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

// Step 6. IDLE loop. Just sit and loop forever (optional):
    for(;;)
    {
        //
        // Turn on LED
        //
    	LED_0_DAT_REG = 0;
        //
        // Delay for a bit.
        //
        for(delay = 0; delay < 2000000; delay++)
        {
        }

        //
        // Turn off LED
        //
        LED_0_DAT_REG = 1;
        //
        // Delay for a bit.
        //
        for(delay = 0; delay < 2000000; delay++)
        {
        }
    }
}





