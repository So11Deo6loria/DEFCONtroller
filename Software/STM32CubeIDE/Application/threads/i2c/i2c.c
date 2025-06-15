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

#include "stdio.h"
#include "stm32f4xx_hal.h"

int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(*ptr++);
    }
    return len;
}

extern __IO char xNFCFlag[32];
#define NTAG_I2C_ADDRESS       0x55
#define EEPROM_OFFSET          0x04
#define EEPROM_END_OFFSET      0x0B
#define EEPROM_BLOCK_SIZE      4
#define LOCK_BLOCK             0x00
#define DYNAMIC_LOCK_BLOCK     0x38
#define MAX_LEN 32
#define XOR_KEY 0x74
#define BLOCK_SIZE 16

extern I2C_HandleTypeDef hi2c3;
static uint8_t nfc_payload[32];

// 32 bytes of arbitrary user data
static const uint8_t user_data_payload[32] = {
    0xDE, 0xAD, 0xBE, 0xEF,
    0xBA, 0xAD, 0xF0, 0x0D,
    0xCA, 0xFE, 0xBA, 0xBE,
    0xFE, 0xED, 0xFA, 0xCE,
    0x12, 0x34, 0x56, 0x78,
    0x87, 0x65, 0x43, 0x21,
    0x00, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77
};

static void MX_I2C3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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



HAL_StatusTypeDef ntag_read_memory(uint8_t page_addr, uint8_t *buffer, uint8_t count)
{
    // 1. Write the address (no STOP)
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c3, NTAG_I2C_ADDRESS << 1, &page_addr, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) return status;

    // 2. Read 4 bytes from that page
    return HAL_I2C_Master_Receive(&hi2c3, (NTAG_I2C_ADDRESS << 1) | 0x01, buffer, count, HAL_MAX_DELAY);
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

static void write_user_data(void)
{
    // Block 1: pages 0x04–0x07
    ntag_write_memory(0x01, &user_data_payload[0], 16);
    HAL_Delay(5);

    // Block 2: pages 0x08–0x0B
    ntag_write_memory(0x02, &user_data_payload[16], 16);
    HAL_Delay(5);
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

static void read_config_registers(void)
{
    uint8_t buf[4];
    for (uint8_t page = 0xE8; page <= 0xE9; ++page) {
        if (ntag_read_memory(page, buf, sizeof(buf)) == HAL_OK) {
            printf("Config Page 0x%02X: %02X %02X %02X %02X\n", page, buf[0], buf[1], buf[2], buf[3]);
        } else {
            printf("Failed to read config page 0x%02X\n", page);
        }
    }
}


void i2c_scan_bus(void)
{
    for (uint8_t addr = 0; addr < 128; ++addr) {
        if (HAL_I2C_IsDeviceReady(&hi2c3, addr << 1, 2, 100) == HAL_OK) {
            printf("Device found at 0x%02X\n", addr);
        }
    }
}

void write_all_user_memory(void)
{
    uint8_t page_data[4];

    for (uint8_t page = 0x01; page <= 0x37; ++page) {
        // Fill with a pattern: page number repeated
        page_data[0] = page;
        page_data[1] = page;
        page_data[2] = page;
        page_data[3] = page;

        if (ntag_write_memory(page, page_data, 4) != HAL_OK) {
            printf("Write failed at page 0x%02X\n", page);
        } else {
            HAL_Delay(5); // Allow EEPROM commit
        }
    }

    printf("All user pages written.\n");
}



void read_all_user_memory(void)
{
    uint8_t page_data[4];

    for (uint8_t page = 0x01; page <= 0x37; ++page) {
        if (ntag_read_memory(page, page_data, 4) == HAL_OK) {
            printf("Page 0x%02X: %02X %02X %02X %02X\n", page,
                page_data[0], page_data[1], page_data[2], page_data[3]);
        } else {
            printf("Read failed at page 0x%02X\n", page);
        }
    }
}
static void writecc(void)
{
	const uint8_t default_cc[4] = {
	    0xE1, // NDEF Magic Number
	    0x10, // Version 1.0
	    0x6D, // Memory size (0x6D * 8 = 876 bytes = ~219 pages)
	    0x00  // Read access without authentication
	};
	ntag_write_memory(0x03, default_cc, 4);
	HAL_Delay(5);

}

void fix_cc_block(void)
{
    uint8_t block0[16] = {0};

    // Read full 16-byte block 0x00
    if (ntag_read_memory(0x00 * 4, block0, sizeof(block0)) != HAL_OK) {
        // Handle error
        return;
    }

    // Set bytes 12-15 (Capability Container)
    block0[0]  = 0x55 << 1; // Address
    block0[12] = 0xE1; // NDEF magic number
    block0[13] = 0x10; // Version
    block0[14] = 0x3E; // Memory size (496 bytes = 0x3E * 8)
    block0[15] = 0x00; // Access conditions (Read/Write allowed)

    // Write back full block 0x00
    if (ntag_write_memory(0x00 * 4, block0, sizeof(block0)) != HAL_OK) {
        // Handle error
        return;
    }

    HAL_Delay(5); // Allow EEPROM write to complete
}

static void read_configs(void)
{
	uint8_t buf[4] = {0};
    ntag_read_memory(0x00, buf, 16);
    osDelay(1000);
    ntag_read_memory(0x38, buf, 16);
    osDelay(1000);
    ntag_read_memory(0x3A, buf, 16);
    osDelay(1000);
}

static void set_config(void)
{
	uint8_t config[] = { 0x00, 0x37, 0xF8, 0x48, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	ntag_write_memory(0x3A, config, 16);
}

static void clear_dynamic_lock_region(void)
{
    uint8_t clear[16] = {0};
    if (ntag_write_memory(0x38, clear, sizeof(clear)) != HAL_OK) {
        // handle error
    } else {
        HAL_Delay(5);  // EEPROM commit delay
    }
}

static uint8_t xor_flag(void)
{
    uint8_t len = strlen((char *)xNFCFlag);

    // Calculate padded length (multiple of 16)
    uint8_t padded_len = (len + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);

    // Pad remaining bytes with 0x00
    for (uint8_t i = len; i < padded_len; i++) {
        xNFCFlag[i] = 0x00;
    }

    for (uint8_t i = 0; i < padded_len; i++) {
        xNFCFlag[i] ^= XOR_KEY;
    }

    return padded_len;
}

void I2CChallengeThread(void *argument)
{
	uint8_t payload_len = 0;
    MX_I2C3_Init();
    HAL_Delay(1000); // Let NTAG boot fully

    // Steps:
    // 1. Make sure the I2C address is accurate
    uint8_t found = 0;
    for (uint8_t addr = 0; addr < 128; ++addr) {
        if (HAL_I2C_IsDeviceReady(&hi2c3, addr << 1, 2, 100) == HAL_OK) {
            if (addr == NTAG_I2C_ADDRESS) {
                found = 1;
            }
        }
    }

    if (!found) {
        while (1) osDelay(1000);
    }

    // 2. Set the Capability Container (reset static lock and address)
    fix_cc_block();

    // 3. Clear dynamic lock bytes
    clear_dynamic_lock_region();

    // 4. XOR flag value
    payload_len = xor_flag();

    // 5. Write flag value
    uint8_t writes = payload_len / 16;

    for (uint8_t page = 1; page <= writes; page++) {
        ntag_write_memory(page, &xNFCFlag[(page-1) * 16], 16);
        osDelay(250);
    }

    while (1) {
        osDelay(2000);
    }
}
