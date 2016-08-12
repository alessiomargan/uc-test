#ifndef __USER_FLASH_H__
#define __USER_FLASH_H__

bool erase_M3_app_flash();
bool erase_C28_app_flash();

uint32_t calc_app_crc(void);
bool test_C28_erase_write_flash(void);

#endif
