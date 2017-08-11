#include "cpp_dog_LCD/c_lcd_iface.h"
#include "cpp_dog_LCD/LCD/DogLCD.h"
#include "cpp_dog_LCD/Graphics/Graphics.h"

#include <cpp_dog_LCD/pics/hellombed.h>
#include <cpp_dog_LCD/pics/logo_BLH.h>
#include <cpp_dog_LCD/pics/iit.h>
#include <cpp_dog_LCD/fonts/font_16x32nums.h>
#include <cpp_dog_LCD/fonts/font_6x8.h>
#include <cpp_dog_LCD/fonts/font_8x16.h>
#include <cpp_dog_LCD/fonts/font_8x8.h>
#include <cpp_dog_LCD/fonts/font_cn_13x24.h>


#include <stdio.h>

DogLCD dog;
Graphics g(&dog);

void lcd_init(unsigned char lcd_pwr) {
	dog.init(lcd_pwr);
}

void lcd_clear_screen(void) {
	dog.clear_screen();
}

void lcd_send_bitmap(const uint8_t * data) {
	dog.send_pic(data);
}

void lcd_send_pic(uint8_t col, uint8_t page, const uint8_t * data) {
	dog.picture(col, page, data);
}

void lcd_string(uint8_t col, uint8_t page, const unsigned char * font_adress, const char * str) {
	dog.string(col, page, font_adress, str);
}




