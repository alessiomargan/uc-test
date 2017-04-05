#ifndef __SOES_HOOK_H__
#define __SOES_HOOK_H__

//#define PACKED 	__attribute__((__packed__))
#define CCS_PACKED
#define IAR_PACKED

typedef IAR_PACKED struct {
    float       pos_ref;  				// motor position reference
#ifdef FLOAT_PDO
    float    	vel_ref;  				// motor velocity reference
    float     	tor_ref;  				// torque reference
    float    	gains[5];
#else
    int16_t  	vel_ref;  				// motor velocity reference
    int16_t     tor_ref;  				// torque reference
    uint16_t    gains[5];
#endif
    uint16_t    fault_ack;
    uint16_t    ts;
    uint16_t    op_idx_aux;    			// op [get/set] , idx
    float       aux;        			// set value
} CCS_PACKED rx_pdo_t;

typedef IAR_PACKED struct {
    float       link_pos;           	// link position (rad)
    float       motor_pos;           	// motor position (rad)
#ifdef FLOAT_PDO
    float      	link_vel;             	// link velocity  (mrad/sec)
    float      	motor_vel;              // motor velocity (mrad/sec)
#else
    int16_t     link_vel;             	// link velocity  (mrad/sec)
    int16_t     motor_vel;              // motor velocity (mrad/sec)
#endif
    float       torque;            	    // Nm
    uint16_t    temperature;     		// �C
    uint16_t    fault;
    uint16_t    rtt;                	// us
    uint16_t    op_idx_ack;         	// op [ack/nack] , idx
    float       aux;                	// get value or nack erro code
} CCS_PACKED tx_pdo_t;

typedef IAR_PACKED struct {

    // 0x8000 flash param
    uint32_t	board_id;
    // 0x8001 ram param
     char 		fw_ver[8];
 	uint16_t 	ctrl_status_cmd;
 	uint16_t 	ctrl_status_cmd_ack;
 	uint16_t 	flash_params_cmd;
 	uint16_t 	flash_params_cmd_ack;
 	uint16_t 	ack_board_faults;

 } CCS_PACKED sdo_t;

 typedef IAR_PACKED struct {
     // 0x8002 aux param
     float volt_ref;
     float current;
     float vout;
     float pos_ref_fb;
     float pwm_duty;

 } CCS_PACKED aux_pdo_t;

extern tx_pdo_t tx_pdo;
extern rx_pdo_t rx_pdo;
extern sdo_t sdo;
extern aux_pdo_t aux_pdo;

void setup_esc_configs(void);
void ecat_process_pdo(void);

#endif 
