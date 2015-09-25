#ifndef __objectlist__
#define __objectlist__

extern uint16 	flash_cmd;
extern uint16 	flash_cmd_ack;
extern uint32 	flash_crc;
extern uint16  crc_ok;
extern uint16  et1100_boot_pin;
extern char 	bl_ver[];

typedef struct PACKED
{
   uint16 subindex;
   uint16 datatype;
   uint16 bitlength;
   uint16 access;
   char const *name;
   uint32 value;
   void *data;
} _objd;

typedef struct PACKED
{
   uint16 index;
   uint16 objtype;
   uint8 maxsub;
   uint8 pad1;
   char const *name;
   _objd const *objdesc;
} _objectlist;

//#define _ac FLASHSTORE char
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

const char acName1000[] = "Device Type";
const char acName1000_0[] = "Device Type";
const char acName1008[] = "Manufacturer Device Name";
const char acName1008_0[] = "Manufacturer Device Name";
const char acName1009[] = "Manufacturer Hardware Version";
const char acName1009_0[] = "Manufacturer Hardware Version";
const char acName100A[] = "Manufacturer Software Version";
const char acName100A_0[] = "Manufacturer Software Version";
const char acName1018[] = "Identity Object";
const char acName1018_0[] = "Number of Elements";
const char acName1018_1[] = "Vendor ID";
const char acName1018_2[] = "Product Code";
const char acName1018_3[] = "Revision Number";
const char acName1018_4[] = "Serial Number";
const char acName1600[] = "Receive PDO Mapping";
const char acName1600_0[] = "Number of Elements";
const char acName1600_n[] = "Mapped Object";
const char acName1A00[] = "Transmit PDO Mapping";
const char acName1A00_0[] = "Number of Elements";
const char acName1A00_n[] = "Mapped Object";
const char acName1C00[] = "Sync Manager Communication Type";
const char acName1C00_0[] = "Number of Elements";
const char acName1C00_1[] = "Communications Type SM0";
const char acName1C00_2[] = "Communications Type SM1";
const char acName1C00_3[] = "Communications Type SM2";
const char acName1C00_4[] = "Communications Type SM3";
const char acName1C12[] = "Sync Manager 2 PDO Assignment";
const char acName1C12_0[] = "Number of Elements";
const char acName1C12_1[] = "PDO Mapping";
const char acName1C13[] = "Sync Manager 3 PDO Assignment";
const char acName1C13_0[] = "Number of Elements";
const char acName1C13_1[] = "PDO Mapping";

const char acName6000[] = "Inputs";
const char acName6000_0[] = "Number of Elements";

const char acName7000[] = "Outputs";
const char acName7000_0[] = "Number of Elements";

const char acName8001[] = "Variables";
const char acName8001_0[] = "Number of Elements";

const _objd SDO1000[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1000_0, 0x01901389, nil},
};
const _objd SDO1008[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 72, ATYPE_RO, acName1008_0, 0, "bl_f2833x"},
};
const _objd SDO1009[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 24, ATYPE_RO, acName1009_0, 0, "1.0"},
};
const _objd SDO100A[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 24, ATYPE_RO, acName100A_0, 0, "1.0"},
};
const _objd SDO1018[] =
{
  {0x0, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName1018_0, 4, nil},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_1, 0x298, nil},	// Vendor ID
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_2, 0xb007, nil},	// Product code
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_3, 0, nil},		// Revision number
  {0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_4, 0, nil},		// Serial number
};

const _objd SDO1C00[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_0, 4, nil},
  {0x1, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_1, 1, nil},
  {0x2, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_2, 2, nil},
  {0x3, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_3, 3, nil},
  {0x4, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_4, 4, nil},
};

const _objd SDO1C12[] =
{
  {0x0, DTYPE_UNSIGNED8, 8,   ATYPE_RO, acName1C12_0, 1, nil},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_1, 0x1600, nil},
};

// RxPdo 1600 
const _objd SDO1600[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName1600_0, 0, nil},
};

const _objd SDO1C13[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_0, 1, nil},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_1, 0x1A00, nil},
};

// TxPdo
const _objd SDO1A00[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName1A00_0, 0, nil},
};

const _objd SDO6000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName6000_0, 0, nil},
};

const _objd SDO7000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName7000_0, 0, nil},
};

const _objd SDO8001[] =
{
  {0x0, DTYPE_UNSIGNED8,     	 8, ATYPE_RO,   acName8001_0,   6, nil},
  {0x1, DTYPE_UNSIGNED16,   	16, ATYPE_RW, 	"flash_cmd",  	0, &flash_cmd},
  {0x2, DTYPE_UNSIGNED16,   	16, ATYPE_RO, 	"flash_cmd_ack",0, &flash_cmd_ack},
  {0x3, DTYPE_UNSIGNED32,   	32, ATYPE_RO, 	"flash_crc",  	0, &flash_crc},
  {0x4, DTYPE_VISIBLE_STRING,   64, ATYPE_RO,   "bl_ver",   	0, &bl_ver},
  {0x5, DTYPE_UNSIGNED16,   	16, ATYPE_RO, 	"crc_ok",  		0, &crc_ok},
  {0x6, DTYPE_UNSIGNED16,   	16, ATYPE_RO, 	"et1100_boot_pin",0, &et1100_boot_pin},

};



const _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 	 0,  0, acName1000, SDO1000},
  {0x1008, OTYPE_VAR, 	 0,  0, acName1008, SDO1008},
  {0x1009, OTYPE_VAR, 	 0,  0, acName1009, SDO1009},
  {0x100A, OTYPE_VAR, 	 0,  0, acName100A, SDO100A},
  {0x1018, OTYPE_RECORD, 4,  0, acName1018, SDO1018},
  {0x1600, OTYPE_RECORD, 0,  0, acName1600, SDO1600},
  {0x1A00, OTYPE_RECORD, 0,  0, acName1A00, SDO1A00},
  {0x1C00, OTYPE_ARRAY,  4,  0, acName1C00, SDO1C00},
  {0x1C12, OTYPE_ARRAY,  1,  0, acName1C12, SDO1C12},
  {0x1C13, OTYPE_ARRAY,  1,  0, acName1C13, SDO1C13},
  {0x6000, OTYPE_RECORD, 0,  0, acName6000, SDO6000},
  {0x7000, OTYPE_RECORD, 0,  0, acName7000, SDO7000},
  {0x8001, OTYPE_RECORD, 6,  0, acName8001, SDO8001},
  {0xffff, 0xff, 0xff, 0xff, nil, nil}
};

#endif
