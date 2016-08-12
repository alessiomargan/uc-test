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
    float        pos_ref;  //link
    int16_t      vel_ref;  //link
    int16_t      tor_ref;  //link
    uint16_t     gains[5];
    uint16_t     fault_ack;
    uint16_t     ts;
    uint16_t     op_idx_aux;    // op [get/set] , idx
    float        aux;        // set value
} CCS_PACKED rx_pdo_t;

typedef IAR_PACKED struct {
    float        link_pos;           // rad
    float        motor_pos;           // rad
    float        link_vel;            // rad
    int16_t      motor_vel;             // rad/s
    int16_t      torque;             // Nm
    uint16_t     max_temperature;     // C
    uint16_t     fault;
    uint16_t     rtt;                // us
    uint16_t     op_idx_ack;         // op [ack/nack] , idx
    float        aux;                // get value or nack erro code
} CCS_PACKED tx_pdo_t;

typedef IAR_PACKED struct {
    float	volt_ref;
    float	current;
    float	vout;
    float   pos_ref_fb;
} CCS_PACKED aux_pdo_t;

typedef IAR_PACKED struct {
    //
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
    int16_t Joint_number;
    int16_t Joint_robot_id;

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
