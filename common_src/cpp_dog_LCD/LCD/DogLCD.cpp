#include "DogLCD.h"

///////////////////////////////////////////////////////////////////////////////
// non portable function
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

#include "pins.h"

#ifdef PART_TM4C123AH6PM
void wait_us(uint32_t us) { SysCtlDelay( (SysCtlClockGet() / (4*1000000)) * us);  }
#else
//inline void wait_us(uint32_t us) { SysCtlDelay( (SysCtlClockGet(SYSTEM_CLOCK_SPEED) / (4*1000000)) * us);  }
void wait_us(uint32_t us) { return;  }
#endif

void lcs_up(void) 	{ GPIOPinWrite(LCD_CS_BASE, LCD_CS, LCD_CS); }
void lcs_dn(void) 	{ GPIOPinWrite(LCD_CS_BASE, LCD_CS, 0); }
void a0_up(void) 	{ GPIOPinWrite(LCD_A0_BASE, LCD_A0, LCD_A0); }
void a0_dn(void) 	{ GPIOPinWrite(LCD_A0_BASE, LCD_A0, 0); }

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



// macro to make sure x falls into range from low to high (inclusive)
#define CLAMP(x, low, high) { if ( (x) < (low) ) x = (low); if ( (x) > (high) ) x = (high); } while (0);

void DogLCD::_send_commands(const unsigned char* buf, size_t size)
{
    // for commands, A0 is low
	//while( SSIBusy(LCD_SSI_BASE) ) { }
	lcs_dn(); //_cs = 0;
	a0_dn();  //_a0 = 0;
	while ( size-- > 0 ) {
        lcd_spi_write(*buf);
    	//SSIDataPut(LCD_SSI_BASE, *buf);
        buf++;
    }
    lcs_up();  //_cs = 1;
}

void DogLCD::_send_data(const unsigned char* buf, size_t size)
{
    // for data, A0 is high
 	//while( SSIBusy(LCD_SSI_BASE) ) { }
	lcs_dn(); //_cs = 0;
	a0_up();  //_a0 = 1;
 	while ( size-- > 0 ) {
        lcd_spi_write(*buf);
		//SSIDataPut(LCD_SSI_BASE, *buf);
        buf++;
	}
    lcs_up(); //_cs = 1;
    a0_dn();  //_a0 = 0;
}

// set column and page number
void DogLCD::_set_xy(int x, int y)
{
    //printf("_set_xy(%d,%d)\n", x, y);
    CLAMP(x, 0, LCDWIDTH-1);
    CLAMP(y, 0, LCDPAGES-1);
    unsigned char cmd[3];
    cmd[0] = 0xB0 | (y&0xF);
    cmd[1] = 0x10 | (x&0xF);
    cmd[2] = (x>>4)&0xF;
    _send_commands(cmd, 3);        
}

// initialize and turn on the display
void DogLCD::init(unsigned char power_ctrl)
{
    unsigned char init_seq[] = {
            0x40,       // display start line + 0-63
            0xA1,       // ADC set 0xA1 + 0 = normal (for reverse view) / +1 = reverse (for normal view)
            0xC0,       // common output mode + 0 = normal / + 0xF  reverse
            0xA6,       // dispaly mode 0xA6 + 0 = normal / 1 = reverse
            0xA2,       // set bias 0xa20 + 0 = 1/9 / +1 = 1/7
    		0x00,
			//0x2B,     // power control: booster off + 2 regulator on + 1 follower on
    		//0x2F,     // power control: 4 booster on + 2 regulator on + 1 follower on
            0xF8, 0x00, // set booster ratio , value 0=4x, 1=5x, 2=6x
    		0x27,       // set voltage regulator (0x20) to 7
            0x81, 0x14, // set electronic volume , value
            0xAC, 0x00, // static indicator set 0xAC +0 = off / +1 = on , 0 = flash mode
            0xAF        // display 0xAE +0 = off / +1 = on
    };

    init_seq[5] = power_ctrl;

    //printf("Reset=L\n");
    _reset = 0;
    //printf("Power=H\n");
    _power = 1;
    //wait_ms(1);
    //printf("Reset=H\n");
    _reset = 1;
    //wait(5);
    //printf("Sending init commands\n");
    _send_commands(init_seq, sizeof(init_seq));
}

void DogLCD::send_pic(const unsigned char* data)
{
    //printf("Sending picture\n");
    for (int i=0; i<LCDPAGES; i++)
    {
        _set_xy(0, i);
        _send_data(data + i*LCDWIDTH, LCDWIDTH);
    }
}

void DogLCD::clear_screen()
{
    //printf("Clear screen\n");
    memset(_framebuffer, 0, sizeof(_framebuffer));
    if ( _updating == 0 )
    {
        send_pic(_framebuffer);
    }
}

void DogLCD::all_on(bool on)
{
    //printf("Sending all on %d\n", on);
    unsigned char cmd = 0xA4 | (on ? 1 : 0);
    _send_commands(&cmd, 1);
}

void DogLCD::pixel(int x, int y, int colour)
{
    CLAMP(x, 0, LCDWIDTH-1);
    CLAMP(y, 0, LCDHEIGHT-1);
    int page = y / 8;
    unsigned char mask = 1<<(y%8);
    unsigned char *byte = &_framebuffer[page*LCDWIDTH + x];
    if ( colour == 0 )
        *byte &= ~mask; // clear pixel
    else
        *byte |= mask; // set pixel
    if ( !_updating )
    {
        _set_xy(x, page);
        _send_data(byte, 1);
    }
}

