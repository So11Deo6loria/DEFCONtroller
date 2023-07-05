/*
 * uart.c
 *
 *  Created on: Jul 3, 2023
 *      Author: calebdavis
 */

#include "main.h"
#include "bsp.h"
#include "stm32f4xx.h" // Replace with your STM32 header file
//#include "stm32f4xx_hal_gpio.h"
//#include "stm32f4xx_hal_uart.h"

#include "stm32f4xx_hal.h"

UART_HandleTypeDef uart_handle;

void UART_Init(void)
{
  // Enable GPIO and UART clock
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_UART5_CLK_ENABLE();

  // Configure GPIO pins for UART5
  GPIO_InitTypeDef GPIO_InitStruct;

  // PC12 for UART5 TX
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // PD2 for UART5 RX
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

//  // Configure UART5
//  UART5->CR1 &= ~(1<<0); // Disable USART while making changes.
//
//  // USART1, PA2 USART1 TX (AF1), PA3 USART3 RX(AF1), PA4 Enable, (Output)
////  RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
////  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
//////
////  GPIOA->MODER  |= ( 2 << (2*2) ) | ( 2 << (2*3) ) | ( 1 << (2*4) );  // Configure Pins
////  GPIOA->AFR[0] |= ( 1 << (4*2) ) | ( 1 << (4*3) );                   // AF1 to configure PA3/PA4 for USART1 TX/RX
////
////  GPIOA->BSRRH  |= ( 1 << 4 );                                        // Set Tx Enable Low
//
//  UART5->BRR  = SystemCoreClock / 115200;
////  UART5->CR3 |= USART_CR3_CTSE;
//  UART5->CR1 |= ( 1 << 3 ) | ( 1 << 2 ); // Enable Rx and Tx
//  UART5->CR1 |= ( 1 << 0 );              // Enable USART

  uart_handle.Instance = UART5;
  uart_handle.Init.BaudRate = 115200;
  uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
  uart_handle.Init.StopBits = UART_STOPBITS_1;
  uart_handle.Init.Parity = UART_PARITY_NONE;
  uart_handle.Init.Mode = UART_MODE_TX_RX;
  uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&uart_handle);
}

void UART_SendByte(uint8_t byte, uint32_t timeout)
{
  uint32_t tick_start = HAL_GetTick();
  while ((UART5->SR & USART_SR_TXE) == 0)
  {
    if (timeout > 0 && (HAL_GetTick() - tick_start) >= timeout)
    {
      // Timeout occurred
      break;
    }
  }

  if ((UART5->SR & USART_SR_TXE) != 0)
  {
    UART5->DR = byte;
  }
}

uint8_t UART_ReceiveByte(uint8_t * byte)
{
  if( UART5->SR & USART_SR_RXNE )
  {
	  // Need to update this?
//	  USART1->RQR |= USART_RQR_RXFRQ;
	  *byte = (uint8_t)UART5->DR;
	  return 1;
  }
  return 0;
}

void UART_SendData(const uint8_t *data, uint32_t length, uint32_t timeout)
{
  for (uint32_t i = 0; i < length; i++)
  {
    UART_SendByte(data[i], timeout);
  }
}

void UART_ReceiveData(uint8_t *buffer, uint32_t length, uint32_t timeout)
{
  for (uint32_t i = 0; i < length; i++)
  {
    UART_ReceiveByte(&buffer[i]);
  }
}
