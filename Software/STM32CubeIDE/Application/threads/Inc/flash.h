/*
 * flash.h
 *
 *  Created on: Jul 3, 2023
 *      Author: calebdavis
 */

#ifndef APPLICATION_THREADS_INC_FLASH_H_
#define APPLICATION_THREADS_INC_FLASH_H_

#include <stdint.h>

#define FLASH_START_ADDRESS   0x00000000
#define FLASH_SECTOR_SIZE     4096

void Flash_Init(void);
void Flash_Erase(void);
void Flash_Write(uint32_t address, uint8_t* data, uint32_t size);

#endif /* APPLICATION_THREADS_INC_FLASH_H_ */
