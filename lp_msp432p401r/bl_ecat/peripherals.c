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
 * SPI ecat config
 */
const eUSCI_SPI_MasterConfig spiEcatConfig =
{
	.selectClockSource		= EUSCI_B_SPI_CLOCKSOURCE_SMCLK,	// SMCLK Clock Source
	.clockSourceFrequency 	= CS_24MHZ,                     	// SMCLK = DCO/2 = 24MHZ
	.desiredSpiClock		= CS_12MHZ,                     	// SPICLK = 12Mhz max with EL9800
	.msbFirst				= EUSCI_B_SPI_MSB_FIRST,        	// MSB First
    .clockPhase				= EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
    .clockPolarity			= EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
    .spiMode				= EUSCI_B_SPI_3PIN              	// 3Wire SPI Mode
};

/*
 * http://processors.wiki.ti.com/index.php/Printf_support_for_MSP430_CCSTUDIO_compiler
 * http://processors.wiki.ti.com/index.php/USCI_UART_Baud_Rate_Gen_Mode_Selection
 * UART Config
 * SMCLK = 24Mhz
 * Baud = 115200
 * 13,0,0
 */
const eUSCI_UART_Config uartConfigOverSampl =
{
	.selectClockSource	= EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
    .clockPrescalar		= 13,                                      // BRDIV = 208
	.firstModReg		= 0,                                       // UCxBRF = 1
	.secondModReg		= 0,                                       // UCxBRS = 0
	.parity				= EUSCI_A_UART_NO_PARITY,                  // No Parity
	.msborLsbFirst		= EUSCI_A_UART_LSB_FIRST,                  // LSB First
	.numberofStopBits	= EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
	.uartMode			= EUSCI_A_UART_MODE,                       // UART mode
	.overSampling		= EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

/**
 * This function sets up UART to be used for a console to display information
 *
 * @author amargan (7/4/2014)
 */
void Configure_UART(void)
{
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(PORT_UART,
    		PIN_UART_RX | PIN_UART_TX, GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_UART, &uartConfigOverSampl);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_UART);

    DPRINT("\n\n%s\n",__FUNCTION__);
}

int fputc(int _c, register FILE *_fp)
{
	while(!(UCA0IFG&UCTXIFG));
	UCA0TXBUF = (unsigned char) _c;
	//MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)_c);
	return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
	unsigned int i, len;

	len = strlen(_ptr);

	for(i=0 ; i<len ; i++)
	{
		while(!(UCA0IFG&UCTXIFG));
		UCA0TXBUF = (unsigned char) _ptr[i];
		//MAP_UART_transmitData(EUSCI_A0_BASE, (unsigned char)_ptr[i]);
	}

	return len;
}

/**
 *	Ethercat
 */
void Configure_EcatPDI (void)
{
    // CS
    MAP_GPIO_setAsOutputPin(PORT_ECAT_CS, PIN_ECAT_CS);
    MAP_GPIO_setOutputHighOnPin(PORT_ECAT_CS, PIN_ECAT_CS);
    // CLK MISO MOSI
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(PORT_SPI_ECAT, ECAT_SPI_PINS,
                                               GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring SPI in 3wire master mode */
    MAP_SPI_initMaster(EUSCI_ECAT, &spiEcatConfig);
    /* Enable SPI module */
    MAP_SPI_enableModule(EUSCI_ECAT);
    // IRQ
    //MAP_GPIO_setAsInputPin(PORT_ECAT_GPIO, PIN_ECAT_EEL);
	MAP_GPIO_setAsInputPin(PORT_ECAT_GPIO, PIN_ECAT_IRQ);
    MAP_GPIO_interruptEdgeSelect(PORT_ECAT_GPIO, PIN_ECAT_IRQ, GPIO_HIGH_TO_LOW_TRANSITION);
    MAP_GPIO_enableInterrupt(PORT_ECAT_GPIO, PIN_ECAT_IRQ);
    MAP_Interrupt_enableInterrupt(INT_ECAT);

    DPRINT("%s\n",__FUNCTION__);

}

/*
 *
 */
void Configure_GPIO(void)
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





