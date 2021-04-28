#include <stdint.h>

#include "inc/hw_flash.h"
#include "inc/hw_types.h"

#include "bl_config.h"
#include "boot_loader/bl_decrypt.h"
#include "boot_loader/bl_flash.h"

#include "foe_flash.h"
#include <soes/esc_foe.h>


extern foe_cfg_t gFOE_config;

//#pragma RETAIN(BLDR_Version)
//#pragma DATA_SECTION(BLDR_Version, ".BLDR_VERSION")
const uint8_t BLDR_Version[8] = "tm4c_1.1";

//#pragma RETAIN(CRC_App)
//#pragma DATA_SECTION(CRC_App, ".CRC_APP")
const uint32_t CRC_App;

uint32_t gCalc_crc;
uint16_t crc_ok;


uint32_t foe_write_flash(foe_writefile_cfg_t * writefile_cfg, uint8_t * data) {

    uint32_t ui32FlashAddr;

    // What address are we about to program to?
    ui32FlashAddr = writefile_cfg->address_offset + writefile_cfg->dest_start_address;;
    DPRINT("FlashAddr 0x%04X\n", ui32FlashAddr);

    // Do not program this data into flash if it is beyond the end of flash.
    if ( ui32FlashAddr < FLASH_APP_END ) {

        // If this is the first block and we have been provided with a start
        // hook function, call it here to indicate that we are about to begin
        // flashing a new image.
#ifdef BL_START_FN_HOOK
        if ( writefile_cfg->address_offset == 0 ) {
            BL_START_FN_HOOK();
        }
#endif
        // Clear any flash error indicator.
        BL_FLASH_CL_ERR_FN_HOOK();

        // If this is the first data packet and code protection is enabled,
        // then erase the entire flash.
#ifdef FLASH_CODE_PROTECTION
        if ( writefile_cfg->address_offset == 0 ) {

            // Loop through the pages in the flash, excluding the pages that
            // contain the boot loader and the optional reserved space.
            for ( ui32Idx = APP_START_ADDRESS; ui32Idx < g_ui32FlashEnd; ui32Idx += FLASH_PAGE_SIZE ) {
                // Erase this block of the flash.
                BL_FLASH_ERASE_FN_HOOK(ui32Idx);
            }
        }
#else
        // Flash code protection is not enabled, so see if the data in this
        // packet will be programmed to the beginning of a flash block.  We
        // assume that the flash block size is always a multiple of 1KB so,
        // the start must always
        // be on a flash page boundary, we can be sure that we will hit the
        // start of each page as we receive packets.
        if ( !(ui32FlashAddr & (FLASH_PAGE_SIZE - 1)) ) {
            // Erase this block of the flash.
            BL_FLASH_ERASE_FN_HOOK(ui32FlashAddr);
        }
#endif

        // Decrypt the data if required.
#ifdef BL_DECRYPT_FN_HOOK
        BL_DECRYPT_FN_HOOK(data, FLASH_PAGE_SIZE);
#endif

        // Program this block of data into flash.
        BL_FLASH_PROGRAM_FN_HOOK(ui32FlashAddr, data, FLASH_PAGE_SIZE);

        // If a progress reporting hook function has been provided, call it
        // here.  we pass 0 as the ui32Total parameter to indicate this.
#ifdef BL_PROGRESS_FN_HOOK
        BL_PROGRESS_FN_HOOK(((ui32FlashAddr - APP_START_ADDRESS) + FLASH_PAGE_SIZE), 0);
#endif
    }

    // Did we see any error?
    if ( BL_FLASH_ERROR_FN_HOOK() ) {
    } else {
        //
    }

    // There is more data to be read.
    return 0;
}


