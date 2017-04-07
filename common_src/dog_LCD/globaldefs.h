#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

/// SPI frequency for method DogMLCD::Flush()
#define DOGMLCD_MHZ 5000000
// Wait time after changing state of A0 in uS
#define DOGMLCD_TIME 8

#ifndef BOUND
    #define BOUND(a,b,c) if( a < b ) a = b; else if( a > c ) a = c;
#endif
#ifndef SWAP
    #define SWAP(a,b) { int c = a; a = b; b = c; }
#endif
#ifndef ORDER
    #define ORDER(a,b) if( a > b ) { int c = a; a = b; b = c; }
#endif

const unsigned char DOGMLCD_on[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
const unsigned char DOGMLCD_off[]= { 254, 253, 251, 247, 239, 223, 191, 127 };

/// Bit mpath operation (raster)
typedef enum{ poke, wipe, inv } doggy_op;

#endif
