#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "F28M36x_Ipc_drivers.h"
#include "definitions.h"

#include "shared_ram.h"

#define C28_FREQ    150         //CPU frequency in MHz

extern void cpu_timer0_isr(void);
extern void cpu_timer1_isr(void);
extern void cpu_timer2_isr(void);
extern void epwm1_isr(void);
extern void xint1_isr(void);
extern void MtoCIPC1IntHandler(void);
extern void MtoCIPC2IntHandler(void);

extern tIpcController  	g_sIpcController1;
extern tIpcController  	g_sIpcController2;

Uint32 *pulMsgRam;

Uint32 bootloaderServiceResult;

extern Uint16 foe_buffer[128];

void bootloaderService(Uint32 param)
{

	switch (param) {

	case FN_ERASE_FLASH :
		bootloaderServiceResult = FN_ERASE_FLASH;
		break;
	case FN_FOE_BUFF :

		bootloaderServiceResult = FN_FOE_BUFF;
		break;
	default:
		bootloaderServiceResult = FN_ERROR;
		break;

	}

}

void Configure_Pie_Vector(void)
{
	EALLOW;  // This is needed to write to EALLOW protected registers

	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.TINT1 = &cpu_timer1_isr;
	PieVectTable.TINT2 = &cpu_timer2_isr;

    PieVectTable.EPWM1_INT = &epwm1_isr;
    //PieVectTable.EPWM2_INT = &epwm2_isr;
    //PieVectTable.EPWM3_INT = &epwm3_isr;

    PieVectTable.XINT1 = &xint1_isr;

    PieVectTable.MTOCIPC_INT1 = &MtoCIPC1IntHandler;
    PieVectTable.MTOCIPC_INT2 = &MtoCIPC2IntHandler;

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
	// Write addresses of variables where words should be written to pulMsgRam
	// array.
	// 0 = Bootloader Service function.
	// 1 = Address of 16-bit Bootloader Service result
	// executed
	pulMsgRam[0] = (unsigned long)&bootloaderService;
	pulMsgRam[1] = (unsigned long)&bootloaderServiceResult;

}

void Configure_C28_Timer(void)
{
	// init timer 0 1 2
	InitCpuTimers();

	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// C28_FREQ in MHz, 1 second Period (in uSeconds)
	ConfigCpuTimer(&CpuTimer0, C28_FREQ, 1000);
	ConfigCpuTimer(&CpuTimer1, C28_FREQ, 10000);
	ConfigCpuTimer(&CpuTimer2, C28_FREQ, 100000);

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

	// PH0_GPIO48/49/50
	GpioG1CtrlRegs.GPBMUX2.bit.GPIO48 = 0;
	GpioG1CtrlRegs.GPBDIR.bit.GPIO48 = 0;
	GpioG1CtrlRegs.GPBQSEL2.bit.GPIO48 = 2;
	GpioG1CtrlRegs.GPBCTRL.bit.QUALPRD3 = 0x02;

	GpioG1CtrlRegs.GPBQSEL2.bit.GPIO49 = 0;
	GpioG1CtrlRegs.GPBQSEL2.bit.GPIO50 = 0;

    GpioG1TripRegs.GPTRIP4SEL.bit.GPTRIP4SEL = 48;

	// Configure XINT1
	XIntruptRegs.XINT1CR.bit.POLARITY = 1;     // Raising edge interrupt
	// Enable XINT1
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;       // Enable XINT1

    EDIS;

	IER |= M_INT1;
	// Enable XINT1_ISR in the PIE: Group 1 interrupt 4
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;


	LED_0_OFF;// turn off LED
	DEBUG_PIN_OFF;
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
	EPwm1Regs.CMPA.half.CMPA = 0; //(PWMPRD >> 1);
	EPwm1Regs.CMPB = 0;									// NOT USED (ACTIVE HI COMPLEMENTARY MODE)

	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;			// Active Hi Complementary
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;		// Enable Dead-band module for both falling and riding edge
	EPwm1Regs.DBFED = DEAD_BAND_GAP;					// Set dead band gap up
	EPwm1Regs.DBRED = DEAD_BAND_GAP;					// Set dead band gap down

	EPwm1Regs.ETSEL.bit.INTSEL = 1;         			// Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 1;         	 			// Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = 1;          			// Generate INT on every event

	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			// CNTRA Shadow Mode Enabled
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;			// CNTRB Shadow Mode Enabled
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;		// load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;		// load on CTR=Zero

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
	EPwm2Regs.CMPA.half.CMPA = 0; //(PWMPRD >> 1);
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
	EPwm3Regs.CMPA.half.CMPA = 0; //(PWMPRD >> 1);
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

	InitEPwm1Gpio();
	InitEPwm2Gpio();
	InitEPwm3Gpio();

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;

	init_PWM1();
	init_PWM2();
	init_PWM3();

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;

    // Enable CPU INT3 which is connected to EPWM1-3 INT:
    IER |= M_INT3;

    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx3 = 1;


}


