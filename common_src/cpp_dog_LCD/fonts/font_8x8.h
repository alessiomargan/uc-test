#ifndef __EA_FONT_8x8_H__
#define __EA_FONT_8x8_H__

/* File '8X8.FV' as include
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

#define FONT_8X8_LEN  776

//#pragma DATA_SECTION(font_8x8,".fonts")
#pragma DATA_SECTION(".fonts")
unsigned char font_8x8[FONT_8X8_LEN] =
{
   70, 86, 32,127,  8,  8,  1,  8,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 95, 95,  0,  0,  0,
    0,  7,  7,  0,  0,  7,  7,  0,
   36,126,126, 36,126,126, 36,  0,
    0, 36, 46,107,107, 58, 18,  0,
    0,102, 54, 24, 12,102, 98,  0,
   48,122, 79, 93, 55,114, 80,  0,
    0,  0,  0,  7,  7,  0,  0,  0,
    0,  0,  0, 62,127, 99, 65,  0,
    0, 65, 99,127, 62,  0,  0,  0,
    8, 42, 62, 28, 28, 62, 42,  8,
    0,  8,  8, 62, 62,  8,  8,  0,
    0,128,224, 96,  0,  0,  0,  0,
    0,  8,  8,  8,  8,  8,  8,  0,
    0,  0,  0, 96, 96,  0,  0,  0,
    0, 96, 48, 24, 12,  6,  3,  0,
    0, 62,127, 73, 69,127, 62,  0,
    0, 64, 66,127,127, 64, 64,  0,
    0, 66, 99,113, 89, 79, 70,  0,
    0, 33, 97, 69, 79,123, 49,  0,
    0, 24, 28, 22,127,127, 16,  0,
    0, 39,103, 69, 69,125, 57,  0,
    0, 62,127, 73, 73,121, 48,  0,
    0,  1,113,121, 13,  7,  3,  0,
    0, 54,127, 73, 73,127, 54,  0,
    0,  6, 79, 73,105, 63, 30,  0,
    0,  0,  0, 54, 54,  0,  0,  0,
    0,  0, 64,118, 54,  0,  0,  0,
    0,  0,  8, 28, 54, 99, 65,  0,
    0, 36, 36, 36, 36, 36, 36,  0,
    0, 65, 99, 54, 28,  8,  0,  0,
    0,  2,  3, 81, 89, 15,  6,  0,
    0, 62,127, 65, 93, 87, 94,  0,
    0,124,126, 19, 19,126,124,  0,
    0,127,127, 73, 73,127, 54,  0,
    0, 62,127, 65, 65, 99, 34,  0,
    0,127,127, 65, 99, 62, 28,  0,
    0,127,127, 73, 73, 73, 65,  0,
    0,127,127,  9,  9,  9,  1,  0,
    0, 62,127, 65, 73,121,121,  0,
    0,127,127,  8,  8,127,127,  0,
    0,  0, 65,127,127, 65,  0,  0,
    0, 32, 96, 64, 64,127, 63,  0,
    0,127,127, 28, 54, 99, 65,  0,
    0,127,127, 64, 64, 64, 64,  0,
  127,127,  6, 12,  6,127,127,  0,
    0,127,127, 14, 28,127,127,  0,
    0, 62,127, 65, 65,127, 62,  0,
    0,127,127,  9,  9, 15,  6,  0,
    0, 62,127, 81, 33,127, 94,  0,
    0,127,127,  9, 25,127,102,  0,
    0, 38,111, 73, 73,123, 50,  0,
    0,  1,  1,127,127,  1,  1,  0,
    0, 63,127, 64, 64,127,127,  0,
    0, 31, 63, 96, 96, 63, 31,  0,
  127,127, 48, 24, 48,127,127,  0,
    0, 99,119, 28, 28,119, 99,  0,
    0,  7, 15,120,120, 15,  7,  0,
    0, 97,113, 89, 77, 71, 67,  0,
    0,  0,  0,127,127, 65, 65,  0,
    0,  3,  6, 12, 24, 48, 96,  0,
    0, 65, 65,127,127,  0,  0,  0,
    8, 12,  6,  3,  6, 12,  8,  0,
   64, 64, 64, 64, 64, 64, 64,  0,
    2,  6, 12,  8,  0,  0,  0,  0,
    0, 32,116, 84, 84,124,120,  0,
    0,127,127, 68, 68,124, 56,  0,
    0, 56,124, 68, 68, 68,  0,  0,
    0, 56,124, 68, 68,127,127,  0,
    0, 56,124, 84, 84, 92, 24,  0,
    0,  4,126,127,  5,  5,  0,  0,
    0,152,188,164,164,252,124,  0,
    0,127,127,  4,  4,124,120,  0,
    0,  0, 68,125,125, 64,  0,  0,
    0,128,128,253,125,  0,  0,  0,
    0,127,127, 16, 56,108, 68,  0,
    0,  0, 65,127,127, 64,  0,  0,
  124,124, 12, 24, 12,124,120,  0,
    0,124,124,  4,  4,124,120,  0,
    0, 56,124, 68, 68,124, 56,  0,
    0,252,252, 68, 68,124, 56,  0,
    0, 56,124, 68, 68,252,252,  0,
    0,124,124,  4,  4, 12,  8,  0,
    0, 72, 92, 84, 84,116, 36,  0,
    0,  4,  4, 62,126, 68, 68,  0,
    0, 60,124, 64, 64,124,124,  0,
    0, 28, 60, 96, 96, 60, 28,  0,
   28,124, 96, 48, 96,124, 28,  0,
    0, 68,108, 56, 56,108, 68,  0,
    0,156,188,160,160,252,124,  0,
    0, 68,100,116, 92, 76, 68,  0,
    0,  0,  8, 62,119, 65, 65,  0,
    0,  0,  0,255,255,  0,  0,  0,
    0, 65, 65,119, 62,  8,  0,  0,
   12,  6,  6, 12, 24, 24, 12,  0,
    0, 96,120, 94, 70, 88, 96,  0
};

#endif
