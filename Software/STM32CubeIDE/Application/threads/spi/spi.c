/*
 * spi.c
 *
 *  Created on: Jul 8, 2022
 *      Author: calebdavis
 */

#include "spi.h"
#include "string.h"


// Global Variables for Use with TouchGFX.
char SPIModelNumber[16] = "NOMODEL";
char SPISerialNumber[16] = "NOSERIAL";
char SPISoftwareVersion[16] = "0.0.0";

uint8_t debugFlagTouchGFX = 0; /// 0 = use to communicate with touchgfx
uint8_t debugUpdatedTouchGFX = 0; /// 0 = false use to communicate with touchgfx
uint8_t SPIExternalFlashUpdated = 0;


// Local Items.
static uint8_t __debugDisabledString[256] = "MODEL=G312432B;SERIAL=23BD-29AF;VERSION=0.1.2;LOGS=ENCRYPTED;DEBUG=0;BLUETOOTH=MDM1234;PIN=5678";
static uint8_t __debugEnabledString[256]  = "MODEL=G312432B;SERIAL=23BD-29AF;VERSION=0.1.2;LOGS=ENCRYPTED;DEBUG=1;BLUETOOTH=MDM1234;PIN=5678";
static uint8_t __emptyPage[256];

#define STM25P80_ID 0x202014
#define DUMMY_BYTE 0xA5
#define STM25P80_PAGESIZE 256

/* Instructions */
#define READ_DATA_BYTES 0x03
#define READ_IDENTIFICATION 0x9F
#define PAGE_PROGRAM 0x02
#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
#define SECTOR_ERASE 0xD8

extern SPI_HandleTypeDef hspi4;
static eDebugMode_t __debugMode = DEBUG_UNSET;
static eSPIState_t 	__spiState 	= SPI_READID;

/* Data Buffers */
static uint16_t __txDataIndex = 0;
static uint16_t __rxDataIndex = 0;
static uint8_t __txData[512];
static uint8_t __rxData[512];

static uint8_t __configString[256];
static uint8_t __key[32];
static uint8_t __value[32];

HAL_SPI_StateTypeDef halSpiState;

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
 */
static void MX_SPI4_Init (void)
{
    /** SPI4 GPIO Configuration
    PC3     ------> CS
    PE2     ------> SPI4_SCK
    PE5     ------> SPI4_MISO
    PE6     ------> SPI4_MOSI
    */
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* CS Configuration */
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);

	/*Configure GPIO CS pin: PC3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init (GPIOC, &GPIO_InitStruct);

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
}

static void __spiRead (uint16_t Length)
{
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); 	// Drive CS Low
	HAL_SPI_Receive_IT (&hspi4, &__rxData[0], Length);	// Blocking Read Call
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High
}

static void __spiWrite (uint16_t Length)
{
	HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); 	// Drive CS Low
	HAL_SPI_Transmit (&hspi4, &__txData[0], Length, 100);	// Blocking Write Call

	__spiRead (10);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High
}

static void __spiTransactionIT (uint16_t Length)
{
	HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); 	// Drive CS Low
	HAL_SPI_TransmitReceive_IT (&hspi4, __txData, __rxData, Length);
}

static void __spiTransaction (uint16_t Length)
{
	HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); 	// Drive CS Low
	HAL_SPI_TransmitReceive (&hspi4, __txData, __rxData, Length, 100);
	HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High

}

static uint8_t __waitForReady (void)
{
	HAL_SPI_StateTypeDef halState;
	while (HAL_SPI_STATE_READY != (halState = HAL_SPI_GetState (&hspi4)))
	{
		osDelay (1);
	}

	return 1;
}

static void __spiWriteRead( uint16_t txLength, uint16_t rxLength )
{
	/* Trying to make sure CS stays low */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); 	// Drive CS Low

	__spiWrite( txLength );
	__waitForReady();
	__spiRead( rxLength );

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High
}

static void __resetBuffers(void)
{
	__txDataIndex = 0;
	__rxDataIndex = 0;
	memset(__txData, 0, sizeof(__txData) );
	memset(__rxData, 0, sizeof(__rxData) );
}



