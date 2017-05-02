/*
 * DogLcd_test.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: amargan
 */


#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "doggy.h"

const char formula[] = { 159, '(', 'x', ')', '=', '2', 227, '+', 's', 'i', 'n', '(', 224, ')', '/', 251, 'x', 0 };
const char ciao[] = "Ciao Stronzo !!";

#pragma DATA_SECTION(lcd_test_buf,".lcd")
char lcd_test_buf[32];

__attribute__((ramfunc))
void lcd_test_sprint(void) {

	XFont = xfont_8;

    static float f;
    float f_incr = 0.01;
	f += f_incr;
    static uint8_t c;

	Rect(  50, 35,  120, 44, DOGMLCD_full, wipe );

	//bzero(lcd_test_buf,sizeof(lcd_test_buf));
	sprintf( lcd_test_buf, "%s = %.3f", "A", 3.1456);
	XString( 0, 35, lcd_test_buf , poke );
	sprintf( lcd_test_buf, "%d", c++);
	XString( 60, 35, lcd_test_buf , poke );
	sprintf( lcd_test_buf, "%.3f", f);
	XString( 85, 35, lcd_test_buf , poke );

}


__attribute__((ramfunc))
void lcd_test_string(void) {

	XFont = xfont_11;
	// create text with symbols:
	XString( 0, 11, formula, poke);
	XString( 0, 22, ciao, poke);

}

__attribute__((ramfunc))
void lcd_test_char(void) {

	int x, y;

	// select font to use:
	XFont = xfont_8;

	x = 0;
	y = 0;
	const char * p = ciao;
	while ( *p ) {
		XChar(x, y, *p, poke);
		x += 8;
		p++;
	}

}

__attribute__((ramfunc))
void lcd_test_2d(void) {

	// paint rectangles with built-in patterns:
	Rect(  0, 48,  15, 63, DOGMLCD_full, poke );
	Rect( 16, 48,  31, 63, DOGMLCD_dark, poke );
	Rect( 32, 48,  47, 63, DOGMLCD_grey, poke );
	Rect( 48, 48,  63, 63, DOGMLCD_lite, poke );

}



