#ifndef __SHARED_RAM_H__
#define __SHARED_RAM_H__

#include <stdint.h>
#ifdef _C28X
	#include "F28M36x_Device.h"
#else
	#include "driverlib/device.h"
#endif

//RAM Function Linker Section
#ifdef __TI_COMPILER_VERSION__
	#if __TI_COMPILER_VERSION__ >= 15009000
		#define ramFuncSection ".TI.ramfunc"
	#else
		#error "MeRDa"
		#define ramFuncSection "ramfuncs"
	#endif
#endif


typedef struct {
	int32_t		v_int32;

} m3_to_c28_data_t;

typedef struct {

#ifdef _C28X
	uint16_t	c28_firmware_version[4];
#else
	char		c28_firmware_version[8];
#endif
    uint32_t	ipc_irq_err_cnt;
    uint32_t	ipc_irq_error;

} c28_to_m3_data_t;

extern m3_to_c28_data_t	m3_rw_data;
extern c28_to_m3_data_t c28_ro_data;

#endif
