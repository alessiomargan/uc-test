#ifndef __SHARED_RAM_H__
#define __SHARED_RAM_H__

#include <stdint.h>


#define FN_ERROR		0xE0E0E0E0

#define FN_ERASE_FLASH	0x00EEEE00
#define FN_FOE_BUFF		0xF0E00BFF

typedef struct {
    int16_t    	v_int16;
    int32_t    	v_int32;
    uint64_t	v_uint64;
    float		v_float;
} m3_rw_data_t;


typedef struct {
    int16_t    	v_int16;
    int32_t    	v_int32;
    uint64_t	v_uint64;
    float		v_float;
    //
    uint16_t	ipc_irq_err_cnt;
    uint32_t	ipc_irq_error;
} c28_rw_data_t;


#endif
