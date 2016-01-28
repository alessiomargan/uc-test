#include <stdbool.h>
#include <stdint.h>

#include "driverlib/device.h"
#include "driverlib/flash.h"

#include "F021_Concerto_Cortex.h"

#include "osal.h"
#include "bl_config.h"
#include "soes_hook.h"
#include "soes/esc_foe.h"

#define FLASH_DEBUG
#ifdef FLASH_DEBUG
    #define DPRINT(...) OSAL_PRINT ("flash: "__VA_ARGS__)
    #define DEBUG_ASSERT(expression)    ASSERT(expression)
#else
    #define DPRINT(...)
    #define DEBUG_ASSERT(expression)
#endif  /* ESC_DEBUG */

#define _16KSector_u32length 	0x1000
#define _64KSector_u32length   	0x4000

extern foe_cfg_t 	foe_config;

Fapi_StatusType Fapi_serviceWatchdogTimer(void)
{
   /* User to add their own watchdog servicing code here */
   return(Fapi_Status_Success);
}

Fapi_StatusType Fapi_setupEepromSectorEnable(void)
{
   Fapi_GlobalInit.m_poFlashControlRegisters->Fbse.u32Register = 0xFFFF;   /* Value must be 0xFFFF to enable erase and programming of the EEPROM bank, 0 to disable */
   FAPI_WRITE_LOCKED_FSM_REGISTER(Fapi_GlobalInit.m_poFlashControlRegisters->FsmSector.u32Register, 0x0U); /* Enables sectors 32-63 for bank and sector erase */
   FAPI_WRITE_LOCKED_FSM_REGISTER(Fapi_GlobalInit.m_poFlashControlRegisters->FsmSector1.u32Register, 0x0U); /* Enables sectors 0-31 for bank and sector erase */
   FAPI_WRITE_LOCKED_FSM_REGISTER(Fapi_GlobalInit.m_poFlashControlRegisters->FsmSector2.u32Register, 0x0U); /* Enables sectors 32-63 for bank and sector erase */

   return(Fapi_Status_Success);
}

Fapi_StatusType Fapi_setupBankSectorEnable(void)
{
   Fapi_GlobalInit.m_poFlashControlRegisters->Fbse.u32Register = 0xFFFF;                  /* Enable sectors 0-15 for erase and programming */
   FAPI_WRITE_LOCKED_FSM_REGISTER(Fapi_GlobalInit.m_poFlashControlRegisters->FsmSector.u32Register, 0x0U);

   return(Fapi_Status_Success);
}


#pragma CODE_SECTION(Example_Error,"ramfuncs");
void Example_Error(Fapi_StatusType status)
{
    //  Error code will be in the status parameter
        for(;;);
}

#pragma CODE_SECTION(Configure_flashAPI,"ramfuncs");
void Configure_flashAPI(void) {

    Fapi_StatusType            oReturnCheck;
    volatile Fapi_LibraryInfoType       oLibInfo;
    volatile Fapi_FlashStatusType       oFlashStatus;
    Fapi_FlashBankSectorsType  oFlashBankSectors;

    // Gain flash pump semaphore
    // This function must reside in RAM
    FlashGainPump();


	#if CPU_FRQ_125MHZ
	oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 125);
	#endif

	#if CPU_FRQ_100MHZ
	oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 100);
	#endif

	#if CPU_FRQ_75MHZ
	oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 75);
	#endif

	#if CPU_FRQ_60MHZ
	oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 60);
	#endif

	if(oReturnCheck != Fapi_Status_Success)
	{
		//Check Flash API documentation for possible errors
		Example_Error(oReturnCheck);
	}

	// Fapi_getLibraryInfo function can be used to get the information
	// specific to the compiled version of the API library
	oLibInfo = Fapi_getLibraryInfo();

	// Fapi_setActiveFlashBank function sets the Flash bank and FMC
	// for further Flash operations to be performed on the bank
	oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
	if(oReturnCheck != Fapi_Status_Success)
	{
		// Check Flash API documentation for possible errors
		Example_Error(oReturnCheck);
	}

	// Fapi_getBankSectors function returns the bank starting address,
	// number of sectors, sector sizes, and bank technology type
	// Above information is returned in a structure oFlashBankSectors of type
	// Fapi_FlashBankSectorsType
	oReturnCheck = Fapi_getBankSectors(Fapi_FlashBank0,&oFlashBankSectors);
	if(oReturnCheck != Fapi_Status_Success)
	{
		// Check Flash API documentation for possible errors
		Example_Error(oReturnCheck);
	}

#if 0
	DPRINT("BankStartAddress 0x%04X\n", oFlashBankSectors.u32BankStartAddress);
	DPRINT("NumberOfSectors %d\n", oFlashBankSectors.u32NumberOfSectors);
	int i = 0;
	for (i=0; i<oFlashBankSectors.u32NumberOfSectors; i++) {
		DPRINT("SectorSizes_%d %d\n", i ,oFlashBankSectors.au8SectorSizes[i]);
	}
#endif
	// Leave control over pump
	FlashLeavePump();
}

