/*
 * pins.h
 *
 *  Created on: Feb 28, 2025
 *      Author: amargan
 */

#ifndef SOES_TEST_INCLUDE_PINS_H_
#define SOES_TEST_INCLUDE_PINS_H_

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// UARTA -> myUART0 Pinmux
//
//
// UARTA_TX - GPIO Settings
//
#define GPIO_PIN_UARTA_TX 42
#define myUART0_UARTTX_GPIO 42
#define myUART0_UARTTX_PIN_CONFIG GPIO_42_UARTA_TX
//
// UARTA_RX - GPIO Settings
//
#define GPIO_PIN_UARTA_RX 43
#define myUART0_UARTRX_GPIO 43
#define myUART0_UARTRX_PIN_CONFIG GPIO_43_UARTA_RX

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myUART0
// ISR need to be defined for the registered interrupts
#define INT_myUART0 INT_UARTA
#define INT_myUART0_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP8
extern __interrupt void UART_RX_IntHandler(void);

//*****************************************************************************
//
// UART Configurations
//
//*****************************************************************************
//
// myUART0 Configuration
//
#define myUART0_BASE UARTA_BASE
#define myUART0_BAUDRATE 115200
#define myUART0_CONFIG_WLEN UART_CONFIG_WLEN_8
#define myUART0_CONFIG_STP2 UART_CONFIG_STOP_ONE
#define myUART0_CONFIG_EPS UART_CONFIG_PAR_ODD
#define myUART0_CONFIG_SPS 0
#define myUART0_FIFO_TXIFLSEL UART_FIFO_TX7_8
#define myUART0_FIFO_RXIFLSEL UART_FIFO_RX1_8

void    myUART0_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void    Board_init();
//void    INTERRUPT_init();
void    UART_init();
void    PinMux_init();

#endif /* SOES_TEST_INCLUDE_PINS_H_ */
