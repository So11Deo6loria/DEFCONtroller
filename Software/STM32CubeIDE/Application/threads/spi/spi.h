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
	SPI_READPAGE_IDLE,
	SPI_READPAGE_READID,
	SPI_READPAGE_READPAGE,
	SPI_PROCESSPAGE,
	SPI_WRITEPAGE,
	SPI_READPAGE_SUCCESS,
	SPI_READPAGE_ERROR
} eSPIReadState_t;

typedef enum {
	SPI_WRITEPAGE_IDLE,
	SPI_WRITEPAGE_WRITE_ENABLE,
	SPI_WRITEPAGE_WRITE_STATUS_REGISTER,
	SPI_WRITEPAGE_CHIP_ERASE,
	SPI_WRITEPAGE_WRITEPAGE,
	SPI_WRITEPAGE_WRITE_DISABLE,
	SPI_WRITEPAGE_SUCCESS,
	SPI_WRITEPAGE_ERROR
} eSPIWriteState_t;

typedef enum {
	SPI_IDLE,
	SPI_READING,
	SPI_WRITING
} eSPIState_t;

typedef enum {
	SPI_SUCCESS,
	SPI_ERROR
} eSPIStatusMode_t;

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
