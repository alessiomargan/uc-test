//*****************************************************************************
//
// Copyright (C) 2012 - 2017 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the
//  distribution.
//
//  Neither the name of Texas Instruments Incorporated nor the names of
//  its contributors may be used to endorse or promote products derived
//  from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// MSP432 Family Interrupt Vector Table for CGT
//
//****************************************************************************

/*
 * Changelog
 * 2017-11-28  0007  FS  Add different header file for MSP432P4111, MSP432P411V, and MSP432P411Y. 
 */

#include <stdint.h>
#if defined (__MSP432P401R__) || defined (__MSP432P401M__)
   #include "MSP432P401x/BSL432_device_file.h"
#elif defined (__MSP432P4111__) || defined (__MSP432P411V__) || defined (__MSP432P411Y__)
   #include "MSP432P4111/BSL432_device_file.h"
#else
#endif

/* Forward declaration of the default fault handlers. */
static void resetISR(void);
static void nmiISR(void);
static void faultISR(void);


/* External declaration for the reset handler that is to be called when the */
/* processor is started                                                     */
extern void _c_int00(void);


/* Linker variable that marks the top of the stack. */
extern unsigned long __STACK_END;


/* External declarations for the interrupt handlers used by the application. */

/* To be added by user */
void BSL432_BSLentry(uint32_t bslParams);  // BSL entry function for boot code or user
extern void main(uint32_t bslParams);  // BSL main routine
extern void (* const BSL432_BSLVECTORS[4])(void);  // BSL vector table

/* Interrupt vector table. Only sparsly filled. Driver Lib will copy it to
 * beginning of RAM for VTOR use. Make sure other interrupts don't interfer
 * with the BSL when it is started from a user application. */
#pragma RETAIN(interruptVectors)
#pragma DATA_ALIGN (interruptVectors, 128)
#pragma DATA_SECTION(interruptVectors, ".intvecs")
void (* const interruptVectors[])(void) =
{
    (void (*)(void))((uint32_t)&__STACK_END),
                                            /* The initial stack pointer */
    resetISR,                               /* The reset handler         */
    nmiISR,                                 /* The NMI handler           */
    faultISR                                /* The hard fault handler    */
};


/* This is the code that gets called when the processor first starts execution */
/* following a reset event.  Only the absolutely necessary set is performed,   */
/* after which the application supplied entry() routine is called.  Any fancy  */
/* actions (such as making decisions based on the reset cause register, and    */
/* resetting the bits in that register) are left solely in the hands of the    */
/* application.                                                                */
void resetISR(void)
{
    /* Jump to the CCS C Initialization Routine. */
    __asm("    .global _c_int00\n"
          "    b.w     _c_int00");
}


/* This is the code that gets called when the processor receives a NMI.  This  */
/* simply enters an infinite loop, preserving the system state for examination */
/* by a debugger.                                                              */
static void nmiISR(void)
{
    /* Fault trap exempt from ULP advisor */
    #pragma diag_push
    #pragma CHECK_ULP("-2.1")

    /* Enter an infinite loop. */
    while(1)
    {
    }

    #pragma diag_pop
}


/* This is the code that gets called when the processor receives a fault        */
/* interrupt.  This simply enters an infinite loop, preserving the system state */
/* for examination by a debugger.                                               */
static void faultISR(void)
{
    /* Fault trap exempt from ULP advisor */
    #pragma diag_push
    #pragma CHECK_ULP("-2.1")

    /* Enter an infinite loop. */
    while(1)
    {
    }

    #pragma diag_pop
}


/* BSL start up */

/**
 * BSL entry table
 * Is positioned at the beginning of BSL memory
 */
#pragma RETAIN(BSL432_BSLentryTable)
#pragma DATA_SECTION(BSL432_BSLentryTable, ".bsl_start")
void (* const BSL432_BSLentryTable[1])(uint32_t) =
{
    BSL432_BSLentry,
};

/**
 * BSL entry function
 */
void BSL432_BSLentry(uint32_t bslParams)
{
    /* Store the BSL parameters in RAM */
    *((uint32_t *)(BSL432_RAM_BSL_PARAMS)) = bslParams;
    /* Initialize the vector table offset to point to our small table */
    SCB->VTOR = (uint32_t)interruptVectors;

    /* Disable watchdog timer to prevent a reset */
    // Watchdog is disabled in BSL API

    /* Jump to the CCS C Initialization Routine. */
    _c_int00();

    /* Call the BSL main routine with the default BSL mode. */
    main(bslParams);
}
