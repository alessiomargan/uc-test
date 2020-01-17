#ifndef __U_PARAMS_H__
#define __U_PARAMS_H__

typedef struct
{
	uint16_t	Params_valid;

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
}
tDriveParameters;


/*********************************************************************************************/
// Default parameter value loaded if data in flash are corrupted or never written
/*********************************************************************************************/

#define DEFAULT_HW_CONFIG_A0			(uint16_t)(0x3226)		// Default A0: curr_sens 20A, link enc 20-bit,  torque sensor analog ext adc
#define DEFAULT_HW_CONFIG_A1			(uint16_t)(0x3136)		// Default A1: curr_sens 20A, link enc 19-bit,  torque sensor deflection 19-bit
#define DEFAULT_HW_CONFIG_A2			(uint16_t)(0x3126)		// Default A2: curr_sens 20A, link enc 19-bit,  torque sensor analog ext adc
#define DEFAULT_HW_CONFIG_B0			(uint16_t)(0x3124)		// Default B0: curr_sens 20A, link enc 19-bit,  torque sensor analog ex adc,  4 pole-pairs	(RBE1211)

#define DEFAULT_HW_CONFIG_CE0101		(uint16_t)(0x3236)		// Default CE0101: curr_sens 20A, link enc 20-bit,  torque deflection 19-bit, 6 pole-pairs	(TBM6025)
#define DEFAULT_HW_CONFIG_CE0102		(uint16_t)(0x3236)		// Default CE0102: curr_sens 20A, link enc 20-bit,  torque deflection 19-bit, 6 pole-pairs	(TBM6013)
#define DEFAULT_HW_CONFIG_CE0103		(uint16_t)(0x3124)		// Default CE0103: curr_sens 20A, link enc 19-bit,  torque analog ext adc,	  4 pole-pairs	(RBE1211)
#define DEFAULT_HW_CONFIG_CE0104		(uint16_t)(0x3124)		// Default CE0104: curr_sens 20A, link enc 19-bit,  torque analog ext adc, 	  4 pole-pairs	(RBE1210)

#define DEFAULT_GEAR_RATIO_CE0101		(uint16_t)(160)			// Default CE0101 motor gear reduction ratio : 160
#define DEFAULT_GEAR_RATIO_CE0102		(uint16_t)(160)			// Default CE0102 motor gear reduction ratio : 160
#define DEFAULT_GEAR_RATIO_CE0103		(uint16_t)(100)			// Default CE0103 motor gear reduction ratio : 100
#define DEFAULT_GEAR_RATIO_CE0104		(uint16_t)(100)			// Default CE0104 motor gear reduction ratio : 100

#define DEFAULT_GEAR_RATIO_B0			(uint16_t)(100)			// Default B0 motor gear reduction ratio : 100

#define DEFAULT_MOTOR_EL_ANGLE_PHASE	(float)(0.0)			// Electrical angle phase
#define DEFAULT_TORION_BAR_STIFFNESS	(float)(0.0)			// Torsion bar stiffness

#define DEFAULT_KP_CURR_CONTROL			(float)(0.0)
#define DEFAULT_KI_CURR_CONTROL			(float)(0.0)

#define DEFAULT_CURRENT_MAX				(float)(50.0)			// 50 A max current in position and impedance control mode
#define DEFAULT_TORQUE_MAX				(float)(200.0)			// 200 Nm max torque in impedance control mode
#define	DEFAULT_SPEED_MAX				(float)(10.0)			// 10 rad/sec max

#define DEFAULT_JOINT_MIN_POSITION		(float)(0.25)
#define DEFAULT_JOINT_MAX_POSITION		(float)(6.0331853)

#define DEFAULT_CALIBRATION_ANGLE		(float)(3.1415926535897932384626433832795) // PI
#define DEFAULT_ENC_OFFSET				(float)(0.0)

#define DEFAULT_JOINT_NUMBER			(int)(-1)
#define DEFAULT_ROBOT_ID				(int)(-1)
/*********************************************************************************************/

/*********************************************************************************************/
// Maximum and minimum parameter value for setting flash parameters
/*********************************************************************************************/

