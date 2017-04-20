/* mbed DogM Graphic Display Library
 * Copyright (c) 2011 Bernard Escaillas (www.midimetric.com)
 */

#ifndef DOGGY_H
#define DOGGY_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// non portable function
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/ssi.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include "pins.h"

#ifdef PART_TM4C123AH6PM
inline void wait_us(uint32_t us) { SysCtlDelay( (SysCtlClockGet() / (4*1000000)) * us);  }
#else
//inline void wait_us(uint32_t us) { SysCtlDelay( (SysCtlClockGet(SYSTEM_CLOCK_SPEED) / (4*1000000)) * us);  }
inline void wait_us(uint32_t us) { return;  }
#endif

inline void cs_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, LCD_CS); }
inline void cs_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, 0); }
inline void a0_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, LCD_A0); }
inline void a0_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, 0); }
#if 0
inline void rst_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, LCD_RST); }
inline void rst_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, 0); }
inline void pwr_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, LCD_VDD); }
inline void pwr_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, 0); }
#else
inline void rst_up(void) 	{ ; }
inline void rst_dn(void) 	{ ; }
inline void pwr_up(void) 	{ ; }
inline void pwr_dn(void) 	{ ; }
#endif
inline void lcd_spi_write(uint8_t data) {
    SSIDataPut(LCD_SSI_BASE, data);
    while( SSIBusy(LCD_SSI_BASE) ) { }
}
inline int32_t abs(int32_t x)  {
	if (x<0) return (-x);
	else return x;
}


//#include "mbed.h"
#include "globaldefs.h"
#include "xfont.h"
#include "xfont_8.h"
#include "xfont_11.h"
#include "patterns.h"

/// DOGM LCD control class
///
/// Simple drawing and text rendering functions for DOGM-6 LCD
/// It should work also with the DOGL.
/// It does not handles up side down display.
///
/// DogM circuit used in the example:
///
/// 1/ LCD accesseories:
///
///    - LCD is powered from the 3.3V of the mbed. This configuration requires 9 x 1 uF capacitors as
///    shown in the DogM datasheet. Personnaly i used 0.820 uF (or code 824) because i did not have 1 uFs...
///    - Amber backlight requires resistors. The data sheet mentions 3 x 47 Ohms resitors (one on each led).
///    actually, i added a 500 ohms pot and a 47 Ohms in serie to allow for backlight attenuation (and to
///    extend its lifetime. Also, drawing too much current from the mbed 'steels' power from the LCD and
///    reduces contrast...
///
/// 2/ mbed connection:
///
///    - dogm 40 (CS)    -->  mbed p8
///    - dogm 39 (Reset) -->  +3.3V (always off to save one pin)
///    - dogm 38 (A0)    -->  mbed p6 (unused miso pin)
///    - dogm 37 (SCLK)  -->  mbed p7 (SPI clock)
///    - dogm 36 (SI)    -->  mbed p5 (MOSI)
///    - dogm 35 (VDD)   -->  +3.3V
///    - dogm 34 (VVD2)  -->  +3.3V
///    - dogm 33 (VSS)   -->  GND
///    - dogm 32 to 21, see datasheet for condensers connection
///
/// 3/ SPI cnofiguration:
///
///    The datasheet states that the DogM handles data rates up to 10MHz
///    This rate is probably achieved with independant current source for the backlight.
///    This library uses by default a 1 MHz for the one time initialization (in case mbed
///    is powering up also and current is not yet fully stabilized)
///    It uses a 5MHz rate for work screen transfer.
///    You can change this by modifying the #DEFINE DOGMLC_MHZ 5000000
///    (use the #undef / #define pair in your code to redefine this constant)
///    Increase the value to achieve faster transmission.
///    Lower the value if some messages are lost.
///
/// Example:
/// @code
/// #include "mbed.h"
/// #include "doggy.h"
///
/// SPI     spi( p5, NC, p7 ); // MOSI, MISCO, CLK
/// DogMLCD dog( spi, p8, p6 ); // SPI, CS, A0
///
/// int main() 
/// {
///     // select font to use:
///     dog.XFont = xfont_11;
///
///     // transmit currently empty work screen (to clear physical display):
///     dog.Flush();
///
///     // create text with symbols:
///     char formula[] = { 159, '(', 'x', ')', '=', '2', 227, '+', 's', 'i', 'n', '(', 224, ')', '/', 251, 'x' };
///     // output text from point(0,0):
///     dog.XString( 0, 0, formula );
///
///     // create text with variables and ouput from point (0,11):
///     char buf[256];
///     sprintf( buf, "%s=%f", "A", 15.894 );
///     dog.XString( 0, 11, buf );
/// 
///     // paint rectangles with built-in patterns:
///     dog.Rect(  0, 48,  15, 63, DOGMLCD_full );
///     dog.Rect( 16, 48,  31, 63, DOGMLCD_dark );
///     dog.Rect( 32, 48,  47, 63, DOGMLCD_grey );
///     dog.Rect( 48, 48,  63, 63, DOGMLCD_lite );
/// 
///     // transmit work screen to physical screen:
///     dog.Flush();
/// }
/// @endcode
class DogMLCD
{
private:
    char  w_[1024];                                         // work screen
    char* b_;                                               // currently used screen
    //SPI& spi_;                                              // attached SPI instance
    //DigitalOut cs_, a0_;                                    // control pins

public:

// implementation in doggy.cpp:

