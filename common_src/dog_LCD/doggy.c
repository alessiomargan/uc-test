#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/ssi.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include "patterns.h"
#include "xfont_8.h"
#include "xfont_11.h"

#include "doggy.h"
#include "pins.h"

#pragma DATA_SECTION(wrk_scr,".lcd")
char  	wrk_scr[1024];                                    // work screen
static char * 	b_;                                               // currently used screen
/// Xfont assignment, assign example:  XFont = xfont_8; , values { xfont_8 (default), xfont_11 }
const XGlyph *	XFont;

const unsigned char DOGMLCD_on[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
const unsigned char DOGMLCD_off[]= { 254, 253, 251, 247, 239, 223, 191, 127 };

///////////////////////////////////////////////////////////////////////////////
// non portable function

#ifdef PART_TM4C123AH6PM
void wait_us(uint32_t us) { SysCtlDelay( (SysCtlClockGet() / (4*1000000)) * us);  }
#else
//inline void wait_us(uint32_t us) { SysCtlDelay( (SysCtlClockGet(SYSTEM_CLOCK_SPEED) / (4*1000000)) * us);  }
void wait_us(uint32_t us) { return;  }
#endif

void lcs_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, LCD_CS); }
void lcs_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, 0); }
void a0_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, LCD_A0); }
void a0_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, 0); }
#if 0
void rst_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, LCD_RST); }
void rst_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, 0); }
void pwr_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, LCD_VDD); }
void pwr_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, 0); }
#else
void rst_up(void) 	{ ; }
void rst_dn(void) 	{ ; }
void pwr_up(void) 	{ ; }
void pwr_dn(void) 	{ ; }
#endif
inline void lcd_spi_write(uint8_t data) {
    SSIDataPut(LCD_SSI_BASE, data);
    while( SSIBusy(LCD_SSI_BASE) ) { }
}
inline int32_t abs(int32_t x)  {
	if (x<0) return (-x);
	else return x;
}

///////////////////////////////////////////////////////////////////////////////


void Lcd_init(void)
{
    int i;
	//b_ = (char*)calloc( 1024, 1 );
	XFont = xfont_8;
    b_ = wrk_scr;
    const unsigned char c[] = { 
        0x40,       // display start line + 0-63
        0xa1,       // ADC set 0xA1 + 0 = normal (for reverse view) / +1 = reverse (for normal view)
        0xc0,       // common output mode + 0 = normal / + 0xF  reverse
        0xa6,       // dispaly mode 0xA6 + 0 = normal / 1 = reverse
        0xa2,       // set bias 0xa20 + 0 = 1/9 / +1 = 1/7
		0x2b,       // power control: booster off + 2 regulator on + 1 follower on
		//0x2f,       // power control: 4 booster on + 2 regulator on + 1 follower on
        //0xf8, 0x00, // set booster ratio , value 0=4x, 1=5x, 2=6x
		0x27,       // set voltage regulator (0x20) to 7
        0x81, 0x14, // set electronic volume , value
        0xac, 0x00, // static indicator set 0xAC +0 = off / +1 = on , 0 = flash mode
        0xaf        // display 0xAE +0 = off / +1 = on
    };
    lcs_dn(); //cs_ = 0;
    a0_dn(); //a0_ = 0;
    wait_us( DOGMLCD_TIME );
    for( i = 0 ; i < sizeof( c ); i++ )
        lcd_spi_write( c[i] ); //spi_.write( c[i] );
    lcs_up(); //cs_ = 1;

    //
    wait_us( DOGMLCD_TIME );
	Clear();
	Flush();
}

void AttachScreen( char* screen )
{
    b_ = screen;
}

void DetachScreen()
{
    b_ = wrk_scr;
}

#define FASTPOKE( x, y ) b_[ ( ( y & 56 ) << 4 ) + ( x & 127 ) ] |= DOGMLCD_on[ y & 7 ];
#define FASTWIPE( x, y ) b_[ ( ( y & 56 ) << 4 ) + ( x & 127 ) ] &= DOGMLCD_off[ y & 7 ];
#define FASTINV( x, y ) b_[ ( ( y & 56 ) << 4 ) + ( x & 127 ) ] ^= DOGMLCD_on[ y & 7 ];


