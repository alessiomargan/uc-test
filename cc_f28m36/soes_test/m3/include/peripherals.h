#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

void ConfigureUART(void);
void ConfigureEcatPDI(void);
void ConfigureLed(void);
void ConfigureTimer(void);

void disable_peripheral_irq(void);
void read_sensors(void);

#endif
