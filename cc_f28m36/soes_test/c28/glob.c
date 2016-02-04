
#include "definitions.h"
#include "shared_ram.h"

// map to RAM S0
#pragma DATA_SECTION(m3_ro_data,"RAM_S0");
volatile m3_rw_data_t	m3_ro_data;

// map to RAM S1
#pragma DATA_SECTION(foe_buffer,"RAM_S1");
volatile Uint16			foe_buffer[128];

// map to RAM S4
#pragma DATA_SECTION(c28_rw_data,"RAM_S4");
volatile c28_rw_data_t	c28_rw_data;

volatile tIpcController g_sIpcController1;
volatile tIpcController g_sIpcController2;

Uint32 *pulMsgRam;

Uint32 bootloaderServiceResult;
