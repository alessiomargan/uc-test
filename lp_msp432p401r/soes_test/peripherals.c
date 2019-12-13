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

#include "pins.h"
#include "globals.h"
#include "soes_hook.h"
#include "peripherals.h"
#include "soes/esc.h"


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
 * SPI dac config
 */
const eUSCI_SPI_MasterConfig spiDacConfig =
{
	.selectClockSource		= EUSCI_A_SPI_CLOCKSOURCE_SMCLK,	// SMCLK Clock Source
	.clockSourceFrequency 	= CS_24MHZ,                     	// SMCLK = DCO/2 = 24MHZ
	.desiredSpiClock		= CS_24MHZ,                     	// SPICLK
	.msbFirst				= EUSCI_A_SPI_MSB_FIRST,        	// MSB First
    .clockPhase				= EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
    .clockPolarity			= EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
    .spiMode				= EUSCI_A_SPI_3PIN              	// 3Wire SPI Mode
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

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
	.selectClockSource		= EUSCI_B_I2C_CLOCKSOURCE_SMCLK,		// SMCLK Clock Source
	.i2cClk					= CS_3MHZ,								// SMCLK = 24MHz
	.dataRate				= EUSCI_B_I2C_SET_DATA_RATE_400KBPS,	// Desired I2C Clock of 400khz
	.byteCounterThreshold	= 0,									// No byte counter threshold
	.autoSTOPGeneration		= EUSCI_B_I2C_NO_AUTO_STOP				// No Autostop
};

/* Timer_A Continuous Mode Configuration Parameter */
const Timer_A_UpModeConfig upModeConfig =
{
	.clockSource		= TIMER_A_CLOCKSOURCE_SMCLK,       	// SMCLK Clock Source
	.clockSourceDivider	= TIMER_A_CLOCKSOURCE_DIVIDER_1,  	// SMCLK/1
	.timerPeriod		= (SMCLK_FREQUENCY/DFLT_SAMPLE_FREQ),
	.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE,   	// Disable Timer ISR
	.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,// Disable CCR0
	.timerClear 		= TIMER_A_DO_CLEAR                    // Clear Counter
};

/* Timer_A Compare Configuration Parameter */
const Timer_A_CompareModeConfig compareConfig =
{
	.compareRegister		= TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
	.compareInterruptEnable	= TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,	// Enable CCR interrupt
	.compareOutputMode		= TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
	.compareValue			= (SMCLK_FREQUENCY/DFLT_SAMPLE_FREQ)
};

/*
 * Timer_A Compare Configuration Parameter
 * CCR1 is used to trigger the ADC14, conversion time is defined by the resolution
 * 14bit -> 16 cycles + 1 cycle (SLAU356d, 20.2.8.3)
 *
 * In this example, 14-bit resolution at 24Mhz ~708ns conversion time
 * Sample time is defined by at least 4 ADC clocks, pulse sample mode
 * Sample period is 1200/24Mhz = 50us
 */
Timer_A_PWMConfig pwmConfig =
{
	.clockSource 		= TIMER_A_CLOCKSOURCE_SMCLK,
	.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1,
	.timerPeriod 		= (SMCLK_FREQUENCY/DFLT_SAMPLE_FREQ),
	.compareRegister 	= TIMER_A_CAPTURECOMPARE_REGISTER_1,
	.compareOutputMode 	= TIMER_A_OUTPUTMODE_SET_RESET,
	.dutyCycle 			= (SMCLK_FREQUENCY/DFLT_SAMPLE_FREQ) * 0.75	// 25% duty cycle
};


void jump_to_bootloader(void) {

	//ResetCtl_initiateHardReset();
	SysCtl_rebootDevice();
}


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
	//while(!(UCA0IFG&UCTXIFG));
	//UCA0TXBUF = (unsigned char) _c;
	MAP_UART_transmitData(EUSCI_UART, (unsigned char)_c);
	return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
	unsigned int i, len;

	len = strlen(_ptr);

	for(i=0 ; i<len ; i++) {
		//while(!(UCA0IFG&UCTXIFG));
		//UCA0TXBUF = (unsigned char) _ptr[i];
		MAP_UART_transmitData(EUSCI_UART, (unsigned char)_ptr[i]);
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
    // EEL
    //MAP_GPIO_setAsInputPin(PORT_ECAT_GPIO, PIN_ECAT_EEL);
    // BOOT
	MAP_GPIO_setAsOutputPin(PORT_ECAT_BOOT, PIN_ECAT_BOOT);
    MAP_GPIO_setOutputLowOnPin(PORT_ECAT_BOOT, PIN_ECAT_BOOT);
    // IRQ
	MAP_GPIO_setAsInputPin(PORT_ECAT_GPIO, PIN_ECAT_IRQ);
    MAP_GPIO_interruptEdgeSelect(PORT_ECAT_GPIO, PIN_ECAT_IRQ, GPIO_HIGH_TO_LOW_TRANSITION);
    MAP_GPIO_enableInterrupt(PORT_ECAT_GPIO, PIN_ECAT_IRQ);
    MAP_Interrupt_enableInterrupt(INT_ECAT);

    DPRINT("%s\n",__FUNCTION__);

}

