/*
 * led.c
 *
 *  Created on: Jul 28, 2023
 *      Author: calebdavis
 */

#include "led.h"
#include "stm32f4xx_hal.h"


void LED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Enable GPIO clock for Port G
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // Configure LED pins as output
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

// Thread function to toggle LEDs
void LEDChallengeThread(void const *argument)
{
	uint16_t pinNumber = GPIO_PIN_13;
	eLEDAuthMode_t ledAuthMode = AUTHORIZED_MODE;
	LED_GPIO_Init();

	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);

	if( ledAuthMode == AUTHORIZED_MODE )
	{
		pinNumber = GPIO_PIN_13;
	}
	else if( ledAuthMode == UNAUTHORIZED_MODE )
	{
		pinNumber = GPIO_PIN_14;
	}

    while (1)
    {
    	HAL_GPIO_TogglePin(GPIOG, pinNumber);
    	osDelay(250);
    }
}
