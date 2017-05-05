#include "cpp_dog_LCD/c_lcd_iface.h"
#include "cpp_dog_LCD/LCD/DogLCD.h"
#include "cpp_dog_LCD/Graphics/Graphics.h"

DogLCD dog;
Graphics g(&dog);

void Lcd_init(void) {
	dog.init();
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
