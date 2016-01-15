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

// -----------------------------------------------------------
// PIE Group 1 - MUXed into CPU INT1
// -----------------------------------------------------------

// INT1.4
__interrupt void xint1_isr(void)
{
    // Insert ISR Code here
    // To receive more interrupts from this PIE group, acknowledge this
    // interrupt
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;


}
// -----------------------------------------------------------
// PIE Group 3 - MUXed into CPU INT3
// -----------------------------------------------------------

// INT 3.1
__interrupt void epwm1_isr(void)     // EPWM-1
{
    // Insert ISR Code here
	// Clear INT flag for this timer
	EPwm1Regs.ETCLR.bit.INT = 1;
    // To receive more interrupts from this PIE group, acknowledge this interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}

