/*
 * can_code.c
 *
 *  Created on: Jul 22, 2025
 *      Author: amargan
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private variables ---------------------------------------------------------*/
CAN_TxHeaderTypeDef txHeader;
CAN_RxHeaderTypeDef rxHeader;
uint8_t txData[8];
uint8_t rxData[8];
uint32_t txMailbox;

void CAN_Config(CAN_HandleTypeDef *hcan)
{

    CAN_FilterTypeDef sFilterConfig;
    /*## Configure the CAN Filter ###########################################*/
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;
    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK) {
        /* Filter configuration Error */
        Error_Handler();
    }
    /*## Start the CAN peripheral ###########################################*/
    if (HAL_CAN_Start(hcan) != HAL_OK) {
        /* Start Error */
        Error_Handler();
    }
    /*## Activate CAN RX notification #######################################*/
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        Error_Handler();
    }

    /*## Configure Transmission process #####################################*/
    txHeader.StdId = 0x100;
    txHeader.ExtId = 0x00, //0x01;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.IDE = CAN_ID_STD;
    txHeader.DLC = 8;
    txHeader.TransmitGlobalTime = DISABLE;
}

/**
 * @brief  Rx Fifo 0 message pending callback
 * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* Get RX message */
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK) {
        /* Reception Error */
        Error_Handler();
    }

    /*  */
    if ((rxHeader.StdId == 0x123) && (rxHeader.IDE == CAN_ID_STD) && (rxHeader.DLC == 2)) {

    } else if ((rxHeader.StdId == 0x5) && (rxHeader.IDE == CAN_ID_STD) && (rxHeader.RTR == CAN_RTR_REMOTE)) {
        txData[0] = 0xDE;
        txData[1] = 0xAD;
        txData[2] = 0xBE;
        txData[3] = 0xEF;

        HAL_CAN_AddTxMessage(hcan, &txHeader, txData, &txMailbox);
    }
}

