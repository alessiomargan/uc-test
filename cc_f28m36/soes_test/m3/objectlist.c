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
const char acName1600_n[] = "Mapped Object";
const char acName1A00[] = "Transmit PDO Mapping";
const char acName1A00_0[] = "Number of Elements";
const char acName1A00_1[] = "Mapped Object 1";
const char acName1A00_2[] = "Mapped Object 2";
const char acName1A00_3[] = "Mapped Object 3";
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
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1018_0, 4, 0},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_1, 664, 0},
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_2, 1234, 0},
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_3, 0, 0},
  {0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_4, 0x00000000, 0},
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
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C12_0, 1, 0},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_1, 0x1600, 0},
};
const _objd SDO1C13[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_0, 1, 0},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_1, 0x1A00, 0},
};

// RxPdo 1600
const _objd SDO1600[] =
{
	{0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "NumElem", 12, 0},
	{0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000120, 0},
	{0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000210, 0},
	{0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000310, 0},
	{0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000410, 0},
	{0x5, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000510, 0},
	{0x6, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000610, 0},
	{0x7, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000710, 0},
	{0x8, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000810, 0},
	{0x9, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000910, 0},
	{0xa, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000a10, 0},
	{0xb, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000b10, 0},
	{0xc, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_n, 0x70000c20, 0},
};

