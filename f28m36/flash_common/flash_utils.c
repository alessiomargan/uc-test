#include "flash_utils.h"
#include <shared_ram.h>

//#define _16KSector_u32length 	0x1000
//#define _64KSector_u32length   	0x4000
#ifdef _C28X
	uint16_t dataBuff[1280];
#else
	uint8_t dataBuff[2560];
#endif

#pragma CODE_SECTION(Example_Error,ramFuncSection);
void Example_Error(Fapi_StatusType status)
{
    //  Error code will be in the status parameter
        for(;;);
}

#pragma CODE_SECTION(Configure_flashAPI,ramFuncSection);
void Configure_flashAPI(void) {

    Fapi_StatusType            oReturnCheck;
    volatile Fapi_LibraryInfoType       oLibInfo;
    volatile Fapi_FlashStatusType       oFlashStatus;
    Fapi_FlashBankSectorsType  oFlashBankSectors;

    // Gain flash pump semaphore
    // This function must reside in RAM
    FlashGainPump();
#ifdef _C28X
    EALLOW;
#endif

	oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, CPU_FRQ);

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

#ifdef _C28X
    EDIS;
#endif
	// Leave control over pump
	FlashLeavePump();
}


#pragma CODE_SECTION(Erase_flash_sector,ramFuncSection);
Fapi_StatusType Erase_flash_sector(uint32_t address) {

	Fapi_StatusType            oReturnCheck;
    //Fapi_FlashStatusWordType   oFlashStatusWord;

    FlashGainPump();
#ifdef _C28X
    EALLOW;
#endif
    // Erase Sector
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t *)address);

    // Wait until FSM is done with erase sector operation
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    // The Erase step itself does a verify as it goes.
    // This verify is a 2nd verification that can be done.
    //oReturnCheck = Fapi_doBlankCheck((uint32_t *)address, _64KSector_u32length, &oFlashStatusWord);

#ifdef _C28X
    EDIS;
#endif
    // Leave control over pump
    FlashLeavePump();

    return oReturnCheck;

}


#pragma CODE_SECTION(Write_Flash,ramFuncSection);
#ifdef _C28X
Fapi_StatusType Write_Flash(uint32_t ui32FlashAddr, uint16_t * dataBuffer, uint32_t dataBufferSize) {
#else
Fapi_StatusType Write_Flash(uint32_t ui32FlashAddr, uint8_t * dataBuffer, uint32_t dataBufferSize) {
#endif
	Fapi_StatusType       		oReturnCheck = Fapi_Status_Success;
    //Fapi_FlashStatusWordType   	oFlashStatusWord;
    //Fapi_FlashStatusType   		oFlashStatus;

    uint32_t unitsWritten = 0;
	uint32_t unitsToWrite = dataBufferSize; // bytes or words
	uint32_t progFlashAddr = ui32FlashAddr;
	uint32_t buffer_chunk_size;

    FlashGainPump();
#ifdef _C28X
    EALLOW;
#endif
    while ( (unitsToWrite > 0) && (oReturnCheck == Fapi_Status_Success)) {

    	// check chunk size
    	if ( unitsToWrite > FLS_BANK_WIDTH) {
    		buffer_chunk_size = FLS_BANK_WIDTH;
    	} else {
    		buffer_chunk_size = unitsToWrite;
    	}

    	//DPRINT("progFlashAddr 0x%04X, chunk size %d\n", progFlashAddr, buffer_chunk_size);

        oReturnCheck = Fapi_issueProgrammingCommand(
        		(uint32 *)progFlashAddr, 		// start address in Flash for the data
				dataBuffer+unitsWritten,		// pointer to the Data buffer address
				buffer_chunk_size,				// number of (M3 bytes)/(C28 16-bit words) in the Data buffer
				0, 0, Fapi_AutoEccGeneration);

        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);

        // on error break loop
        if(oReturnCheck != Fapi_Status_Success) {
        	//DPRINT("FAIL Fapi_issueProgrammingCommand 0x%04X\n", progFlashAddr);
        	break;
        }

#if 0
        // Read FMSTAT register contents to know the status of FSM after
        // program command for any debug
        //oFlashStatus = Fapi_getFsmStatus();
        // Verify the values programmed.  The Program step itself does a verify as it goes.
        // This verify is a 2nd verification that can be done.
        oReturnCheck = Fapi_doVerifyByByte((uint8 *)progFlashAddr, buffer_chunk_size, dataBuffer+bytesWritten, &oFlashStatusWord);
        // on error break loop
        if(oReturnCheck != Fapi_Status_Success) {
        	DPRINT("FAIL Fapi_doVerifyByByte 0x%04X\n", progFlashAddr);
        	break;
        }
#endif
        unitsToWrite  -= buffer_chunk_size;
        unitsWritten  += buffer_chunk_size;
        progFlashAddr += buffer_chunk_size;
	}

#ifdef _C28X
    EDIS;
#endif
    FlashLeavePump();

	return oReturnCheck;

}

#pragma CODE_SECTION(Test_EraseWrite_flash,ramFuncSection);
Fapi_StatusType Test_EraseWrite_flash(uint32_t sectorAddr)
{
	Fapi_StatusType       		oReturnCheck = Fapi_Error_Fail;
	uint16_t i;

	for (i=0; i<sizeof(dataBuff); i++ ) { dataBuff[i] = i; }

	oReturnCheck = Erase_flash_sector(sectorAddr);
    if(oReturnCheck != Fapi_Status_Success) {
    	//DPRINT("FAIL Test_EraseWrite_flash 0x%04X\n", sectorAddr);
    	return oReturnCheck;
    }

	oReturnCheck = Write_Flash(sectorAddr,dataBuff,sizeof(dataBuff));
    if(oReturnCheck != Fapi_Status_Success) {
    	//DPRINT("FAIL Test_EraseWrite_flash 0x%04X\n", sectorAddr);
    	return oReturnCheck;
    }

    return oReturnCheck;
}