#define PARAMS_TRQ_SENS_TYPE_MIN		(uint16_t)(0)
#define PARAMS_TRQ_SENS_TYPE_MAX		(uint16_t)(2)
#define PARAMS_MOT_POLE_PAIRS_MIN		(uint16_t)(0)
#define PARAMS_MOT_POLE_PAIRS_MAX		(uint16_t)(20)
#define PARAMS_MOT_GEAR_RATIO_MIN		(uint16_t)(10)
#define PARAMS_MOT_GEAR_RATIO_MAX		(uint16_t)(1000)

#define PARAMS_MOT_EL_PH_ANGLE_MIN		(float)(0.0)
#define PARAMS_MOT_EL_PH_ANGLE_MAX		(float)(6.2831853071796)	// TWO PI

#define PARAMS_TORS_BAR_STIFF_MIN		(float)(0.0)
#define PARAMS_TORS_BAR_STIFF_MAX		(float)(50000.0)

#define PARAMS_CURR_KP_MIN				(float)(0.0)
#define PARAMS_CURR_KP_MAX				(float)(10.0)
#define PARAMS_CURR_KI_MIN				(float)(0.0)
#define PARAMS_CURR_KI_MAX				(float)(1.0)

#define PARAMS_MAX_CUR_MIN				(float)(0.0)
#define PARAMS_MAX_CUR_MAX				(float)(90.00)	// A
#define PARAMS_MAX_TOR_MIN				(float)(0.0)
#define PARAMS_MAX_TOR_MAX				(float)(200.00)	// Nm
#define PARAMS_MAX_SPEED_MIN			(float)(0.0)
#define PARAMS_MAX_SPEED_MAX			(float)(20.00)	// rad/sec

#define PARAMS_MIN_JOINT_POS_MIN		(float)(0.28)
#define PARAMS_MIN_JOINT_POS_MAX		(float)(6.00)
#define PARAMS_MAX_JOINT_POS_MIN		(float)(0.28)
#define PARAMS_MAX_JOINT_POS_MAX		(float)(6.00)

#define PARAMS_CALIB_ANGLE_MIN			(float)(0.0)
#define PARAMS_CALIB_ANGLE_MAX			(float)(6.2831853071796)	// TWO PI

#define PARAMS_ENC_OFFS_MIN				(float)(0.0)
#define PARAMS_ENC_OFFS_MAX				(float)(6.2831853071796)	// TWO PI


/*********************************************************************************************/
// Maximum and minimum values for setting PDO variables
/*********************************************************************************************/

#define PDO_POS_KP_MIN					0
#define PDO_POS_KP_MAX					10000
#define PDO_POS_KD_MIN					0
#define PDO_POS_KD_MAX					1000

/*********************************************************************************************/


/*********************************************************************************************/
// CONTROL COMMAND CODES
/*********************************************************************************************/

// Control commands
#define CTRL_POWER_MOD_ON		0x00A5
#define CTRL_POWER_MOD_OFF		0x005A

// Current controller
#define CTRL_SET_DIRECT_MODE	0x004F

// Position controllers
#define CTRL_SET_POS_MODE		0x003B

// Impedance controller
#define CTRL_SET_IMPED_MODE		0x00D4

// Various
#define CTRL_FAN_ON				0x0026
#define CTRL_FAN_OFF			0x0062
#define CTRL_LED_ON				0x0019
#define CTRL_LED_OFF			0x0091

#define CTRL_SET_CALIB_POSITION	0x00AB
#define CTRL_TORQUE_CALIB		0x0085
#define CTRL_END_TORQUE_CALIB	0x0058
#define CTRL_PHASE_CALIB		0x0095
#define CTRL_END_PHASE_CALIB	0x0059

#define CTRL_CMD_DONE			0x7800
#define CTRL_CMD_WORKING		0xD000
#define CTRL_CMD_ERROR			0xAA00

/*********************************************************************************************/
// FLASH PARAMS COMMAND CODES
/*********************************************************************************************/

#define FLASH_PARAMS_LOADED		0xDEAD
#define DEFAULT_PARAMS_LOADED	0xBEEF

#define SAVE_PARAMS_TO_FLASH	0x0012
#define LOAD_PARAMS_FROM_FLASH	0x0034
#define LOAD_DEFAULT_PARAMS		0x0056
#define ERASE_FLASH_PARAMS		0x00F1
#define ERASE_ENC_CALIBRATION  	0x00F3

#define PARAMS_CMD_DONE			0x7800
#define PARAMS_CMD_ERROR		0xAA00
#define PARAMS_CMD_FLASH_BLANK	0x8500

#endif
