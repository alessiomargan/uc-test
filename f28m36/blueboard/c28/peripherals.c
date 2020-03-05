#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "F28M36x_Ipc_drivers.h"

#include "c28/include/definitions.h"
#include "common/include/shared_ram.h"



void Configure_Pie_Vector(void)
{
	EALLOW;  // This is needed to write to EALLOW protected registers

	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.TINT1 = &cpu_timer1_isr;
	PieVectTable.TINT2 = &cpu_timer2_isr;

    PieVectTable.EPWM1_INT = &epwm1_isr;
    //PieVectTable.EPWM2_INT = &epwm2_isr;
    //PieVectTable.EPWM3_INT = &epwm3_isr;

    PieVectTable.ADCINT1 = &adc1_isr;

    PieVectTable.XINT1 = &xint1_isr;

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
	// Enable MTOCIPCINT1 e MTOCIPCINT2 in the PIE: Group 11 interrupt 1 e 2
	PieCtrlRegs.PIEIER11.bit.INTx1 = 1; // MTOCIPC INT1
	PieCtrlRegs.PIEIER11.bit.INTx2 = 1; // MTOCIPC INT2


	// Point array to address in CTOM MSGRAM for passing variable locations
	pulMsgRam = (void *)0x0003FBF4;
	// Write addresses of variables where words should be written to pulMsgRam array.
	// 0 = Ipc Service function.
	// 1 = Address of 16-bit Ipc Service result
	// executed
	pulMsgRam[0] = (unsigned long)&IpcService;
	pulMsgRam[1] = (unsigned long)&ipcServiceResult;

}

void Configure_C28_Timer(void)
{
	// init timer 0 1 2
	InitCpuTimers();

	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// C28_FREQ in MHz, 1 second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, CPU_FRQ, 1000);
	ConfigCpuTimer(&CpuTimer1, CPU_FRQ, 250000);
	ConfigCpuTimer(&CpuTimer2, CPU_FRQ, 500000);

	// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
	// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in F28M36x_CpuTimers.h), the
	// below settings must also be updated.
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	CpuTimer2Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

	// Enable CPU int1  which is connected to CPU-Timer 0
	//        CPU int13 which is connected to CPU-Timer 1
	//        CPU int14 which is connected to CPU-Timer 2
	IER |= M_INT1;
	IER |= M_INT13;
	IER |= M_INT14;
	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}

void Configure_C28_Gpio(void)
{
	/*
	 * LEDs
	 */
	EALLOW;
	GpioG1CtrlRegs.GPCMUX2.bit.GPIO86 = 0;  // GPIO86 = GPIO	--> DEBUG YELLOW LED
	GpioG1CtrlRegs.GPCDIR.bit.GPIO86 = 1;	// OUTPUT
	GpioG1CtrlRegs.GPCMUX2.bit.GPIO87 = 0;  // GPIO87 = GPIO	--> DEBUG ORANGE LED
	GpioG1CtrlRegs.GPCDIR.bit.GPIO87 = 1;	// OUTPUT
	EDIS;

	YLW_LED_OFF;
	ORG_LED_OFF;

#ifdef HALL
	/*
	 * HALL
	 */
	EALLOW;
	GpioG1CtrlRegs.GPCMUX2.bit.GPIO88 = 0;  // GPIO88 = GPIO		--> HALL_A
	GpioG1CtrlRegs.GPCDIR.bit.GPIO88 = 0;	// INPUT
	//GpioG1CtrlRegs.GPCQSEL2.bit.GPIO88 = 0;	// XINT1 Synch to SYSCLKOUT  see 4.2.5.2 of TRM
	//GpioG1TripRegs.GPTRIP4SEL.bit.GPTRIP4SEL = 88;
	GpioG1CtrlRegs.GPCMUX2.bit.GPIO89 = 0;  // GPIO88 = GPIO		--> HALL_B
	GpioG1CtrlRegs.GPCDIR.bit.GPIO89 = 0;	// INPUT
	//GpioG1CtrlRegs.GPCQSEL2.bit.GPIO89 = 0;	// XINT1 Synch to SYSCLKOUT  see 4.2.5.2 of TRM
	//GpioG1TripRegs.GPTRIP4SEL.bit.GPTRIP4SEL = 89;
	GpioG1CtrlRegs.GPCMUX2.bit.GPIO90 = 0;  // GPIO89 = GPIO		--> HALL_C
	GpioG1CtrlRegs.GPCDIR.bit.GPIO90 = 0;	// INPUT
	//GpioG1CtrlRegs.GPCQSEL2.bit.GPIO90 = 0;	// XINT1 Synch to SYSCLKOUT  see 4.2.5.2 of TRM
	//GpioG1TripRegs.GPTRIP4SEL.bit.GPTRIP4SEL = 90;
	EDIS;

	// Enable CPU INT1  which is connected to XINT1
	//IER |= M_INT1;
	// Enable XINT1 in the PIE: Group 1 interrupt 4
	//PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
	// Configure XINT1
	//XIntruptRegs.XINT1CR.bit.POLARITY = 1;     // Rising edge interrupt
	// Enable XINT1
	//XIntruptRegs.XINT1CR.bit.ENABLE = 1;
#endif

}


