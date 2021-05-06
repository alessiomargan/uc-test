#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

//#define UARTprintf	printf

#define ADC0_SEQ_NR	2

void Configure_UART(void);
void Configure_EcatPDI(void);
void Configure_GPIO(void);
void Configure_Timer(void);
void Configure_DAC(void);
void Configure_ADC(void);
void Configure_ADC_temp(void);
void Configure_I2C(uint8_t slave_address);

void jump_to_bootloader(void);

#endif
