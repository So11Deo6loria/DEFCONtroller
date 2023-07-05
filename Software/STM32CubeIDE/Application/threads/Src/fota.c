/*
 * fota.c
 *
 *  Created on: Jul 3, 2023
 *      Author: calebdavis
 */

#include "main.h"
#include "stm32f4xx.h" // Replace with your STM32 header file
#include "bsp.h"
#include "xmodem.h"
#include "xmodem_global_handle.h"
//#include "flash.h"

#define SECRET_MESSAGE "START_UPDATE" // Secret message to trigger firmware update

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;

static uint8_t __PageWritePosition = 0;

static void __uartInit(void)
{
	/**
	 * UART5 GPIO Configuration
	 * PC12     ------> UART5_TX
	 * PD2     ------> UART5_RX
	 */
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
}

static void __TimerStart( uint32_t timeout )
{
}

static void __TimerKill( void )
{

}

static uint8_t __TimerExpired( void )
{
//  return BspSystemTimeHasExpired( __ComTimeoutStartTime, __ComTimeoutLength );
}

static void __ComWriteSlaveMode( uint8_t Byte )
{
	UART_SendByte( Byte, 100 );
}

static uint8_t __ComReadSlaveMode( uint8_t * Byte )
{
	return UART_ReceiveByte( Byte );
}

static uint8_t __ComPayloadCallbackSlaveMode( uint8_t * Payload )
{
  uint16_t FirstBytes = 0;

//  BspImageRead( (uint8_t*)&FirstBytes, 0, 2 );
//  BspImageWrite( Payload, __PageWritePosition, 1024 );
//  BspImageRead( (uint8_t*)&FirstBytes, 0, 2 );

  // Assuming Page Writes @ 256 Bytes
  __PageWritePosition += 4;

  return 1;
}

void FOTAChallengeThread(void * argument)
{
	__uartInit();
	// Creating Single Global Xmodem Handle to share across the project.
	XModemGlobalHandle  = XModem1KCreate( __ComWriteSlaveMode,
	  __ComReadSlaveMode,
	  __TimerStart,
	  __TimerKill,
	  __TimerExpired      );

	__PageWritePosition = 0;
	XModem1KReceiveFileInit( XModemGlobalHandle );
	XModem1KReceiveFile( XModemGlobalHandle, __ComPayloadCallbackSlaveMode );

//  BspImageRead( (uint8_t*)&__ImageLengths, 0, 8 );
//  __ImageLengths[0] = __PageWritePosition;
//  BspImageWrite( (uint8_t*)&__ImageLengths, 0, 8 );
}