#define PWMPRD			3750					// PWM overflow value --> 20 KHz @ 150 MHz
#define DEAD_BAND_GAP	150						// Dead band:  75 = 0.5us | 150 = 1us | 300 = 2us | 450 = 3us | 600 = 4 us


static void init_PWM1(void)
{
	EPwm1Regs.TBPHS.all = 0;							// Set Phase register to zero

	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// Symmetrical mode
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;				// ePWM1 is Master module

	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;				// TIMER Shadow Mode Enabled
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;			// Sync down-stream module
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;         	// TBCLK = SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;				// TBCLK = SYSCLKOUT

	EPwm1Regs.TBPRD = PWMPRD;
	EPwm1Regs.CMPA.half.CMPA = (PWMPRD >> 1);
	EPwm1Regs.CMPB = 0;									// NOT USED (ACTIVE HI COMPLEMENTARY MODE)

	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;			// Active Hi Complementary
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;		// Enable Dead-band module for both falling and riding edge
	EPwm1Regs.DBFED = DEAD_BAND_GAP;					// Set dead band gap up
	EPwm1Regs.DBRED = DEAD_BAND_GAP;					// Set dead band gap down

	//Set event triggers (SOCA) for ADC SOC1
	EPwm1Regs.ETSEL.bit.SOCAEN  = 1;      				// Enable SOC on A group
	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;			// Generate SOCA when counter == 0
	EPwm1Regs.ETPS.bit.SOCAPRD  = 3;      				// Generate pulse on every 3rd event
	//
	//EPwm1Regs.ETSEL.bit.INTSEL = 1;         			// Select INT on Zero event
	//EPwm1Regs.ETSEL.bit.INTEN = 1;         	 		// Enable INT
	//EPwm1Regs.ETPS.bit.INTPRD = 1;          			// Generate INT on every event

	// Setup shadow register load on ZERO
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			// CNTRA Shadow Mode Enabled
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;			// CNTRB Shadow Mode Enabled
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;		// load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;		// load on CTR=Zero

	// Set actions
	EPwm1Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
	EPwm1Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
	EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
	EPwm1Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

	EPwm1Regs.AQCTLB.bit.ZRO = AQ_NO_ACTION;
	EPwm1Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
	EPwm1Regs.AQCTLB.bit.CAU = AQ_NO_ACTION;
	EPwm1Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
	EPwm1Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
	EPwm1Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;
}

