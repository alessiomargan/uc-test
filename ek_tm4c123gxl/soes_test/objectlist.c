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
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "#Elements", 12, 0},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 1", 0x70000120, 0},
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 2", 0x70000210, 0},
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 3", 0x70000310, 0},
  {0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 4", 0x70000410, 0},
  {0x5, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 5", 0x70000510, 0},
  {0x6, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 6", 0x70000610, 0},
  {0x7, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 7", 0x70000710, 0},
  {0x8, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 8", 0x70000810, 0},
  {0x9, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 9", 0x70000910, 0},
  {0xa, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 10",0x70000a10, 0},
  {0xb, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 11",0x70000b10, 0},
  {0xc, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 12",0x70000c20, 0},

};

// RxPdo 1600 
const _objd SDO7000[] =
{
  {0x0, DTYPE_UNSIGNED8,   	8, ATYPE_RO,  "#Elements", 12, 0},
  {0x1, DTYPE_REAL32, 		32, ATYPE_RW, "pos_ref",	0, &rx_pdo.pos_ref},
  {0x2, DTYPE_INTEGER16,  	16, ATYPE_RW, "vel_ref",	0, &rx_pdo.vel_ref},
  {0x3, DTYPE_INTEGER16,  	16, ATYPE_RW, "tor_ref",	0, &rx_pdo.tor_ref},
  {0x4, DTYPE_UNSIGNED16, 	16, ATYPE_RW, "gain_kp_m",	0, &rx_pdo.gain_kp_m},
  {0x5, DTYPE_UNSIGNED16, 	16, ATYPE_RW, "gain_kp_l",	0, &rx_pdo.gain_kp_l},
  {0x6, DTYPE_UNSIGNED16, 	16, ATYPE_RW, "gain_kd_m",	0, &rx_pdo.gain_kd_m},
  {0x7, DTYPE_UNSIGNED16, 	16, ATYPE_RW, "gain_kd_l",	0, &rx_pdo.gain_kd_l},
  {0x8, DTYPE_UNSIGNED16, 	16, ATYPE_RW, "gain_ki",	0, &rx_pdo.gain_ki},
  {0x9, DTYPE_UNSIGNED16, 	16, ATYPE_RW, "fault_ack",	0, &rx_pdo.fault_ack},
  {0xa, DTYPE_UNSIGNED16, 	16, ATYPE_RW, "ts",			0, &rx_pdo.ts},
  {0xb, DTYPE_UNSIGNED16,	16, ATYPE_RW, "op_idx_aux",	0, &rx_pdo.op_idx_aux},
  {0xc, DTYPE_REAL32, 		32, ATYPE_RW, "aux",		0, &rx_pdo.aux},

};

// TxPdo
const _objd SDO1A00[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "#Elements", 10, 0},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 1", 0x60000120, 0},
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 2", 0x60000220, 0},
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 3", 0x60000320, 0},
  {0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 4", 0x60000410, 0},
  {0x5, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 5", 0x60000510, 0},
  {0x6, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 6", 0x60000610, 0},
  {0x7, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 7", 0x60000710, 0},
  {0x8, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 8", 0x60000810, 0},
  {0x9, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 9", 0x60000910, 0},
  {0xa, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Obj 10",0x60000a20, 0},

};
// TxPdo
const _objd SDO6000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "#Elements", 10, 0},
  {0x1, DTYPE_REAL32,		32, ATYPE_RO, "link_pos",		0, &tx_pdo.link_pos},
  {0x2, DTYPE_REAL32,   	32, ATYPE_RO, "motor_pos",		0, &tx_pdo.motor_pos},
  {0x3, DTYPE_REAL32,   	32, ATYPE_RO, "link_vel",		0, &tx_pdo.link_vel},
  {0x4, DTYPE_INTEGER16,  	16, ATYPE_RO, "motor_vel",		0, &tx_pdo.motor_vel},
  {0x5, DTYPE_INTEGER16,  	16, ATYPE_RO, "torque",			0, &tx_pdo.torque},
  {0x6, DTYPE_UNSIGNED16,	16, ATYPE_RO, "temperature",	0, &tx_pdo.temperature},
  {0x7, DTYPE_UNSIGNED16,  	16, ATYPE_RO, "fault",			0, &tx_pdo.fault},
  {0x8, DTYPE_UNSIGNED16,	16, ATYPE_RO, "rtt", 			0, &tx_pdo.rtt},
  {0x9, DTYPE_UNSIGNED16,   16, ATYPE_RO, "op_idx_ack",		0, &tx_pdo.op_idx_ack},
  {0xa, DTYPE_REAL32,     	32, ATYPE_RO, "aux",			0, &tx_pdo.aux},

};

const _objd SDO8000[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "#Elements", 14, 0},
  {0x1, DTYPE_REAL32, 32, ATYPE_RO, "PosGainP",		0, &sdo.PosGainP},
  {0x2, DTYPE_REAL32, 32, ATYPE_RO, "PosGainI",		0, &sdo.PosGainI},
  {0x3, DTYPE_REAL32, 32, ATYPE_RO, "PosGainD",		0, &sdo.PosGainD},
  {0x4, DTYPE_REAL32, 32, ATYPE_RO, "TorGainP",		0, &sdo.TorGainP},
  {0x5, DTYPE_REAL32, 32, ATYPE_RO, "TorGainI",		0, &sdo.TorGainI},
  {0x6, DTYPE_REAL32, 32, ATYPE_RO, "TorGainD",		0, &sdo.TorGainD},
  {0x7, DTYPE_REAL32, 32, ATYPE_RO, "Pos_I_lim",	0, &sdo.Pos_I_lim},
  {0x8, DTYPE_REAL32, 32, ATYPE_RO, "Tor_I_lim",	0, &sdo.Tor_I_lim},
  {0x9, DTYPE_REAL32, 32, ATYPE_RO, "Min_pos",		0, &sdo.Min_pos},
  {0xa, DTYPE_REAL32, 32, ATYPE_RO, "Max_pos",		0, &sdo.Max_pos},
  {0xb, DTYPE_REAL32, 32, ATYPE_RO, "Max_tor",		0, &sdo.Max_tor},
  {0xc, DTYPE_REAL32, 32, ATYPE_RO, "Max_cur",		0, &sdo.Max_cur},
  {0xd, DTYPE_REAL32, 32, ATYPE_RO, "ImpPosGainP",	0, &sdo.ImpedancePosGainP},
  {0xe, DTYPE_REAL32, 32, ATYPE_RO, "ImpPosGainD",	0, &sdo.ImpedancePosGainD},
};

const _objd SDO8001[] =
{
  {0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "#Elements", 9, 0},
  {0x1, DTYPE_VISIBLE_STRING,	64, ATYPE_RO, "fw_ver",     		0, &sdo.fw_ver},
  {0x2, DTYPE_UNSIGNED32,      	32, ATYPE_RW, "ack_board_faults",   0, &sdo.ack_board_faults},
  {0x3, DTYPE_UNSIGNED16,      	16, ATYPE_RW, "ctrl_status_cmd",    0, &sdo.ctrl_status_cmd},
  {0x4, DTYPE_UNSIGNED16,      	16, ATYPE_RO, "ctrl_status_cmd_ack",0, &sdo.ctrl_status_cmd_ack},
  {0x5, DTYPE_REAL32,      		32, ATYPE_RW, "direct_ref",     	0, &sdo.direct_ref},
  {0x6, DTYPE_REAL32,    		32, ATYPE_RO, "abs_pos",        	0, &sdo.abs_pos},
  {0x7, DTYPE_REAL32,    		32, ATYPE_RO, "m_current",      	0, &sdo.m_current},
  {0x8, DTYPE_UNSIGNED16,      	16, ATYPE_RW, "flash_params_cmd",   0, &sdo.flash_params_cmd},
  {0x9, DTYPE_UNSIGNED16,      	16, ATYPE_RO, "flash_params_cmd_ack",0, &sdo.flash_params_cmd_ack},

};

const _objd SDO8002[] =
{
  {0x0,  DTYPE_UNSIGNED8,    8, ATYPE_RO, "NumElem",  	3, 0},
  {0x1,  DTYPE_REAL32,      32, ATYPE_RO, "pos_ref_fb", 0, &aux_pdo.pos_ref_fb},
  {0x2,  DTYPE_REAL32,      32, ATYPE_RO, "iq_ref",     0, &aux_pdo.iq_ref},
  {0x3,  DTYPE_REAL32,      32, ATYPE_RO, "iq_out",     0, &aux_pdo.iq_out}
};

const _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 0, 0, acName1000, SDO1000},
  {0x1008, OTYPE_VAR, 0, 0, acName1008, SDO1008},
  {0x1009, OTYPE_VAR, 0, 0, acName1009, SDO1009},
  {0x100A, OTYPE_VAR, 0, 0, acName100A, SDO100A},
  {0x1018, OTYPE_RECORD,  4, 0, acName1018, SDO1018},
  {0x1600, OTYPE_RECORD, 12, 0,	"RxPDO Map", SDO1600},
  {0x1A00, OTYPE_RECORD, 10, 0,	"TxPDO Map", SDO1A00},
  {0x1C00, OTYPE_ARRAY, 4, 0,	acName1C00, SDO1C00},
  {0x1C12, OTYPE_ARRAY, 1, 0,	acName1C12, SDO1C12},
  {0x1C13, OTYPE_ARRAY, 1, 0,	acName1C13, SDO1C13},
  {0x6000, OTYPE_RECORD, 10, 0,	"TxPDO",	SDO6000},
  {0x7000, OTYPE_RECORD, 12, 0,	"RxPDO",	SDO7000},
  {0x8000, OTYPE_RECORD, 14, 0,	"Flash_par",SDO8000},
  {0x8001, OTYPE_RECORD,  9, 0,	"Ram_par",	SDO8001},
  {0x8002, OTYPE_RECORD,  3, 0, "AuxPdo",   SDO8002},
  {0xffff, 0xff, 0xff, 0xff, 0, 0}
};

#endif
