#ifndef __objectlist__
#define __objectlist__

#include "soes_hook.h"

typedef FLASHSTORE struct PACKED
{
   uint16 subindex;
   uint16 datatype;
   uint16 bitlength;
   uint16 access;
   char FLASHSTORE *name;
   uint32 value;
   void *data;
} _objd;

typedef FLASHSTORE struct PACKED
{
   uint16 index;
   uint16 objtype;
   uint8 maxsub;
   uint8 pad1;
   char FLASHSTORE *name;
   _objd FLASHSTORE *objdesc;
} _objectlist;

#define _ac FLASHSTORE char
#define nil 0

#define OTYPE_DOMAIN            0x0002
#define OTYPE_DEFTYPE           0x0005
#define OTYPE_DEFSTRUCT         0x0006
#define OTYPE_VAR               0x0007
#define OTYPE_ARRAY             0x0008
#define OTYPE_RECORD            0x0009

#define DTYPE_BOOLEAN           0x0001
#define DTYPE_INTEGER8          0x0002
#define DTYPE_INTEGER16         0x0003
#define DTYPE_INTEGER32         0x0004
#define DTYPE_UNSIGNED8         0x0005
#define DTYPE_UNSIGNED16        0x0006
#define DTYPE_UNSIGNED32        0x0007
#define DTYPE_REAL32            0x0008
#define DTYPE_VISIBLE_STRING    0x0009
#define DTYPE_OCTET_STRING      0x000A
#define DTYPE_UNICODE_STRING    0x000B
#define DTYPE_INTEGER24         0x0010
#define DTYPE_UNSIGNED24        0x0016
#define DTYPE_INTEGER64         0x0015
#define DTYPE_UNSIGNED64        0x001B
#define DTYPE_REAL64            0x0011
#define DTYPE_PDO_MAPPING       0x0021
#define DTYPE_IDENTITY          0x0023
#define DTYPE_BIT1              0x0030
#define DTYPE_BIT2              0x0031
#define DTYPE_BIT3              0x0032
#define DTYPE_BIT4              0x0033
#define DTYPE_BIT5              0x0034
#define DTYPE_BIT6              0x0035
#define DTYPE_BIT7              0x0036
#define DTYPE_BIT8              0x0037

#define ATYPE_RO                0x07
#define ATYPE_RW                0x3F
#define ATYPE_RWpre             0x0F
#define ATYPE_RXPDO             0x40
#define ATYPE_TXPDO             0x80

_ac acName1000[] = "Device Type";
_ac acName1000_0[] = "Device Type";
_ac acName1008[] = "Manufacturer Device Name";
_ac acName1008_0[] = "Manufacturer Device Name";
_ac acName1009[] = "Manufacturer Hardware Version";
_ac acName1009_0[] = "Manufacturer Hardware Version";
_ac acName100A[] = "Manufacturer Software Version";
_ac acName100A_0[] = "Manufacturer Software Version";
_ac acName1018[] = "Identity Object";
_ac acName1018_0[] = "Number of Elements";
_ac acName1018_1[] = "Vendor ID";
_ac acName1018_2[] = "Product Code";
_ac acName1018_3[] = "Revision Number";
_ac acName1018_4[] = "Serial Number";
_ac acName1600[] = "Receive PDO Mapping";
_ac acName1600_0[] = "Number of Elements";
_ac acName1600_1[] = "Mapped Object 1";
_ac acName1600_2[] = "Mapped Object 2";
_ac acName1600_3[] = "Mapped Object 3";
_ac acName1A00[] = "Transmit PDO Mapping";
_ac acName1A00_0[] = "Number of Elements";
_ac acName1A00_1[] = "Mapped Object 1";
_ac acName1A00_2[] = "Mapped Object 2";
_ac acName1A00_3[] = "Mapped Object 3";
_ac acName1C00[] = "Sync Manager Communication Type";
_ac acName1C00_0[] = "Number of Elements";
_ac acName1C00_1[] = "Communications Type SM0";
_ac acName1C00_2[] = "Communications Type SM1";
_ac acName1C00_3[] = "Communications Type SM2";
_ac acName1C00_4[] = "Communications Type SM3";
_ac acName1C12[] = "Sync Manager 2 PDO Assignment";
_ac acName1C12_0[] = "Number of Elements";
_ac acName1C12_1[] = "PDO Mapping";
_ac acName1C13[] = "Sync Manager 3 PDO Assignment";
_ac acName1C13_0[] = "Number of Elements";
_ac acName1C13_1[] = "PDO Mapping";
_ac acName6000[] = "Inputs";
_ac acName6000_0[] = "Number of Elements";
_ac acName6000_1[] = "type";
_ac acName6000_2[] = "value";
_ac acName6000_3[] = "ts";
_ac acName7000[] = "Outputs";
_ac acName7000_0[] = "Number of Elements";
_ac acName7000_1[] = "bit";
_ac acName7000_2[] = "bits";
_ac acName7000_3[] = "xyz";
_ac acName8000[] = "Parameter";
_ac acName8000_0[] = "Number of Elements";
_ac acName8000_1[] = "par_1";
_ac acName8000_2[] = "par_2";

