#include "doggy.h"

void DogMLCD::LineH( int x0, int y, int x1, doggy_op raster )
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
void DogMLCD::LineV( int x, int y0, int y1, doggy_op raster )
{
    if( x & 0xFF80 ) return; // line is out of screen
    BOUND( y0, 0, 63 )
    BOUND( y1, 0, 63 )
    ORDER( y0, y1 )
    RasterOp op = ((RasterOp[]){ &DogMLCD::Poke, &DogMLCD::Wipe, &DogMLCD::Inv })[raster];
    for( int y = y0 ; y <= y1 ; y++ ) 
        (this->*op)( x, y );
}
void DogMLCD::Line( int x0, int y0, int x1, int y1, doggy_op raster )
{
    bool steep = abs( y1 - y0 ) > abs( x1 - x0 );
    if( steep ) { SWAP( x0, y0 ) SWAP( x1, y1 ) }
    if( x0 > x1) { SWAP( x0, x1 ) SWAP( y0, y1 ) }

    int dx = x1 - x0;
    int dy = abs( y1 - y0 );
    int e = dx / 2;
    int ystep = y0 < y1 ? 1 : -1;
    RasterOp op = ((RasterOp[]){ &DogMLCD::Poke, &DogMLCD::Wipe, &DogMLCD::Inv })[raster];
    
    if( steep ) for( int x = x0, y = y0 ; x <= x1 ; x++ )
    {
        (this->*op)( y, x );
        if( ( e -= dy ) < 0 )
        {
            y += ystep;
            e += dx;
        }
    }
    else for( int x = x0, y = y0 ; x <= x1 ; x++ )
    {
        (this->*op)( x, y );
        if( ( e -= dy ) < 0 )
        {
            y += ystep;
            e += dx;
        }
    }
}
void DogMLCD::Frame( int x0, int y0, int x1, int y1, doggy_op raster )
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
void DogMLCD::Rect( int x0, int y0, int x1, int y1, const unsigned char* p, doggy_op raster )
{
    BOUND( x0, 0, 127 )
    BOUND( x1, 0, 127 )
    BOUND( y0, 0, 63 )
    BOUND( y1, 0, 63 )
    ORDER( x0, x1 )
    ORDER( y0, y1 )
    RasterOp op = ((RasterOp[]){ &DogMLCD::Poke, &DogMLCD::Wipe, &DogMLCD::Inv })[raster];
    for( int x = x0 ; x <= x1 ; x++ )
        for( int y = y0 ; y <= y1 ; y++ )
            if( p[x & 7] & ( 1 << ( y & 7 ) ) )
                (this->*op)( x, y );
}
