/*
 * spi.h
 *
 *  Created on: Jul 8, 2022
 *      Author: calebdavis
 */

#ifndef SRC_THREADS_SPI_SPI_H_
#define SRC_THREADS_SPI_SPI_H_

#include "main.h"
#include "cmsis_os.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"

typedef enum {
	DEBUG_DISABLED,
	DEBUG_ENABLED,
	DEBUG_UNSET
} eDebugMode_t;

typedef enum {
	SPI_READID,
	SPI_READPAGE,
	SPI_WRITEPAGE,
	SPI_ERROR
} eSPIState_t;

typedef enum {
	SPI_WRITE_DISABLE,
	SPI_WRITE_ENABLE
} eWriteEnableMode_t;

typedef enum {
	KEY,
	VALUE
} eConfigParseState_t;

void SPIChallengeThread( void * argument );
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);


#endif /* SRC_THREADS_SPI_SPI_H_ */
