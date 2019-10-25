/******************************************************************************
 *
 * Default Linker Command file for the Texas Instruments TM4C123AH6PM
 *
 * This is derived from revision 14351 of the TivaWare Library.
 *
 *****************************************************************************/

--retain=g_pfnVectors

#define FLASH_APP 	0x00005000
//#define FLASH_APP   0x00000000

MEMORY
{
    FLASH   (RX) : origin = FLASH_APP, length = 0x00032000
    PAR_APP (RW) : origin = 0x0039000, length = 0x00001000
    CALIB   (RW) : origin = 0x0038000, length = 0x00001000
    EMPTY   (RW) : origin = 0x0037000, length = 0x00001000

    SRAM (RWX) : origin = 0x20000000, length = 0x00008000
}

/* The following command line options are set as part of the CCS project.    */
/* If you are building using the command line, or for some reason want to    */
/* define them here, you can uncomment and modify these lines as needed.     */
/* If you are using CCS for building, it is probably better to make any such */
/* modifications in your CCS project and leave this file alone.              */
/*                                                                           */
/* --heap_size=0                                                             */
/* --stack_size=256                                                          */
/* --library=rtsv7M4_T_le_eabi.lib                                           */

/* Section allocation in memory */

SECTIONS
{
    .intvecs:   > FLASH_APP
    .text   :   > FLASH
    .const  :   > FLASH
    .cinit  :   > FLASH
    .pinit  :   > FLASH
    .init_array : > FLASH

    .vtable :   > 0x20000000
    .data   :   > SRAM
    .bss    :   > SRAM
    .sysmem :   > SRAM
    .stack  :   > SRAM

    .lcd    :   > SRAM
    .font_8 :   > SRAM
    .font_11:   > SRAM
    // cpp LCD
    .pics   :   > SRAM
    .fonts   :   > SRAM

    /* flash parameters */
    .PAR_APP    : > PAR_APP
    /* calibration  */
    .CALIB      : > CALIB

    // see http://processors.wiki.ti.com/index.php/Placing_functions_in_RAM
    .TI.ramfunc : {} load=FLASH, run=SRAM, table(BINIT)
    .binit : {} > FLASH

}

__STACK_TOP = __stack + __STACK_SIZE;
