/*
 * uart.h
 *
 *  Created on: Jul 8, 2022
 *      Author: calebdavis
 */


#include "main.h"
#include "cmsis_os.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

void UARTChallengeThread( void * argument );

void UARTChallengeSeatWarmerButtonPressed (uint8_t State);
void UARTChallengeDoorLockButtonPressed (uint8_t State);
void UARTChallengeIgnitionButtonPressed (uint8_t State);
