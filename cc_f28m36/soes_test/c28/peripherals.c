#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "definitions.h"

#define C28_FREQ    150         //CPU frequency in MHz

extern void cpu_timer0_isr(void);
extern void cpu_timer1_isr(void);
extern void cpu_timer2_isr(void);

void Configure_C28_Timer(void)
{
	// init timer 0 1 2
	InitCpuTimers();

	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// C28_FREQ in MHz, 1 second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, C28_FREQ, 1000);
	ConfigCpuTimer(&CpuTimer1, C28_FREQ, 10000);
	ConfigCpuTimer(&CpuTimer2, C28_FREQ, 100000);

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.TINT1 = &cpu_timer1_isr;
	PieVectTable.TINT2 = &cpu_timer2_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
	// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in F28M36x_CpuTimers.h), the
	// below settings must also be updated.

	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	CpuTimer2Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

	// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
	// which is connected to CPU-Timer 1, and CPU int 14, which is connected
	// to CPU-Timer 2:
	IER |= M_INT1;
	IER |= M_INT13;
	IER |= M_INT14;

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
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
