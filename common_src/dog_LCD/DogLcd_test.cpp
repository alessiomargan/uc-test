/*
 * DogLcd_test.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: amargan
 */

//#include "DogLcd.h"
#include "dog_LCD/doggy.h"

extern "C" void lcd_init(void);
extern "C" void lcd_self_test(void);
extern "C" void lcd_flush(void);
extern "C" void lcd_printf_i(int , int , int );
extern "C" void lcd_printf_f(int , int , float );
extern "C" void lcd_printf_s(int , int , const char * );

DogMLCD lcd;



void lcd_init(void) {
	lcd.init();
	//lcd.Rect( 0, 0, 127, 63, DOGMLCD_full, wipe);
	lcd.Flush();
}

void lcd_flush(void) {
	lcd.Flush();
}

void lcd_self_test(void) {

	int x, y;
	char buf[32];
	const char formula[] = { 159, '(', 'x', ')', '=', '2', 227, '+', 's', 'i', 'n', '(', 224, ')', '/', 251, 'x' };
	const char *p = "Ciao !";

#if 0
	// select font to use:
	lcd.XFont = xfont_8;

	x = 0;
	y = 0;
	while ( *p ) {
		lcd.XChar(x, y, *p, poke);
		x += 8;
		p++;
	}

	lcd.XFont = xfont_11;

	// create text with symbols:
	// output text from point(0,0):
	lcd.XString( 0, 11, formula );
	// create text with variables and ouput from point (0,11):
	sprintf( buf, "%s=%f", "A", 15.894 );
	lcd.XString( 0, 22, buf );
#endif

	// paint rectangles with built-in patterns:
	lcd.Rect(  0, 48,  15, 63, DOGMLCD_full );
	lcd.Rect( 16, 48,  31, 63, DOGMLCD_dark );
	lcd.Rect( 32, 48,  47, 63, DOGMLCD_grey );
	lcd.Rect( 48, 48,  63, 63, DOGMLCD_lite );

	// transmit work screen to physical screen:
	lcd.Flush();
}

void lcd_printf_i( int x, int y, int i ) {
	lcd.Rect( x, y, 63, 48, DOGMLCD_full, wipe);
	lcd.XString( x, y, i, poke);
}

void lcd_printf_f( int x, int y, float f ) {
	lcd.XString( x, y, f, poke);
}
void lcd_printf_s( int x, int y, const char * buf ) {
	lcd.XString( x, y, buf, poke);
}



