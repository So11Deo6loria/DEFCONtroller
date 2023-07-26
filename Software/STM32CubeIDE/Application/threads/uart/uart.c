/*
 * uart.c
 *
 *  Created on: Jul 7, 2022
 *      Author: calebdavis
 */

#include "uart.h"
#include "stm32f4xx_hal_uart.h"
#include <stdbool.h>

bool DoorLock_Toggle;
bool AutoIgnition_Toggle;
bool SeatWarmer_Toggle;

uint8_t DoorLock_ToggleState;
uint8_t DoorLock_Toggle;
uint8_t SeatWarmer_ToggleState;
uint8_t SeatWarmer_Toggle;
uint8_t AutoIgnition_ToggleState;
uint8_t AutoIgnition_Toggle;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;

extern uint8_t debugFlagTouchGFX;
extern uint8_t debugUpdatedTouchGFX;

static uint8_t __prompt [] = "> ";
static uint8_t __unsupportedEntry[] = "Unsupported Entry";
static uint8_t __rxBuffer[256]; // Shout out to Drew W. for silly requirements on this that still haunt me.


/* Commands */
static uint8_t __infoPrompt[] = "PRO Medical Devices\r\n"
								"Manf. Date 04.20.2022\r\n"
								"Device Serial: PR071V171\r\n"
								"Software Version: 1.0\r\n"
								"Type \"help\" to view options.\r\n";

static uint8_t __helpPrompt[] = "(help) - This menu\r\n"
								"(info) - Display device information\r\n"
								"(config) - View and adjust device configuration\r\n";

static uint8_t __configErrorPrompt[] = "ERROR - command not recognized. Try \"config --help\".\r\n";

static uint8_t __configHelpPrompt[] = 	"USAGE: config [option]\r\n"
										"OPTIONS:\r\n"
										"\tdebug [on/off] - Turn debug mode on or off.\r\n";

static uint8_t __configDebugOnPrompt[] = 	"Debug mode set to ON.\r\n"
											"WARNING: Debug mode may allow blood glucose to reach unsafe levels. "
											"Ensure the device is not connected to a patient while in debug mode.\r\n";

static uint8_t __configDebugOffPrompt[] = "Debug mode set to OFF.\r\n";

static uint8_t __errorPrompt[] = "ERROR - command not recognized. Try typing \"help\".\r\n";

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

void UARTChallengeThread( void * argument )
{
	uint8_t byte;
	uint16_t rxBufferIndex = 0;
	MX_USART1_UART_Init();
	MX_UART5_Init();
	uint8_t waitingForMessage;
	HAL_UART_Transmit( &huart1, (uint8_t*)__infoPrompt, sizeof(__infoPrompt)-1, 100 );

	for(;;)
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__prompt, sizeof(__prompt)-1, 100 );
		waitingForMessage = 1;
		while( waitingForMessage )
		{
			if( HAL_OK == HAL_UART_Receive( &huart1, &byte, 1, 100 ) )
			{
				switch( byte ){
					case '\r':
					case '\n':
						if( rxBufferIndex ){
							waitingForMessage = 0;
						}
						break;
					case '\177':
						if( rxBufferIndex ){
							__rxBuffer[--rxBufferIndex] = 0;
						}
						break;
					default:
						__rxBuffer[rxBufferIndex++] = byte;
				}
			}
		}

		// TODO: Handle CRLF
		if( 0 == strncmp( "info",  __rxBuffer, 5 ) )
		{
			HAL_UART_Transmit( &huart1, (uint8_t*)__infoPrompt, sizeof(__infoPrompt)-1, 100 );
		}
		else if( 0 == strncmp( "help",  __rxBuffer, 5 ) )
		{
			HAL_UART_Transmit( &huart1, (uint8_t*)__helpPrompt, sizeof(__helpPrompt)-1, 100 );
		}
		else if( 0 == strncmp( "config",  __rxBuffer, 6 ) )
		{
			if ( 0 == strncmp( "config --help",  __rxBuffer , 14) || 0 == strncmp( "config",  __rxBuffer , 7) )
			{
				HAL_UART_Transmit( &huart1, (uint8_t*)__configHelpPrompt, sizeof(__configHelpPrompt)-1, 100 );
			}
			else if( (0 == strncmp( "config debug on",  __rxBuffer, 16 ) ) )
			{
				HAL_UART_Transmit( &huart1, (uint8_t*)__configDebugOnPrompt, sizeof(__configDebugOnPrompt)-1, 100 );
				debugFlagTouchGFX = 5;
				debugUpdatedTouchGFX = 1;

			}
			else if( (0 == strncmp( "config debug off",  __rxBuffer, 17 ) ) )
			{
				HAL_UART_Transmit( &huart1, (uint8_t*)__configDebugOffPrompt, sizeof(__configDebugOffPrompt)-1, 100 );
				debugFlagTouchGFX = 0;
				debugUpdatedTouchGFX = 1;
			}
			else if ( (0 == strncmp( "config seatwarmer", __rxBuffer, 21)) && (debugFlagTouchGFX == 1)){
				SeatWarmer_Toggle = 1;
			}
			else if ( (0 == strncmp( "config autoignition", __rxBuffer, 23)) && (debugFlagTouchGFX == 1)){
				AutoIgnition_Toggle = 1;
			}
			else
			{
				HAL_UART_Transmit( &huart1, (uint8_t*)__configErrorPrompt, sizeof(__configErrorPrompt)-1, 100 );
			}

		}
		else
		{
			HAL_UART_Transmit( &huart1, (uint8_t*)__errorPrompt, sizeof(__errorPrompt)-1, 100 );
		}

		byte = 0;
		rxBufferIndex = 0;
		memset( __rxBuffer, 0, sizeof(__rxBuffer));
		osDelay(500);
	}
	  /* USER CODE END 5 */
}

// Called By TouchGFX when a button is pressed.
void SeatWarmerButtonPressed (uint8_t ToggleState)
{
	if (ToggleState)
	{
		SeatWarmer_ToggleState = 1;
		SeatWarmer_Toggle = 1;
	}
	else
	{
		SeatWarmer_ToggleState = 0;
		SeatWarmer_Toggle= 1;
	}
}

// Called By TouchGFX when a button is pressed.
void DoorLockButtonPressed (uint8_t ToggleState)
{
	if (ToggleState)
	{
		DoorLock_ToggleState = 1;
		DoorLock_Toggle = 1;
	}
	else
	{
		DoorLock_ToggleState = 0;
		DoorLock_Toggle= 1;
	}
}

// Called By TouchGFX when a button is pressed.
void AutoIgnitionButtonPressed (uint8_t ToggleState)
{
	if (ToggleState)
	{
		AutoIgnition_ToggleState = 1;
		AutoIgnition_Toggle = 1;
	}
	else
	{
		AutoIgnition_ToggleState = 0;
		AutoIgnition_Toggle= 1;
	}
}
