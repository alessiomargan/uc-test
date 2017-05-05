#ifndef __C_LCD_IFACE_H__
#define __C_LCD_IFACE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

void Lcd_init(void);
void lcd_clear_screen(void);
void lcd_send_bitmap(const uint8_t * data);
void lcd_send_pic(uint8_t col, uint8_t page, const uint8_t * data);
void lcd_string(uint8_t col, uint8_t page, const unsigned char * font_adress, const char * str);

#ifdef __cplusplus
}
#endif


#endif
