#ifndef __C_LCD_IFACE_H__
#define __C_LCD_IFACE_H__

#include <stdint.h>
#include <stdbool.h>

#define BOOSTER_OFF	0x2B
#define BOOSTER_ON	0x2F

extern unsigned char Image_iit[];
extern unsigned char pic_hellombed[];
extern unsigned char ea_logo[];
extern unsigned char font_6x8[];
extern unsigned char font_8x16[];
extern unsigned char font_8x8[];
extern unsigned char font_16x32num[];


#ifdef __cplusplus
extern "C"
{
#endif

void lcd_init(unsigned char);
void lcd_clear_screen(void);
void lcd_send_bitmap(const uint8_t * data);
void lcd_send_pic(uint8_t col, uint8_t page, const uint8_t * data);
void lcd_string(uint8_t col, uint8_t page, const unsigned char * font_adress, const char * str);
void lcd_sprint(uint8_t col, uint8_t page, const unsigned char * font_adress);

void lcd_loop(void);

#ifdef __cplusplus
}
#endif


#endif
