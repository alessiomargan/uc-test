#ifndef __BL_IPC_H__
#define __BL_IPC_H__

void Configure_Ipc(void);

bool jump_to_C28_app(void);

uint32_t ipc_c28_service(uint32_t param);

void ipc_c28_bits(uint32_t bits);


#endif