const _objd SDO7000[] =
{
	{0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "NumElem", 12, 0},
	{0x1, DTYPE_REAL32,     32, ATYPE_RW, "Pos_ref",		0, &rx_pdo.pos_ref},
	{0x2, DTYPE_INTEGER16,  16, ATYPE_RW, "Vel_ref",		0, &rx_pdo.vel_ref},
	{0x3, DTYPE_INTEGER16,  16, ATYPE_RW, "Tor_ref",		0, &rx_pdo.tor_ref},
	{0x4, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Gain_0", 		0, &rx_pdo.gains[0]}, // kp
	{0x5, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Gain_1",			0, &rx_pdo.gains[1]}, // kd
	{0x6, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Gain_2", 		0, &rx_pdo.gains[2]},
	{0x7, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Gain_3", 		0, &rx_pdo.gains[3]},
	{0x8, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Gain_4", 		0, &rx_pdo.gains[4]},
	{0x9, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Fault_Ack", 		0, &rx_pdo.fault_ack},
	{0xa, DTYPE_UNSIGNED16, 16, ATYPE_RW, "ts", 			0, &rx_pdo.ts},
	{0xb, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Op_idx_aux", 	0, &rx_pdo.op_idx_aux},
	{0xc, DTYPE_REAL32,     32, ATYPE_RW, "Aux",			0, &rx_pdo.aux},
};

// TxPdo
const _objd SDO1A00[] =
{
	{0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "NumElem", 10, 0},
	{0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000120, 0},
	{0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000220, 0},
	{0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000320, 0},
	{0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000410, 0},
	{0x5, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000510, 0},
	{0x6, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000610, 0},
	{0x7, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000710, 0},
	{0x8, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000810, 0},
	{0x9, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000910, 0},
	{0xa, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_n, 0x60000a20, 0},
};

const _objd SDO6000[] =
{
	{0x0, DTYPE_UNSIGNED8,    8, ATYPE_RO, "NumElem", 10, 0},
	{0x1, DTYPE_REAL32,      32, ATYPE_RO, "Link_position",			0, &tx_pdo.link_pos},
	{0x2, DTYPE_REAL32,      32, ATYPE_RO, "Motor_position",		0, &tx_pdo.motor_pos},
	{0x3, DTYPE_REAL32,      32, ATYPE_RO, "Link_velocity",			0, &tx_pdo.link_vel},
	{0x4, DTYPE_INTEGER16,   16, ATYPE_RO, "Motor_velocity", 		0, &tx_pdo.motor_vel},
	{0x5, DTYPE_INTEGER16,   16, ATYPE_RO, "Torque", 				0, &tx_pdo.torque},
	{0x6, DTYPE_UNSIGNED16,  16, ATYPE_RO, "Temperature", 			0, &tx_pdo.max_temperature},
	{0x7, DTYPE_UNSIGNED16,  16, ATYPE_RO, "Fault", 				0, &tx_pdo.fault},
	{0x8, DTYPE_UNSIGNED16,  16, ATYPE_RO, "Rtt", 					0, &tx_pdo.rtt},
	{0x9, DTYPE_UNSIGNED16,  16, ATYPE_RO, "Op_idx_ack", 			0, &tx_pdo.op_idx_ack},
	{0xa, DTYPE_REAL32,  	 32, ATYPE_RO, "Aux", 					0, &tx_pdo.aux},
};



const _objd SDO8000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "NumElem", 6, 0},
  {0x1, DTYPE_REAL32,  32, ATYPE_RW, "PosGainP", 0, &sdo.PosGainP},
  {0x2, DTYPE_REAL32,  32, ATYPE_RW, "PosGainI", 0, &sdo.PosGainI},
  {0x3, DTYPE_REAL32,  32, ATYPE_RW, "PosGainD", 0, &sdo.PosGainD},
  {0x4, DTYPE_REAL32,  32, ATYPE_RW, "TorGainP", 0, &sdo.TorGainP},
  {0x5, DTYPE_REAL32,  32, ATYPE_RW, "TorGainI", 0, &sdo.TorGainI},
  {0x6, DTYPE_REAL32,  32, ATYPE_RW, "TorGainD", 0, &sdo.TorGainD},
};

const _objd SDO8001[] =
{
  {0x0,  DTYPE_UNSIGNED8,        8,  ATYPE_RO, "NumElem",  6, 0},
  {0x1,  DTYPE_VISIBLE_STRING,   64, ATYPE_RO, "fw_ver",  			0, &sdo.fw_ver},
  {0x2,  DTYPE_UNSIGNED32,       32, ATYPE_RO, "board_fault",  		0, &sdo.board_faults},
  {0x3,  DTYPE_UNSIGNED16,       16, ATYPE_RW, "ctrl_status_cmd",  	0, &sdo.ctrl_status_cmd},
  {0x4,  DTYPE_UNSIGNED16,       16, ATYPE_RO, "ctrl_status_cmd_ack",  	0, &sdo.ctrl_status_cmd_ack},
  {0x5,  DTYPE_UNSIGNED16,       16, ATYPE_RW, "flash_params_cmd",  	0, &sdo.flash_params_cmd},
  {0x6,  DTYPE_UNSIGNED16,       16, ATYPE_RO, "flash_params_cmd_ack", 	0, &sdo.flash_params_cmd_ack},
};

const _objd SDO8002[] =
{
  {0x0,  DTYPE_UNSIGNED8,    8,  ATYPE_RO, "NumElem",  4, 0},
  {0x1,  DTYPE_REAL32,   	32, ATYPE_RW, "volt_ref",  		0, &aux_pdo.volt_ref},
  {0x2,  DTYPE_REAL32,      32, ATYPE_RO, "current",  		0, &aux_pdo.current},
  {0x3,  DTYPE_REAL32,      32, ATYPE_RO, "vout",  			0, &aux_pdo.vout},
  {0x4,  DTYPE_REAL32,      32, ATYPE_RO, "pos_ref_fb",  	0, &aux_pdo.pos_ref_fb},
};

const _objectlist SDOobjects[] =
{
	{0x1000, OTYPE_VAR, 	0,  0, acName1000, SDO1000},
	{0x1008, OTYPE_VAR, 	0,  0, acName1008, SDO1008},
	{0x1009, OTYPE_VAR, 	0,  0, acName1009, SDO1009},
	{0x100A, OTYPE_VAR,		0,  0, acName100A, SDO100A},
	{0x1018, OTYPE_RECORD,	4,  0, acName1018,	SDO1018},
	{0x1600, OTYPE_RECORD, 12,	0, "RxPDOmap",	SDO1600},
	{0x1A00, OTYPE_RECORD, 10,	0, "TxPDOmap",	SDO1A00},
	{0x1C00, OTYPE_ARRAY,	4,  0, acName1C00,	SDO1C00},
	{0x1C12, OTYPE_ARRAY,  	1,  0, acName1C12,	SDO1C12},
	{0x1C13, OTYPE_ARRAY,  	1,  0, acName1C13,	SDO1C13},
	{0x6000, OTYPE_RECORD, 10,	0, "Inputs",	SDO6000},
	{0x7000, OTYPE_RECORD, 12,	0, "Outputs", 	SDO7000},
	{0x8000, OTYPE_RECORD,	6,	0, "FlashPar",	SDO8000},
	{0x8001, OTYPE_RECORD,  6,	0, "RamPar",	SDO8001},
	{0x8002, OTYPE_RECORD,  4,	0, "AuxPdo",	SDO8002},
	{0xffff, 0xff, 0xff, 0xff,	0, 0}
};

#endif