static void init_PWM2(void)
{
	EPwm2Regs.TBPHS.all = 0;							// Set Phase register to zero

	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// Symmetrical mode
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;				// ePWM1 is Master module

	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;				// TIMER Shadow Mode Enabled
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;			// Sync to EPWM1
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;         	// TBCLK = SYSCLKOUT
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;				// TBCLK = SYSCLKOUT

	EPwm2Regs.TBPRD = PWMPRD;
	EPwm2Regs.CMPA.half.CMPA = (PWMPRD >> 1);
	EPwm2Regs.CMPB = 0;	// NOT USED (ACTIVE HI COMPLEMENTARY MODE)

	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;			// Active Hi Complementary
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;		// Enable Dead-band module for both falling and riding edge
	EPwm2Regs.DBFED = DEAD_BAND_GAP;					// Set dead band gap up
	EPwm2Regs.DBRED = DEAD_BAND_GAP;					// Set dead band gap down

	EPwm2Regs.ETSEL.bit.INTSEL = 0;         			// Select INT on Zero event
	EPwm2Regs.ETSEL.bit.INTEN = 0;         	 			// Diable INT
	EPwm2Regs.ETPS.bit.INTPRD = 0;          			// Never generate INT

	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			// CNTRA Shadow Mode Enabled
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;			// CNTRB Shadow Mode Enabled
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;		// load on CTR=Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;		// load on CTR=Zero

	EPwm2Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
	EPwm2Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
	EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;

	EPwm2Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
	EPwm2Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

	EPwm2Regs.AQCTLB.bit.ZRO = AQ_NO_ACTION;
	EPwm2Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
	EPwm2Regs.AQCTLB.bit.CAU = AQ_NO_ACTION;
	EPwm2Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
	EPwm2Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
	EPwm2Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;
}

static void init_PWM3(void)
{
	EPwm3Regs.TBPHS.all = 0;							// Set Phase register to zero

	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// Symmetrical mode
	EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;				// ePWM1 is Master module

	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;				// TIMER Shadow Mode Enabled
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;			// Sync to EPWM1
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;         	// TBCLK = SYSCLKOUT
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;				// TBCLK = SYSCLKOUT

	EPwm3Regs.TBPRD = PWMPRD;
	EPwm3Regs.CMPA.half.CMPA = (PWMPRD >> 1);
	EPwm3Regs.CMPB = 0;	// NOT USED (ACTIVE HI COMPLEMENTARY MODE)

	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;			// Active Hi Complementary
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;		// Enable Dead-band module for both falling and riding edge
	EPwm3Regs.DBFED = DEAD_BAND_GAP;					// Set dead band gap up
	EPwm3Regs.DBRED = DEAD_BAND_GAP;					// Set dead band gap down

	EPwm3Regs.ETSEL.bit.INTSEL = 0;         			// Select INT on Zero event
	EPwm3Regs.ETSEL.bit.INTEN = 0;         	 			// Diable INT
	EPwm3Regs.ETPS.bit.INTPRD = 0;          			// Never generate INT

	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			// CNTRA Shadow Mode Enabled
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;			// CNTRB Shadow Mode Enabled
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;		// load on CTR=Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;		// load on CTR=Zero

	EPwm3Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
	EPwm3Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
	EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;
	EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;

	EPwm3Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
	EPwm3Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

	EPwm3Regs.AQCTLB.bit.ZRO = AQ_NO_ACTION;
	EPwm3Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
	EPwm3Regs.AQCTLB.bit.CAU = AQ_NO_ACTION;
	EPwm3Regs.AQCTLB.bit.CAD = AQ_NO_ACTION;
	EPwm3Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
	EPwm3Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;
}

