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
    uint32_t	timer_cnt;
    uint16_t	hall_status;

} c28_to_m3_data_t;

extern m3_to_c28_data_t	m3_rw_data;
extern c28_to_m3_data_t c28_ro_data;


#define DACREG_CH_A		0
#define DACREG_CH_B		1

struct DAC08 {
	uint16_t spare:4;
    uint16_t data:8;
    uint16_t pd0:1;    	// Power down modes: 00: Normal operation
    uint16_t pd1:1;
    uint16_t buf:1;		// 0: Unbuffered - 1: Buffered
    uint16_t addx:1;
};

typedef union {
    uint16_t all;
    struct DAC08 dac_data;
} DAC08_UNION;


#endif
