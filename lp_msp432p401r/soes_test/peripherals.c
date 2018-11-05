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

#include "pins.h"
#include "soes_hook.h"
#include "peripherals.h"
#include "soes/esc.h"

#define BSL_PARAM			BSL_DEFAULT_PARAM // I2C slave address = 0x48, Interface selection = Auto

// globals used for adc14 temp sensor
uint32_t	cal30;
uint32_t	cal85;
float 		calDifference;
float		tempC;
float		tempF;
uint8_t		adc_idx;
uint16_t 	conv_adc[256][16];

/*
 * SPI master Config
 */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,	// SMCLK Clock Source
		CS_48MHZ,                       // SMCLK = DCO = 24MHZ
		//CS_24MHZ,                       // SMCLK = DCO/2 = 24MHZ
		CS_12MHZ,                       // SPICLK = 12Mhz max with EL9800
		EUSCI_B_SPI_MSB_FIRST,          // MSB First
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_B_SPI_3PIN              	// 3Wire SPI Mode
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
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        26,                                      // BRDIV = 208
        0,                                       // UCxBRF = 1
        111,                                     // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
		EUSCI_B_I2C_CLOCKSOURCE_SMCLK,			// SMCLK Clock Source
		CS_3MHZ,								// SMCLK = 24MHz
		EUSCI_B_I2C_SET_DATA_RATE_400KBPS,		// Desired I2C Clock of 400khz
		0,										// No byte counter threshold
		EUSCI_B_I2C_NO_AUTO_STOP				// No Autostop
};

/* Timer_A Continuous Mode Configuration Parameter */
const Timer_A_UpModeConfig upModeConfig =
{
		TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK Clock Source
		TIMER_A_CLOCKSOURCE_DIVIDER_2,       // SMCLK = 48MHz
		//TIMER_A_CLOCKSOURCE_DIVIDER_1,     // SMCLK = 24MHz
		//120,                                 // 20kHz
        240,                            	 // 10KHz
		//480,								 //  5kHz
		//1200,							     //  2KHz
		//2400,							     //  1KHz
		TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0
        TIMER_A_DO_CLEAR                     // Clear Counter
};

/* Timer_A Compare Configuration Parameter */
const Timer_A_CompareModeConfig compareConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
		//TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,	// Enable CCR interrupt
		TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
        //120,
		240,                                      	// Period
        //480                                       // Period
        //120                                       // Period
		//2400
};

void jump_to_bootloader(void) {

	ResetCtl_initiateHardReset();
}


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
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfigOverSampl);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);
}

int fputc(int _c, register FILE *_fp)
{
	//while(!(UCA0IFG&UCTXIFG));
	//UCA0TXBUF = (unsigned char) _c;
	MAP_UART_transmitData(EUSCI_A0_BASE, (uint8_t)_c);
	return((uint8_t)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
	uint32_t i, len;

	len = strlen(_ptr);

	for(i=0 ; i<len ; i++) {
		//while(!(UCA0IFG&UCTXIFG));
		//UCA0TXBUF = (unsigned char) _ptr[i];
		MAP_UART_transmitData(EUSCI_A0_BASE, (uint8_t)_ptr[i]);
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
    MAP_GPIO_interruptEdgeSelect(ECAT_GPIO_PORT, ECAT_IRQ_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    MAP_GPIO_enableInterrupt(ECAT_GPIO_PORT, ECAT_IRQ_PIN);
    MAP_Interrupt_enableInterrupt(INT_PORT5);

    DPRINT("%s\n",__FUNCTION__);

}

/*
 * Leds Button Switches and Probes
 */
void Configure_GPIO(void)
{
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, PIN_ALL8);
	// Led
	// Set P1.0 to output direction
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    // Set P2.[0,1,2] to output direction
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);
    // IO probe
    // Set P3.6 to output direction
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
    // Set P6.[0,1] to output direction
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0|GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0|GPIO_PIN1);
    // Buttons switch
    /* Configuring P1.[1,4] as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    MAP_Interrupt_enableInterrupt(INT_PORT1);

    DPRINT("%s\n",__FUNCTION__);
}


/*
 *
 */
void Configure_Timer(void)
{
    /*
     * Configuring Timer32 to 480000 (10ms) of MCLK in periodic mode
     * */
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
            TIMER32_PERIODIC_MODE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Timer32_setCount(TIMER32_0_BASE,480000);
    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    DPRINT("%s\n",__FUNCTION__);
}

/*
 *
 */
void Configure_ADC(void)
{
    /*
     * Initializing ADC (SMCLK/1/2) ==> 24MHz < 25MHz max ADC clock freq
     * */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_2, ADC_TEMPSENSEMAP);

    MAP_ADC14_setPowerMode(ADC_UNRESTRICTED_POWER_MODE);
    /* Setting reference voltage to 2.5 */
    MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    MAP_REF_A_enableReferenceVoltage();

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    		GPIO_PIN5|GPIO_PIN4,
    		GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
    		GPIO_PIN7|GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1,
    		GPIO_TERTIARY_MODULE_FUNCTION);

    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM8, true);
    //ch 9
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0,  false); //P5.5
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1,  false); //P5.4
    MAP_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A6,  false); //P4.7
    MAP_ADC14_configureConversionMemory(ADC_MEM3, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A7,  false); //P4.6
    MAP_ADC14_configureConversionMemory(ADC_MEM4, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A8,  false); //P4.5
    MAP_ADC14_configureConversionMemory(ADC_MEM5, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9,  false); //P4.4
    MAP_ADC14_configureConversionMemory(ADC_MEM6, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A10, false); //P4.3
    MAP_ADC14_configureConversionMemory(ADC_MEM7, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A11, false); //P4.2
    MAP_ADC14_configureConversionMemory(ADC_MEM8, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A12, false); //P4.1

    MAP_ADC14_enableInterrupt(ADC_INT8);
    /* Configuring Timer_A in continuous mode and sourced from ACLK */
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);
    /* Configuring Timer_A0 in CCR1 to trigger at 16000 (0.5s) */
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);
    /* Configuring the sample trigger to be sourced from Timer_A0  and setting it
     * to automatic iteration after it is triggered
     * */
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);
    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    /* Enable conversion */
    MAP_ADC14_enableConversion();
    /* Starting the Timer */
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

}