    /// Xfont assignment, assign example:  XFont = xfont_8; , values { xfont_8 (default), xfont_11 }
    const XGlyph* XFont;
    
    /// Create DogMLCD instance and intialize display
    ///
    /// @param spi Instance object of an initialized SPI port (see SPI library)
    /// @param cs Digital pin output to activate slave SPI
    /// @param a0 Digital pin output to switch DogM from command mode to data mode 
    DogMLCD( void );
    void init(void);
    
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
    void Flush( unsigned char page, int x0 = 0, int x1 = 127 );

    /// Transmit several pages of the virtual screen to physical display
    ///
    /// Physical screen is organized into 8 horizontal bands called pages. Each band is 8 lines high.
    /// Call time is about 150 us + 400 us per page at 5MHz spi frequency
    /// @param page0 number of the first page to transmit, from 0 to 7.
    /// @param page1 number of the last page to transmit, from 1 to 7 (0 means ignore argument).
    void Page( unsigned char page0, unsigned char page1 = 0 );

    /// Paste a custom screen over the internal work screen using raster op
    ///
    /// @param screen pointer to a custom screen made of 1024 chars
    /// @param op raster operation, can be { poke(default), wipe, inv }
    void Paste( char* screen, doggy_op op = poke );
    
// implementation in draw2D.cpp:
    
    /// Draw an horizontal line 
    ///
    /// @param x0 left coordinate from 0 to 127 included
    /// @param y  vertical coordinate from 0 to 63 included
    /// @param x1 right coordinate from 0 to 127 included
    /// @param op bit math operation (raster), values { poke (default), wipe, inv }
    void LineH( int x0, int y, int x1, doggy_op op = poke );

    /// Draw a vertical line 
    ///
    /// @param x horizontal coordinate from 0 to 127 included
    /// @param y0 top coordinate from 0 to 63 included
    /// @param y1 bottom coordinate from 0 to 63 included
    /// @param op bit math operation (raster), values { poke (default), wipe, inv }
    void LineV( int x, int y0, int y1, doggy_op op = poke );
    
    /// Draw an diagonal line
    ///
    /// @param x0 start horizontal coordinate from 0 to 127 included
    /// @param y0 start vertical coordinate from 0 to 63 included
    /// @param x1 end horizontal coordinate from 0 to 127 included
    /// @param y1 end vertical coordinate from 0 to 63 included
    void Line( int x0, int y0, int x1, int y1, doggy_op = poke );
    
    /// Draw an empty rectangle by combining 2 LineH and 2 LineV calls 
    ///
    /// @param x0 top left corner, horizontal coordinate from 0 to 127 included
    /// @param y0 top left corner, vertical coordinate from 0 to 63 included
    /// @param x1 bottom right corner, horizontal coordinate from 0 to 127 included
    /// @param y1 bottom right corner, vertical coordinate from 0 to 63 included
    /// @param op bit math operation (raster), values { poke (default), wipe, inv }
    void Frame( int x0, int y0, int x1, int y1, doggy_op op = poke );

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
    void Rect( int x0, int y0, int x1, int y1, const unsigned char* pattern = DOGMLCD_full, doggy_op op = poke );

// Implementation in xchar.cpp:

    /// Returns the XGlyph structure describing a single character bitmap 
    ///
    /// If code is not found in the font, returns the character of code 0 (an empty square)
    ///
    /// @param code character code 0, 32 ~ 255 (xfont actually implements the extended US ascii character set)
    /// @return an XGlyph structure { code, width, height, ...crening..., ...bitmap chars... }
    XGlyph GetGlyph( int code );

    /// Draw a XFont style character at position (x,y) 
    ///
    /// @param x top left corner, horizontal coordinate from 0 to 127 included
    /// @param y top left corner, vertical coordinate from 0 to 63 included
    /// @param code US extended ascii code
    /// @param op bottom right corner, vertical coordinate from 0 to 63 included
    void XChar( int x, int y, int code, doggy_op op = poke );

    /// Draw a XFont style character at position (x,y) 
    ///
    /// @param x top left corner, horizontal coordinate from 0 to 127 included
    /// @param y top left corner, vertical coordinate from 0 to 63 included
    /// @param f Xglyph structure of the character
    /// @param op bit math operation (raster), values { poke (default), wipe, inv }
    void XChar( int x, int y, XGlyph f, doggy_op op = poke );

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
    int XString( int x, int y, const char* s, doggy_op op = poke );
    int XString( int x, int y, int i, doggy_op = poke );
    int XString( int x, int y, float f, doggy_op = poke );
};

/// Type definition for RasterOp
///
/// is a pointer to a metthod of DogMLCD taking two int arguments
typedef void (DogMLCD::*RasterOp)(int,int);
#endif
