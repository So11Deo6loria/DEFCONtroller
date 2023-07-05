/*
 * bspblah.h
 *
 *  Created on: Jul 3, 2023
 *      Author: calebdavis
 */

#include <stdint.h>

/* UART */
void UART_Init(void);
void UART_SendByte(uint8_t byte, uint32_t timeout);
uint8_t UART_ReceiveByte(uint8_t * byte);
void UART_SendData(const uint8_t *data, uint32_t length, uint32_t timeout);
void UART_ReceiveData(uint8_t *buffer, uint32_t length, uint32_t timeout);