/*
 *
 */
void Configure_ADC_temp(void)
{
    /*
     * Initializing ADC (MCLK/1/2) with temperature sensor routed
     * */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_2, ADC_TEMPSENSEMAP);

    MAP_ADC14_setPowerMode(ADC_UNRESTRICTED_POWER_MODE);
    /* Setting reference voltage to 2.5 and enabling temperature sensor */
    MAP_REF_A_enableTempSensor();
    MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    MAP_REF_A_enableReferenceVoltage();
    cal30 = MAP_SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_30_DEGREES_C);
    cal85 = MAP_SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_85_DEGREES_C);
    calDifference = cal85 - cal30;

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    		GPIO_PIN5|GPIO_PIN4,
    		GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
    		GPIO_PIN7|GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1,
    		GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM7, ADC_MEM16, true);
    // internal temperature
    MAP_ADC14_configureConversionMemory(ADC_MEM7, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A22, false);
    // 9 channels
    MAP_ADC14_configureConversionMemory(ADC_MEM8,  ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0,  false); //P5.5
    MAP_ADC14_configureConversionMemory(ADC_MEM9,  ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1,  false); //P5.4
    MAP_ADC14_configureConversionMemory(ADC_MEM10, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A6,  false); //P4.7
    MAP_ADC14_configureConversionMemory(ADC_MEM11, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A7,  false); //P4.6
    MAP_ADC14_configureConversionMemory(ADC_MEM12, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A8,  false); //P4.5
    MAP_ADC14_configureConversionMemory(ADC_MEM13, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9,  false); //P4.4
    MAP_ADC14_configureConversionMemory(ADC_MEM14, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A10, false); //P4.3
    MAP_ADC14_configureConversionMemory(ADC_MEM15, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A11, false); //P4.2
    MAP_ADC14_configureConversionMemory(ADC_MEM16, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A12, false); //P4.1

    /* Enabling the interrupt when a conversion on channel x (end of sequence)
     * is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT16);
    /*
     * Configuring the sample/hold time
     * There are two values in the ADCC module. The first value controls
	 * ADC memory locations ADC_MEMORY_0 through ADC_MEMORY_7 and
     * ADC_MEMORY_24 through ADC_MEMORY_31, while the second value
     * controls memory locations ADC_MEMORY_8 through ADC_MEMORY_23.
     * */
    //ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192,ADC_PULSE_WIDTH_192);
    MAP_ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_96,ADC_PULSE_WIDTH_16);

    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Configuring Timer_A in continuous mode and sourced from SMCLK */
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);
    /* Configuring Timer_A0 in CCR1 to trigger at 16000 (0.5s) */
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);
    /* Configuring the sample trigger to be sourced from Timer_A0  and setting it
     * to automatic iteration after it is triggered */
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    /* Enable conversion */
    MAP_ADC14_enableConversion();

    //MAP_ADC14_toggleConversionTrigger();
    /* Starting the Timer */
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);


}


void Configure_I2C(uint8_t slave_address)
{
    /* Select Port 6 for I2C - Set Pin 5, 4 to input Primary Module Function,
     *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
            GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
    /* Initializing I2C Master to SMCLK at 400kbs with no autostop */
	MAP_I2C_initMaster(EUSCI_B1_BASE, &i2cConfig);
	/* Specify slave address */
	MAP_I2C_setSlaveAddress(EUSCI_B1_BASE, slave_address);
	/* Set Master in transmit mode */
	MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
	/* Set Master in receive mode */
	//I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
	/* Enable I2C Module to start operations */
	MAP_I2C_enableModule(EUSCI_B1_BASE);
	/* Enable and clear the interrupt flag */
	MAP_I2C_clearInterruptFlag(EUSCI_B1_BASE,
			EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_NAK_INTERRUPT);
	//Enable master Receive interrupt
	MAP_I2C_enableInterrupt(EUSCI_B1_BASE,
			EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_NAK_INTERRUPT);

	MAP_Interrupt_enableInterrupt(INT_EUSCIB1);
}




