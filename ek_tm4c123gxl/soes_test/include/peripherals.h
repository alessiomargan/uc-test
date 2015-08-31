#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#define ADC0_SEQ_NR	2

void ConfigureUART(void);
void ConfigureEcatPDI(void);
void ConfigureLed(void);
void ConfigureTimer(void);
void Configure_ADC(void);

void disable_peripheral_irq(void);
void read_sensors(void);

#endif
