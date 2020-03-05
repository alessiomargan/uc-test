#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "F28M36x_Ipc_drivers.h"

#include "c28/include/definitions.h"
#include "common/include/shared_ram.h"


__interrupt void MtoC_ipc1_isr(void)
{
    tIpcMessage sMessage;

    // Continue processing messages as long as MtoC GetBuffer1 is full
    while (IpcGet (&g_sIpcController1, &sMessage, DISABLE_BLOCKING)!= STATUS_FAIL)
    {
        switch (sMessage.ulcommand)
        {
        case IPC_SET_BITS:
            IPCMtoCSetBits(&sMessage);
            break;
        case IPC_CLEAR_BITS:
            IPCMtoCClearBits(&sMessage);
            break;
        case IPC_DATA_WRITE:
            IPCMtoCDataWrite(&sMessage);
            break;
        case IPC_DATA_READ:
            IPCMtoCDataRead(&g_sIpcController1, &sMessage, ENABLE_BLOCKING);
            break;
        case IPC_FUNC_CALL:
            IPCMtoCFunctionCall(&sMessage);
            break;
        default:
        	c28_rw_data.ipc_irq_err_cnt ++;
        	c28_rw_data.ipc_irq_error = sMessage.ulcommand;
        	break;
        }
    }

    // Acknowledge IPC INT1 Flag and PIE to receive more interrupts from group
    // 11
    CtoMIpcRegs.MTOCIPCACK.bit.IPC1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;
}

__interrupt void MtoC_ipc2_isr (void)
{
    tIpcMessage sMessage;

    // Continue processing messages as long as MtoC GetBuffer2 is full
    while (IpcGet (&g_sIpcController2, &sMessage, DISABLE_BLOCKING)!= STATUS_FAIL)
    {
        switch (sMessage.ulcommand)
        {
        case IPC_BLOCK_WRITE:
            IPCMtoCBlockWrite(&sMessage);
            break;
        case IPC_BLOCK_READ:
            IPCMtoCBlockRead(&sMessage);
            break;
        default:
        	c28_rw_data.ipc_irq_err_cnt ++;
        	c28_rw_data.ipc_irq_error = sMessage.ulcommand;
        	break;
        }
    }

    // Acknowledge IPC INT2 Flag and PIE to receive more interrupts from group
    // 11
    CtoMIpcRegs.MTOCIPCACK.bit.IPC2 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;
}


__interrupt void cpu_timer0_isr(void)
{
	CpuTimer0.InterruptCount++;

	//Toggle debug pin
	//if ( CpuTimer0.InterruptCount % 2 ) {
	if ( m3_ro_data.v_int32 % 2 ) {
		//DEBUG_PIN_TOGGLE;
	}
	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void cpu_timer1_isr(void)
{
	EALLOW;
	CpuTimer1.InterruptCount++;
	ORG_LED_TGL;
	// The CPU acknowledges the interrupt.
	EDIS;
}

__interrupt void cpu_timer2_isr(void)
{
	EALLOW;
	CpuTimer2.InterruptCount++;
	c28_rw_data.timer_cnt = CpuTimer2.InterruptCount;
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
	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;


}
// -----------------------------------------------------------
// PIE Group 3 - MUXed into CPU INT3
// -----------------------------------------------------------

// INT 3.1
__interrupt void epwm1_isr(void)     // EPWM-1
{
    // Insert ISR Code here
	EPwm1Regs.ETCLR.bit.INT = 1;				// Clear INT flag for this timer
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;		// Acknowledge this interrupt to PIE
}

// INT 1.1
__interrupt void  adc1_isr(void)
{
	static uint16_t smpl_cnt;

	c28_rw_data.adc_cnt++;
	//c28_rw_data.hall_status = (uint16_t)(HALL_A_DATA || HALL_B_DATA << 1 || HALL_C_DATA << 2);

	ain_raw[0][smpl_cnt] = Adc1Result.ADCRESULT0;
	ain_raw[1][smpl_cnt] = Adc1Result.ADCRESULT1;
	ain_raw[2][smpl_cnt] = Adc1Result.ADCRESULT2;
	ain_raw[3][smpl_cnt] = Adc1Result.ADCRESULT3;

	if ( smpl_cnt < MAX_AIN_SAMPLES ) {
		smpl_cnt++;
	} else {
		smpl_cnt = 0;
	}

	Adc1Regs.ADCINTFLGCLR.bit.ADCINT1 = 1;  	//Clear ADCINT1 flag reinitialize for next SOC
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; 	// Acknowledge interrupt to PIE

}