/**
 *	DAC
 */
void Configure_DAC (void)
{
#ifndef LAUNCHPAD
    // CLR
    MAP_GPIO_setAsOutputPin(PORT_DAC_P2, PIN_DAC_CLR);
    MAP_GPIO_setOutputHighOnPin(PORT_DAC_P2, PIN_DAC_CLR);
    // SYNC LDAC
    MAP_GPIO_setAsOutputPin(PORT_DAC_P1, PIN_DAC_SYNC|PIN_DAC_LDAC);
    MAP_GPIO_setOutputHighOnPin(PORT_DAC_P1, PIN_DAC_SYNC|PIN_DAC_LDAC);

    // CLK MOSI
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(PORT_DAC_P2,
    		PIN_DAC_CLK|PIN_DAC_DIN,
			GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring SPI in 3wire master mode */
    MAP_SPI_initMaster(EUSCI_DAC, &spiDacConfig);
    /* Enable SPI module */
    MAP_SPI_enableModule(EUSCI_DAC);

    DPRINT("%s\n",__FUNCTION__);
#endif
}

/*
 * Leds Button Switches and Probes
 */
void Configure_GPIO(void)
{
#ifdef LAUNCHPAD
	/*
	 * Led
	 */
	// Set P1.0 to output direction
    MAP_GPIO_setAsOutputPin(PORT_LED_RED, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(PORT_LED_RED, GPIO_PIN0);
    // Set P2.[0,1,2] to output direction
    MAP_GPIO_setAsOutputPin(PORT_LED_RED, LED_PINS);
    MAP_GPIO_setOutputLowOnPin(PORT_LED_RED, LED_PINS);
    /*
     * IO probe
     */
    // Set P3.6 to output direction
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
    // Set P6.[0,1] to output direction
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0|GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN0|GPIO_PIN1);
    /*
     * Buttons switch
     */
    // Configuring P1.[1,4] as an input and enabling interrupts
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    /*
     * PWM timerA out
     */
    // Configuring P2.4 as peripheral output for PWM timerA
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
            GPIO_PRIMARY_MODULE_FUNCTION);
#else
	// LEDs
	MAP_GPIO_setAsOutputPin(PORT_LED_RG, PIN_LED_R|PIN_LED_G);
    MAP_GPIO_setOutputLowOnPin(PORT_LED_RG, PIN_LED_R|PIN_LED_G);
	//
    MAP_GPIO_setAsOutputPin(PORT_LED_B, PIN_LED_B);
    MAP_GPIO_setOutputLowOnPin(PORT_LED_B, PIN_LED_B);
    // DBG
    MAP_GPIO_setAsOutputPin(PORT_DBG, PIN_DBG_1|PIN_DBG_2);
    MAP_GPIO_setOutputLowOnPin(PORT_DBG, PIN_DBG_1|PIN_DBG_2);

#endif
    DPRINT("%s\n",__FUNCTION__);
}


/*
 *
 */
void Configure_Timer(void)
{
    // Configuring Timer32 to 48000 (1ms) of MCLK in periodic mode
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
            TIMER32_PERIODIC_MODE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Timer32_setCount(TIMER32_0_BASE,48000);
    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    DPRINT("%s\n",__FUNCTION__);
}

/*
 *
 */
