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



void CanChallangeThread(void *Blah) {

	// Initialize CAN Peripheral
	_CanInitialize();

	// Loop and Send Place Holder Message
	for (;;)
	{
		osDelay(1000);

		if( !CanTask_BSUpdated )
		{

			if (!CanTask_ToggleState && CanTask_BSValue > 400)
			{
				CanTask_BSValue++;
			}
			else if (CanTask_BSValue > 0)
			{
				CanTask_BSValue--;
			}

			if ((!debugFlagTouchGFX) && (CanTask_BSValue > 202))
			{
				CanChallengeButtonPressed (1);
			}

			if ((!debugFlagTouchGFX) && (CanTask_BSValue < 175))
			{
				CanChallengeButtonPressed (0);
			}

			if( (CanTask_BSValue > 200) || (CanTask_BSValue < 60))
			{
				CanTask_isBSGood = 0;
			}
			else
			{
				CanTask_isBSGood = 1;
			}

			CanTask_BSUpdated = 1;
			HAL_GPIO_TogglePin(GPIOG, LD3_Pin);
		}
	}
}

// Called By TouchGFX when a button is pressed.
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

/*
 * Local Static Functions
 */
static void _CanSendMessage(uint32_t ID, uint8_t Length, uint8_t * MessageBuffer) {

	static uint32_t Mailbox;
	 CAN_TxHeaderTypeDef CanTxHeader = {
			.StdId = ID,
			.ExtId = 0x00,
			.IDE = CAN_ID_STD,
			.RTR = CAN_RTR_DATA,
			.DLC = Length,
			.TransmitGlobalTime = DISABLE };

	HAL_CAN_AddTxMessage(&hcan2, &CanTxHeader, MessageBuffer, &Mailbox);
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

	.FilterMode = CAN_FILTER_DISABLE };

	if (HAL_OK != HAL_CAN_ConfigFilter(&hcan2, &CanFilterConfig)) {
		Error_Handler();
	}

	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_FULL | CAN_IT_RX_FIFO0_MSG_PENDING);

	HAL_CAN_Start(&hcan2);
}

//void CanRxMsgPendingCallback(CAN_HandleTypeDef *hcan) {
//	HAL_GPIO_TogglePin(GPIOG, LD4_Pin);
//}