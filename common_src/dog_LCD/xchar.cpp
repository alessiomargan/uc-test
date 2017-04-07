#include "doggy.h"

#include <stdio.h>

XGlyph DogMLCD::GetGlyph( int code )
{
    if( code > 31 && code < 256 )
        return XFont[code - 31];
    return XFont[0];
}

void DogMLCD::XChar( int x, int y, int code, doggy_op raster )
{
    XChar( x, y, GetGlyph( code ) );
}
void DogMLCD::XChar( int x, int y, XGlyph f, doggy_op raster )
{
    const char* p = f.bmp;
    RasterOp op = ((RasterOp[]){ &DogMLCD::Poke, &DogMLCD::Wipe, &DogMLCD::Inv })[raster];
    int y1 = y + f.hei;
    for( int w = 0 ; w < f.wid ; w++ )
    {
        for( int z = y, m = 1 ; z < y1 ; z++ )
        {
            if( m == 256 ) { m = 1; p++; }
            if( *p & m )
                (this->*op)( x, z );
            m <<= 1;
        }
        p++;
        x++;
    }
}
int DogMLCD::XString( int x, int y, int i, doggy_op raster )
{
    char buf[32];
    sprintf( buf, "%d", i );
    return XString( x, y, buf, raster );
}
int DogMLCD::XString( int x, int y, float f, doggy_op raster )
{
    char buf[32];
    sprintf( buf, "%f", f );
    return XString( x, y, buf, raster );
}
int DogMLCD::XString( int x, int y, const char* s, doggy_op raster )
{
    int oldx = x;
    int trc = 0, brc = 0, inter = XFont[0].hei;
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
                    XChar( x, y, f, raster );
                    x += f.wid + 1;
                 }
            }
            else
            {
                XChar( x, y, f, raster );
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
