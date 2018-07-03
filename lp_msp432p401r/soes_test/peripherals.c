/**
 * 
 */

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//#include <utils/uartstdio.h>


#include "osal.h"
#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"
#include "soes/utypes.h"
#include "soes/esc.h"

const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        48000000,                                  // SMCLK = DCO = 48MHZ
        7500000,                                   // SPICLK = 7.5Mhz
        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
};


static void disable_peripheral_irq(void)
{
}

void jump_to_bootloader(void) {

    // disable periphearals irq ....
	disable_peripheral_irq();

    DPRINT ("Jump to bootloader\n");
    //
    // Return control to the boot loader.  This is a call to the SVC
    // handler in the boot loader.
    //
    (*((void (*)(void))(*(uint32_t *)0x2c)))();

}


/**
 * This function sets up UART0 to be used for a console to display information
 *
 * @author amargan (7/4/2014)
 */
void Configure_UART(void)
{
}

/**
 *
 */
void Configure_EcatPDI (void)
{
    // Set P3.0 to output direction CS
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);
    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(SPI_ECAT_PORT,
                                               ECAT_SPI_PINS,
                                               GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring SPI in 3wire master mode */
    SPI_initMaster(EUSCI_ECAT, &spiMasterConfig);
    /* Enable SPI module */
    SPI_enableModule(EUSCI_ECAT);
    // Configure the SPI INT pin as an input.
    // Configure the SPI EPROM_LOADED pin as an input.
    // Set P5.0 P5.2 to input direction
    GPIO_setAsInputPin(ECAT_GPIO_PORT, ECAT_IRQ_PIN | ECAT_EEL_PIN);
    GPIO_interruptEdgeSelect(ECAT_GPIO_PORT, ECAT_IRQ_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(ECAT_GPIO_PORT, ECAT_IRQ_PIN);
    Interrupt_enableInterrupt(INT_PORT5);

	//UARTprintf("%s\n",__FUNCTION__);

}

/*
 *
 */
void Configure_Led(void)
{
    // Set P1.0 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    // Set P2.[0,1,2] to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
    // Set P3.6 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
    // Set P3.6 to output direction
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);

    //UARTprintf("%s\n",__FUNCTION__);
}

/*
 *
 */
void Configure_Switch(void)
{
    /* Configuring P1.[1,4] as an input and enabling interrupts */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    Interrupt_enableInterrupt(INT_PORT1);

}

/*
 *
 */
void Configure_Timer(void)
{
    /* Configuring Timer32 to 48000 (1ms) of MCLK in periodic mode */
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
            TIMER32_PERIODIC_MODE);
    Interrupt_enableInterrupt(INT_T32_INT1);
    Timer32_setCount(TIMER32_0_BASE,48000);
    Timer32_startTimer(TIMER32_0_BASE, false);

    //UARTprintf("%s\n",__FUNCTION__);
}


void Configure_ADC(void)
{

}



