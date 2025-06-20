/*
 * spi.c
 *
 *  Created on: Jul 8, 2022
 *      Author: calebdavis
 */

#include "spi.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "device_status.h"


// Global Variables for Use with TouchGFX.
char SPIModelNumber[16] = "NOMODEL";
char SPISerialNumber[16] = "NOSERIAL";
char SPISoftwareVersion[16] = "0.0.0";

uint8_t debugFlagTouchGFX = 0; /// 0 = use to communicate with touchgfx. Setting the initial valu here?
uint8_t debugFlagUpdated = 0; /// 0 = false use to communicate with touchgfx
uint8_t SPIExternalFlashUpdated = 0;
char deviceID[9] = {0};  // 8-digit string + null terminator

// Local Items.
static uint8_t __debugModeDisabledString[256] = "PIN=2005;HUD_STATE=REcon;DEBUG_MODE=DISABLED;FIRMWARE_VERSION=0.3;";
static uint8_t __debugModeEnabledString[256] = "PIN=2005;HUD_STATE=REcon;DEBUG_MODE=ENABLED;FIRMWARE_VERSION=0.3;";
static uint8_t __emptyPage[256];

#define STM25P80_ID 0x202014
#define DUMMY_BYTE 0xA5
#define STM25P80_PAGESIZE 256

#define SST_CMD_WRITE_ENABLE 	   	0x06
#define SST_CMD_WRITE_DISABLE   	0x04
#define SST_CMD_READ_STATUS_REG 	0x05
#define SST_CMD_WRITE_STATUS_REG 	0x01
#define SST_CMD_READ_DATA       	0x03
#define SST_CMD_FAST_READ_DATA    	0x0B
#define SST_CMD_AAI_WORD_PROGRAM	0xAD
#define SST_CMD_SECTOR_ERASE    	0xD8
#define SST_CMD_CHIP_ERASE      	0xC7
#define SST_CMD_READ_DEVICE_ID  	0x9F
#define SST_DEVICE_ID 				0xBF258E
#define SST_PAGESIZE				256


/* Instructions */
#define READ_DATA_BYTES 0x03
#define READ_IDENTIFICATION 0x9F
#define PAGE_PROGRAM 0x02
#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
#define SECTOR_ERASE 0xD8

extern SPI_HandleTypeDef hspi4;
static eDebugMode_t __debugMode = DEBUG_UNSET;
static eSPIReadState_t 	__spiReadState 	= SPI_READPAGE_IDLE;
static eSPIWriteState_t __spiWriteState = SPI_WRITEPAGE_WRITE_ENABLE;
static eSPIState_t __spiState = SPI_IDLE;
static eSPIStatusMode_t __spiStatus = SPI_ERROR;

static QueueHandle_t __spiQueue;

/* Data Buffers */
static uint16_t __txDataIndex = 0;
static uint16_t __rxDataIndex = 0;
static uint8_t __txData[512];
static uint8_t __rxData[512];
static uint8_t __writePageIndex = 0;

static uint8_t __configString[256];
static uint8_t __key[32];
static uint8_t __value[32];

HAL_SPI_StateTypeDef halSpiState;

static void __computeSimpleDeviceID(void)
{
    // 1. Use last 2 digits of pin
    int pinSuffix = 0;
    size_t pinLen = strlen(gDeviceStatus.pin);
    if (pinLen >= 2)
        pinSuffix = (gDeviceStatus.pin[pinLen - 2] - '0') * 10 + (gDeviceStatus.pin[pinLen - 1] - '0');

    // 2. Sum of ASCII values of HUD state, mod 100
    uint16_t hudSum = 0;
    for (size_t i = 0; i < strlen(gDeviceStatus.hudState); ++i)
        hudSum += (uint8_t)gDeviceStatus.hudState[i];
    hudSum %= 100;

    // 3. Use debug flag directly
    uint8_t debugFlag = gDeviceStatus.debugEnabled ? 1 : 0;

    // 4. Parse firmware version: "X.Y" → X*100 + Y
    uint16_t fwValue = 0;
    int major = 0, minor = 0;
    sscanf(gDeviceStatus.firmwareVersion, "%d.%d", &major, &minor);
    fwValue = (major * 100) + minor;

    // 5. Format into global string
    snprintf(deviceID, sizeof(deviceID), "%02d%02d%d%03d",
             pinSuffix, hudSum, debugFlag, fwValue);
}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
 */
