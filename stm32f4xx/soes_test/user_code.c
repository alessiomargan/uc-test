/*
 * user_code.c
 *
 *  Created on: Nov 6, 2024
 *      Author: amargan
 */
#include <soes/esc.h>
#include <soes/ecat_slv.h>
#include <globals.h>
#include <params.h>
#include <flash_utils.h>
#include <user_code.h>

#include <stdio.h>
#include <main.h>
#include <stm32f4xx_hal.h>

extern void CAN_Config(CAN_HandleTypeDef *hcan);
extern uint32_t ESC_SYNCactivation(void);
extern esc_cfg_t config;
uint32_t uid[3];

///////////////////////////////////////////////////////////////////////////
// NOTE : in syscall.c function _write is defined "weak"
// the big difference seems the use of
// 		HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
// instead of a loop over "len" of
// 		HAL_UART_Transmit(&printf_uart, (uint8_t *)&ch, 1, 0xFFFF);
///////////////////////////////////////////////////////////////////////////

#if 0
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&printf_uart, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
#endif

int _write(int file, char *ptr, int len)
{

    HAL_StatusTypeDef hstatus;
    hstatus = HAL_UART_Transmit(&printf_uart, (uint8_t*) ptr, len,
    HAL_MAX_DELAY);
    if (hstatus == HAL_OK) return len;
    return -1;
}

#ifdef EE_LOAD_Pin
/*
 * defined weak in esc_hw_et1100.c
 * The LAN9252 doesn't use a dedicated "EEPROM loaded" pin
 */
void assert_EE_LOAD(void) {

	while ( HAL_GPIO_ReadPin(EE_LOAD_GPIO_Port, EE_LOAD_Pin) != GPIO_PIN_SET ) {
		HAL_Delay(3);
	}
	DPRINT ("EE_LOAD pin OK\n");
}
#endif

void read_UID(void)
{

    uid[0] = *(uint32_t*) UID_BASE;
    uid[1] = *(uint32_t*) (UID_BASE + 4);
    uid[2] = *(uint32_t*) (UID_BASE + 8);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//if (htim->Instance == TIM6) {
	//	HAL_IncTick();
	//}

	if (htim->Instance == TIM7) {
		DBG_2_ON;
		if ( ! ESC_SYNCactivation() ) {
			ecat_slv();
		}
		DBG_2_OFF;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

    bool ret = false;

    if (GPIO_Pin == ECAT_IRQ_Pin) {
        DBG_1_ON;
        ecat_slv();
        DBG_1_OFF;
    } else if (GPIO_Pin == BTN_1_Pin) {
        sdo.flash._signature_ = FLASH_SIGN_VALID;
        ret = Write_flash((uint32_t) &flash_sdo, (void*) &sdo.flash, sizeof(flash_sdo));
        DPRINT("%s %p ret=%d\n", __FUNCTION__, (uint32_t) &flash_sdo, ret);
    }

}

void user_code_init(void)
{

    /* Disable I/O buffering for STDOUT stream, so that
     * chars are sent out as soon as they are printed. */

    setvbuf(stdout, NULL, _IONBF, 0);

    read_UID();
    DPRINT("+++ Start Application +++\n");
    print_build_info();
    if (Read_Flash_Params() == PARAMS_CMD_ERROR) {
        //
        //glob_fault.bit.warn_read_flash = 1;
        DPRINT("Read_Flash_Params FAIL\n");
        if (Load_Default_Params() == PARAMS_CMD_ERROR) {
            // FATAL ERROR
            Error_Handler();
        }
        DPRINT("Load_Default_Params\n");
    }
    DPRINT("sdo.ram.fw_ver=%s\n", sdo.ram.fw_ver);
    DPRINT("FLASH_SDO\n");
    print_sdo(&flash_sdo);
    DPRINT("DFLT_FLASH_SDO\n");
    print_sdo(&dflt_flash_sdo);
    DPRINT("SDO\n");
    print_sdo(&sdo.flash);
    /* Init soes */
    ecat_slv_init(&config);
    /* */
    CAN_Config(&hcan1);
    /* timer initialization with interrupt mode */
    HAL_TIM_Base_Start_IT(&htim7);

}

void user_code_loop(void)
{

    HAL_Delay(500);
    LED_1_TGL;

}

//extern void jump_to_bootloader(void)  __attribute__((weak, alias("default_jump_to_bootloader")));
void jump_to_bootloader(void)
{

    HAL_TIM_Base_Stop_IT(&htim7);
    HAL_NVIC_SystemReset();
}
