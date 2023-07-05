/*
 * flash.c
 *
 *  Created on: Jul 3, 2023
 *      Author: calebdavis
 */


#include "flash.h
#include "stm32f4xx.h"

#define SPIx                   SPI1
#define SPIx_CLK               RCC_APB2Periph_SPI1
#define SPIx_GPIO              GPIOE
#define SPIx_GPIO_CLK          RCC_AHB1Periph_GPIOE
#define SPIx_SCK_PIN           GPIO_Pin_2
#define SPIx_SCK_PIN_SOURCE    GPIO_PinSource2
#define SPIx_MISO_PIN          GPIO_Pin_5
#define SPIx_MISO_PIN_SOURCE   GPIO_PinSource5
#define SPIx_MOSI_PIN          GPIO_Pin_6
#define SPIx_MOSI_PIN_SOURCE   GPIO_PinSource6
#define SPIx_CS_GPIO           GPIOC
#define SPIx_CS_PIN            GPIO_Pin_3

void Flash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef SPI_InitStruct;

    // Enable the SPI peripheral clock
    RCC_APB2PeriphClockCmd(SPIx_CLK, ENABLE);

    // Enable the GPIO peripheral clocks
    RCC_AHB1PeriphClockCmd(SPIx_GPIO_CLK, ENABLE);

    // Configure SPI GPIO pins
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStruct.GPIO_Pin = SPIx_SCK_PIN;
    GPIO_Init(SPIx_GPIO, &GPIO_InitStruct);
    GPIO_PinAFConfig(SPIx_GPIO, SPIx_SCK_PIN_SOURCE, GPIO_AF_SPI1);

    GPIO_InitStruct.GPIO_Pin = SPIx_MISO_PIN;
    GPIO_Init(SPIx_GPIO, &GPIO_InitStruct);
    GPIO_PinAFConfig(SPIx_GPIO, SPIx_MISO_PIN_SOURCE, GPIO_AF_SPI1);

    GPIO_InitStruct.GPIO_Pin = SPIx_MOSI_PIN;
    GPIO_Init(SPIx_GPIO, &GPIO_InitStruct);
    GPIO_PinAFConfig(SPIx_GPIO, SPIx_MOSI_PIN_SOURCE, GPIO_AF_SPI1);

    // Configure CS pin
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = SPIx_CS_PIN;
    GPIO_Init(SPIx_CS_GPIO, &GPIO_InitStruct);
    GPIO_SetBits(SPIx_CS_GPIO, SPIx_CS_PIN);

    // Configure SPI settings
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_Init(SPIx, &SPI_InitStruct);

    SPI_Cmd(SPIx, ENABLE);
}

void Flash_CS_Low(void)
{
    GPIO_ResetBits(SPIx_CS_GPIO, SPIx_CS_PIN);
}

void Flash_CS_High(void)
{
    GPIO_SetBits(SPIx_CS_GPIO, SPIx_CS_PIN);
}

void Flash_SendByte(uint8_t byte)
{
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPIx, byte);
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPIx);
}

void Flash_SendCommand(uint8_t command)
{
    Flash_CS_Low();
    Flash_SendByte(command);
    Flash_CS_High();
}

void Flash_SendAddress(uint32_t address)
{
    Flash_SendByte((address >> 16) & 0xFF);
    Flash_SendByte((address >> 8) & 0xFF);
    Flash_SendByte(address & 0xFF);
}

void Flash_WaitForReady(void)
{
    uint8_t status;

    Flash_CS_Low();
    Flash_SendByte(CMD_READ_STATUS_REG);
    do
    {
        status = Flash_ReceiveByte();
    } while (status & 0x01);
    Flash_CS_High();
}

void Flash_Erase(void)
{
    Flash_CS_Low();
    Flash_SendByte(CMD_WRITE_ENABLE);
    Flash_CS_High();

    Flash_CS_Low();
    Flash_SendByte(CMD_SECTOR_ERASE);
    Flash_SendAddress(FLASH_START_ADDRESS);
    Flash_CS_High();

    Flash_WaitForReady();
}

void Flash_Write(uint32_t address, uint8_t* data, uint32_t size)
{
    Flash_CS_Low();
    Flash_SendByte(CMD_WRITE_ENABLE);
    Flash_CS_High();

    Flash_CS_Low();
    Flash_SendByte(CMD_PAGE_PROGRAM);
    Flash_SendAddress(address);
    for (uint32_t i = 0; i < size; i++)
    {
        Flash_SendByte(data[i]);
    }
    Flash_CS_High();

    Flash_WaitForReady();
}

uint8_t Flash_ReceiveByte(void)
{
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPIx, 0xFF);
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPIx);
}
