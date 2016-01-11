
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

__interrupt void cpu_timer0_isr(void)
{
	CpuTimer0.InterruptCount++;
	//Toggle pin 70 for confirmation of timer operation
	if(GpioG1DataRegs.GPCDAT.bit.GPIO70 == 0) {
		GpioG1DataRegs.GPCSET.bit.GPIO70 = 1;
	}else{
		GpioG1DataRegs.GPCCLEAR.bit.GPIO70 = 1;
	}
	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void cpu_timer1_isr(void)
{
	CpuTimer1.InterruptCount++;
	// The CPU acknowledges the interrupt.
	EDIS;
}

__interrupt void cpu_timer2_isr(void)
{
	EALLOW;
	CpuTimer2.InterruptCount++;
	// The CPU acknowledges the interrupt.
	EDIS;
}
