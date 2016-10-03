#ifndef __SOES_HOOK_H__
#define __SOES_HOOK_H__

#ifdef ccs
	#define IAR_PACKED
	#define CCS_PACKED	__attribute__((__packed__))
#else
	#define IAR_PACKED 	__packed
	#define CCS_PACKED
#endif


typedef IAR_PACKED struct {
	float		pos_ref;  //link
    float      	vel_ref;  //link
    float      	tor_ref;  //link
    float     	gains[5];
    uint16_t    fault_ack;
    uint16_t    ts;
    uint16_t    op_idx_aux;    // op [get/set] , idx
    float       aux;        // set value
} CCS_PACKED rx_pdo_t;

typedef IAR_PACKED struct {
    float   	link_pos;           // rad
    float       motor_pos;           // rad
    float       link_vel;            // rad
    float      	motor_vel;             // rad/s
    float      	torque;             // Nm
    uint16_t    max_temperature;     // C
    uint16_t    fault;
    uint16_t    rtt;                // us
    uint16_t    op_idx_ack;         // op [ack/nack] , idx
    float		aux;                // get value or nack erro code
} CCS_PACKED tx_pdo_t;

typedef IAR_PACKED struct {
	float   pos_ref_fb;
	float	iq_ref;
	float	iq_out;
} CCS_PACKED aux_pdo_t;


typedef IAR_PACKED struct {

	uint16_t 	Hardware_configuration;	// 0xABCD
	    									// A = curr_sensor_type: 	0 = none 	1 = 6A 			2 = 10A 			3 = 20A			4 = 35A
											// B = link enc type: 		0 = none	1 = 19-bit	 	2 = 20-bit
											// C = torque_sensor_type: 	0 = none 	1 = analog DSP 	2 = analog_ext_ADC  3 = defl 19-bit 4 = defl 20-bit
											// D = number of pole pair

	uint16_t	Motor_gear_ratio;

	float		Motor_electrical_phase_angle;
	float 		Torsion_bar_stiffness;

	float		CurrGainP;
	float		CurrGainI;

	float 		Max_cur;
	float 		Max_tor;
	float 		Max_vel;

	float 		Joint_Min_pos;
	float 		Joint_Max_pos;

	float		Calibration_angle;
	float 		Enc_offset;

	int  		Joint_number;
	int  		Joint_Robot_ID;

	//
    float   PosGainP;
    float   PosGainI;
    float   PosGainD;
    float   TorGainP;
    float   TorGainI;
    float   TorGainD;
    float   Pos_I_lim;
    float   Tor_I_lim;

    char 		fw_ver[8];
    uint32_t	board_faults;
    uint16_t	ctrl_status_cmd;
    uint16_t	ctrl_status_cmd_ack;
    uint16_t	flash_params_cmd;
    uint16_t	flash_params_cmd_ack;

} CCS_PACKED sdo_t;

extern tx_pdo_t		tx_pdo;
extern rx_pdo_t		rx_pdo;
extern aux_pdo_t	aux_pdo;
extern sdo_t 		sdo;

extern const char fw_ver[];


void setup_esc_configs(void);

void ecat_process_pdo(void);

#endif 