static void MX_SPI4_Init (void)
{
    /** SPI4 GPIO Configuration
    PE4     ------> CS
    PE2     ------> SPI4_SCK
    PE5     ------> SPI4_MISO
    PE6     ------> SPI4_MOSI
    */
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* CS Configuration */
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin (GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

	/*Configure GPIO CS pin: PE4 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init (GPIOE, &GPIO_InitStruct);

	/* SPI4 parameter configuration*/
	hspi4.Instance = SPI4;
	hspi4.Init.Mode = SPI_MODE_MASTER;
	hspi4.Init.Direction = SPI_DIRECTION_2LINES;
	hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi4.Init.NSS = SPI_NSS_HARD_OUTPUT;
	hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi4.Init.CRCPolynomial = 7;
	if (HAL_SPI_Init (&hspi4) != HAL_OK)
	{
		Error_Handler ();
	}

    HAL_NVIC_SetPriority(SPI4_IRQn, 10, 10);
    HAL_NVIC_EnableIRQ(SPI4_IRQn);
}

static void __setCS(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
}

static void __resetCS(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
}

static void __spiRead (uint16_t Length)
{
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); 	// Drive CS Low
	HAL_SPI_Receive_IT (&hspi4, &__rxData[0], Length);	// Blocking Read Call
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High
}

static void __spiWrite (uint16_t Length)
{
	__resetCS(); 	// Drive CS Low
	HAL_SPI_Transmit (&hspi4, &__txData[0], Length, 100);	// Blocking Write Call

	__spiRead (10);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High
}

static void __spiTransactionIT (uint16_t Length)
{
	__resetCS(); 	// Drive CS Low
	HAL_SPI_TransmitReceive_IT (&hspi4, __txData, __rxData, Length);
}

static void __spiWriteIT(uint16_t Length)
{
	__resetCS(); 	// Drive CS Low
	HAL_SPI_Transmit_IT (&hspi4, __txData, Length);
}

static void __spiTransaction (uint16_t Length)
{
	__resetCS(); 	// Drive CS Low
	HAL_SPI_TransmitReceive (&hspi4, __txData, __rxData, Length, 100);
	__setCS(); 	// Drive CS High

}

static void __resetBuffers(void)
{
	__txDataIndex = 0;
	__rxDataIndex = 0;
	memset(__txData, 0, sizeof(__txData) );
	memset(__rxData, 0, sizeof(__rxData) );
}

static uint8_t __readStatusRegister(void)
{
	__resetBuffers();
	__txData[__txDataIndex++] = SST_CMD_READ_STATUS_REG;
	__spiTransaction(2); // 1 Byte Command
	return __rxData[1]; // 1 Byte Reponse
}

static uint8_t __waitForReady (void)
{
	HAL_SPI_StateTypeDef halState;
	while (HAL_SPI_STATE_READY != (halState = HAL_SPI_GetState (&hspi4)))
	{
		osDelay(1);
	}

	while(__readStatusRegister() & (1 << 0) != 0 )
	{
		osDelay(10);
	}

	return 1;
}

static void __spiWriteRead( uint16_t txLength, uint16_t rxLength )
{
	/* Trying to make sure CS stays low */
	__resetCS(); 	// Drive CS Low

	__spiWrite( txLength );
	__waitForReady();
	__spiRead( rxLength );

	__setCS(); 	// Drive CS High
}


static uint8_t __readID(void)
{
	__resetBuffers();
	__txData[0] = 0x9F;
	__spiTransactionIT(4);
	return 1;
}

static void __readData(void)
{
	__resetBuffers();
	__txData[__txDataIndex++] = READ_DATA_BYTES;
	 // Start at Address 0x000000
	__txData[__txDataIndex++] = 0x00;
	__txData[__txDataIndex++] = 0x00;
	__txData[__txDataIndex++] = 0x00;

	__spiTransactionIT(128+4);
}
//
//static void __writePage(uint8_t * writeBuffer)
//{
//	// Should only run this when setting up flash components.
//	uint16_t length = STM25P80_PAGESIZE;
//
//	/* Write Enable */
//	__resetBuffers();
//	__txData[__txDataIndex++] = WRITE_ENABLE;
//	__spiTransaction(1); // 1 Byte Command
//
//	/* Make Sure You Don't Write Too Quickly. May not need this HAL might do it for you. */
//	__waitForReady();
//
//	/* Page Program */
//	__resetBuffers();
//	__txData[__txDataIndex++] = PAGE_PROGRAM;
//	__txData[__txDataIndex++] = 0x00;
//	__txData[__txDataIndex++] = 0x00;
//	__txData[__txDataIndex++] = 0x00;
//	memcpy(&__txData[__txDataIndex], writeBuffer, length);
//	__spiTransaction(__txDataIndex+length); // Confirm this value
//}

static void __setDebugMode (eDebugMode_t debugMode)
{
	if (__debugMode != debugMode)
	{
		__debugMode = debugMode;
		if (debugMode && (0 == strncmp(&__value[0], deviceID, strlen(deviceID)))) // If the Debug Value is not set in our buffer, but we have mysteriously got here...
		{
			// D*** You SPI
			debugFlagTouchGFX |= (1<<2);
			// Notify TouchGFX
			debugFlagUpdated = 1; //flag set to true
		}
		else
		{
			debugFlagTouchGFX &= ~(1<<2);
		}
	}
}

