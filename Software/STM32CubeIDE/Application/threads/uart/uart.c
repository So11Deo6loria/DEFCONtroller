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
uint8_t AutoIgnitionState;
uint8_t AutoIgnitionUpdated;

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
static uint8_t __titlePrompt[] = "DVHID Manufacturer's Interface\r\n";

static uint8_t __infoPrompt[] = "Device Info:\r\n"
								"Odometer: 89109\r\n"
								"Miles to Empty: 31\r\n"
								"Oil Life %: 42\r\n"
								"VIN: G02D3FC0N2023\r\n"
								"Firmware Version: 0.3\r\n";

static uint8_t __helpPrompt[] = "(help) - Help Menu\r\n"
								"(info) - Device Info\r\n"
								"(config) - Device Config\r\n"
								"(seatwarmer) - Button\r\n"
								"(autoignition) - Button\n";

/* Config Prompts */
static uint8_t __configSelfDestructOnCommand[] = "config selfdestruct enabled";
static uint8_t __configSelfDestructOffCommand[] = "config selfdestruct disabled";
static uint8_t __configErrorPrompt[] = "ERROR - command not recognized. Try \"config --help\".\r\n";
static uint8_t __configHelpPrompt[] = 	"USAGE: config [option]\r\n"
										"OPTIONS:\r\n"
										"\tdebug [on/off] - Turn debug mode on or off.\r\n";
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

/* Seat Warmer Prompts/Commands */
static uint8_t __seatWarmerCommand[] = "seatwarmer";
static uint8_t __seatWarmerOnCommand[] = "seatwarmer on";
static uint8_t __seatWarmerOffCommand[] = "seatwarmer off";
static uint8_t __seatWarmerHelpCommand[] = "seatwarmer --help";
static uint8_t __seatWarmerOnPrompt[] = "Seat Warmer ON\r\n";
static uint8_t __seatWarmerOffPrompt[] = "Seat Warmer OFF\r\n";
static uint8_t __seatWarmerErrorPrompt[] = "ERROR - command not recognized. Try \"seatwarmer --help\".\r\n";
static uint8_t __seatWarmerHelpPrompt[] = 	"USAGE: seatwarmer [option]\r\n"
										"OPTIONS:\r\n"
										"\t[on/off] - Enable and disable the seat warmers.\r\n";

/* Auto Start Prompts/Commands */
static uint8_t __autoIgnitionCommand[] = "autoignition";
static uint8_t __autoIgnitionOnCommand[] = "autoignition on";
static uint8_t __autoIgnitionOffCommand[] = "autoignition off";
static uint8_t __autoIgnitionHelpCommand[] = "autoignition --help";
static uint8_t __autoIgnitionEnabledPrompt[] 	= "Auto Start ON\r\n";
static uint8_t __autoIgnitionDisabledPrompt[] 	= "Auto Start OFF\r\n";
static uint8_t __autoIgnitionErrorPrompt[] 		= "ERROR - command not recognized. Try \"autoignition --help\".\r\n";
static uint8_t __autoIgnitionHelpPrompt[] 		= "USAGE: autoignition [option]\r\n"
												  "OPTIONS:\r\n"
												  "\t[on/off] - Enable and disable auto start.\r\n";

static uint8_t __errorPrompt[] = "ERROR - command not recognized. Try typing \"help\".\r\n";

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
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

static void __handleSeatWarmerCommands(void)
{
	// The strlen calc below is plus 1 to catch only commands
	if( 0 == strncmp( (char*)__seatWarmerHelpCommand, (char*)__rxBuffer, strlen((char*)__seatWarmerHelpCommand) ) ||  0 == strncmp( (char*)__seatWarmerCommand, (char*)__rxBuffer, strlen((char*)__seatWarmerCommand)+1 ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__seatWarmerHelpPrompt, sizeof(__seatWarmerHelpPrompt)-1, 100 );
	}
	else if( 0 == strncmp( (char*)__seatWarmerOnCommand, (char*)__rxBuffer, strlen((char*)__seatWarmerOnCommand) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__seatWarmerOnPrompt, sizeof(__seatWarmerOnPrompt)-1, 100 );
		SeatWarmerState = 1;
		SeatWarmerUpdated = 1;
	}
	else if( 0 == strncmp( (char*)__seatWarmerOffCommand, (char*)__rxBuffer, strlen((char*)__seatWarmerOffCommand) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__seatWarmerOffPrompt, sizeof(__seatWarmerOffPrompt)-1, 100 );
		SeatWarmerState = 0;
		SeatWarmerUpdated = 1;
	}
	else
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__seatWarmerErrorPrompt, sizeof(__seatWarmerErrorPrompt)-1, 100 );
	}
}

static void __handleAutoIgnitionCommands(void)
{
	// The strlen calc below is plus 1 to catch only commands
	if( 0 == strncmp( (char*)__autoIgnitionHelpCommand, (char*)__rxBuffer, strlen((char*)__autoIgnitionHelpCommand) ) || 0 == strncmp( (char*)__autoIgnitionCommand, (char*)__rxBuffer, strlen((char*)__autoIgnitionCommand)+1 ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__autoIgnitionHelpPrompt, sizeof(__autoIgnitionHelpPrompt)-1, 100 );
	}
	else if( 0 == strncmp( (char*)__autoIgnitionOnCommand, (char*)__rxBuffer, strlen((char*)__autoIgnitionOnCommand) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__autoIgnitionEnabledPrompt, sizeof(__autoIgnitionEnabledPrompt)-1, 100 );
		AutoIgnitionState = 1;
		AutoIgnitionUpdated = 1;
	}
	else if( 0 == strncmp( (char*)__autoIgnitionOffCommand, (char*)__rxBuffer, strlen((char*)__autoIgnitionOffCommand) ) )
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__autoIgnitionDisabledPrompt, sizeof(__autoIgnitionDisabledPrompt)-1, 100 );
		AutoIgnitionState = 0;
		AutoIgnitionUpdated = 1;
	}
	else
	{
		HAL_UART_Transmit( &huart1, (uint8_t*)__autoIgnitionErrorPrompt, sizeof(__autoIgnitionErrorPrompt)-1, 100 );
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
		else if( 0 == strncmp( (char*)__autoIgnitionCommand, (char*)__rxBuffer, strlen((char*)__autoIgnitionCommand) ) )
		{
			__handleAutoIgnitionCommands();
		}
		else if( 0 == strncmp( (char*)__seatWarmerCommand, (char*)__rxBuffer, strlen((char*)__seatWarmerCommand) ) )
		{
			__handleSeatWarmerCommands();
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
void UARTChallengeSeatWarmerButtonPressed (uint8_t ToggleState)
{
	if (ToggleState)
	{
		SeatWarmerState = 1;
	}
	else
	{
		SeatWarmerState = 0;
	}

	SeatWarmerUpdated= 1;
}

// Called By TouchGFX when a button is pressed.
void UARTChallengeDoorLockButtonPressed (uint8_t ToggleState)
{
	if (ToggleState)
	{
		DoorLockState = 1;
	}
	else
	{
		DoorLockState = 0;
	}

	DoorLockUpdated= 1;
}

// Called By TouchGFX when a button is pressed.
void UARTChallengeIgnitionButtonPressed (uint8_t ToggleState)
{
	if (ToggleState)
	{
		AutoIgnitionState = 1;
	}
	else
	{
		AutoIgnitionState = 0;
	}

	AutoIgnitionUpdated= 1;
}
