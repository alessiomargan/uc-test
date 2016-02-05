#ifndef __SHARED_RAM_H__
#define __SHARED_RAM_H__

#include <stdint.h>
#ifdef _C28X
	#include "F28M36x_Device.h"
#else
	#include "driverlib/device.h"
#endif


#define FN_ERROR		0xE0E0E0E0

#define FN_ERASE_FLASH	0x00EEEE00
#define FN_FOE_BUFF		0xF0E00BFF

// M3 RW shared RAM

typedef struct {
    int16_t    	v_int16;
    int32_t    	v_int32;
    uint64_t	v_uint64;
    float		v_float;
    //
#ifdef _C28X
    uint16_t	foe_buffer[128];
#else
    uint8_t		foe_buffer[256];
#endif
    uint32_t	foe_flashAddr;

} m3_rw_data_t;


// C28x RW shared RAM

typedef struct {

	int16_t    	v_int16;
    int32_t    	v_int32;
    uint64_t	v_uint64;
    float		v_float;
    //
    uint16_t	ipc_irq_err_cnt;
    uint32_t	ipc_irq_error;
    //

} c28_rw_data_t;

#endif
