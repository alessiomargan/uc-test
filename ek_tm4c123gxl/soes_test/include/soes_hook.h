#ifndef __SOES_HOOK_H__
#define __SOES_HOOK_H__


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
} __attribute__((__packed__)) rx_pdo_t;


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
} __attribute__((__packed__)) tx_pdo_t;



extern tx_pdo_t tx_pdo;
extern rx_pdo_t rx_pdo;

extern int par_1;
extern int par_2;

void setup_esc_configs(void);

void ecat_process_pdo(void);

#endif 
