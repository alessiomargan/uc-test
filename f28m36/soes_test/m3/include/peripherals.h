#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

void Configure_SysTick(void);
void Configure_UART(void);
void Configure_EcatPDI(void);
void Configure_Link_Enc_BissC(void);
void Configure_Led(void);
void Configure_Timer_0A(void);
void Configure_Timer_1A(void);
void Configure_LCD(void);
void Configure_USB_host(void);
void Watchdog0Reset(void);
unsigned long GetTickms(void);
void read_sensors(void);

#endif
