#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

void Configure_UART(void);
void Configure_EcatPDI(void);
void Configure_Led(void);
void Configure_Gpio(void);
void Configure_HallGpio(void);
void Configure_Timer_0A(void);
void Configure_Timer_1A(void);
void Watchdog0Reset(void);

unsigned long GetTickms(void);
void Configure_SysTick(void);

void disable_peripheral_irq(void);

/*
 *  functions.c prototypes
 */
#define DBG_PIN_ON			GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_PIN, GPIO_DBG_PIN)
#define DBG_PIN_OFF			GPIOPinWrite(GPIO_DBG_BASE, GPIO_DBG_PIN, 0)

void setDAC_A(uint16_t value);
void setDAC_B(uint16_t value);

#endif