FLASHSTORE _objd SDO1000[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1000_0, 0x01901389, nil},
};
FLASHSTORE _objd SDO1008[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 72, ATYPE_RO, acName1008_0, 0, "slavedemo"},
};
FLASHSTORE _objd SDO1009[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 24, ATYPE_RO, acName1009_0, 0, "1.0"},
};
FLASHSTORE _objd SDO100A[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 24, ATYPE_RO, acName100A_0, 0, "1.0"},
};
FLASHSTORE _objd SDO1018[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1018_0, 4, nil},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_1, 664, nil},
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_2, 1234, nil},
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_3, 0, nil},
  {0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_4, 0x00000000, nil},
};
#if 0
FLASHSTORE _objd SDO1600[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1600_0, 1, nil},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_1, 0x70000108, nil},
};
FLASHSTORE _objd SDO1A00[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A00_0, 3, nil},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000108, nil},
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_2, 0x60000208, nil},
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_3, 0x60000320, nil},
};
#endif
// RxPdo 1600 
FLASHSTORE _objd SDO1600[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName1600_0, 3, nil},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_1, 0x70000110, nil},
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_2, 0x70000220, nil},
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_3, 0x70000340, nil},
};

// TxPdo
FLASHSTORE _objd SDO1A00[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName1A00_0, 11, nil},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000108, nil},
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_2, 0x60000208, nil},
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_3, 0x60000308, nil},
  {0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000408, nil},
  {0x5, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000510, nil},
  {0x6, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000610, nil},
  {0x7, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000720, nil},
  {0x8, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000820, nil},
  {0x9, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000940, nil},
  {0xa, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000a40, nil},
  {0xb, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_1, 0x60000b20, nil},
};

FLASHSTORE _objd SDO1C00[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_0, 4, nil},
  {0x1, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_1, 1, nil},
  {0x2, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_2, 2, nil},
  {0x3, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_3, 3, nil},
  {0x4, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_4, 4, nil},
};
FLASHSTORE _objd SDO1C12[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C12_0, 1, nil},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_1, 0x1600, nil},
};
FLASHSTORE _objd SDO1C13[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_0, 1, nil},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_1, 0x1A00, nil},
};
#if 0
FLASHSTORE _objd SDO6000[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_0, 3, nil},
  {0x1, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_1, 0, &Rb.button1},
  {0x2, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_2, 0, &Rb.button2},
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6000_3, 0, &Rb.heartbeat},
};
FLASHSTORE _objd SDO7000[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_0, 1, nil},
  {0x1, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName7000_1, 0, &Wb.leds},
};
#endif
FLASHSTORE _objd SDO6000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName6000_0, 11, nil},
  {0x1, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName6000_1, 0, &tx_pdo.____bits},
  {0x2, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName6000_2, 0, &tx_pdo._bits},
  {0x3, DTYPE_INTEGER8,    8, ATYPE_RO, acName6000_3, 0, &tx_pdo._sint},
  {0x4, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName6000_3, 0, &tx_pdo._usint},
  {0x5, DTYPE_INTEGER16,  16, ATYPE_RO, acName6000_3, 0, &tx_pdo._int},
  {0x6, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6000_3, 0, &tx_pdo._uint},
  {0x7, DTYPE_INTEGER32,  32, ATYPE_RO, acName6000_3, 0, &tx_pdo._dint},
  {0x8, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6000_3, 0, &tx_pdo._udint},
  {0x9, DTYPE_INTEGER64,  64, ATYPE_RO, acName6000_3, 0, &tx_pdo._lint},
  {0xa, DTYPE_UNSIGNED64, 64, ATYPE_RO, acName6000_3, 0, &tx_pdo._ulint},
  {0xb, DTYPE_REAL32,     32, ATYPE_RO, acName6000_3, 0, &tx_pdo._real},
};
FLASHSTORE _objd SDO7000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName7000_0, 3, nil},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RW, acName7000_1, 0, &rx_pdo._type},
  {0x2, DTYPE_INTEGER32,  32, ATYPE_RW, acName7000_2, 0, &rx_pdo._value},
  {0x3, DTYPE_UNSIGNED64, 64, ATYPE_RW, acName7000_3, 0, &rx_pdo._ts},
};

FLASHSTORE _objd SDO8000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName8000_0, 2, nil},
  {0x1, DTYPE_INTEGER32,  32, ATYPE_RW, acName8000_1, 0, &par_1},
  {0x2, DTYPE_INTEGER32,  32, ATYPE_RW, acName8000_2, 0, &par_2},
};

FLASHSTORE _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 0, 0, acName1000, SDO1000},
  {0x1008, OTYPE_VAR, 0, 0, acName1008, SDO1008},
  {0x1009, OTYPE_VAR, 0, 0, acName1009, SDO1009},
  {0x100A, OTYPE_VAR, 0, 0, acName100A, SDO100A},
  {0x1018, OTYPE_RECORD, 4, 0, acName1018, SDO1018},
  {0x1600, OTYPE_RECORD, 3, 0, acName1600, SDO1600},
  {0x1A00, OTYPE_RECORD, 11, 0, acName1A00, SDO1A00},
  {0x1C00, OTYPE_ARRAY, 4, 0, acName1C00, SDO1C00},
  {0x1C12, OTYPE_ARRAY, 1, 0, acName1C12, SDO1C12},
  {0x1C13, OTYPE_ARRAY, 1, 0, acName1C13, SDO1C13},
  {0x6000, OTYPE_RECORD, 11, 0, acName6000, SDO6000},
  {0x7000, OTYPE_RECORD, 3, 0, acName7000, SDO7000},
  {0x8000, OTYPE_RECORD, 2, 0, acName8000, SDO8000},
  {0xffff, 0xff, 0xff, 0xff, nil, nil}
};

#endif
