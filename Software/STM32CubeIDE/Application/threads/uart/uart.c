/*
 * uart.c
 *
 *  Created on: Jul 7, 2022
 *      Author: calebdavis
 */

#include "string.h"
#include "uart.h"
#include "stm32f4xx_hal_uart.h"
#include <stdbool.h>

uint8_t DoorLockState;
uint8_t DoorLockUpdated;
uint8_t SeatWarmerState;
uint8_t SeatWarmerUpdated;
uint8_t payoadState;
uint8_t payloadUpdated;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;

extern uint8_t debugFlagTouchGFX;
extern uint8_t debugFlagUpdated;

static uint8_t __prompt [] = "> ";
static uint8_t __unsupportedEntry[] = "Unsupported Entry";
static char __rxBuffer[256]; // Shout out to Drew W. for silly requirements on this that still haunt me.


/* Commands */
// TODO: Create an interface to update these values in the GUI?
// TODO: Maybe throw this in SPI?
static uint8_t __titlePrompt[] = "COM Interface\r\n";

static uint8_t __infoPrompt[] = "Device Info:\r\n"
								"Pin: 2005\r\n"
								"HUD State: REcon\r\n"
								"Debug Mode: Disabled\r\n"
								"Firmware Version: 0.3\r\n";

static uint8_t __helpPrompt[] = "(help) - Help Menu\r\n"
								"(info) - Device Info\r\n"
								"(config) - Device Config\r\n"
								"(arm) - Arm Payload\n";

/* Config Prompts */
static uint8_t __configSelfDestructOnCommand[] = "config selfdestruct enabled";
static uint8_t __configSelfDestructOffCommand[] = "config selfdestruct disabled";
static uint8_t __configErrorPrompt[] = "ERROR - command not recognized. Try \"config --help\".\r\n";
static uint8_t __configHelpPrompt[] = 	"USAGE: config [option]\r\n"
										"OPTIONS:\r\n"
										"\tdebug [on/off] - Turn debug mode on or off.\r\n"
										"\tselfdestruct [enabled/disabled]\r\n";
static uint8_t __configDebugOnPrompt[] = 	"Debug Mode ON\r\n"
											"WARNING: Debug mode may expose unintended functionality to other users.\r\n";
static uint8_t __configDebugOffPrompt[] = "Debug Mode OFF\r\n";
static uint8_t __configSelfDestructOnPrompt[] = "SELF DESTRUCT INITIATED!!! I can't believe you've done this...\r\n";
static uint8_t __configSelfDestructOffPrompt[] = "Phew that was a close one. Thank you for being awesome and saving the day. Here is a cookie:\r\n"
											     "      _______     \r\n"
											     "   .-' ____  `.   \r\n"
											     "  / .-'    `-. \\  \r\n"
											     " | / ^  ^ ^ \\ |  \r\n"
											     " | |  0 0 0  | |  \r\n"
											     " | \\   ._.   / | \r\n"
											     "  \\ '.___.' /  / \r\n"
											     "   '._____.'  /  \r\n"
											     "     `-----'`    \r\n";

/* Payload Prompts/Commands */
static uint8_t __payloadCommand[] = "payload";
static uint8_t __payloadEnabledCommand[] = "payload arm";
static uint8_t __payloadDisabledCommand[] = "payload disarm";
static uint8_t __payloadHelpCommand[] = "payload --help";
static uint8_t __payloadEnabledPrompt[] 	= "Payload Armed\r\n";
static uint8_t __payloadDisabledPrompt[] 	= "Payload Disarmed\r\n";
static uint8_t __payloadErrorPrompt[] 		= "ERROR - command not recognized. Try \"payload --help\".\r\n";
static uint8_t __payloadHelpPrompt[] 		= "USAGE: payload [option]\r\n"
												  "OPTIONS:\r\n"
												  "\t[arm/disarm] - Arm and disarm payload.\r\n";

