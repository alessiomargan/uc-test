/******************************************************************************
*
* Copyright (C) 2012 - 2018 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*  Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Default linker command file for Texas Instruments MSP432P401R
*
* File creation date: 01/26/18
*
*****************************************************************************/

--retain=flashMailbox

#define APP_START 0x20000
//#define APP_START 0x00000

MEMORY
{
    // Info memory 16 Kb
    INFO       (RX) : origin = 0x00200000, length = 0x00004000
    // Main memory 256 Kb :
    // bank_0
    MAIN_BL     (RX) : origin = 0x00000000, length = 0x00010000
    BLDR_VERSION (R) : origin = 0x0010000,  length = 0x00000008   // BANK_0 SECTOR_16
    CRC_APP      (R) : origin = 0x0011000,  length = 0x00000004   // BANK_0 SECTOR_17
    // bank_1
    MAIN_APP   (RX) : origin = 0x00020000, length = 0x00010000
    PAR_APP    (RW) : origin = 0x003F000,  length = 0x00001000   // BANK_1 SECTOR_31
    CALIB      (RW) : origin = 0x003E000,  length = 0x00001000   // BANK_1 SECTOR_30

    ALIAS
    {
        SRAM_CODE  (RWX): origin = 0x01000000
        SRAM_DATA  (RW) : origin = 0x20000000
    } length = 0x00010000
}


/* The following command line options are set as part of the CCS project.    */
/* If you are building using the command line, or for some reason want to    */
/* define them here, you can uncomment and modify these lines as needed.     */
/* If you are using CCS for building, it is probably better to make any such */
/* modifications in your CCS project and leave this file alone.              */
/*                                                                           */
/* A heap size of 1024 bytes is recommended when you plan to use printf()    */
/* for debug output to the console window.                                   */
/*                                                                           */
/* --heap_size=1024                                                          */
/* --stack_size=512                                                          */
/* --library=rtsv7M4_T_le_eabi.lib                                           */

/* Section allocation in memory */

SECTIONS
{
    .intvecs:   > APP_START
    .text   :   > MAIN_APP
    .const  :   > MAIN_APP
    .cinit  :   > MAIN_APP
    .pinit  :   > MAIN_APP
    .init_array   :     > MAIN_APP
    .binit        : {}  > MAIN_APP

    /* flash parameters */
    .PAR_APP    : > PAR_APP
    /* flash parameters */
    .CALIB      : > CALIB

    .vtable :   > SRAM_DATA
    .data   :   > SRAM_DATA
    .bss    :   > SRAM_DATA
    .sysmem :   > SRAM_DATA
    .stack  :   > SRAM_DATA (HIGH)

    .TI.ramfunc : {} load=MAIN_APP, run=SRAM_CODE, table(BINIT)
}

/* Symbolic definition of the WDTCTL register for RTS */
WDTCTL_SYM = 0x4000480C;

