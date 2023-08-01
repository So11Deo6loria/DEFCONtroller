/*
 * led.h
 *
 *  Created on: Jul 28, 2023
 *      Author: calebdavis
 */

#ifndef APPLICATION_THREADS_LED_LED_H_
#define APPLICATION_THREADS_LED_LED_H_osThreadId_t

typedef enum {
	AUTHORIZED_MODE,
	UNAUTHORIZED_MODE
} eLEDAuthMode_t;

void LED_GPIO_Init(void);
void LEDChallengeThread(void const *argument);


#endif /* APPLICATION_THREADS_LED_LED_H_ */
