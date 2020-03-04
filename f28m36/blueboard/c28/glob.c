
#include "c28/include/definitions.h"
#include "common/include/shared_ram.h"

// map to RAM S1
#pragma DATA_SECTION(m3_ro_data,"RAM_S1");
volatile m3_to_c28_data_t	m3_ro_data;

// map to RAM S5
#pragma DATA_SECTION(c28_rw_data,"RAM_S5");
volatile c28_to_m3_data_t	c28_rw_data;

volatile tIpcController g_sIpcController1;
volatile tIpcController g_sIpcController2;

uint32_t * pulMsgRam;
uint32_t ipcServiceResult;

uint16_t ain_raw[4][MAX_AIN_SAMPLES];
