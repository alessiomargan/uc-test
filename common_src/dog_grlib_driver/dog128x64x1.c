//*****************************************************************************
//
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"
#include "dog128x64x1.h"

#include <pins.h>

#define LCDWIDTH 128
#define LCDHEIGHT 64
#define LCDPAGES  (LCDHEIGHT+7)/8

uint8_t _framebuffer[LCDWIDTH*LCDPAGES];
int _updating = 0;
//*****************************************************************************
//
// An array that holds a set of commands that are sent to the display when
// it is initialized.
//
//*****************************************************************************
static const uint8_t g_ui8DisplayInitCommands[] = {
	0x40,    //Display start line 0
	0xa1,    //ADC reverse
	0xc0,    //Normal COM0...COM63
	0xa6,    //Display normal
	0xa2,    //Set Bias 1/9 (Duty 1/65)
	0x2b,    //Booster Off, Regulator and Follower On
	//0x2f,    //Booster, Regulator and Follower On
	//0xf8,    //Set internal Booster to 4x
	//0x00,
	0x27,    //Contrast set
	0x81,
	0x16,
	0xac,    //No indicator
	0x00,
	0xaf,    //Display on
};

#define NUM_INIT_BYTES sizeof(g_ui8DisplayInitCommands)

//*****************************************************************************
//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color.
//
// \return Returns the display-driver specific color.
//
// 24-bit format: XXXX XXXX RRRR RRRR GGGG GGGG BBBB BBBB
// 16-bit format: ---- ---- ---- ---- RRRR RGGG GGGB BBBB
//  8-bit format: ---- ---- ---- ---- ---- ---- RRRG GGBB
//
//
//*****************************************************************************
#define DPYCOLORTRANSLATE16(c)  ((((c) & 0x00f80000) >> 8) |                  \
                                 (((c) & 0x0000fc00) >> 5) |                  \
                                 (((c) & 0x000000f8) >> 3))
#define DPYCOLORTRANSLATE8(c)   ((((c) & 0x00e00000) >> 16) |                 \
                                 (((c) & 0x0000e000) >> 11) |                 \
                                 (((c) & 0x000000c0) >> 6))
#define DPYCOLORTRANSLATE DPYCOLORTRANSLATE8

inline void lcs_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, LCD_CS); }
inline void lcs_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, 0); }
inline void a0_up(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, LCD_A0); }
inline void a0_dn(void) 	{ GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, 0); }

inline void lcd_spi_write(uint8_t data) {
    SSIDataPut(LCD_SSI_BASE, data);
    while( SSIBusy(LCD_SSI_BASE) ) { }
}

static void _send_commands(const uint8_t *pi8Cmd, uint32_t ui32Count)
{
    // for commands, A0 is low
	while( SSIBusy(LCD_SSI_BASE) ) { }
	lcs_dn(); //_cs = 0;
	a0_dn();  //_a0 = 0;
	while ( ui32Count-- > 0 ) {
		lcd_spi_write(*pi8Cmd);
	    //SSIDataPut(LCD_SSI_BASE, *pi8Cmd);
	    pi8Cmd++;
	}
	lcs_up();  //_cs = 1;
}

static void _send_data(const uint8_t *pi8Data, uint32_t ui32Count)
{
    // for data, A0 is high
 	while( SSIBusy(LCD_SSI_BASE) ) { }
	lcs_dn(); //_cs = 0;
	a0_up();  //_a0 = 1;
 	while ( ui32Count-- > 0 ) {
        lcd_spi_write(*pi8Data);
		//SSIDataPut(LCD_SSI_BASE, *pi8Data);
		pi8Data++;
	}
    lcs_up(); //_cs = 1;
    a0_dn();  //_a0 = 0;
}

