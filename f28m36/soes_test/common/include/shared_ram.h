#ifndef __SHARED_RAM_H__
#define __SHARED_RAM_H__

#include <stdint.h>
#ifdef _C28X
	#include "F28M36x_Device.h"
#else
	#include "driverlib/device.h"
#endif

//RAM Function Linker Section
#ifdef __TI_COMPILER_VERSION__
	#if __TI_COMPILER_VERSION__ >= 15009000
		#define ramFuncSection ".TI.ramfunc"
	#else
		#error "MeRDa"
		#define ramFuncSection "ramfuncs"
	#endif
#endif



struct PROC_FAULT {
	uint16_t  m3_bits:12;
    uint16_t  c28_bits:4;
};

struct BIT_FAULT {

	uint16_t  m3_rxpdo_pos_ref:1;
	uint16_t  m3_rxpdo_vel_ref:1;
	uint16_t  m3_rxpdo_tor_ref:1;
	uint16_t  m3_flag_1:1;
	uint16_t  m3_fault_hardware:1;
	uint16_t  m3_params_out_of_range:1;
	uint16_t  m3_flag_2:1;
	uint16_t  m3_flag_3:1;
	uint16_t  m3_link_enc_error_reading:1;
	uint16_t  m3_link_enc_hw_error:1;
	uint16_t  m3_defl_enc_error_reading:1;
	uint16_t  m3_defl_enc_hw_error:1;

	uint16_t  c28_motor_enc_error_reading:1;
	uint16_t  c28_motor_enc_hw_error:1;
	uint16_t  c28_Max_cur_limited_for_temp:1;
	uint16_t  c28_irq_alive:1;
};

typedef union{
    uint16_t all;
    struct PROC_FAULT proc_fault;
    struct BIT_FAULT bit;
    } FAULT;


typedef struct {
	/***** Flash parameters (M3 to C28) *****/
	uint16_t	hw_configuration;	// 0xABCD
									// A = curr_sensor_type: 	0 = none 	1 = 6A 			2 = 10A 			3 = 20A			4 = 35A
									// B = link enc type: 		0 = none	1 = 19-bit	 	2 = 20-bit
									// C = analog_torque_type: 	0 = none 	1 = analog DSP 	2 = analog_ext_ADC  3 = defl 19-bit 4 = defl 20-bit
									// D = SPARE

    float		inverse_motor_gear_ratio;
    float    	motor_phase_angle;
    float		ccor_pid_p;
	float		ccor_pid_i;
	float		Currrent_sensor_gain;
    float		max_current;
    float		max_tor;
    float		max_vel;
    float		reflected_motor_inertia;
    float		inverse_reflected_torque_constant;
    float		DOB_filter_frequency_Hz;
    float		sampling_time_over_inertia;

    /****************************************/

    /*** PDO or SDO Variables (M3 to C28) ***/
    float		position_ref;		// PDO Pos_ref
    float		velocity_ref;		// PDO Vel_ref
    float		torque_ref;			// PDO Tor_ref
	float		Gain_0;				// PDO Gain 0
	float		Gain_1;				// PDO Gain 1
	float		Gain_2;				// PDO Gain 2
	float		Gain_3;				// PDO Gain 3
	float		Gain_4;				// PDO Gain 4
	float		Pdo_aux;			// PDO Aux
	float    	iq_ref;				// SDO field
	float		id_ref;				// SDO field
	/****************************************/

	/* M3 read sensor variables (M3 to C28) */
	float		enc_link_position;
	float		enc_link_velocity;
	float		torque_read;
	float		enc_link_position_zero;
	/****************************************/

	/* M3 variables (M3 to C28) */
	uint16_t	Joint_control;		// Set by SDO command
	uint16_t	Joint_control_mode;	// Set by SDO command
	/****************************************/

	int32_t		v_int32;

} m3_to_c28_data_t;

typedef struct {

#ifdef _C28X
	uint16_t	c28_firmware_version[4];
#else
	char		c28_firmware_version[8];
#endif

	FAULT		c28_fault;
	float		motor_pos;
	float		motor_vel;
	float		iq_ref_fb;
	float		iq_read_fb;
	float		id_ref_fb;
	float		id_read_fb;
	float		max_limited_current;

	/***** C28 Analog Variables *****/
	float    	v_batt;
    float    	i_batt;
    float		torque_c28;
    float		torque_average_c28;
    float		board_temp;
    float		motor_temp;
    float		max_temp;

    uint32_t	ipc_irq_err_cnt;
    uint32_t	ipc_irq_error;

} c28_to_m3_data_t;

extern m3_to_c28_data_t	m3_rw_data;
extern c28_to_m3_data_t c28_ro_data;

#endif