static uint8_t __processPage(void)
{
	uint16_t i = 0, keyIndex = 0, valueIndex = 0;

	uint32_t error = 0;
	error = HAL_SPI_GetError( &hspi4 );

	__computeSimpleDeviceID();

	eConfigParseState_t parseState = KEY;
//	__emptyPage[0] = 0xFF;
//	__emptyPage[1] = 0xBF;
	if( 0 == memcmp( &__rxData[__txDataIndex], &__emptyPage, 128 ) )
	{
		// Should only be when empty or that weird 0xFE thing.
		return 0;
	}
	else
	{
		size_t maxLen = strnlen((char *)&__rxData[__txDataIndex], sizeof(__rxData) - __txDataIndex);
		memcpy(__configString, &__rxData[__txDataIndex], maxLen);
		__configString[maxLen] = '\0';  // Ensure null-termination

		for( i = 0; i < strlen(__configString); i++ )
		{
			if( __configString[i] == '=' )
			{
				parseState = VALUE;
			}
			else if( __configString[i] == ';' )
			{
				parseState = KEY;
				if( 0 == strncmp( &__key[0], "SECRET", strlen("SECRET") ) )
				{
					if( 0 == strncmp( &__value[0], deviceID, strlen(deviceID) ) )
					{
						__setDebugMode(DEBUG_ENABLED);
					}
					else
					{
						__setDebugMode(DEBUG_DISABLED);
					}
				}
				// Stranger danger....
				else if( 0 == strncmp( &__key[0], "PIN", strlen("PIN") ) )
				{
					memcpy(gDeviceStatus.pin, __value, sizeof(gDeviceStatus.pin) );
				}
				else if( 0 == strncmp( &__key[0], "HUD_STATE", strlen("HUD_STATE") ) )
				{
					memcpy(gDeviceStatus.hudState, __value, sizeof(gDeviceStatus.hudState) );
				}
				else if( 0 == strncmp( &__key[0], "DEBUG_MODE", strlen("DEBUG_MODE") ) )
				{
					//memcpy(gDeviceStatus.debugEnabled, __value, sizeof() );
					if( 0 == strncmp( &__value[0], "ENABLED", strlen("ENABLED") ) )
					{
						gDeviceStatus.debugEnabled = 1;
					}
					else
					{
						gDeviceStatus.debugEnabled = 0;
					}
				}
				else if( 0 == strncmp( &__key[0], "FIRMWARE_VERSION", strlen("FIRMWARE_VERSION") ) )
				{
					memcpy(gDeviceStatus.firmwareVersion, __value, sizeof(gDeviceStatus.firmwareVersion) );
				}
				memset( __key, 0, sizeof( __key ) );
				memset( __value, 0, sizeof( __value ) );
				keyIndex = 0;
				valueIndex = 0;
			}
			else
			{
				if( parseState == KEY )
				{
					__key[keyIndex++] = __configString[i];
				}
				else if( parseState == VALUE )
				{
					__value[valueIndex++] = __configString[i];
				}
			}
		}
		return 1;
	}
}

static void __writeEnable(void)
{
	__resetBuffers();
	__txData[0] = SST_CMD_WRITE_ENABLE;
	__spiTransactionIT(1);
}

static void __writeDisable(void)
{
	__resetBuffers();
	__txData[0] = SST_CMD_WRITE_DISABLE;
	__spiTransactionIT(1);
}

static void __writeChipErase(void)
{
	__resetBuffers();
	__txData[__txDataIndex++] = SST_CMD_CHIP_ERASE;
	__spiTransactionIT(1);
}

static void __writeStatusRegister(void)
{
	__resetBuffers();
	__txData[__txDataIndex++] = SST_CMD_WRITE_STATUS_REG;
	__txData[__txDataIndex++] = 0; // Clear the BP0-3 bits. I believe i can just wipe out the other bits too.
	__spiTransactionIT(2);
}

static void __writeData(uint8_t * data, uint8_t firstPacket)
{
	__resetBuffers();
	if( firstPacket )
	{
		__txData[__txDataIndex++] = SST_CMD_AAI_WORD_PROGRAM;
		__txData[__txDataIndex++] = 0x00;
		__txData[__txDataIndex++] = 0x00;
		__txData[__txDataIndex++] = 0x00;
		memcpy(&__txData[__txDataIndex++], data, 2);
		__spiTransactionIT(6);
	}
	else
	{
		__txData[__txDataIndex++] = SST_CMD_AAI_WORD_PROGRAM;
		memcpy(&__txData[__txDataIndex++], data, 2);
		__spiTransactionIT(3);
	}
}

