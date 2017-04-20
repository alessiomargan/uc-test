/* mbed DogM Graphic Display Library
 * Copyright (c) 2011 Bernard Escaillas (www.midimetric.com)
 */

#ifndef DOGGY_H
#define DOGGY_H

#include "globaldefs.h"
#include "xfont.h"


extern const unsigned char DOGMLCD_full[];
extern const unsigned char DOGMLCD_dark[];
extern const unsigned char DOGMLCD_grey[];
extern const unsigned char DOGMLCD_lite[];

extern const XGlyph * XFont;
extern XGlyph xfont_8[];
extern XGlyph xfont_11[];

///////////////////////////////////////////////////////////////////////////////
//
// implementation in doggy.cpp:

void Lcd_init(void);

/// Use custom screen buffer
///
/// @param screen pointer to an array of 1024 chars (128 colonnes of 8 pages)
void AttachScreen( char* screen );

/// Stop using custom screen buffer (revet to internal work screen)
void DetachScreen();

/// Activate pixel
///
/// @param x horizontal coordinate from 0 to 127 included
/// @param y vertical coordinate from 0 to 63
void Poke( int x, int y );

/// Clear pixel
///
/// @param x horizontal coordinate from 0 to 127 included
/// @param y vertical coordinate from 0 to 63 included
void Wipe( int x, int y );

/// Invert pixel
///
/// @param x horizontal coordinate from 0 to 127 included
/// @param y vertical coordinate from 0 to 63 included
void Inv( int x, int y );

/// Clear virtual screen
///
/// Note : to clear only a part of the screen, use Rect( ..., DOGMLCD_full, wipe );
void Clear();

/// Transmit virtual screen to physical display
///
/// note: this is the more time consuming method, it should take about 3ms under normal conditions.
/// For faster transmit you can limit it to the part of the screen that has effectively changed
/// with Flush(page) or Flush(y0,y1)
void Flush();

/// Transmit one virtual screen page (or part of it) to physical display
///
/// Physical screen is organized into 8 horizontal bands called pages. Each band is 8 lines high.
/// @param page number of the page to transmit, from 0 to 7.
/// @param x0 horizontal coordinate of first pixel to transmit, from 0 to 127.
/// @param x1 horizontal coordinate of last pixel to transmit, from 0 to 127.
void Flush_pg( unsigned char page, int x0, int x1 );

/// Transmit several pages of the virtual screen to physical display
///
/// Physical screen is organized into 8 horizontal bands called pages. Each band is 8 lines high.
/// Call time is about 150 us + 400 us per page at 5MHz spi frequency
/// @param page0 number of the first page to transmit, from 0 to 7.
/// @param page1 number of the last page to transmit, from 1 to 7 (0 means ignore argument).
void Page( unsigned char page0, unsigned char page1 );

/// Paste a custom screen over the internal work screen using raster op
///
/// @param screen pointer to a custom screen made of 1024 chars
/// @param op raster operation, can be { poke(default), wipe, inv }
void Paste( char* screen, doggy_op op );
    
///////////////////////////////////////////////////////////////////////////////
//
// implementation in draw2D.cpp:
    
/// Draw an horizontal line
///
/// @param x0 left coordinate from 0 to 127 included
/// @param y  vertical coordinate from 0 to 63 included
/// @param x1 right coordinate from 0 to 127 included
/// @param op bit math operation (raster), values { poke (default), wipe, inv }
void LineH( int x0, int y, int x1, doggy_op op );

/// Draw a vertical line
///
/// @param x horizontal coordinate from 0 to 127 included
/// @param y0 top coordinate from 0 to 63 included
/// @param y1 bottom coordinate from 0 to 63 included
/// @param op bit math operation (raster), values { poke (default), wipe, inv }
void LineV( int x, int y0, int y1, doggy_op op );

/// Draw an diagonal line
///
/// @param x0 start horizontal coordinate from 0 to 127 included
/// @param y0 start vertical coordinate from 0 to 63 included
/// @param x1 end horizontal coordinate from 0 to 127 included
/// @param y1 end vertical coordinate from 0 to 63 included
void Line( int x0, int y0, int x1, int y1, doggy_op );

/// Draw an empty rectangle by combining 2 LineH and 2 LineV calls
///
/// @param x0 top left corner, horizontal coordinate from 0 to 127 included
/// @param y0 top left corner, vertical coordinate from 0 to 63 included
/// @param x1 bottom right corner, horizontal coordinate from 0 to 127 included
/// @param y1 bottom right corner, vertical coordinate from 0 to 63 included
/// @param op bit math operation (raster), values { poke (default), wipe, inv }
void Frame( int x0, int y0, int x1, int y1, doggy_op op );

/// Draw a filled rectangle by applying bitmap patterns
///
/// Check patterns.h for built-in patterns names.
/// Use your own pattern by passing (const unsigned char[]){ col 1, col 2...,col 8 }
///
/// @param x0 top left corner, horizontal coordinate from 0 to 127 included
/// @param y0 top left corner, vertical coordinate from 0 to 63 included
/// @param x1 bottom right corner, horizontal coordinate from 0 to 127 included
/// @param y1 bottom right corner, vertical coordinate from 0 to 63 included
/// @param pattern a 8x8 bitmap pattern defined by an array of 8 chars
/// @param op bit math operation (raster), values { poke (default), wipe, inv }
void Rect( int x0, int y0, int x1, int y1, const unsigned char* pattern, doggy_op op );

///////////////////////////////////////////////////////////////////////////////
//
// Implementation in xchar.cpp:


/// Draw a XFont style character at position (x,y)
///
/// @param x top left corner, horizontal coordinate from 0 to 127 included
/// @param y top left corner, vertical coordinate from 0 to 63 included
/// @param code US extended ascii code
/// @param op bottom right corner, vertical coordinate from 0 to 63 included
void XChar( int x, int y, int code, doggy_op op );

/// Draw a XFont style character at position (x,y)
///
/// @param x top left corner, horizontal coordinate from 0 to 127 included
/// @param y top left corner, vertical coordinate from 0 to 63 included
/// @param f Xglyph structure of the character
/// @param op bit math operation (raster), values { poke (default), wipe, inv }
void XChar_glyph( int x, int y, XGlyph f, doggy_op op );

/// Draw a XFont style sequence of characters starting at position (x,y)
///
/// Proportional font : Xfonts are proportionnal : i.e. not all characters have the same width.
///
/// Crening (of Kerning): in most cases, there is a character spacing of one pixel beetween chars.
/// But some character combinations allow space saving. For instance, "T.", T followed by dot does not need
/// the extra pixel to ensure characters are not touching each other. Same for "aV" or "L'" or "=1" .
///
/// New line: string can contain the new line '\\n' or (13)
///
/// Wrapping: if the ouput reaches the right side of the screen, it will wrap to next line at position x.
/// wrapping is not space dependant, it happens anywhere in the string (inside words)
/// if wrapped line happens to begins with a space, the space is skipped
///
/// @param x top left corner, horizontal coordinate from 0 to 127 included
/// @param y top left corner, vertical coordinate from 0 to 63 included
/// @param f Xglyph structure of the character
/// @param op bit math operation (raster), values { poke (default), wipe, inv }
///
/// @return the last y coordinate used to output chars (may be different than initial argument if string was wrapped)
int XString( int x, int y, const char* s, doggy_op op );
int XString_i( int x, int y, int i, doggy_op );
int XString_f( int x, int y, float f, doggy_op );

/// Type definition for RasterOp
///
/// is a pointer to a metthod of DogMLCD taking two int arguments
typedef void (*RasterOp)(int,int);
#endif
