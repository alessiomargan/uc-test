#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_nvic.h>
#include <inc/hw_types.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_gpio.h>
#include <inc/hw_pwm.h>
#include <driverlib/rom.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/timer.h>
#include <driverlib/pin_map.h>
#include <driverlib/pwm.h>

#include "pins.h"

#define PWM_OUT_BITS    (PWM_OUT_2_BIT | PWM_OUT_3_BIT) 

#define PWM_OFF()       PWMOutputState(PWM1_BASE, PWM_OUT_BITS, false); 
#define PWM_ON()        PWMOutputState(PWM1_BASE, PWM_OUT_BITS, true); 

void init_PWM(void)
{
    // Set the PWM clock to the system clock / 4.
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_4);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    ROM_GPIOPinConfigure(GPIO_PA6_M1PWM2);
    ROM_GPIOPinConfigure(GPIO_PA7_M1PWM3);

    ROM_GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);


    // Configure the three PWM generators for up/down counting mode, synchronous updates
    PWMGenConfigure(PWM1_BASE, PWM_GEN_1, (PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC ));

    
    // Set the PWM period to 250Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 250Hz) * 16MHz = 64000 cycles.  Note that
    // the maximum period you can set is 2^16 - 1.
    

    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, 40000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 10);


    // Synchronize the time base of the generators.
    //PWMSyncTimeBase(PWM1_BASE, PWM_GEN_0_BIT | PWM_GEN_1_BIT | PWM_GEN_2_BIT);

    // Set all six PWM outputs to go to the inactive state when a fault event
    // occurs (which includes debug events).
    //PWMOutputFault(PWM1_BASE, PWM_OUT_BITS, true);

    // Disable all six PWM outputs.
    //PWM_OFF();

    // Ensure that all outputs are not-inverted.
    //PWMOutputInvert(PWM1_BASE, PWM_OUT_BITS, false);

    PWMDeadBandEnable(PWM1_BASE, PWM_GEN_1, 400, 400);

    // Configure an interrupt on the zero event of the first generator, and an
    // ADC trigger on the load event of the first generator.
    PWMGenIntClear(PWM1_BASE, PWM_GEN_1, PWM_INT_CNT_ZERO);
    PWMGenIntTrigEnable(PWM1_BASE, PWM_GEN_1, PWM_INT_CNT_ZERO | PWM_TR_CNT_LOAD);
    PWMIntEnable(PWM1_BASE, PWM_INT_GEN_1);
    IntEnable(INT_PWM1_1);



    ////////////////////////////////////////////

    PWM_ON();

    // Enable the PWM generators.
    PWMGenEnable(PWM1_BASE, PWM_GEN_1);

}

void TrapModulate(uint8_t ulHall)
{
    
}

void PWMSetDutyCycle(long DutyCycle)
{

}
