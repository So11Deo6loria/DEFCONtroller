/*
 * i2c.c
 *
 *  Created on: Jul 8, 2022
 *      Author: calebdavis
 */

#define SHT3X_I2C_DEVICE_ADDRESS_ADDR_PIN_LOW 0x44
#define SHT3X_I2C_TIMEOUT 100

#include "i2c.h"
#include "main.h"

#define TEMP_MIN_F 40.0
#define TEMP_MAX_F 110.0


//global Variables
float i2c_temperature = 75.0;
float i2c_humidity = 00;
uint8_t i2c_isPumpAllowed = 1;
uint8_t i2c_lowerTempThreshold; //TODO: Maybe configure from UART?
uint8_t i2c_upperTempThreshold;
uint8_t i2c_isValueUpdated = 1;


extern I2C_HandleTypeDef hi2c3;

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Devaddress = SHT3X_I2C_DEVICE_ADDRESS_ADDR_PIN_LOW;
//  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c3.Init.OwnAddress2 = 0;
//  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */
}

/**
 * Registers addresses.
 */
typedef enum
{
	SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH = 0x2c06,
	SHT3X_COMMAND_CLEAR_STATUS = 0x3041,
	SHT3X_COMMAND_SOFT_RESET = 0x30A2,
	SHT3X_COMMAND_HEATER_ENABLE = 0x306d,
	SHT3X_COMMAND_HEATER_DISABLE = 0x3066,
	SHT3X_COMMAND_READ_STATUS = 0xf32d,
	SHT3X_COMMAND_FETCH_DATA = 0xe000,
	SHT3X_COMMAND_MEASURE_HIGHREP_10HZ = 0x2737,
	SHT3X_COMMAND_MEASURE_LOWREP_10HZ = 0x272a
} sht3x_command_t;

static uint8_t calculate_crc(const uint8_t *data, size_t length)
{
	uint8_t crc = 0xff;
	for (size_t i = 0; i < length; i++) {
		crc ^= data[i];
		for (size_t j = 0; j < 8; j++) {
			if ((crc & 0x80u) != 0) {
				crc = (uint8_t)((uint8_t)(crc << 1u) ^ 0x31u);
			} else {
				crc <<= 1u;
			}
		}
	}
	return crc;
}

static uint8_t sht3x_send_command(I2C_HandleTypeDef *handle, sht3x_command_t command)
{
	uint8_t command_buffer[2] = {(command & 0xff00u) >> 8u, command & 0xffu};

	if (HAL_I2C_Master_Transmit(handle, handle->Devaddress << 1u, command_buffer, sizeof(command_buffer),
	                            SHT3X_I2C_TIMEOUT) != HAL_OK) {
		return 0;
	}

	return 1;
}

static uint16_t uint8_to_uint16(uint8_t msb, uint8_t lsb)
{
	return (uint16_t)((uint16_t)msb << 8u) | lsb;
}

static uint8_t sht3x_init(I2C_HandleTypeDef *handle)
{
	//	assert(handle->Init.NoStretchMode == I2C_NOSTRETCH_DISABLE);
	// TODO: Assert i2c frequency is not too high

	uint8_t status_reg_and_checksum[3];
	while(osOK != AquireI2CPerpheral() );

	if (HAL_I2C_Mem_Read(handle, handle->Devaddress << 1u, SHT3X_COMMAND_READ_STATUS, 2, (uint8_t*)&status_reg_and_checksum,
					  sizeof(status_reg_and_checksum), SHT3X_I2C_TIMEOUT) != HAL_OK) {
		ReleaseI2CPeripheral();
		return 0;
	}

	ReleaseI2CPeripheral();

	uint8_t calculated_crc = calculate_crc(status_reg_and_checksum, 2);

	if (calculated_crc != status_reg_and_checksum[2]) {
		return 0;
	}

	return 1;
}

static uint8_t sht3x_read_temperature_and_humidity(I2C_HandleTypeDef *handle, float *temperature, float *humidity)
{
	while(osOK != AquireI2CPerpheral() );
	sht3x_send_command(handle, SHT3X_COMMAND_MEASURE_LOWREP_10HZ);

	HAL_Delay(1);

	uint8_t buffer[6];

	if (HAL_I2C_Master_Receive(handle, handle->Devaddress << 1u, buffer, sizeof(buffer), SHT3X_I2C_TIMEOUT) != HAL_OK) {
		ReleaseI2CPeripheral();
		return 0;
	}

	ReleaseI2CPeripheral();

	uint8_t temperature_crc = calculate_crc(buffer, 2);
	uint8_t humidity_crc = calculate_crc(buffer + 3, 2);
	if (temperature_crc != buffer[2] || humidity_crc != buffer[5]) {
		return 0;
	}

	int16_t temperature_raw = (int16_t)uint8_to_uint16(buffer[0], buffer[1]);
	uint16_t humidity_raw = uint8_to_uint16(buffer[3], buffer[4]);

	*temperature = -45.0f + 175.0f * (float)temperature_raw / 65535.0f;
	*humidity = 100.0f * (float)humidity_raw / 65535.0f;

	return 1;
}

static uint8_t sht3x_set_header_enable(I2C_HandleTypeDef *handle, uint8_t enable)
{
	if (enable) {
		return sht3x_send_command(handle, SHT3X_COMMAND_HEATER_ENABLE);
	} else {
		return sht3x_send_command(handle, SHT3X_COMMAND_HEATER_DISABLE);
	}
}

static float __celsiusToFahrenheit( float celsiusTemp )
{
	float fahrenheitTemp;

	fahrenheitTemp = ( celsiusTemp * 9 / 5 ) + 32;

	return fahrenheitTemp;
}

static uint8_t __isPumpAllowed( float temp )
{
	if( temp < TEMP_MIN_F )
	{
		return 0;
	}
	else
	{
		return 1;
	}

	if( temp > TEMP_MAX_F )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


void I2CChallengeThread( void * argument )
{
	uint8_t initStatus = 0;
	float celsTemp = 0.0;
	MX_I2C3_Init();
	initStatus = sht3x_init(&hi2c3);

	while( 0 == sht3x_init(&hi2c3))
	{
		// Issue with Sensor Init. Try every 2s.
		osDelay(2000);
	}

	for(;;)
	{
		if( !(i2c_isValueUpdated) )
		{
			sht3x_read_temperature_and_humidity(&hi2c3, &celsTemp, &i2c_humidity);
			i2c_temperature = __celsiusToFahrenheit(celsTemp);
			i2c_isPumpAllowed = __isPumpAllowed(i2c_temperature);
			i2c_isValueUpdated = 1;
		}
		osDelay(500);
	}
	  /* USER CODE END 5 */
}
