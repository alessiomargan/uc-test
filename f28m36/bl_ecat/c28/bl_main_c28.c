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

#include "c28/include/definitions.h"
#include "common/include/shared_ram.h"

const char 	c28_firmware_version[16] = "c28_F28M3x_1.0"; // YYMMDDHH

#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
#endif

inline void jump_to_app(void) { DINT; asm("	LB 0x137FF0"); }

inline void Copy_string_to_M3(int16 *src, int16 *dst, uint16_t byte_length)  {

	int n = 0;
	while(*src && byte_length--) {
		__byte(dst,n) = *src;
		n++;
		src++;
	}
	__byte(dst,n) = *src;
}




void main(void)
{
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

	Configure_Pie_Vector();
	Configure_C28_Gpio();
	Configure_flashAPI();
	Configure_C28_Ipc();


	// Enable global Interrupts and higher priority real-time debug events:
	EINT;  // Enable Global interrupt INTM
	ERTM;  // Enable Global realtime interrupt DBGM

	c28_bl_rw_data.flash_crc = *(uint32_t*)(C28_APP_CRC_ADDR);
	c28_bl_rw_data.calc_crc = calc_app_crc();
	c28_bl_rw_data.test_type_uint32 = 0xdeadbeef;
	c28_bl_rw_data.test_type_uint16 = 0xcafe;
	c28_bl_rw_data.test_type_uint64 = 0xf1cadeadbeefdad0;
	Copy_string_to_M3((int16*)c28_firmware_version, (int16*)c28_bl_rw_data.bld_ver, 16);

	//Synchronize the two CPUs.
	IpcSync(IPC_FLAG17);


	for(;;) {
    	// loop
#ifdef CONTROL_CARD
		if (bootM3bits) { LED_0_ON; } else { LED_0_OFF; }
#else
		if (bootM3bits) { DEBUG_ORG_LED_ON; } else { DEBUG_ORG_LED_OFF; }
#endif
		if ( m3_bl_ro_data.boot_stage == 0xB007 ) { jump_to_app(); }
    }
}





