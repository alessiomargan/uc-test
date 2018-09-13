/******************************************************************************
 * MSP432 Project
 *
 * Description:
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "pins.h"
#include "peripherals.h"

extern void soes_init(void);

//variable detect if we interrupt occurs
volatile bool jumpToBsl = false;

int main(void)
{
    int i = 0;

    /* Stop Watchdog  */
    WDT_A_holdTimer();

    Configure_UART();
    Configure_Led_IO();
    Configure_Switch();
    Configure_EcatPDI();
    Configure_Timer();

    soes_init();

    Interrupt_setPriority(INT_PORT5,    (char)(2)<<5);
    Interrupt_setPriority(INT_T32_INT1, (char)(2)<<5);

    Interrupt_enableMaster();

    while(1)
    {
        // Delay
        for(i=100000; i>0; i--);

        if ( jumpToBsl ) {
        	jump_to_bootloader();
        }


    }
}
