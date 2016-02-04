#include <stdint.h>


#if defined (_C28X)
	#include "F021_Concerto_C28x.h"
	#include "F28M36x_Device.h"
	#include "F28M36x_GlobalPrototypes.h"
	#include "F28M36x_Flash.h"
#else
	#include "F021_Concerto_Cortex.h"
	#include "driverlib/device.h"
	#include "driverlib/flash.h"
#endif


#define _16KSector_u32length 	0x1000
#define _64KSector_u32length   	0x4000


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

	#if (CPU_FRQ_150MHZ || DSP28_F28M36Px_150MHZ || DSP28_F28M36Hx_150MHZ)
	oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 150);
	#endif
	#if (CPU_FRQ_125MHZ || DSP28_F28M36Px_125MHZ)
	oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 125);
	#endif
	#if (CPU_FRQ_100MHZ || DSP28_F28M36Hx_100MHZ)
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


#pragma CODE_SECTION(erase_flash_sector,"ramfuncs");
Fapi_StatusType erase_flash_sector(uint32_t address) {

	Fapi_StatusType            oReturnCheck;
    Fapi_FlashStatusWordType   oFlashStatusWord;

    FlashGainPump();

    // Erase Sector
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t *)address);

    // Wait until FSM is done with erase sector operation
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    // The Erase step itself does a verify as it goes.
    // This verify is a 2nd verification that can be done.
    oReturnCheck = Fapi_doBlankCheck((uint32_t *)address, _64KSector_u32length, &oFlashStatusWord);

    // Leave control over pump
    FlashLeavePump();

    return oReturnCheck;

}
