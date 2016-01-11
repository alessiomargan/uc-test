#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define C28_FREQ    150         //CPU frequency in MHz

extern void cpu_timer0_isr(void);
extern void cpu_timer1_isr(void);
extern void cpu_timer2_isr(void);

 void Configure_C28_Timer()
 {
	 // init timer 0 1 2
	 InitCpuTimers();

	 // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	 // C28_FREQ in MHz, 1 second Period (in uSeconds)
	 ConfigCpuTimer(&CpuTimer0, C28_FREQ, 1000000);
	 ConfigCpuTimer(&CpuTimer1, C28_FREQ, 1000000);
	 ConfigCpuTimer(&CpuTimer2, C28_FREQ, 1000000);

	 // Interrupts that are used in this example are re-mapped to
	 // ISR functions found within this file.
	 EALLOW;  // This is needed to write to EALLOW protected registers
	 PieVectTable.TINT0 = &cpu_timer0_isr;
	 PieVectTable.TINT1 = &cpu_timer1_isr;
	 PieVectTable.TINT2 = &cpu_timer2_isr;
	 EDIS;    // This is needed to disable write to EALLOW protected registers

	 // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
	 // which is connected to CPU-Timer 1, and CPU int 14, which is connected
	 // to CPU-Timer 2:
	 IER |= M_INT1;
	 IER |= M_INT13;
	 IER |= M_INT14;

	 // Enable TINT0 in the PIE: Group 1 interrupt 7
	 PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
 }