static uint8_t __errorPrompt[] = "ERROR - command not recognized. Try typing \"help\".\r\n";

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
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
}

static void __handleConfigCommands(void)
{
	// The strlen calc below is plus 1 to catch only commands
	if ( 0 == strncmp( "config --help",  __rxBuffer , 14) || 0 == strncmp( "config",  __rxBuffer , 7) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__configHelpPrompt, sizeof(__configHelpPrompt)-1, 100 );
	}
	else if( (0 == strncmp( "config debug on",  __rxBuffer, 16 ) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__configDebugOnPrompt, sizeof(__configDebugOnPrompt)-1, 100 );
		debugFlagTouchGFX |= (1<<1);
		debugFlagUpdated = 1;
	}
	else if( (0 == strncmp( "config debug off",  __rxBuffer, 17 ) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__configDebugOffPrompt, sizeof(__configDebugOffPrompt)-1, 100 );
		debugFlagTouchGFX &= ~(1<<1);
		debugFlagUpdated = 1;
	}
	else if( 0 == strncmp( (char*)__configSelfDestructOnCommand, (char*)__rxBuffer, strlen((char*)__configSelfDestructOnCommand) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__configSelfDestructOnPrompt, sizeof(__configSelfDestructOnPrompt)-1, 100 );
		// TODO: TouchGFX Stuff
	}
	else if( 0 == strncmp( (char*)__configSelfDestructOffCommand, (char*)__rxBuffer, strlen((char*)__configSelfDestructOffCommand) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__configSelfDestructOffPrompt, sizeof(__configSelfDestructOffPrompt)-1, 100 );
		// TODO: TouchGFX Stuff
	}
	else
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__configErrorPrompt, sizeof(__configErrorPrompt)-1, 100 );

	}
}

static void __handlePayloadCommands(void)
{
	// The strlen calc below is plus 1 to catch only commands
	if( 0 == strncmp( (char*)__payloadHelpCommand, (char*)__rxBuffer, strlen((char*)__payloadHelpCommand) ) || 0 == strncmp( (char*)__payloadCommand, (char*)__rxBuffer, strlen((char*)__payloadCommand)+1 ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__payloadHelpPrompt, sizeof(__payloadHelpPrompt)-1, 100 );
	}
	else if( 0 == strncmp( (char*)__payloadEnabledCommand, (char*)__rxBuffer, strlen((char*)__payloadEnabledCommand) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__payloadEnabledPrompt, sizeof(__payloadEnabledPrompt)-1, 100 );
		payoadState = 1;
		payloadUpdated = 1;
	}
	else if( 0 == strncmp( (char*)__payloadDisabledCommand, (char*)__rxBuffer, strlen((char*)__payloadDisabledCommand) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__payloadDisabledPrompt, sizeof(__payloadDisabledPrompt)-1, 100 );
		payoadState = 0;
		payloadUpdated = 1;
	}
	else
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__payloadErrorPrompt, sizeof(__payloadErrorPrompt)-1, 100 );
	}
}

void UARTChallengeThread( void * argument )
{
	uint8_t byte;
	uint16_t rxBufferIndex = 0;
	MX_USART1_UART_Init();
	uint8_t waitingForMessage;

	vTaskDelay(1000);

	HAL_UART_Transmit( &huart1, (uint8_t*)__titlePrompt, sizeof(__titlePrompt)-1, 100 );

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
			__handleConfigCommands();
		}
		else if( 0 == strncmp( (char*)__payloadCommand, (char*)__rxBuffer, strlen((char*)__payloadCommand) ) )
		{
			__handlePayloadCommands();
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
}

// Called By TouchGFX when a button is pressed.
void UARTChallengePayloadButtonPressed (uint8_t ToggleState)
{
	if (ToggleState)
	{
		payoadState = 1;
	}
	else
	{
		payoadState = 0;
	}

	payloadUpdated = 1;
}
