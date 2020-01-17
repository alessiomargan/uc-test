/**
 * 
 */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_nvic.h>
#include <inc/hw_types.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/ssi.h>
#include <driverlib/timer.h>
#include <driverlib/uart.h>
#include <driverlib/watchdog.h>

#include <utils/uartstdio.h>

#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"
#include "flash_utils.h"
#include "shared_ram.h"

#include "soes/utypes.h"
#include "soes/esc.h"


#define		FLASH_C_ADDX (0x002E8000)


uint32_t 	rls_link_read = 0;
uint16_t 	rls_link_error, rls_link_warning;
float		enc_link_pos = 0, previous_enc_link_pos = 0;

#define LINK_ENC_AKSIM_19_BIT 	0x0019
#define LINK_ENC_AKSIM_20_BIT 	0x0020
uint32_t Link_encoder_type = LINK_ENC_AKSIM_19_BIT;

uint16_t RLS_Link_BISS_ReadData(uint16_t encoder_type, float *pnt) {

	uint16_t i;
	uint16_t rls_data[3];

	// wait until SLO pin is low...
	// while(RLS_A_SPI_MISO == 0);

	for(i=0;i<3;i++) {
		SSIDataPut(LINK_ENC_SSI_BASE, 0);
		while( SSIBusy(LINK_ENC_SSI_BASE) );
		SSIDataGet(LINK_ENC_SSI_BASE, &rls_data[i]);
	}

	if(encoder_type == LINK_ENC_AKSIM_19_BIT) {
		rls_link_read = (((uint32_t)(rls_data[1])<<3)|((rls_data[2]&0xE000)>>13));
		rls_link_error = ((rls_data[2]&0x1000)>>12);
		rls_link_warning = ((rls_data[2]&0x0800)>>11);
		// crc_packet = (rls_data[2]&0x07E0)>>5;
		*pnt = ((rls_link_read)*(float)(0.0000119842249));			// angle in radians
	} else if(encoder_type == LINK_ENC_AKSIM_20_BIT) {
		rls_link_read = (((uint32_t)(rls_data[1])<<4)|((rls_data[2]&0xF000)>>12));
		rls_link_error = ((rls_data[2]&0x0800)>>11);
		rls_link_warning = ((rls_data[2]&0x0400)>>10);
		// crc_packet = (rls_data[2]&0x03F0)>>4;
		*pnt =((rls_link_read)*(float)(0.000005992112452678286)); 	// angle in radians
	} else {
		return(0);
	}

	if(rls_link_error == 0)
		return(0);

	return(1);
}


#define		SIZE_OF_LINK_ZERO_ARRAY		200
#define 	WAIT_LINK_ZERO_FROM_PWR_UP	8000	// wait 2 seconds from power up before calculating the zero link postion

uint16_t	timer_wait_link_zero = 0;
uint16_t	zero_link_idx = 0;
float		zero_link_array[SIZE_OF_LINK_ZERO_ARRAY];

void Process_Link_encoder_read(void) {

	float tmpf;


	if(RLS_Link_BISS_ReadData(Link_encoder_type,&tmpf)) {
		//m3_rw_data. v_float = tmpf;
		UARTprintf("link %f\n", tmpf);
	} else {
		UARTprintf("error %d\n", rls_link_error);
	}

}


#define AD7680_VOLTAGE_CONVERSION	(float)(0.0000762951094834821)	// 5V over 65535 for 16-bit ADC

uint16_t 	adc_ext_data[3];
uint16_t 	adc_ext_read = 0;

void Process_ADC_EXT_AD7680(void) {

	//CS_LO
	GPIOPinWrite(LINK_ENC_SSI_GPIO_PORTBASE, GPIO_PIN_3, 0);

	//DELAY_US(2);

	SSIDataPut(LINK_ENC_SSI_BASE, 0);
	while(SSIBusy(LINK_ENC_SSI_BASE));
	SSIDataGet(LINK_ENC_SSI_BASE, &adc_ext_data[0]);

	SSIDataPut(LINK_ENC_SSI_BASE, 0);
	while(SSIBusy(LINK_ENC_SSI_BASE));
	SSIDataGet(LINK_ENC_SSI_BASE, &adc_ext_data[1]);

	SSIDataPut(LINK_ENC_SSI_BASE, 0);
	while(SSIBusy(LINK_ENC_SSI_BASE));
	SSIDataGet(LINK_ENC_SSI_BASE, &adc_ext_data[2]);

	//AD7680_CS_HI
	GPIOPinWrite(LINK_ENC_SSI_GPIO_PORTBASE, GPIO_PIN_3, GPIO_PIN_3);

	adc_ext_read = (
			(adc_ext_data[0] << 11) |
			(adc_ext_data[1] << 3)  |
			(adc_ext_data[1] >> 5) );

	// adc_ext_read = ((adc_ext_data[0] << 8) | (adc_ext_data[1] >> 4));

	//Torque_rough_read =  (float)(adc_ext_read)*(AD7680_VOLTAGE_CONVERSION);	// Real voltage read at input (multiplication by V_REF/2^16)
}