void Configure_ADC(void)
{
    // Setting reference voltage to 2.5
    MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    MAP_REF_A_enableReferenceVoltage();

	// Initializing ADC (SMCLK/1/1) ==> 24MHz < 25MHz max ADC clock freq
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    MAP_ADC14_setPowerMode(ADC_UNRESTRICTED_POWER_MODE);
#ifdef LAUNCHPAD
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(PORT_ADC_P4, AIN_P4_PINS,
    		GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(PORT_ADC_P5, PIN_A0|PIN_A1,
    		GPIO_TERTIARY_MODULE_FUNCTION);
#else
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(PORT_ADC_P4, PIN_A6|PIN_A7|PIN_A8,
    		GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(PORT_ADC_P5, AIN_P5_PINS,
    		GPIO_TERTIARY_MODULE_FUNCTION);
#endif

    /* Configuring ADC Memory
     * true  ==> cycle after the first round of sample/conversions
     * false ==> single sequence of channels */
	MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM8, false);
    //ch 9
#ifdef LAUNCHPAD
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0,  false); //P5.5
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1,  false); //P5.4
    MAP_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A6,  false); //P4.7
    MAP_ADC14_configureConversionMemory(ADC_MEM3, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A7,  false); //P4.6
    MAP_ADC14_configureConversionMemory(ADC_MEM4, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A8,  false); //P4.5
    MAP_ADC14_configureConversionMemory(ADC_MEM5, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9,  false); //P4.4
    MAP_ADC14_configureConversionMemory(ADC_MEM6, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A10, false); //P4.3
    MAP_ADC14_configureConversionMemory(ADC_MEM7, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A11, false); //P4.2
    MAP_ADC14_configureConversionMemory(ADC_MEM8, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A12, false); //P4.1
#else
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false); //P5.5
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1, false); //P5.4
    MAP_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A2, false); //P5.3
    MAP_ADC14_configureConversionMemory(ADC_MEM3, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A3, false); //P5.2
    MAP_ADC14_configureConversionMemory(ADC_MEM4, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A4, false); //P5.1
    MAP_ADC14_configureConversionMemory(ADC_MEM5, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A5, false); //P5.0
    MAP_ADC14_configureConversionMemory(ADC_MEM6, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A6, false); //P4.7
    MAP_ADC14_configureConversionMemory(ADC_MEM7, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A7, false); //P4.6
    MAP_ADC14_configureConversionMemory(ADC_MEM8, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A8, false); //P4.5
#endif
    MAP_ADC14_enableInterrupt(ADC_INT8);

    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
#ifndef ADC_TRG_SRC_PWM
    /* Configuring Timer_A in continuous mode and sourced from ACLK */
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);
    /* Configuring Timer_A0 in CCR1 to trigger at 16000 (0.5s) */
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);
#endif
    /* Configuring the sample trigger to be sourced from Timer_A0  and setting it
     * to automatic iteration after it is triggered
     * */
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);
    // Enabling Interrupts
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    // Enable conversion
    MAP_ADC14_enableConversion();
    // Start timer
#ifndef ADC_TRG_SRC_PWM
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
#else
    /*
     * use flash params for configuring periphs
     */
    pwmConfig.timerPeriod = (SMCLK_FREQUENCY/sdo.flash.analog_sample_freq);
    pwmConfig.dutyCycle   = (SMCLK_FREQUENCY/sdo.flash.analog_sample_freq) * 0.75;  // 25% duty cycle

    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
#endif
}

/*
 *
 */
void Configure_ADC_temp(void)
{
	// Setting reference voltage to 2.5 and enabling temperature sensor
	MAP_REF_A_enableTempSensor();
	MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
	MAP_REF_A_enableReferenceVoltage();
#if defined(__MCU_HAS_SYSCTL__)
	cal30 = MAP_SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_30_DEGREES_C);
	cal85 = MAP_SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_85_DEGREES_C);
#endif
#if defined(__MCU_HAS_SYSCTL_A__)
	cal30 = MAP_SysCtl_getTempCalibrationConstant(SYSCTL_A_2_5V_REF, SYSCTL_A_30_DEGREES_C);
	cal85 = MAP_SysCtl_getTempCalibrationConstant(SYSCTL_A_2_5V_REF, SYSCTL_A_85_DEGREES_C);
#endif
	calDifference = cal85 - cal30;

	// Initializing ADC (SMCLK/1/1) ==> 24MHz < 25MHz max ADC clock freq
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);

    MAP_ADC14_setPowerMode(ADC_UNRESTRICTED_POWER_MODE);

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,
    		GPIO_PIN5|GPIO_PIN4,
    		GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
    		GPIO_PIN7|GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1,
    		GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory
     * true  ==> cycle after the first round of sample/conversions
     * false ==> single sequence of channels */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM7, ADC_MEM16, false);
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
    MAP_ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_96,ADC_PULSE_WIDTH_4);
	MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

#ifndef ADC_TRG_SRC_PWM
    /* Configuring Timer_A in continuous mode and sourced from ACLK */
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);
    /* Configuring Timer_A0 in CCR1 to trigger at 16000 (0.5s) */
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);
#endif

    /* Configuring the sample trigger to be sourced from Timer_A0  and setting it
     * to automatic iteration after it is triggered */
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);
    // Enabling Interrupts
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    // Enable conversion
    MAP_ADC14_enableConversion();
    // Start timer
#ifndef ADC_TRG_SRC_PWM
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
#else
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
#endif
}