#pragma CODE_SECTION(erase_prog_flash,"ramfuncs");
bool erase_prog_flash() {

	Fapi_StatusType            oReturnCheck;
    Fapi_FlashStatusWordType   oFlashStatusWord;

    DPRINT("Erase flash 0x%04X and 0x%04X\n", APP_START_ADDRESS, APP_CRC_ADDRESS);

    FlashGainPump();

    // Erase App Sector
    // Sector M N have the example code so leave it programmed
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t *)APP_START_ADDRESS);

    // Wait until FSM is done with erase sector operation
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    // The Erase step itself does a verify as it goes.
    // This verify is a 2nd verification that can be done.
    oReturnCheck = Fapi_doBlankCheck((uint32_t *)APP_START_ADDRESS, _64KSector_u32length, &oFlashStatusWord);

    // Erase crc app Sector
    // Sector K have the example code so leave it programmed
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t *)APP_CRC_ADDRESS);

    // Wait until FSM is done with erase sector operation
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    // Leave control over pump
    FlashLeavePump();

    return (oReturnCheck == Fapi_Status_Success);

}


#pragma CODE_SECTION(write_flash,"ramfuncs");
Fapi_StatusType write_flash(uint32_t ui32FlashAddr, uint8_t * buffer, uint32_t sizeByte) {

	Fapi_StatusType       		oReturnCheck = Fapi_Status_Success;
    Fapi_FlashStatusWordType   	oFlashStatusWord;
    //Fapi_FlashStatusType   		oFlashStatus;

    uint32_t bytesWritten = 0;
	uint32_t bytesToWrite = sizeByte;
	uint32_t progFlashAddr = ui32FlashAddr;
	uint32_t buffer_chunk_size;

    FlashGainPump();

    while ( (bytesToWrite > 0) && (oReturnCheck == Fapi_Status_Success)) {

    	// check chunk size
    	if ( bytesToWrite > 16) {
    		buffer_chunk_size = 16;
    	} else {
    		buffer_chunk_size = bytesToWrite;
    	}

    	//DPRINT("progFlashAddr 0x%04X, chunk size %d\n", progFlashAddr, buffer_chunk_size);

        oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)progFlashAddr, buffer+bytesWritten, buffer_chunk_size, 0, 0, Fapi_AutoEccGeneration);

        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);

        // on error break loop
        if(oReturnCheck != Fapi_Status_Success) {
        	DPRINT("FAIL Fapi_issueProgrammingCommand 0x%04X\n", progFlashAddr);
        	break;
        }

        // Read FMSTAT register contents to know the status of FSM after
        // program command for any debug
        //oFlashStatus = Fapi_getFsmStatus();

        // Verify the values programmed.  The Program step itself does a verify as it goes.
        // This verify is a 2nd verification that can be done.
        oReturnCheck = Fapi_doVerifyByByte((uint8 *)progFlashAddr, buffer_chunk_size, buffer+bytesWritten, &oFlashStatusWord);

        // on error break loop
        if(oReturnCheck != Fapi_Status_Success) {
        	DPRINT("FAIL Fapi_doVerifyByByte 0x%04X\n", progFlashAddr);
        	break;
        }

        bytesToWrite  -= buffer_chunk_size;
		bytesWritten  += buffer_chunk_size;
        progFlashAddr += buffer_chunk_size;
	}

    FlashLeavePump();

	return oReturnCheck;

}

/*
 * return 0 on success
 */
#pragma CODE_SECTION(foe_write_flash,"ramfuncs");
uint32_t foe_write_flash(foe_writefile_cfg_t * writefile_cfg, uint8_t * data) {

    uint32_t ui32FlashAddr;
    uint32_t bytestowrite = foe_config.buffer_size;

    // What address are we about to program to?
	ui32FlashAddr = writefile_cfg->address_offset + writefile_cfg->dest_start_address;
    DPRINT("write FlashAddr 0x%04X with %d bytes\n", ui32FlashAddr, bytestowrite);

    if ( write_flash(ui32FlashAddr, data, bytestowrite) != Fapi_Status_Success ) {
		DPRINT("FAIL foe_write_flash write FlashAddr 0x%04X with %d bytes\n",
				ui32FlashAddr, bytestowrite);
		return bytestowrite;
	}
	return 0;
}

#pragma CODE_SECTION(foe_write_shared_RAM,"ramfuncs");
uint32 foe_write_shared_RAM(foe_writefile_cfg_t * writefile_cfg, uint8 * data) {

    uint32_t ui32FlashAddr;

    // What address are we about to program to?
	ui32FlashAddr = writefile_cfg->address_offset + writefile_cfg->dest_start_address;
    DPRINT("write FlashAddr 0x%04X with %d bytes\n", ui32FlashAddr, foe_config.buffer_size);

    //

	return 0;
}

#pragma CODE_SECTION(calc_app_crc,"ramfuncs");
uint32_t calc_app_crc(void) {

	uint32 i, k;
	uint32 crc_calc = 0;

	FlashGainPump();
    for(i=0, k=0; k < FLASH_APP_SIZE; i++, k=i*4  ) {
		crc_calc ^= *(uint32*)(APP_START_ADDRESS+k);
	}
    FlashLeavePump();
	//crc_calc = 0xF0CACC1A;

	return crc_calc;
}

/**
 * called in FOE_close
 **/
#pragma CODE_SECTION(write_app_crc,"ramfuncs");
void write_app_crc(void) {

	uint32_t	crc;
    uint32_t 	ui32FlashAddr = APP_CRC_ADDRESS;

	crc = calc_app_crc();
	if ( write_flash(ui32FlashAddr, (uint8 *)&crc, sizeof(crc)) == Fapi_Status_Success ) {
		gFlash_crc = *(uint32*)APP_CRC_ADDRESS;
		DPRINT("write_flash appCRC 0x%04X\n", gFlash_crc);
	} else {
		DPRINT("FAIL write_flash appCRC !!!\n");
	}
	return;
}