void DogLCD::fill(int x, int y, int width, int height, int colour)
{
    /*
      If we need to fill partial pages at the top:
      
      ......+---+---+.....
       ^    | = | = |     = : don't touch
       |    | = | = |     * : update 
      y%8   | = | = |      
       |    | = | = |      
       v    | = | = |      
    y---->  | * | * |      
            | * | * |      
            | * | * |
            +---+---+      
    */
    //printf("fill(x=%d, y=%d, width=%d, height=%d, colour=%x)\n",  x, y, width, height, colour);
    CLAMP(x, 0, LCDWIDTH-1);
    CLAMP(y, 0, LCDHEIGHT-1);
    CLAMP(width, 0, LCDWIDTH - x);
    CLAMP(height, 0, LCDHEIGHT - y);
    int page = y/8;
    int firstpage = page;
    int partpage = y%8;
    if ( partpage != 0 )
    {
        // we need to process partial bytes in the top page
        unsigned char mask = (1<<partpage) - 1; // this mask has 1s for bits we need to leave
        unsigned char *bytes = &_framebuffer[page*LCDWIDTH + x];
        for ( int i = 0; i < width; i++, bytes++ )
        {
          // clear "our" bits
          *bytes &= mask;
          if ( colour != 0 )
            *bytes |= ~mask; // set our bits
        }
        height -= partpage;
        page++;
    }
    while ( height >= 8 )
    {
        memset(&_framebuffer[page*LCDWIDTH + x], colour == 0 ? 0 : 0xFF, width);
        page++;
        height -= 8; 
    } 
    if ( height != 0 )
    {
        // we need to process partial bytes in the bottom page
        unsigned char mask = ~((1<<partpage) - 1); // this mask has 1s for bits we need to leave
        unsigned char *bytes = &_framebuffer[page*LCDWIDTH + x];
        for ( int i = 0; i < width; i++, bytes++ )
        {
          // clear "our" bits
          *bytes &= mask;
          if ( colour != 0 )
            *bytes |= ~mask; // set our bits
        }
        page++;
    }
    //printf("_updating=%d\n", _updating);
    if ( !_updating )
    {
        int laststpage = page;
        for ( page = firstpage; page < laststpage; page++)
        {
            //printf("setting x=%d, page=%d\n", x, page);
            _set_xy(x, page);
            //printf("sending %d bytes at offset %x\n", width, page*LCDWIDTH + x);
            _send_data(&_framebuffer[page*LCDWIDTH + x], width);
        }
    }
}     

void DogLCD::beginupdate()
{
    _updating++;
    //printf("beginupdate: %d\n", _updating);
}

void DogLCD::endupdate()
{    
    _updating--;
    //printf("endupdate: %d\n", _updating);
    if ( _updating == 0 )
    {
        send_pic(_framebuffer);
    }
}

/*
 *
 *
 *
 */

/*----------------------------
Func: picture
Desc: shows a BLH-picture on the display (see BitMapEdit EA LCD-Tools (http://www.lcd-module.de/support.html))
Vars: column (0..127/131) and page(0..3/7), program memory adress of data
------------------------------*/
void DogLCD::picture(unsigned char column, unsigned char page, const unsigned char *pic_adress)
{
	unsigned char c,p;
	unsigned int byte_cnt = 2;
	unsigned char width, page_cnt;

	width = pic_adress[0];
	page_cnt = (pic_adress[1] + 7) / 8; //height in pages, add 7 and divide by 8 for getting the used pages (byte boundaries)

  if(width + column > 128 ) //stay inside display area
		width = 128 - column;
  if(page_cnt + page > 8 )
		page_cnt = 8 - page;

	for(p=0; p<page_cnt; p++)
	{
		_set_xy(column, page+p);
		lcs_dn(); //_cs = 0;
		a0_up();  //_a0 = 1;
		_send_data(&pic_adress[byte_cnt], width);
		byte_cnt += width;
		lcs_up(); //_cs = 1;
	}
}

/*----------------------------
Func: string
Desc: shows string with selected font on position
Vars: column (0..127/131), page(0..3/7),  font adress in programm memory, stringarray
------------------------------*/
void DogLCD::string(unsigned char column, unsigned char page, const unsigned char *font_adress, const char *str)
{
	unsigned int pos_array; 										//Postion of character data in memory array
	unsigned char x, y, column_cnt, width_max;								//temporary column and page adress, couloumn_cnt tand width_max are used to stay inside display area
	unsigned char start_code, last_code, width, page_height, bytes_p_char;	//font information, needed for calculation
	const char *string;


	start_code 	 = font_adress[2];  //get first defined character
	last_code	 = font_adress[3];  //get last defined character
	width		 = font_adress[4];  //width in pixel of one char
	page_height  = font_adress[6];  //page count per char
	bytes_p_char = font_adress[7];  //bytes per char

	if(page_height + page > 8) //stay inside display area
		page_height = 8 - page;

	//The string is displayed character after character. If the font has more then one page,
	//the top page is printed first, then the next page and so on
	for(y = 0; y < page_height; y++)
	{
		_set_xy(column, page+y); //set startpositon and page
		column_cnt = column; //store column for display last column check
		string = str;             //temporary pointer to the beginning of the string to print
		lcs_dn();
		a0_up();
		while(*string != 0)
		{
			if((unsigned char)*string < start_code || (unsigned char)*string > last_code) //make sure data is valid
				string++;
			else
			{
				//calculate positon of ascii character in font array
				//bytes for header + (ascii - startcode) * bytes per char)
				pos_array = 8 + (unsigned int)(*string++ - start_code) * bytes_p_char;
				pos_array += y*width; //get the dot pattern for the part of the char to print

				if(column_cnt + width > 128) //stay inside display area
					width_max = 128-column_cnt;
				else
					width_max = width;

				_send_data(&font_adress[pos_array], width_max);
			}
		}
		lcs_up();
	}
}

