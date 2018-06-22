#ifndef __objectlist__
#define __objectlist__


#include "soes/esc_coe.h"
#include "soes_hook.h"

const char Number_of_elemets[] = "Number of Elements";

const _objd SDO1000[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Device Type", 0x01901389, 0},
};

const _objd SDO1008[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 72, ATYPE_RO, "Manufacturer Device Name", 0, "Test"},
};

const _objd SDO1009[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 24, ATYPE_RO, "Manufacturer Hardware Version", 0, "1.0"},

};

const _objd SDO100A[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 24, ATYPE_RO, "Manufacturer Software Version", 0, "1.0"},
};

const _objd SDO1018[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, Number_of_elemets, 4, 0},
  {0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Vendor ID", 664, 0},				// Vendor ID
  {0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Product Code", 0, 0},			// Product code
  {0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Revision Number", 0, 0},			// Revision number
  {0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Serial Number", 0x00000000, 0},	// Serial number
};

// RxPdo 1600
const _objd SDO1600[] =
{
	{0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, Number_of_elemets, 12, 0},
	{0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Pos_ref", 	0x70000120, 0},
	{0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Vel_ref", 	0x70000220, 0},
	{0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Tor_ref", 	0x70000320, 0},
	{0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Gain_0", 	0x70000420, 0},
	{0x5, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Gain_1", 	0x70000520, 0},
	{0x6, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Gain_2", 	0x70000620, 0},
	{0x7, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Gain_3", 	0x70000720, 0},
	{0x8, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Gain_4", 	0x70000820, 0},
	{0x9, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Fault_Ack", 	0x70000910, 0},
	{0xa, DTYPE_UNSIGNED32, 32, ATYPE_RO, "ts", 		0x70000a10, 0},
	{0xb, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Op_idx_aux", 0x70000b10, 0},
	{0xc, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Aux", 		0x70000c20, 0},
};

// TxPdo
const _objd SDO1A00[] =
{
	{0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, Number_of_elemets, 10, 0},
	{0x1, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Link_position", 	0x60000120, 0},
	{0x2, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Motor_position", 0x60000220, 0},
	{0x3, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Link_velocity",	0x60000320, 0},
	{0x4, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Motor_velocity", 0x60000420, 0},
	{0x5, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Torque", 		0x60000520, 0},
	{0x6, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Temperature", 	0x60000610, 0},
	{0x7, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Fault",			0x60000710, 0},
	{0x8, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Rtt", 			0x60000810, 0},
	{0x9, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Op_idx_ack", 	0x60000910, 0},
	{0xa, DTYPE_UNSIGNED32, 32, ATYPE_RO, "Aux", 			0x60000a20, 0},
};

const _objd SDO1C00[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, Number_of_elemets, 4, 0},
  {0x1, DTYPE_UNSIGNED8, 8, ATYPE_RO, "Communications Type SM0", 1, 0},
  {0x2, DTYPE_UNSIGNED8, 8, ATYPE_RO, "Communications Type SM1", 2, 0},
  {0x3, DTYPE_UNSIGNED8, 8, ATYPE_RO, "Communications Type SM2", 3, 0},
  {0x4, DTYPE_UNSIGNED8, 8, ATYPE_RO, "Communications Type SM3", 4, 0},
};
const _objd SDO1C12[] =
{
  {0x0, DTYPE_UNSIGNED8, 	8, 	ATYPE_RO, Number_of_elemets, 1, 0},
  {0x1, DTYPE_UNSIGNED16, 	16, ATYPE_RO, "PDO Mapping", 0x1600, 0},
};
const _objd SDO1C13[] =
{
  {0x0, DTYPE_UNSIGNED8, 8, ATYPE_RO, Number_of_elemets, 1, 0},
  {0x1, DTYPE_UNSIGNED16, 16, ATYPE_RO, "PDO Mapping", 0x1A00, 0},
};

const _objd SDO6000[] =
{
	{0x0, DTYPE_UNSIGNED8,    8, ATYPE_RO, Number_of_elemets,	10, 0},
	{0x1, DTYPE_REAL32,      32, ATYPE_RO, "Link_position",	    0, &tx_pdo.link_pos},
	{0x2, DTYPE_REAL32,      32, ATYPE_RO, "Motor_position",    0, &tx_pdo.motor_pos},
	{0x3, DTYPE_REAL32,      32, ATYPE_RO, "Link_velocity",		0, &tx_pdo.link_vel},
	{0x4, DTYPE_REAL32,   	 32, ATYPE_RO, "Motor_velocity", 	0, &tx_pdo.motor_vel},
	{0x5, DTYPE_REAL32,   	 32, ATYPE_RO, "Torque", 			0, &tx_pdo.torque},
	{0x6, DTYPE_UNSIGNED16,  16, ATYPE_RO, "Temperature", 		0, &tx_pdo.temperature},
	{0x7, DTYPE_UNSIGNED16,  16, ATYPE_RO, "Fault", 			0, &tx_pdo.fault},
	{0x8, DTYPE_UNSIGNED16,  16, ATYPE_RO, "Round_rip_time", 	0, &tx_pdo.rtt},
	{0x9, DTYPE_UNSIGNED16,  16, ATYPE_RO, "Op_idx_ack", 		0, &tx_pdo.op_idx_ack},
	{0xa, DTYPE_REAL32,  	 32, ATYPE_RO, "Aux", 				0, &tx_pdo.aux},
};

const _objd SDO7000[] =
{
	{0x0, DTYPE_UNSIGNED8,   8, ATYPE_RO, "NumElem",		12, 0},
	{0x1, DTYPE_REAL32,     32, ATYPE_RW, "Pos_ref",		0, &rx_pdo.pos_ref},
	{0x2, DTYPE_REAL32,  	32, ATYPE_RW, "Vel_ref",		0, &rx_pdo.vel_ref},
	{0x3, DTYPE_REAL32,  	32, ATYPE_RW, "Tor_ref",		0, &rx_pdo.tor_ref},
	{0x4, DTYPE_REAL32, 	32, ATYPE_RW, "Gain_0", 		0, &rx_pdo.gains[0]},
	{0x5, DTYPE_REAL32, 	32, ATYPE_RW, "Gain_1",			0, &rx_pdo.gains[1]},
	{0x6, DTYPE_REAL32, 	32, ATYPE_RW, "Gain_2", 		0, &rx_pdo.gains[2]},
	{0x7, DTYPE_REAL32, 	32, ATYPE_RW, "Gain_3", 		0, &rx_pdo.gains[3]},
	{0x8, DTYPE_REAL32, 	32, ATYPE_RW, "Gain_4", 		0, &rx_pdo.gains[4]},
	{0x9, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Fault_Ack", 		0, &rx_pdo.fault_ack},
	{0xa, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Timestamp", 		0, &rx_pdo.ts},
	{0xb, DTYPE_UNSIGNED16, 16, ATYPE_RW, "Op_idx_aux", 	0, &rx_pdo.op_idx_aux},
	{0xc, DTYPE_REAL32,     32, ATYPE_RW, "Aux",			0, &rx_pdo.aux},
};

const _objd SDO8000[] =
{
  {0x0, DTYPE_UNSIGNED8,   		8,	ATYPE_RO, "NumElem"				, 1, 0},
  {0x1, DTYPE_UNSIGNED32,      32,	ATYPE_RW, "Board_id"			, 0, &sdo.board_id},
};

const _objd SDO8001[] =
{
  {0x0, DTYPE_UNSIGNED8,   		8,  ATYPE_RO, "NumElem"				, 6, 0},
  {0x1, DTYPE_VISIBLE_STRING,   64, ATYPE_RO, "firmware_version"	, 0, &sdo.fw_ver},
  {0x2, DTYPE_UNSIGNED16,       16, ATYPE_RW, "Control status cmd"	, 0, &sdo.ctrl_status_cmd},
  {0x3, DTYPE_UNSIGNED16,       16, ATYPE_RO, "Control status ack"	, 0, &sdo.ctrl_status_cmd_ack},
  {0x4, DTYPE_UNSIGNED16,       16, ATYPE_RW, "Flash params cmd"	, 0, &sdo.flash_params_cmd},
  {0x5, DTYPE_UNSIGNED16,       16, ATYPE_RO, "Flash params ack"	, 0, &sdo.flash_params_cmd_ack},
  {0x6, DTYPE_UNSIGNED16,       16, ATYPE_RO, "Ack Board_fault"		, 0, &sdo.ack_board_faults},
};

const _objd SDO8002[] =
{
  {0x0,  DTYPE_UNSIGNED8,    8, ATYPE_RO, "NumElem",  	4, 0},
  {0x1,  DTYPE_REAL32,      32, ATYPE_RO, "aux_rtt",	0, &aux_pdo_tx.rtt},
  {0x2,  DTYPE_REAL32,      32, ATYPE_RO, "pos_ref_fb", 0, &aux_pdo_tx.pos_ref_fb},
  {0x3,  DTYPE_REAL32,      32, ATYPE_RO, "current", 	0, &aux_pdo_tx.current},
  {0x4,  DTYPE_REAL32,      32, ATYPE_RO, "volt_ref",   0, &aux_pdo_tx.volt_ref},
  {0x5,  DTYPE_REAL32,      32, ATYPE_RO, "vout",     	0, &aux_pdo_tx.vout},
};

const _objd SDO8003[] =
{
  {0x0,  DTYPE_UNSIGNED8,    8, ATYPE_RO, "NumElem",  	2, 0},
  {0x1,  DTYPE_REAL32,      32, ATYPE_RW, "aux_ts",		0, &aux_pdo_rx.ts},
  {0x2,  DTYPE_REAL32,      32, ATYPE_RW, "pwm_duty",   0, &aux_pdo_rx.pwm_duty},
};


const _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 		 0,  0, "Device Type", SDO1000},
  {0x1008, OTYPE_VAR, 		 0,  0, "Manufacturer Device Name", SDO1008},
  {0x1009, OTYPE_VAR, 		 0,  0, "Manufacturer Hardware Version", SDO1009},
  {0x100A, OTYPE_VAR, 		 0,  0, "Manufacturer Software Version", SDO100A},
  {0x1018, OTYPE_RECORD, 	 4,  0, "Identity Object", SDO1018},
  {0x1600, OTYPE_RECORD, 	 12, 0, "Receive PDO Mapping", SDO1600},
  {0x1A00, OTYPE_RECORD, 	 10, 0, "Transmit PDO Mapping", SDO1A00},
  {0x1C00, OTYPE_ARRAY, 	 4,  0, "Sync Manager Communication Type", SDO1C00},
  {0x1C12, OTYPE_ARRAY, 	 1,  0, "Sync Manager 2 PDO Assignment", SDO1C12},
  {0x1C13, OTYPE_ARRAY, 	 1,  0, "Sync Manager 3 PDO Assignment", SDO1C13},
  {0x6000, OTYPE_RECORD, 	 10, 0, "Inputs", SDO6000},
  {0x7000, OTYPE_RECORD, 	 12, 0, "Outputs", SDO7000},
  {0x8000, OTYPE_RECORD, 	 1,  0, "Flash Parameters", SDO8000},
  {0x8001, OTYPE_RECORD, 	 6,  0, "Ram Parameters", SDO8001},
  {0x8002, OTYPE_RECORD,     4,  0, "AuxPdo RO",   SDO8002},
  {0x8003, OTYPE_RECORD,     1,  0, "AuxPdo WR",   SDO8003},
    {0xffff, 0xff, 0xff, 0xff, 0,  0}
};

#endif