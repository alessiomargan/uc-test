#include "soes/utypes.h"
#include "soes/esc_foe.h"

foe_writefile_cfg_t      firmware_foe_files[] = {
    { "soes_tm4c.bin"	, 0, 0, 0, 0xB007, 0 },
    { "fake"			, 0, 0, 0, 0xDEAD, 0 },
    { "$#@!"			, 0, 0, 0, 0xBEEF, 0 },
    { 0					, 0, 0, 0, 0, 0 }
};

