#include "doggy.h"

///////////////////////////////////////////////////////////////////////////////
// non portable function
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/ssi.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include "pins.h"

inline void wait_us(uint32_t us) { SysCtlDelay( (SysCtlClockGet() / 1000000) * us);  }

inline void cs_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, LCD_CS); }
inline void cs_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, 0); }
inline void a0_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, LCD_A0); }
inline void a0_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, 0); }
inline void rst_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, LCD_RST); }
inline void rst_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, 0); }
inline void pwr_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, LCD_VDD); }
inline void pwr_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, 0); }

inline void lcd_spi_write(uint8_t data) {
    uint32_t ret = 0;
    SSIDataPut(LCD_SSI_BASE, data);
    while( SSIBusy(LCD_SSI_BASE) ) { }
    SSIDataGet(LCD_SSI_BASE, &ret);
}

///////////////////////////////////////////////////////////////////////////////


DogMLCD::DogMLCD( void ) {}

void DogMLCD::init(void)
{
    //b_ = (char*)calloc( 1024, 1 );
	XFont = xfont_8;
    memset((void*)w_, 0, sizeof(w_));
	b_ = w_;
    Clear();
    const unsigned char c[] = { 
        0x40,       // display start line + 0-63
        0xa1,       // ADC set 0xA1 + 0 = normal (for reverse view) / +1 = reverse (for normal view)
        0xc0,       // common output mode + 0 = normal / + 0xF  reverse
        0xa6,       // dispaly mode 0xA6 + 0 = normal / 1 = reverse
        0xa2,       // set bias 0xa20 + 0 = 1/9 / +1 = 1/7
        //0x2f,       // power control: 4 booster on + 2 regulator on + 1 follower on
        //0xf8, 0x00, // set booster ratio , value 0=4x, 1=5x, 2=6x
		0x2b,       // power control: 4 booster off + 2 regulator on + 1 follower on
		//0xf8, 0x00, // set booster ratio , value 0=4x, 1=5x, 2=6x
		0x27,       // set voltage regulator (0x20) to 7
        0x81, 0x14, // set electronic volume , value
        0xac, 0x00, // static indicator set 0xAC +0 = off / +1 = on , 0 = flash mode
        0xaf        // display 0xAE +0 = off / +1 = on
    };
    rst_dn();
    pwr_up();
    wait_us(1);
    rst_up();
    cs_dn(); //cs_ = 0;
    a0_dn(); //a0_ = 0;
    wait_us( DOGMLCD_TIME );
    for( int i = 0 ; i < sizeof( c ); i++ )
        lcd_spi_write( c[i] ); //spi_.write( c[i] );
    cs_up(); //cs_ = 1;
}

void DogMLCD::AttachScreen( char* screen )
{
    b_ = screen;
}

void DogMLCD::DetachScreen()
{
    b_ = w_;
}

#define FASTPOKE( x, y ) b_[ ( ( y & 56 ) << 4 ) + ( x & 127 ) ] |= DOGMLCD_on[ y & 7 ];
#define FASTWIPE( x, y ) b_[ ( ( y & 56 ) << 4 ) + ( x & 127 ) ] &= DOGMLCD_off[ y & 7 ];
#define FASTINV( x, y ) b_[ ( ( y & 56 ) << 4 ) + ( x & 127 ) ] ^= DOGMLCD_on[ y & 7 ];


void DogMLCD::Poke( int x, int y )
{
    if( ( x & 0xFF80 ) == 0 && ( y & 0xFFC0 ) == 0 )
        FASTPOKE( x, y )
}
void DogMLCD::Wipe( int x, int y )
{
    if( ( x & 0xFF80 ) == 0 && ( y & 0xFFC0 ) == 0 )
        FASTWIPE( x, y )
}
void DogMLCD::Inv( int x, int y )
{
    if( ( x & 0xFF80 ) == 0 && ( y & 0xFFC0 ) == 0 )
        FASTINV( x, y )
}
void DogMLCD::Clear()
{
    int i = 1024;
    char* p = b_;
    while( i-- )
        *p++ = 0;
}
void DogMLCD::Flush()
{
    char* p = b_;
    //spi_.format( 8, 0 );
    //spi_.frequency( DOGMLCD_MHZ );
    cs_dn(); //cs_ = 0;
    for( int page = 0xB0 ; page < 0xB8 ; page++ )
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
    cs_up(); //cs_ = 1;
}
void DogMLCD::Flush( unsigned char page, int x0, int x1 )
{
    page &= 7;
    BOUND( x0, 0, 127 );
    BOUND( x1, 0, 127 );
    ORDER( x0 , x1 )
    char* p = b_ + ( page << 7 ) + x0;
    //spi_.format( 8, 0 );
    //spi_.frequency( DOGMLCD_MHZ );
    cs_dn(); //cs_ = 0;
	a0_dn(); //a0_ = 0;
	wait_us( DOGMLCD_TIME );
    //spi_.write( 0xB0 + page ); spi_.write( 0x10 ); spi_.write( 0x00 );
	lcd_spi_write( 0xB0+page ); lcd_spi_write( 0x10 ); lcd_spi_write( 0x00 );
        
	a0_up(); //a0_ = 1;
	wait_us( DOGMLCD_TIME );
	int i = x1 - x0 + 1;
    while( i-- )
    	lcd_spi_write( *p++ ); //spi_.write( *p++ );
    cs_up(); //cs_ = 1;
}
void DogMLCD::Page( unsigned char page0, unsigned char page1 )
{
    page0 &= 7;
    if( page1 == 0 ) page1 = page0;
    else
    {
        page1 &= 7;
        ORDER( page0, page1 );
    }
    char* p = b_ + ( page0 << 7 );
    //spi_.format( 8, 0 );
    //spi_.frequency( DOGMLCD_MHZ );
    cs_dn(); //cs_ = 0;
    for( int page = 0xB0 + page0 ; page <= ( 0xB0 + page1 ) ; page++ )
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
    cs_up(); //cs_ = 1;
}
void DogMLCD::Paste( char* screen, doggy_op op )
{
    int i = 1024;
    char* p = b_;
    char* q = screen;

    if( op == poke )       while( i-- )  *p++ |= *q++;
    else if( op == wipe )  while( i-- )  *p++ &= ~(*q++);
    else                   while( i-- )  *p++ ^= *q++;
}


    
