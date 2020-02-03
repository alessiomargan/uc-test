#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_nvic.h>
#include <inc/hw_types.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_gpio.h>
#include <inc/hw_epi.h>

#include <driverlib/sysctl.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>

#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"
#include "flash_utils.h"
#include "shared_ram.h"

#define DAC_SPI_SYNC_H		GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_DAC_CS, GPIO_DBG_DAC_CS)
#define DAC_SPI_SYNC_L		GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_DAC_CS, 0)
#define DAC_SPI_CLK_H		GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_DAC_CLK, GPIO_DBG_DAC_CLK)
#define DAC_SPI_CLK_L		GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_DAC_CLK, 0)
#define DAC_SPI_DO_H		GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_DAC_MOSI, GPIO_DBG_DAC_MOSI)
#define DAC_SPI_DO_L		GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_DAC_MOSI, 0)
#define DAC_SPI_LDAC_H		GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_DAC_LDAC, GPIO_DBG_DAC_LDAC)
#define DAC_SPI_LDAC_L		GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_DAC_LDAC, 0)

static DAC08_UNION dac08_union;

static inline void DAC_08_WriteData(uint16_t channel, uint16_t data) {

	uint16_t j;

	dac08_union.dac_data.pd0 = 0;      	// No power down mode
	dac08_union.dac_data.pd1 = 0;      	// No power down mode
	dac08_union.dac_data.buf = 0;    	// Update all channels
	dac08_union.dac_data.spare = 0;

	dac08_union.dac_data.addx = channel;
	dac08_union.dac_data.data = data;

	DAC_SPI_LDAC_L;		// make input buffer transparent (update immediatly)
	DAC_SPI_SYNC_L;		// cs active low

	for(j=0;j<16;j++) {

		DAC_SPI_CLK_H;

		if(dac08_union.all > 0x7FFF)	// Guardo solo l'MSB
			DAC_SPI_DO_H;
		else
			DAC_SPI_DO_L;

		DAC_SPI_CLK_L;
		dac08_union.all = (dac08_union.all << 1);
	}

	DAC_SPI_SYNC_H;	// release cs
}

void setDAC_A(uint16_t value) {
#ifdef _DBG_DAC_M3
	DAC_08_WriteData(DACREG_CH_A, value);
#endif
}

void setDAC_B(uint16_t value) {
#ifdef _DBG_DAC_M3
	DAC_08_WriteData(DACREG_CH_B, value);
#endif
}
