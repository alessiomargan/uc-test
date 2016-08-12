
#include "c28/include/definitions.h"
#include "common/include/shared_ram.h"

// map to RAM S0
#pragma DATA_SECTION(m3_bl_ro_data,"RAM_S0");
volatile m3_bl_rw_data_t	m3_bl_ro_data;

// map to RAM S4
#pragma DATA_SECTION(c28_bl_rw_data,"RAM_S4");
volatile c28_bl_rw_data_t	c28_bl_rw_data;

volatile tIpcController g_sIpcController1;
volatile tIpcController g_sIpcController2;

Uint32 *pulMsgRam;

Uint32 bootloaderServiceResult;
Uint32 bootM3bits;