void Poke( int x, int y )
{
    if( ( x & 0xFF80 ) == 0 && ( y & 0xFFC0 ) == 0 )
        FASTPOKE( x, y )
}
void Wipe( int x, int y )
{
    if( ( x & 0xFF80 ) == 0 && ( y & 0xFFC0 ) == 0 )
        FASTWIPE( x, y )
}
void Inv( int x, int y )
{
    if( ( x & 0xFF80 ) == 0 && ( y & 0xFFC0 ) == 0 )
        FASTINV( x, y )
}
void Clear()
{
    int i = 1024;
    char* p = b_;
    while( i-- )
        *p++ = 0;
}
void Flush()
{
	int page;
    char* p = b_;
    lcs_dn(); //cs_ = 0;
    for( page = 0xB0 ; page < 0xB8 ; page++ )
    {
    	a0_dn(); //a0_ = 0;
    	wait_us( DOGMLCD_TIME );
        //spi_.write( page ); spi_.write( 0x10 ); spi_.write( 0x00 );
    	lcd_spi_write( page ); lcd_spi_write( 0x10 ); lcd_spi_write( 0x00 );

        a0_up(); //a0_ = 1;
        wait_us( DOGMLCD_TIME );
        int i = 128;
        while( i-- )
            lcd_spi_write( *p++ ); //spi_.write( *p++ );
    }
    lcs_up(); //cs_ = 1;
}
void Flush_pg( unsigned char page, int x0, int x1 )
{
	page &= 7;
    BOUND( x0, 0, 127 );
    BOUND( x1, 0, 127 );
    ORDER( x0 , x1 )
    char* p = b_ + ( page << 7 ) + x0;
    lcs_dn(); //cs_ = 0;
	a0_dn(); //a0_ = 0;
	wait_us( DOGMLCD_TIME );
    //spi_.write( 0xB0 + page ); spi_.write( 0x10 ); spi_.write( 0x00 );
	lcd_spi_write( 0xB0+page ); lcd_spi_write( 0x10 ); lcd_spi_write( 0x00 );
        
	a0_up(); //a0_ = 1;
	wait_us( DOGMLCD_TIME );
	int i = x1 - x0 + 1;
    while( i-- )
    	lcd_spi_write( *p++ ); //spi_.write( *p++ );
    lcs_up(); //cs_ = 1;
}
void Page( unsigned char page0, unsigned char page1 )
{
    int page;
	page0 &= 7;
    if( page1 == 0 ) page1 = page0;
    else
    {
        page1 &= 7;
        ORDER( page0, page1 );
    }
    char* p = b_ + ( page0 << 7 );
    lcs_dn(); //cs_ = 0;
    for( page = 0xB0 + page0 ; page <= ( 0xB0 + page1 ) ; page++ )
    {
        //a0_ = 0;
    	a0_dn(); //a0_ = 0;
		wait_us( DOGMLCD_TIME );
		//spi_.write( page ); spi_.write( 0x10 ); spi_.write( 0x00 );
    	lcd_spi_write( page ); lcd_spi_write( 0x10 ); lcd_spi_write( 0x00 );
        
        a0_up(); //a0_ = 1;
        wait_us( DOGMLCD_TIME );
        int i = 128;
        while( i-- )
        	lcd_spi_write( *p++ ); //spi_.write( *p++ );
    }
    lcs_up(); //cs_ = 1;
}
void Paste( char* screen, doggy_op op )
{
    int i = 1024;
    char* p = b_;
    char* q = screen;

    if( op == poke )       while( i-- )  *p++ |= *q++;
    else if( op == wipe )  while( i-- )  *p++ &= ~(*q++);
    else                   while( i-- )  *p++ ^= *q++;
}


///////////////////////////////////////////////////////////////////////////////
//
//


