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
#define FN_APP_CRC		0x00000C4C
#define FN_TEST_FLS     0x00007E57
#define FN_JUMP_APP		0x06A77ACA

// M3 RW shared RAM
typedef struct {

    uint32_t	foe_flashAddr;
#ifdef _C28X
    uint16_t	foe_buffer[128];
#else
    uint8_t		foe_buffer[256];
#endif

} m3_bl_rw_data_t;


// C28x RW shared RAM

typedef struct {

	uint32_t	test_type_uint32;
	uint16_t	test_type_uint16;
	uint64_t	test_type_uint64;
	//
	uint32_t	flash_crc;
	uint32_t	calc_crc;
	//
	uint32_t	ipc_irq_err_cnt;
	uint32_t	ipc_irq_error;

} c28_bl_rw_data_t;


#endif
