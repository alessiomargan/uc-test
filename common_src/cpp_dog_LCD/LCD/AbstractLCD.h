#ifndef MBED_ABSTRACTLCD_H
#define MBED_ABSTRACTLCD_H
 
//#include "mbed.h"

/* Class: AbstractLCD
 *
 */

class AbstractLCD
{
public:
    virtual ~AbstractLCD() {};
    // return LDC width
    virtual int width() = 0;
    // return LDC height
    virtual int height() = 0;
    // put a pixel on the screen
    virtual void pixel(int x, int y, int colour) = 0;
    // fill a rectangular area
    virtual void fill(int x, int y, int width, int height, int colour) = 0;
    // begin an update sequence: 
    // remember drawing operations but do not update the display
    virtual void beginupdate() = 0;
    // end an update sequence
    // update display to reflect all queued operations
    virtual void endupdate() = 0;
};

#endif
