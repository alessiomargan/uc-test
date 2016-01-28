#ifndef __objectlist__
#define __objectlist__

#include "soes/esc_coe.h"
#include "soes_hook.h"

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
const char acName1600_1[] = "Mapped Object 1";
const char acName1600_2[] = "Mapped Object 2";
const char acName1600_3[] = "Mapped Object 3";
const char acName1A00[] = "Transmit PDO Mapping";
const char acName1A00_0[] = "Number of Elements";
const char acName1A00_1[] = "Mapped Object 1";
const char acName1A00_2[] = "Mapped Object 2";
const char acName1A00_3[] = "Mapped Object 3";
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
const char acName7000[] = "Outputs";
const char acName8000[] = "Parameter";
const char acName8001[] = "Flash Par";

const _objd SDO1000[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1000_0, 0x01901389, 0},
};
const _objd SDO1008[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 72, ATYPE_RO, acName1008_0, 0, "slavedemo"},
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
  {0x0, DTYPE_UNSIGNED8,  8,  ATYPE_RO, acName1018_0, 4, 0},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_1, 0x298, 0},	// Vendor ID
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_2, 0xb007, 0},	// Product code
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_3, 0, 0},		// Revision number
  {0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_4, 0, 0},		// Serial number
};

const _objd SDO1C00[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_0, 4, 0},
  {0x1, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_1, 1, 0},
  {0x2, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_2, 2, 0},
  {0x3, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_3, 3, 0},
  {0x4, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_4, 4, 0},
};

const _objd SDO1C12[] =
{
  {0x0, DTYPE_UNSIGNED8, 8,   ATYPE_RO, acName1C12_0, 1, 0},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_1, 0x1600, 0},
};

// RxPdo 1600 
const _objd SDO1600[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, acName1600_0, 0, 0},
};

const _objd SDO1C13[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_0, 1, 0},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_1, 0x1A00, 0},
};

// TxPdo
const _objd SDO1A00[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "Number of Elements", 0, 0},
};

const _objd SDO6000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "Number of Elements", 0, 0},
};

const _objd SDO7000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "Number of Elements", 0, 0},
};

const _objd SDO8001[] =
{
  {0x0, DTYPE_UNSIGNED8,     	 8, ATYPE_RO,   "Number of Elements",   6, 0},
  {0x1, DTYPE_UNSIGNED16,   	16, ATYPE_RW, 	"flash_cmd",  		0, &gFlash_cmd},
  {0x2, DTYPE_UNSIGNED16,   	16, ATYPE_RO, 	"flash_cmd_ack",	0, &gFlash_cmd_ack},
  {0x3, DTYPE_UNSIGNED32,   	32, ATYPE_RO, 	"flash_crc",  		0, &gFlash_crc},
  {0x4, DTYPE_VISIBLE_STRING,   64, ATYPE_RO,   "bl_ver",   		0, &gBL_ver},
  {0x5, DTYPE_UNSIGNED16,   	16, ATYPE_RO, 	"crc_ok",  			0, &gCrc_ok},
  {0x6, DTYPE_UNSIGNED16,   	16, ATYPE_RO, 	"et1100_boot_pin",0, &gET1100_boot_pin},

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
  {0xffff, 0xff, 0xff, 0xff, 0, 0}
};

#endif