static void sendStatusToQueueFromISR(eSPIStatusMode_t spiStatus)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(__spiQueue, &spiStatus, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


static eSPIWriteState_t __writePage(void)
{
	__spiState = SPI_WRITING;
    switch (__spiWriteState)
    {
        case SPI_WRITEPAGE_WRITE_ENABLE:
        	__writeEnable();
        	__spiWriteState = SPI_WRITEPAGE_WRITE_STATUS_REGISTER;
        	break;
        case SPI_WRITEPAGE_WRITE_STATUS_REGISTER:
        	__writeStatusRegister();
        	__spiWriteState = SPI_WRITEPAGE_CHIP_ERASE;
            break;
        case SPI_WRITEPAGE_CHIP_ERASE:
        	__writeChipErase();
        	__spiWriteState = SPI_WRITEPAGE_WRITEPAGE;
        	break;
        case SPI_WRITEPAGE_WRITEPAGE:
        	// First Write
        	if( __writePageIndex == 0 )
        	{
        		__writeData(&__debugModeDisabledString[__writePageIndex], 1);
        		__writePageIndex += 2;
        	}
        	else
        	{
        		__writeData(&__debugModeDisabledString[__writePageIndex], 0);
        		__writePageIndex += 2;
        		if( __writePageIndex == strlen(__debugModeDisabledString) )
        		{
            		__spiWriteState = SPI_WRITEPAGE_WRITE_DISABLE;
        		}
        	}
            break;
        case SPI_WRITEPAGE_WRITE_DISABLE:
        	__writeDisable();
        	__spiWriteState = SPI_WRITEPAGE_SUCCESS;
        	break;
        case SPI_WRITEPAGE_SUCCESS:
        	__spiStatus = SPI_SUCCESS;
			sendStatusToQueueFromISR(__spiStatus);
            break;
        case SPI_WRITEPAGE_ERROR:
        default:
			__spiStatus = SPI_ERROR;
			sendStatusToQueueFromISR(__spiStatus);
            break;
    }
}

static eSPIReadState_t __readPage(void)
{
	uint32_t deviceID = 0;
	__spiState = SPI_READING;
	switch( __spiReadState )
	{
		case SPI_READPAGE_IDLE:
			__spiReadState = SPI_READPAGE_READID;
			__readID();
			break;
		case SPI_READPAGE_READID:
//			deviceID = (__rxData[3] << 16) | (__rxData[2] << 8) | __rxData[1];
			deviceID = (__rxData[1] << 16) | (__rxData[2] << 8) | __rxData[3];
			if( deviceID == SST_DEVICE_ID )
			{
				__spiReadState = SPI_READPAGE_READPAGE;
				__readData();
			}
			else
			{
				__spiStatus = SPI_ERROR;
				sendStatusToQueueFromISR(__spiStatus);
			}
			break;
		case SPI_READPAGE_READPAGE:
			__spiStatus = SPI_SUCCESS;
			sendStatusToQueueFromISR(__spiStatus);
			break;
		case SPI_READPAGE_ERROR:
		default:
			__spiStatus = SPI_ERROR;
			sendStatusToQueueFromISR(__spiStatus);
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    __setCS();
    if( __spiState == SPI_READING )
    {
    	__readPage();
    }
    else if( __spiState == SPI_WRITING )
    {
    	__writePage();
    }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)
{
	__setCS();
	__readPage();
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi)
{
	__setCS();
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
//	while(1);
	__resetBuffers();
	__spiState = SPI_IDLE;
}

void SPI4_IRQHandler(void)
{
    HAL_SPI_IRQHandler(&hspi4); // Call the HAL SPI IRQ handler, which will in turn call the appropriate SPI interrupt callback functions.
}

void SPIChallengeThread( void * argument )
{
	uint8_t test = 0;
//	__setDebugMode(1);
	eSPIWriteState_t writeState = SPI_WRITEPAGE_IDLE;
	eSPIReadState_t readState = SPI_READPAGE_IDLE;
	MX_SPI4_Init();
	eSPIStatusMode_t spiStatus;

	__computeSimpleDeviceID();
	__spiQueue = xQueueCreate(5, sizeof(eSPIStatusMode_t));
	memset( __emptyPage, 0xFF, sizeof(__emptyPage) ); // TODO: there has got to be a better way to do this

	__setCS();

	for( ;; )
	{
		__spiReadState = SPI_READPAGE_IDLE;
		readState = __readPage();
		configASSERT(__spiQueue != NULL);
		if( pdTRUE == xQueueReceive(__spiQueue, &spiStatus, 2025))
		{
			taskYIELD();
			if( spiStatus == SPI_SUCCESS )
			{
				if( 0 == __processPage() )
				{

					// Write Empty Page
					// Writing page wasn't working for some reason.
					writeState = __writePage();

				}
			}
			vTaskDelay(2025);
		}
	}
}
