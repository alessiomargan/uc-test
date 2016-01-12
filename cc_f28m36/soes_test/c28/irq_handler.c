#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "definitions.h"
#include "shared_ram.h"

extern m3_rw_data_t		m3_ro_data;
extern c28_rw_data_t	c28_rw_data;

__interrupt void cpu_timer0_isr(void)
{
	CpuTimer0.InterruptCount++;

	//Toggle debug pin
	if ( m3_ro_data.v_int32 % 2 ) {
		DEBUG_PIN_TOGGLE;
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

	LED_0_TOGGLE;

	// The CPU acknowledges the interrupt.
	EDIS;
}
