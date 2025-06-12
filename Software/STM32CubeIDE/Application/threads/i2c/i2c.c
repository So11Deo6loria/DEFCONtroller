/*
 * i2c.c
 *
 *  Created on: Jul 8, 2022
 *      Author: calebdavis
 */

#include <stdio.h>
#include <string.h>
#include "i2c.h"
#include "main.h"

#define NTAG_I2C_ADDRESS       0x55
#define EEPROM_OFFSET          0x04
#define EEPROM_END_OFFSET      0x0A
#define EEPROM_BLOCK_SIZE      4
#define LOCK_BLOCK             0x00
#define DYNAMIC_LOCK_BLOCK     0x38

extern I2C_HandleTypeDef hi2c3;

static const uint8_t ndef_uri_payload[] = {
    0x03, 0x17, 0xD1, 0x01,
    0x01, 0x13, 'U', 0x01,
    'm','i','k','r','o','e','.','c','o','m','/',
    'f','l','a','g','=','d','e','a','d',
    0xFE
};

static void MX_I2C3_Init(void)
{
    hi2c3.Instance = I2C3;
    hi2c3.Init.ClockSpeed = 100000;
    hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;

    if (HAL_I2C_Init(&hi2c3) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK) {
        Error_Handler();
    }
}

static HAL_StatusTypeDef ntag_read_memory(uint16_t mem_offset, uint8_t *buffer, uint16_t count)
{
    return HAL_I2C_Mem_Read(&hi2c3, NTAG_I2C_ADDRESS << 1, mem_offset, I2C_MEMADD_SIZE_8BIT, buffer, count, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef ntag_write_memory(uint16_t mem_offset, const uint8_t *buffer, uint16_t count)
{
    return HAL_I2C_Mem_Write(&hi2c3, NTAG_I2C_ADDRESS << 1, mem_offset, I2C_MEMADD_SIZE_8BIT, (uint8_t *)buffer, count, HAL_MAX_DELAY);
}

static void clear_lock_bytes(void)
{
    uint8_t clear_block[16] = {0};
    ntag_write_memory(LOCK_BLOCK * 4, clear_block, sizeof(clear_block));
    ntag_write_memory(DYNAMIC_LOCK_BLOCK * 4, clear_block, sizeof(clear_block));
}

static void write_ndef(void)
{
    uint8_t clear_buf[888] = {0};
    ntag_write_memory(0, clear_buf, 888);
    ntag_write_memory(0, (uint8_t*)ndef_uri_payload, sizeof(ndef_uri_payload));
}

static void read_ndef(void)
{
    uint8_t buf[4];
    for (uint8_t i = EEPROM_OFFSET; i <= EEPROM_END_OFFSET; ++i) {
        if (ntag_read_memory(i * EEPROM_BLOCK_SIZE, buf, sizeof(buf)) == HAL_OK) {
            printf("Page 0x%02X: %02X %02X %02X %02X\n", i, buf[0], buf[1], buf[2], buf[3]);
        } else {
            printf("Read failed at page 0x%02X\n", i);
        }
    }
}

void I2CChallengeThread(void *argument)
{
    MX_I2C3_Init();

    HAL_Delay(100);

    printf("Clearing lock bytes...\n");
    clear_lock_bytes();
    HAL_Delay(100);

    printf("Reading before write...\n");
    read_ndef();
    HAL_Delay(100);

    printf("Writing NDEF URI...\n");
    write_ndef();
    HAL_Delay(100);

    printf("Verifying write...\n");
    read_ndef();

    while (1) {
        osDelay(2000);
    }
}
