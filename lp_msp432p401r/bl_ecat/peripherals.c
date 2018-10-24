/**
 * 
 */

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//#include <utils/uartstdio.h>

#include <soes_hook.h>
#include <soes/esc.h>

#include <pins.h>
#include <peripherals.h>

/*
 * SPI master Config
 */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        CS_48MHZ,                                  // SMCLK = DCO = 48MHZ
        CS_12MHZ,                                  // SPICLK = 12Mhz
		EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
};

/*
 * http://processors.wiki.ti.com/index.php/Printf_support_for_MSP430_CCSTUDIO_compiler
 * http://processors.wiki.ti.com/index.php/USCI_UART_Baud_Rate_Gen_Mode_Selection
 * UART Config
 * SMCLK = 48Mhz
 * Baud = 115200
 */
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        26,                                      // BRDIV = 26
        1,                                       // UCxBRF = 1
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

/**
 * This function sets up UART0 to be used for a console to display information
 *
 * @author amargan (7/4/2014)
 */
void Configure_UART(void)
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
    		                                   GPIO_PIN2 | GPIO_PIN3,
											   GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);
}

int fputc(int _c, register FILE *_fp)
{
	//while(!(UCA0IFG&UCTXIFG));
	//UCA0TXBUF = (unsigned char) _c;
	MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)_c);
	return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
	unsigned int i, len;

	len = strlen(_ptr);

	for(i=0 ; i<len ; i++)
	{
		//while(!(UCA0IFG&UCTXIFG));
		//UCA0TXBUF = (unsigned char) _ptr[i];
		MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)_ptr[i]);
	}

	return len;
}

/**
 *
 */
void Configure_EcatPDI (void)
{
    // Set P3.0 to output direction CS
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);
    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(SPI_ECAT_PORT,
                                               ECAT_SPI_PINS,
                                               GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring SPI in 3wire master mode */
    MAP_SPI_initMaster(EUSCI_ECAT, &spiMasterConfig);
    /* Enable SPI module */
    MAP_SPI_enableModule(EUSCI_ECAT);
    // Configure the SPI INT pin as an input.
    // Configure the SPI EPROM_LOADED pin as an input.
    // Set P5.0 P5.2 to input direction
    MAP_GPIO_setAsInputPin(ECAT_GPIO_PORT, ECAT_IRQ_PIN | ECAT_EEL_PIN);
    //GPIO_interruptEdgeSelect(ECAT_GPIO_PORT, ECAT_IRQ_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    //GPIO_enableInterrupt(ECAT_GPIO_PORT, ECAT_IRQ_PIN);
    //Interrupt_enableInterrupt(INT_PORT5);

    DPRINT("%s\n",__FUNCTION__);

}

/*
 *
 */
void Configure_Led(void)
{
    // Set P1.0 to output direction
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    // Set P2.[0,1,2] to output direction
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
    // Set P3.6 to output direction
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
    // Set P4.6 to output direction
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);

    DPRINT("%s\n",__FUNCTION__);
}

/*
 *
 */
void Configure_Switch(void)
{
	/* Configuring P1.[1,4] as an input and enabling interrupts */
	MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN0);
	/* Configuring P1.[1,4] as an input and enabling interrupts */
	MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    //GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    //GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    //GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    //Interrupt_enableInterrupt(INT_PORT1);

    DPRINT("%s\n",__FUNCTION__);
}

/*
 *
 */
void Configure_Timer(void)
{
    /* Configuring Timer32 to 48000 (1ms) of MCLK in periodic mode */
	MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
            TIMER32_PERIODIC_MODE);
	MAP_Interrupt_enableInterrupt(INT_T32_INT1);
	MAP_Timer32_setCount(TIMER32_0_BASE,48000);
	MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    DPRINT("%s\n",__FUNCTION__);
}





