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
extern "C" void lcd_printf(int , int , const char * );

DogMLCD lcd;

void lcd_init(void) { lcd.init(); }

void lcd_self_test(void) {

	// select font to use:
	lcd.XFont = xfont_11;

	// transmit currently empty work screen (to clear physical display):
	lcd.Flush();

	// create text with symbols:
	char formula[] = { 159, '(', 'x', ')', '=', '2', 227, '+', 's', 'i', 'n', '(', 224, ')', '/', 251, 'x' };
	// output text from point(0,0):
	lcd.XString( 0, 0, formula );

	// create text with variables and ouput from point (0,11):
	char buf[256];
	sprintf( buf, "%s=%f", "A", 15.894 );
	lcd.XString( 0, 11, buf );

	// paint rectangles with built-in patterns:
	lcd.Rect(  0, 48,  15, 63, DOGMLCD_full );
	lcd.Rect( 16, 48,  31, 63, DOGMLCD_dark );
	lcd.Rect( 32, 48,  47, 63, DOGMLCD_grey );
	lcd.Rect( 48, 48,  63, 63, DOGMLCD_lite );

	// transmit work screen to physical screen:
	lcd.Flush();
}

void lcd_printf( int x, int y, const char * buf ) {
	lcd.XString( x, y, buf, poke);
	lcd.Flush();
}