void Configure_C28_ePWM(void) {

	EALLOW;
	// drv8332 PWM and IO
	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   	// GPIO0 = EPWM1A	--> PWM_A
	GpioG1CtrlRegs.GPADIR.bit.GPIO0 = 1;  	// OUTPUT
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;		// GPIO1 = EPWM1B	--> #RES_A
	GpioG1CtrlRegs.GPADIR.bit.GPIO1 = 1;  	// OUTPUT
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   	// GPIO2 = EPWM2B	--> PWM_C
	GpioG1CtrlRegs.GPADIR.bit.GPIO2 = 1;  	// OUTPUT
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;		// GPIO3 = EPWM2B	--> #RES_B
	GpioG1CtrlRegs.GPADIR.bit.GPIO3 = 1;  	// OUTPUT
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   	// GPIO4 = EPWM3A	--> PWM_C
	GpioG1CtrlRegs.GPADIR.bit.GPIO4 = 1;  	// OUTPUT
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;		// GPIO5 = EPWM3B	--> #RES_C
	GpioG1CtrlRegs.GPADIR.bit.GPIO5 = 1;  	// OUTPUT
	GpioG1CtrlRegs.GPAMUX1.bit.GPIO6 = 0; 	// GPIO6 = GPIO		--> #OTW
	GpioG1CtrlRegs.GPADIR.bit.GPIO6 = 0;  	// INPUT
	GpioG1CtrlRegs.GPAMUX1.bit.GPIO7 = 0; 	// GPIO7 = GPIO		--> #FAULT
	GpioG1CtrlRegs.GPADIR.bit.GPIO7 = 0;  	// INPUT

	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;

	init_PWM1();
	init_PWM2();
	init_PWM3();

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;

    // Enable CPU INT3 which is connected to EPWM1-3
    IER |= M_INT3;

    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx3 = 1;


}


/*
 * The term SOC is the configuration set defining the single conversion of a single channel.
 * In that set there are three configurations:
 *  - the trigger source that starts the conversion
 *  - the channel to convert
 *  - the acquisition (sample) window size
 *
 */
void Configure_C28_adc(void) {

	InitAdc1();

	// Configure ADC
	EALLOW;

	Adc1Regs.ADCCTL2.bit.ADCNONOVERLAP = 1;     // Set ADC to non-overlap mode
	Adc1Regs.ADCCTL1.bit.INTPULSEPOS   = 1;     // EOC trips after conversion result is latched

	Adc1Regs.INTSEL1N2.bit.INT1E       = 1;     // Enabled ADCINT1
	Adc1Regs.INTSEL1N2.bit.INT1CONT    = 0;     // Disable ADCINT1 Continuous mode
	Adc1Regs.INTSEL1N2.bit.INT1SEL     = 3;     // setup EOC3 to trigger ADCINT1

	// trigger source
	Adc1Regs.ADCSOC0CTL.bit.TRIGSEL    = 5;     // Assign EPWM1SOCA to SOC0 TRIGSEL
	Adc1Regs.ADCSOC1CTL.bit.TRIGSEL    = 5;    	// Assign EPWM1SOCA to SOC1 TRIGSEL
	Adc1Regs.ADCSOC2CTL.bit.TRIGSEL    = 5;    	// Assign EPWM1SOCA to SOC2 TRIGSEL
	Adc1Regs.ADCSOC3CTL.bit.TRIGSEL    = 5;    	// Assign EPWM1SOCA to SOC3 TRIGSEL

	// ADC input channels
	Adc1Regs.ADCSOC0CTL.bit.CHSEL      = 0;     // set SOC0 channel select to ADC1A0
	Adc1Regs.ADCSOC1CTL.bit.CHSEL      = 2;     // set SOC1 channel select to ADC1A2
	Adc1Regs.ADCSOC2CTL.bit.CHSEL      = 3;     // set SOC2 channel select to ADC1A3
	Adc1Regs.ADCSOC3CTL.bit.CHSEL      = 4;     // set SOC3 channel select to ADC1A4

	// Set S/H window of 7 ADC clock cycles
	Adc1Regs.ADCSOC0CTL.bit.ACQPS      = 6;     // S/H = ACQPS + 1
	Adc1Regs.ADCSOC1CTL.bit.ACQPS      = 6;
	Adc1Regs.ADCSOC2CTL.bit.ACQPS      = 6;
	Adc1Regs.ADCSOC3CTL.bit.ACQPS      = 6;

	// Selecting triggers for SOCs
	AnalogSysctrlRegs.TRIG1SEL.all     = 5;     // Assigning EPWM1SOCA to TRIGGER 1 of analog subsystem

	EDIS;

	// Enable CPU INT1
	IER |= M_INT1;
	// Enable XINT1 in the PIE: Group 1 interrupt 1
	PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
}
