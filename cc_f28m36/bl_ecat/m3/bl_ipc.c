#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ram.h"
#include "inc/hw_ipc.h"
#include "driverlib/debug.h"
#include "driverlib/flash.h"
#include "driverlib/ipc.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/ram.h"
#include "driverlib/ipc.h"

#include "utils/uartstdio.h"

#include "shared_ram.h"

volatile tIpcController g_sIpcController1;
volatile tIpcController g_sIpcController2;

unsigned long *	pulMsgRam = (void *)0x2007F7E8;

void CtoMIPC1IntHandler(void);
void CtoMIPC2IntHandler(void);

void Configure_Ipc(void) {

	//  Register M3 interrupt handlers
	IntRegister(INT_CTOMPIC1, CtoMIPC1IntHandler);
	IntRegister(INT_CTOMPIC2, CtoMIPC2IntHandler);
	// Initialize IPC Controllers
	IPCMInitialize (&g_sIpcController1, IPC_INT1, IPC_INT1);
	IPCMInitialize (&g_sIpcController2, IPC_INT2, IPC_INT2);
	//  Enable the IPC interrupts.
	IntEnable(INT_CTOMPIC1);
	IntEnable(INT_CTOMPIC2);

}

#pragma CODE_SECTION(ipc_c28_service,"ramfuncs");
uint32_t ipc_c28_service(uint32_t param) {

	uint32_t res = FN_ERROR;
	// Call FunctionCall() function on C28
	IPCMtoCFunctionCall(&g_sIpcController1, pulMsgRam[0], param, ENABLE_BLOCKING);

	// Read status variable to check if function was entered.
	// Use IPC Flag 17 to check when read data is ready.
	IPCMtoCDataRead(&g_sIpcController1, pulMsgRam[1], &res,
					IPC_LENGTH_32_BITS, ENABLE_BLOCKING, IPC_FLAG17);

    // Wait until read data is ready (by checking IPC Response Flag is cleared).
    // Then check status variables to see if function was entered.
    while (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_MTOCIPCFLG_IPC17) { }

    return res;
}

#pragma CODE_SECTION(ipc_c28_bits,"ramfuncs");
void ipc_c28_bits(uint32_t bits) {

	IPCMtoCDataWrite(&g_sIpcController1, pulMsgRam[2], bits,
				 	 IPC_LENGTH_32_BITS, DISABLE_BLOCKING, NO_FLAG);
}

#pragma CODE_SECTION(jump_to_C28_app,"ramfuncs");
bool jump_to_C28_app(void) {

	//IPCMtoCFunctionCall(&g_sIpcController1, pulMsgRam[0], FN_JUMP_APP, 0);
	return (ipc_c28_service(FN_JUMP_APP) == FN_JUMP_APP );
}

//*****************************************************************************
// CtoM IPC INT1 Interrupt Handler -
// Handles writes into M3 addresses as a result of read commands to the C28.
//*****************************************************************************
void CtoMIPC1IntHandler (void)
{
    tIpcMessage sMessage;

    // Continue processing messages as long as MtoC GetBuffer1 is full
    while (IpcGet(&g_sIpcController1, &sMessage,
                  DISABLE_BLOCKING) != STATUS_FAIL)
    {
        switch (sMessage.ulcommand)
        {
        case IPC_DATA_WRITE:
            IPCCtoMDataWrite(&sMessage);
            break;
        default:
            break;
        }
    }

    // Acknowledge IPC INT1 Flag
    HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= IPC_CTOMIPCACK_IPC1;
}

//*****************************************************************************
// CtoM IPC INT2 Interrupt Handler -
// Should never reach this ISR. This is an optional placeholder for
// g_sIpcController2.
//*****************************************************************************
void CtoMIPC2IntHandler (void)
{
    // Should never reach here - Placeholder for Debug

    // Acknowledge IPC INT2 Flag
    HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= IPC_CTOMIPCACK_IPC2;
}
