#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

/// SPI frequency for method DogMLCD::Flush()
#define DOGMLCD_MHZ 5000000
// Wait time after changing state of A0 in uS
#define DOGMLCD_TIME 1

#ifndef BOUND
    #define BOUND(a,b,c) if( a < b ) a = b; else if( a > c ) a = c;
#endif
#ifndef SWAP
    #define SWAP(a,b) { int c = a; a = b; b = c; }
#endif
#ifndef ORDER
    #define ORDER(a,b) if( a > b ) { int c = a; a = b; b = c; }
#endif


/// Bit mpath operation (raster)
typedef enum{ poke, wipe, inv } doggy_op;

#endif
