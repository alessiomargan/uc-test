#ifndef __SOES_HOOK_H__
#define __SOES_HOOK_H__

#ifdef ccs
	#define IAR_PACKED
	#define CCS_PACKED	__attribute__((__packed__))
#else
	#define IAR_PACKED 	__packed
	#define CCS_PACKED
#endif


typedef struct {
    float       pos_ref;  //link
    int16_t     vel_ref;  //link
    int16_t     tor_ref;  //link
    uint16_t    gain_kp_m;
    uint16_t    gain_kp_l;
    uint16_t    gain_kd_m;
    uint16_t    gain_kd_l;
    uint16_t    gain_ki;
    uint16_t    fault_ack;
    uint16_t    ts;
    uint16_t    op_idx_aux;  // op [get/set] , idx
    float       aux;         // set value
} CCS_PACKED rx_pdo_t;


typedef struct {
    float        link_pos;           // rad
    float        motor_pos;          // rad
    float        link_vel;           // rad TBD on the firmware
    int16_t      motor_vel;          // rad/s
    int16_t      torque;             // Nm
    uint16_t     temperature;        // C
    uint16_t     fault;
    uint16_t     rtt;                // us
    uint16_t     op_idx_ack;         // op [ack/nack] , idx
    float        aux;                // get value or nack erro code
} CCS_PACKED tx_pdo_t;

typedef struct {
	// Flash
    float   PosGainP;
    float   PosGainI;
    float   PosGainD;
    float   TorGainP;
    float   TorGainI;
    float   TorGainD;
    float   Pos_I_lim;
    float   Tor_I_lim;
    float   Min_pos;
    float   Max_pos;
    float   Max_tor;
    float   Max_cur;
    int16_t ConfigFlags;
    int16_t ConfigFlags2;
    float   ImpedancePosGainP;
    float   ImpedancePosGainD;
    int     MaxPWM;
    int16_t Joint_number;
    int16_t Joint_robot_id;
    float   Target_velocity;
	// Ram
	char fw_ver[8];
    unsigned short ctrl_status_cmd;
    unsigned short ctrl_status_cmd_ack;
    unsigned short flash_params_cmd;
    unsigned short flash_params_cmd_ack;
    unsigned int ack_board_faults;
    float direct_ref;
    float abs_pos;
    float m_current;

} CCS_PACKED sdo_t;

typedef struct {
	float   pos_ref_fb;
	float	iq_ref;
	float	iq_out;
} CCS_PACKED aux_pdo_t;

extern tx_pdo_t 	tx_pdo;
extern rx_pdo_t 	rx_pdo;
extern aux_pdo_t	aux_pdo;
extern sdo_t		sdo;

void setup_esc_configs(void);

void ecat_process_pdo(void);

#endif 
