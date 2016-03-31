#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "F28M36x_Ipc_drivers.h"

#include "c28/include/definitions.h"
#include "common/include/shared_ram.h"



void Configure_Pie_Vector(void)
{
	EALLOW;  // This is needed to write to EALLOW protected registers

    PieVectTable.MTOCIPC_INT1 = &MtoC_ipc1_isr;
    PieVectTable.MTOCIPC_INT2 = &MtoC_ipc2_isr;

	EDIS;    // This is needed to disable write to EALLOW protected registers

}

void Configure_C28_Ipc(void)
{
	IPCCInitialize (&g_sIpcController1, IPC_INT1, IPC_INT1);
	IPCCInitialize (&g_sIpcController2, IPC_INT2, IPC_INT2);

	// Enable CPU INT1 which is connected to MTOCIPC INT1-4:
	IER |= M_INT11;

	PieCtrlRegs.PIEIER11.bit.INTx1 = 1; // MTOCIPC INT1
	PieCtrlRegs.PIEIER11.bit.INTx2 = 1; // MTOCIPC INT2


	// Point array to address in CTOM MSGRAM for passing variable locations
	pulMsgRam = (void *)0x0003FBF4;
	// Write addresses of variables where words should be written to pulMsgRam array.
	// 0 = Bootloader Service function.
	// 1 = Address of 16-bit Bootloader Service result executed
	// 2
	pulMsgRam[0] = (unsigned long)&bootloaderService;
	pulMsgRam[1] = (unsigned long)&bootloaderServiceResult;
	pulMsgRam[2] = (unsigned long)&bootM3bits;

}


void Configure_C28_Gpio(void)
{
	EALLOW;

	// control card led_0
	GpioG1CtrlRegs.GPADIR.bit.GPIO31 = 1;
    // debug pin
	GpioG1CtrlRegs.GPCDIR.bit.GPIO70 = 1;

    EDIS;

	LED_0_OFF;// turn off LED
	DEBUG_PIN_OFF;
}


