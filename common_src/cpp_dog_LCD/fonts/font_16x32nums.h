#ifndef __EA_FONT_16x32NUM_H__
#define __EA_FONT_16x32NUM_H__

/* File '16X32_numbers.FV' as include
The font was generated with EA FontEditor. Please contact ELECTRONIC ASSEMBLY
for more details (techik@lcd-module.de)

 the array starts with a 8 byte header:
  1st Byte: 'F' first 2 bytes are always FV
  2nd Byte: 'V' for FONT VERTICAL
  3rd Byte: First code to define
  4th Byte: Last  code to define
  5th Byte: Width of character in dots
  6th Byte: Height of character in dots
  7th Byte: Height of character in bytes
  8th Byte: Bytes needed for each character (1..255)
            or 0 for big fonts calculate WidthInDots * HeightInBytes
  After that font data will follow */

#define FONT_16X32NUMS_LEN  1096

//#pragma DATA_SECTION(font_16x32num,".fonts")
#pragma DATA_SECTION(".fonts")
unsigned char font_16x32num[FONT_16X32NUMS_LEN] =
{
   70, 86, 42, 58, 16, 32,  4, 64,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  6, 14, 28, 56,112,224,192,224,112, 56, 28, 14,  6,  0,
    0,  0, 48, 56, 28, 14,  7,  3,  1,  3,  7, 14, 28, 56, 48,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,192,192,192,192,192,255,255,255,192,192,192,192,192,  0,
    0,  0,  1,  1,  1,  1,  1,127,127,127,  1,  1,  1,  1,  1,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,254,254,126, 30,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,192,192,192,192,192,192,192,192,192,192,192,192,192,  0,
    0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 28, 62, 62, 28,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,128,128,  0,  0,  0,  0,  0,  0,  0,
    0,  0,192,192,192,192,195,199,199,195,192,192,192,192,192,  0,
    0,  0,  1,  1,  1,  1, 97,241,241, 97,  1,  1,  1,  1,  1,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,224,248,252,252, 62, 30, 30, 30, 62,252,252,248,224,  0,  0,
    0,255,255,255,255,  0,  0,192,240, 60,255,255,255,255,  0,  0,
    0,255,255,255,255, 60, 15,  3,  0,  0,255,255,255,255,  0,  0,
    0,  7, 31, 63, 63,124,120,120,120,124, 63, 63, 31,  7,  0,  0,
    0,  0,128,192,224,240,248,252,254,254,254,252,  0,  0,  0,  0,
    0,  3,  7,  7,  7,  3,  1,255,255,255,255,255,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 63,127,127,127, 63,  0,  0,  0,  0,
    0,112,248,252,124, 62, 30, 30, 30, 62,252,252,248,224,  0,  0,
    0,  0,  0,  0,  0,  0,  0,128,192,240,255,255,255, 63,  0,  0,
    0,128,224,248,252,254,255, 63, 15,  7,  3,  1,  0,  0,  0,  0,
    0, 63,127,127,127,127,120,120,120,120,120,120,120, 48,  0,  0,
    0, 48,120,124, 60, 30, 30, 30, 62,252,252,248,240,192,  0,  0,
    0,  0,  0,128,192,192,192,192,224,255,255,255,127, 31,  0,  0,
    0,  0,  0,  1,  3,  3,  3,  3,  7,255,255,255,254,248,  0,  0,
    0, 12, 30, 62, 60,120,120,120,124, 63, 63, 31, 15,  3,  0,  0,
    0,  0,  0,  0,  0,  0,  0,192,240,252,254,254,254,  0,  0,  0,
    0,  0,  0,192,240,252,255, 63, 15,255,255,255,255,  0,  0,  0,
    0,252,255,255,255,195,192,192,192,255,255,255,255,192,  0,  0,
    0,  1,  1,  1,  1,  1,  1,  1,  1,127,127,127,127,  1,  0,  0,
    0,252,254,254,254, 30, 30, 30, 30, 30, 30, 30, 30, 12,  0,  0,
    0,255,255,255,255,192,224,224,224,224,192,128,  0,  0,  0,  0,
    0,  1,  3,  3,  3,  3,  1,  1,  1,  3,255,255,255,252,  0,  0,
    0, 12, 30, 62, 60,120,120,120,120, 60, 63, 31, 15,  3,  0,  0,
    0,192,240,248,252, 60, 30, 30, 30, 30, 60,124,120, 48,  0,  0,
    0,255,255,255,255,192,224,224,224,224,192,128,  0,  0,  0,  0,
    0,255,255,255,255,  3,  1,  1,  1,  3,255,255,255,252,  0,  0,
    0,  3, 15, 31, 63, 60,120,120,120, 60, 63, 31, 15,  3,  0,  0,
    0, 12, 30, 30, 30, 30, 30, 30, 30, 30,254,254,254,252,  0,  0,
    0,  0,  0,  0,  0,  0,128,224,248,254,255, 63, 15,  3,  0,  0,
    0,  0,  0,  0,248,254,255,255, 15,  3,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0, 63,127,127, 63,  0,  0,  0,  0,  0,  0,  0,  0,
    0,192,240,248,252, 60, 30, 30, 30, 60,252,248,240,192,  0,  0,
    0, 15, 63,127,255,240,224,224,224,240,255,127, 63, 15,  0,  0,
    0,248,254,255,255,  7,  3,  3,  3,  7,255,255,254,248,  0,  0,
    0,  3, 15, 31, 63, 60,120,120,120, 60, 63, 31, 15,  3,  0,  0,
    0,192,240,248,252, 60, 30, 30, 30, 60,252,248,240,192,  0,  0,
    0, 63,255,255,255,192,128,128,128,192,255,255,255,255,  0,  0,
    0,  0,  0,  1,  3,  3,  7,  7,  7,  3,255,255,255,255,  0,  0,
    0, 12, 30, 62, 60,120,120,120,120, 60, 63, 31, 15,  3,  0,  0,
    0,  0,  0,  0,  0,  0,128,128,128,128,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 15, 15, 15, 15,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,248,248,248,248,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

#endif
