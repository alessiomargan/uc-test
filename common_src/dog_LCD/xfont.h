#ifndef FONT_H
#define FONT_H

typedef struct {
    unsigned char   cod; // unicode value
    unsigned char   wid; // data width in pixels
    unsigned char   hei; // data height in pixels (multiple of 8)
    unsigned char   tlc; // form allows left top crening
    unsigned char   trc; // form allows right top crening
    unsigned char   blc; // form allows left bottom crening
    unsigned char   brc; // form allows right bottom crening
    const char*     bmp;
} XGlyph;

#endif