static uint8_t __readID(void)
{
	uint32_t readID = 0;
	__resetBuffers();
	__txData[0] = 0x9F;
	__spiTransaction(4); // 1 Byte Command + 3 Byte Response

	readID = (__rxData[1] << 16) | (__rxData[2] << 8) | __rxData[3];
	if( readID == STM25P80_ID )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static void __writePage(uint8_t * writeBuffer)
{
	// Should only run this when setting up flash components.
	uint16_t length = STM25P80_PAGESIZE;

	/* Write Enable */
	__resetBuffers();
	__txData[__txDataIndex++] = WRITE_ENABLE;
	__spiTransaction(1); // 1 Byte Command

	/* Make Sure You Don't Write Too Quickly. May not need this HAL might do it for you. */
	__waitForReady();

	/* Page Program */
	__resetBuffers();
	__txData[__txDataIndex++] = PAGE_PROGRAM;
	__txData[__txDataIndex++] = 0x00;
	__txData[__txDataIndex++] = 0x00;
	__txData[__txDataIndex++] = 0x00;
	memcpy(&__txData[__txDataIndex], writeBuffer, length);
	__spiTransaction(__txDataIndex+length); // Confirm this value
}

static void __setDebugMode (eDebugMode_t debugMode)
{
	if (!debugFlagTouchGFX) // Only Check if No flag has not been set.
	{
		if (__debugMode != debugMode)
		{
			__debugMode = debugMode;
			debugUpdatedTouchGFX = 1;

			if (debugMode && (0 != strncmp(&__value[0], "1", strlen('1')))) // If the Debug Value is not set in our buffer, but we have mysteriously got here...
			{
				// Damn You Reverse Engineering!
				debugFlagTouchGFX = 7;
			}
			else
			{
				debugFlagTouchGFX = __debugMode; //TODO just pass __debugMode directly but had weird build issue with that poss because it is static
			}

			// Notify TouchGFX
			debugUpdatedTouchGFX = 1; //flag set to true
		}
	}
}

static void __processPage(void)
{
	uint16_t i = 0, keyIndex = 0, valueIndex = 0;

	uint32_t error = 0;
	error = HAL_SPI_GetError( &hspi4 );

	eConfigParseState_t parseState = KEY;
	if( 0 == memcmp( &__rxData[__txDataIndex], &__emptyPage, sizeof( __emptyPage ) ) )
	{
		// Should only be when empty or that weird 0xFE thing.

		__writePage( (uint8_t*)&__debugDisabledString );
	}
	else
	{
		memcpy( &__configString, &__rxData[__txDataIndex], strlen( &__rxData[__txDataIndex] ) );
		for( i = 0; i < strlen(__configString); i++ )
		{
			if( __configString[i] == '=' )
			{
				parseState = VALUE;
			}
			else if( __configString[i] == ';' )
			{
				parseState = KEY;
				if( 0 == strncmp( &__key[0], "DEBUG", strlen("DEBUG") ) )
				{
					if( 0 == strncmp( &__value[0], "1", strlen("1") ) )
					{
						__setDebugMode(DEBUG_ENABLED);
					}
					else
					{
						__setDebugMode(DEBUG_DISABLED);
					}
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
	}
}

static void __readPage(void)
{
	__resetBuffers();
	__txData[__txDataIndex++] = READ_DATA_BYTES;
	__txData[__txDataIndex++] = 0x00; // Start at Address 0x00
	__txData[__txDataIndex++] = 0x00; // Start at Address 0x00
	__txData[__txDataIndex++] = 0x00; // Start at Address 0x00

	__spiTransaction(STM25P80_PAGESIZE+4); // Command (1) + Address (3) + PAGE (256)
	__processPage();
}


static void __eraseSector(void)
{
	/* Write Enable */
	__resetBuffers();
	__txData[__txDataIndex++] = WRITE_ENABLE;
	__spiTransaction(1); // 1 Byte Command

	/* Make Sure You Don't Write Too Quickly. May not need this HAL might do it for you. */
	__waitForReady();

	__resetBuffers();
	__txData[__txDataIndex++] = SECTOR_ERASE;
	__txData[__txDataIndex++] = 0x00; // Start at Address 0x00
	__txData[__txDataIndex++] = 0x00; // Start at Address 0x00
	__txData[__txDataIndex++] = 0x00; // Start at Address 0x00

	__spiTransaction(4);
}

static void __processData( void )
{
	uint32_t readID = 0;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
	switch( __spiState )
	{
		case SPI_READID:
			readID = (__rxData[1] << 16) | (__rxData[2] << 8) | __rxData[3];
			if( readID == STM25P80_ID )
			{
				__spiState = SPI_READPAGE;
				__readPage();
			}
			else
			{
				__spiState = SPI_ERROR;
			}
			break;
		case SPI_READPAGE:
			break;
		case SPI_WRITEPAGE:
			break;
		case SPI_ERROR:
			// Leaving this blank to hit the while loop
		default:
			// ERROR State:
			while(1);
	}
}

static void __prepareFlash(uint8_t * writeBuffer)
{
	__eraseSector();
	osDelay(2000);
	__waitForReady();
	__writePage( writeBuffer );	// Testing purposes only
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High
	while(1);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High
	__processData();
}

void SPIChallengeThread( void * argument )
{
	uint8_t test = 0;
	__setDebugMode(1);
	MX_SPI4_Init();
	//__setDebugMode(DEBUG_DISABLED); //matt debuging TODO delete
	memset( __emptyPage, 0xFF, sizeof(__emptyPage) ); // TODO: there has got to be a better way to do this
	HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 	// Drive CS High

	if( 1 == test ) // Write Debug Disabled
	{
		__prepareFlash((uint8_t*)&__debugDisabledString);
		__waitForReady();
		osDelay(2000); // Quit being stupid and poll the status register

	}
	else if( 2 == test ) // Write Debug Enabled
	{
		__prepareFlash((uint8_t*)&__debugEnabledString);
		__waitForReady();
		osDelay(2000); // Quit being stupid and poll the status register
	}

	if( !__readID() )
	{
		while( 1 )
		{
			osDelay(500); // Error condition. Beware
		}
	}

	for(;;)
	{
		__readPage();
		osDelay(2022);
	}
}

