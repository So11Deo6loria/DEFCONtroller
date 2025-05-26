/*
 * can.c
 *
 *  Created on: Jul 7, 2022
 *      Author: nate
 */

#include "can.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "threads/can/can.h"


#define HAL_CAN_MODULE_ENABLED

// Global Parameters (For TouchGFX Only)
uint8_t CanTask_BSUpdated = 0;
uint16_t CanTask_BSValue = 175;
uint8_t CanTask_isBSGood = 0;
uint8_t CanTask_ToggleState = 0;
extern uint8_t debugFlagTouchGFX;

// Local Variables
extern CAN_HandleTypeDef hcan2;



// Local Values
static const uint16_t _CanMsgDeviceON = 0xFFFF;
static const uint16_t _CanMsgDeviceOFF = 0x0000;

// Local Functions
static void _CanInitialize( void );
static void _CanSendMessage(uint32_t ID, uint8_t Length, uint8_t * MessageBuffer);

// Called By TouchGFX when a button is pressed.
// 1 Enabled - 0 Disabled
void CanChallengeButtonPressed (uint8_t ToggleState)
{
	if (ToggleState)
	{
		CanTask_ToggleState = 1;
		_CanSendMessage (0xAF, 2, (uint8_t*) &_CanMsgDeviceON);
		CanTask_BSUpdated = 1;
	}
	else
	{
		CanTask_ToggleState = 0;
		_CanSendMessage (0xAF, 2, (uint8_t*) &_CanMsgDeviceOFF);
		CanTask_BSUpdated= 1;
	}
}

void CanChallangeThread(void *Blah) {

	// Initialize CAN Peripheral
	_CanInitialize();
	CanChallengeButtonPressed( 0 );


	// Loop and Send Place Holder Message
	for (;;)
	{
		osDelay(1000);
	}
}

/*
 * Local Static Functions
 */
static void _CanSendMessage(uint32_t ID, uint8_t Length, uint8_t * MessageBuffer)
{
	HAL_StatusTypeDef status;

	static uint32_t Mailbox;
	 CAN_TxHeaderTypeDef CanTxHeader = {
			.StdId = ID,
			.ExtId = 0x00,
			.IDE = CAN_ID_STD,
			.RTR = CAN_RTR_DATA,
			.DLC = Length,
			.TransmitGlobalTime = DISABLE };

	 status = HAL_CAN_AddTxMessage(&hcan2, &CanTxHeader, MessageBuffer, &Mailbox);
	 vTaskDelay(1);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef CanRxHeader;
    uint8_t RXBuffer[8];

    if (hcan->Instance == CAN2) {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CanRxHeader, RXBuffer) == HAL_OK) {
            // Example: Only process messages with ID 0x0AF and length 2
            if (CanRxHeader.StdId == 0x0AF && CanRxHeader.DLC == 2) {
                uint16_t value = (RXBuffer[0] << 8) | RXBuffer[1];

                // Act on value
                if (value == 0xFFFF) {
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET); // for example
                } else {
                    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
                }

                // Optional: log or set flags
                CanTask_BSValue = value;
                CanTask_BSUpdated = 1;
            }
        }
    }
}


// TODO: RX if need be.
//	CAN_RxHeaderTypeDef CanRxHeader;
//	uint8_t RXBuffer[8];
//	HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0, &CanRxHeader, &RXBuffer[0]);

static void _CanInitialize(void) {

	// Initialize the CAN Interface...
	hcan2.Instance = CAN2;
	hcan2.Init.Prescaler = 14;
	hcan2.Init.Mode = CAN_MODE_NORMAL;
	hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan2.Init.TimeSeg1 = CAN_BS1_12TQ;
	hcan2.Init.TimeSeg2 = CAN_BS2_2TQ;
	hcan2.Init.TimeTriggeredMode = DISABLE;
	hcan2.Init.AutoBusOff = DISABLE;
	hcan2.Init.AutoWakeUp = DISABLE;
	hcan2.Init.AutoRetransmission = DISABLE;
	hcan2.Init.ReceiveFifoLocked = DISABLE;
	hcan2.Init.TransmitFifoPriority = DISABLE;
//	hcan2.RxFifo0MsgPendingCallback = &CanRxMsgPendingCallback;

	if (HAL_CAN_Init(&hcan2) != HAL_OK) {
		Error_Handler();
	}

	CAN_FilterTypeDef CanFilterConfig = {
	    .FilterBank = 0,
	    .FilterMode = CAN_FILTERMODE_IDMASK,
	    .FilterScale = CAN_FILTERSCALE_32BIT,
	    .FilterIdHigh = 0x0000,
	    .FilterIdLow = 0x0000,
	    .FilterMaskIdHigh = 0x0000,
	    .FilterMaskIdLow = 0x0000,
	    .FilterFIFOAssignment = CAN_RX_FIFO0,
	    .FilterActivation = ENABLE,
	    .SlaveStartFilterBank = 14
	};

	if (HAL_OK != HAL_CAN_ConfigFilter(&hcan2, &CanFilterConfig)) {
		Error_Handler();
	}

	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_FULL | CAN_IT_RX_FIFO0_MSG_PENDING);

	HAL_CAN_Start(&hcan2);

	HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
}

//void CanRxMsgPendingCallback(CAN_HandleTypeDef *hcan) {
//	HAL_GPIO_TogglePin(GPIOG, LD4_Pin);
//}
