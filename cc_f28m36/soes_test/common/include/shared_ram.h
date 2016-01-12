#ifndef __SHARED_RAM_H__
#define __SHARED_RAM_H__

#include <stdint.h>

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
} c28_rw_data_t;


#endif
