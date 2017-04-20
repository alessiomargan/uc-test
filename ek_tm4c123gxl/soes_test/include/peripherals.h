#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#define ADC0_SEQ_NR	2

void Configure_UART(void);
void Configure_EcatPDI(void);
void Configure_Led(void);
void Configure_Timer_0A(void);
void Configure_Timer_1A(void);
void Configure_LCD(void);
void Configure_ADC(void);

void disable_peripheral_irq(void);
void read_sensors(void);


void GPIOB_IntHandler(void);
void Timer0A_IntHandler(void);
void Timer1A_IntHandler(void);
void ADC2_IntHandler(void);

#endif
