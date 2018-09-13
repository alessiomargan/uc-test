#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#define UARTprintf	printf

#define ADC0_SEQ_NR	2

void Configure_UART(void);
void Configure_EcatPDI(void);
void Configure_Led_IO(void);
void Configure_Timer(void);
void Configure_Switch(void);
//void Configure_LCD(void);
//void Configure_ADC(void);

void jump_to_bootloader(void);

#endif