void LineH( int x0, int y, int x1, doggy_op raster )
{
    if( y & 0xFFC0 ) return; // line is out of screen
    BOUND( x0, 0, 127 )
    BOUND( x1, 0, 127 )
    ORDER( x0, x1 )

    int n = x1 - x0 + 1;
    char* p = b_ + (( y & 0x38 ) << 4) + x0;
    if( raster == poke )
    {
        char b = DOGMLCD_on[ y & 7 ];
        while( n-- )
            *p++ |= b;
    }
    else if( raster == wipe )
    {
        char b = DOGMLCD_off[ y & 7 ];
        while( n-- )
            *p++ &= b;
    }
    else
    {
        char b = DOGMLCD_on[ y & 7 ];
        while( n-- )
            *p++ ^= b;
    }
}
void LineV( int x, int y0, int y1, doggy_op raster )
{
    int y;
	if( x & 0xFF80 ) return; // line is out of screen
    BOUND( y0, 0, 63 )
    BOUND( y1, 0, 63 )
    ORDER( y0, y1 )
    RasterOp op = ((RasterOp[]){ &Poke, &Wipe, &Inv })[raster];
    for( y = y0 ; y <= y1 ; y++ )
        (*op)( x, y );
}
void Line( int x0, int y0, int x1, int y1, doggy_op raster )
{
    int x,y;
	bool steep = abs( y1 - y0 ) > abs( x1 - x0 );
    if( steep ) { SWAP( x0, y0 ) SWAP( x1, y1 ) }
    if( x0 > x1) { SWAP( x0, x1 ) SWAP( y0, y1 ) }

    int dx = x1 - x0;
    int dy = abs( y1 - y0 );
    int e = dx / 2;
    int ystep = y0 < y1 ? 1 : -1;
    RasterOp op = ((RasterOp[]){ &Poke, &Wipe, &Inv })[raster];

    if( steep ) {
    	for( x = x0, y = y0 ; x <= x1 ; x++ )
    	{
    		(*op)( y, x );
    		if( ( e -= dy ) < 0 ) {
    			y += ystep;
    			e += dx;
    		}
    	}
    } else {
    	for( x = x0, y = y0 ; x <= x1 ; x++ )
    	{
    		(*op)( x, y );
    		if( ( e -= dy ) < 0 ) {
    			y += ystep;
    			e += dx;
    		}
    	}
    }
}
void Frame( int x0, int y0, int x1, int y1, doggy_op raster )
{
    ORDER( x0, x1 )
    ORDER( y0, y1 )

    LineH( x0, y0, x1, raster );
    if( y1 > y0 ) LineH( x0, y1, x1, raster );
    
    y0++; // don't overlap at angles
    y1--;
    if( y1 >= y0 ) // don't overlap if horizontal lines where adjacent
    {
        LineV( x0, y0, y1, raster );
        if( x1 > x0 ) LineV( x1, y0, y1, raster );
    }
}
void Rect( int x0, int y0, int x1, int y1, const unsigned char* p, doggy_op raster )
{
    int x,y;
	BOUND( x0, 0, 127 )
    BOUND( x1, 0, 127 )
    BOUND( y0, 0, 63 )
    BOUND( y1, 0, 63 )
    ORDER( x0, x1 )
    ORDER( y0, y1 )
    RasterOp op = ((RasterOp[]){ &Poke, &Wipe, &Inv })[raster];
    for( x = x0 ; x <= x1 ; x++ )
        for( y = y0 ; y <= y1 ; y++ )
            if( p[x & 7] & ( 1 << ( y & 7 ) ) )
                (*op)( x, y );
}

///////////////////////////////////////////////////////////////////////////////
//
//

XGlyph GetGlyph( int code )
{
    if( code > 31 && code < 256 )
        return XFont[code - 31];
    return XFont[0];
}

void XChar( int x, int y, int code, doggy_op raster )
{
	XGlyph f = GetGlyph( code );
	XChar_glyph(x, y, f, raster);
}

void XChar_glyph( int x, int y, XGlyph f, doggy_op raster )
{

	int w,z,m;
	const char* p = f.bmp;
    RasterOp op = ((RasterOp[]){ &Poke, &Wipe, &Inv })[raster];
    int y1 = y + f.hei;
    for( w = 0 ; w < f.wid ; w++ )
    {
        for( z = y, m = 1 ; z < y1 ; z++ )
        {
            if( m == 256 ) { m = 1; p++; }
            if( *p & m )
                (*op)( x, z );
            m <<= 1;
        }
        p++;
        x++;
    }
}
int XString_i( int x, int y, int i, doggy_op raster )
{
    char buf[32];
    sprintf( buf, "%d", i );
    return XString( x, y, buf, raster );
}
int XString_f( int x, int y, float f, doggy_op raster )
{
    char buf[32];
    sprintf( buf, "%f", f );
    return XString( x, y, buf, raster );
}
int XString( int x, int y, const char * s, doggy_op raster )
{
    int oldx = x;
    int trc = 0;
    int brc = 0;
    int inter = XFont[0].hei;
    while( *s )
    {
        if( *s == 13 )
        {
            x = oldx;
            y += inter;
        }
        else if( *s > 31 )
        {
            XGlyph f = GetGlyph( *s );

            while( ( x > 0 ) && ( trc + f.tlc ) > 0 && ( brc + f.blc ) > 0 )
            {
                x--;
                trc--;
                brc--;
            }

            if( x + f.wid > 128 ) // simplified form of [ x + f.wid - 1 > 127 ]
            {
                x = oldx;
                y += inter;
                if( *s != 32 ) // don't ouput begining space on new line
                {
                    XChar_glyph( x, y, f, raster );
                    x += f.wid + 1;
                 }
            }
            else
            {
            	XChar_glyph( x, y, f, raster );
                x += f.wid + 1;
            }
            if( *s != 32 ) // for space keep crening of previous char
            {
                trc = f.trc;
                brc = f.brc;
            }
        }
        s++;
    }
    return y;
}