// set column and page number
static void _set_xy(int32_t i32X, int32_t i32Y)
{
	uint8_t cmd[3];
	int page = i32Y/8;
    cmd[0] = 0xB0 | (page&0xF);
    cmd[1] = 0x10 | (i32X&0xF);
    cmd[2] = (i32X>>4)&0xF;
    _send_commands(cmd, 3);
}
//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the pixel.
//! \param i32Y is the Y coordinate of the pixel.
//! \param ui32Value is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void DogPixelDraw(void *pvDisplayData, int32_t i32X, int32_t i32Y,
                      uint32_t ui32Value)
{
	int page = i32Y/8;
	uint8_t mask = 1<<(i32Y%8);
	uint8_t *byte = &_framebuffer[page*LCDWIDTH + i32X];
	if ( ui32Value == 0 )
		*byte &= ~mask; // clear pixel
	else
		*byte |= mask; // set pixel
//	if ( !_updating )
//	{
//		_set_xy(i32X, page);
//		_send_data(byte, 1);
//	}
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the first pixel.
//! \param i32Y is the Y coordinate of the first pixel.
//! \param i32X0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param i32Count is the number of pixels to draw.
//! \param i32BPP is the number of bits per pixel; must be 1, 4, or 8 optionally
//! ORed with various flags unused by this driver.
//! \param pui8Data is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pui8Palette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void DogPixelDrawMultiple(void *pvDisplayData,
		               	   	   	 int32_t i32X, int32_t i32Y, int32_t i32X0,
                                 int32_t i32Count, int32_t i32BPP,
                                 const uint8_t *pui8Data,
                                 const uint8_t *pui8Palette)
{
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X1 is the X coordinate of the start of the line.
//! \param i32X2 is the X coordinate of the end of the line.
//! \param i32Y is the Y coordinate of the line.
//! \param ui32Value is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void DogLineDrawH(void *pvDisplayData, int32_t i32X1, int32_t i32X2, int32_t i32Y,
                      	 uint32_t ui32Value)
{
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param i32X is the X coordinate of the line.
//! \param i32Y1 is the Y coordinate of the start of the line.
//! \param i32Y2 is the Y coordinate of the end of the line.
//! \param ui32Value is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void DogLineDrawV(void *pvDisplayData, int32_t i32X, int32_t i32Y1, int32_t i32Y2,
                         uint32_t ui32Value)
{
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ui32Value is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both i16XMin and
//! i16XMax are drawn, aint32_t with i16YMin and i16YMax).
//!
//! \return None.
//
//*****************************************************************************
static void DogRectFill(void *pvDisplayData, const tRectangle *pRect, uint32_t ui32Value)
{
	uint32_t x = pRect->i16XMin;
	uint32_t y = pRect->i16YMin;
	uint32_t width = pRect->i16XMax - pRect->i16XMin;
	uint32_t height = pRect->i16YMax - pRect->i16YMin;

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
	//CLAMP(x, 0, LCDWIDTH-1);
	//CLAMP(y, 0, LCDHEIGHT-1);
	//CLAMP(width, 0, LCDWIDTH - x);
	//CLAMP(height, 0, LCDHEIGHT - y);
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
		  if ( ui32Value != 0 )
			*bytes |= ~mask; // set our bits
		}
		height -= partpage;
		page++;
	}
	while ( height >= 8 )
	{
		memset(&_framebuffer[page*LCDWIDTH + x], ui32Value == 0 ? 0 : 0xFF, width);
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
		  if ( ui32Value != 0 )
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



//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ui32Value is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static uint32_t DogColorTranslate(void *pvDisplayData, uint32_t ui32Value)
{
    //
    // Translate from a 24-bit RGB color to a 3-3-2 RGB color.
    //
    return(DPYCOLORTRANSLATE(ui32Value));
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.  Since no memory
//! based frame buffer is used for this driver, the flush is a no operation.
//!
//! \return None.
//
//*****************************************************************************
static void DogFlush(void *pvDisplayData)
{
	//ASSERT(pvDisplayData);
    for (int i=0; i<LCDPAGES; i++)
    {
        _set_xy(0, i);
        _send_data(&_framebuffer[i*LCDWIDTH], LCDWIDTH);
    }

}

//*****************************************************************************
//
//! The display structure that describes the driver for the DOG EA
//! CFAL9664-F-B1 OLED panel with SSD 1332 controller.
//
//*****************************************************************************
const tDisplay g_sDog128x64x1 =
{
    sizeof(tDisplay),
    0,
    128,
    64,
    DogPixelDraw,
    DogPixelDrawMultiple,
    DogLineDrawH,
    DogLineDrawV,
    DogRectFill,
    DogColorTranslate,
    DogFlush
};

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the SSD1332 display controller on the panel,
//! preparing it to display data.
//!
//! \return None.
//
//*****************************************************************************
void Dog128x64x1Init(void)
{
    tRectangle sRect;

	// peripherals must be enabled for use.
	SysCtlPeripheralEnable(LCD_SSI_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(LCD_SSI_GPIO_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(LCD_GPIO_SYSCTL_PERIPH);

    // Configure the pin muxing for SSI functions on port
    GPIOPinConfigure(GPIO_PD0_SSI1CLK);
    GPIOPinConfigure(GPIO_PD3_SSI1TX); // MOSI

    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    GPIOPinTypeSSI(LCD_SSI_GPIO_PORTBASE, LCD_SSI_PINS);

    // Configure and enable the SSI port for SPI master mode.
    // Use SSI, system clock supply, idle clock level high and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    SSIConfigSetExpClk(LCD_SSI_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 10000000, 8);
    // Enable the SSI module.
    SSIEnable(LCD_SSI_BASE);

    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_A0);
    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_CS);
    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_RST);
    GPIOPinTypeGPIOOutput(LCD_GPIO_PORTBASE, LCD_VDD);

    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_A0, 0);
    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_CS, LCD_CS);
    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_RST, 0);
    GPIOPinWrite(LCD_GPIO_PORTBASE, LCD_VDD, 0);

    // Send the initial configuration command bytes to the display
    _send_commands(g_ui8DisplayInitCommands, sizeof(g_ui8DisplayInitCommands));
    SysCtlDelay(1000);

    // Fill the entire display with a black rectangle, to clear it.
    sRect.i16XMin = 0;
    sRect.i16XMax = 127;
    sRect.i16YMin = 0;
    sRect.i16YMax = 63;
    DogRectFill(0, &sRect, 0);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
