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
        	c28_bl_rw_data.ipc_irq_err_cnt ++;
        	c28_bl_rw_data.ipc_irq_error = sMessage.ulcommand;
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
        	c28_bl_rw_data.ipc_irq_err_cnt ++;
        	c28_bl_rw_data.ipc_irq_error = sMessage.ulcommand;
        	break;
        }
    }

    // Acknowledge IPC INT2 Flag and PIE to receive more interrupts from group
    // 11
    CtoMIpcRegs.MTOCIPCACK.bit.IPC2 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;
}


