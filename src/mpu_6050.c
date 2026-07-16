#include "MPU_6050.h"

void MPU6050_Init(I2C_HandleTypeDef *hi2c, uint8_t GYRO_MODE, uint8_t ACCEL_MODE)
{
	uint8_t buffer;
	uint8_t clear = 0;
	HAL_I2C_Mem_Read(hi2c, MPU_ADDR, WHO_AM_I, 1, &buffer, 1, HAL_MAX_DELAY); // Read from the WHO_AM_I Register...
	if (buffer == 0x70 || buffer == 0x68)
	{
		printf("0x%0x The Device is working properly...\n", buffer);
		printf("Powering on.....\n");
		HAL_I2C_Mem_Write(hi2c, MPU_ADDR, WAKE, 1, &clear, 1, HAL_MAX_DELAY);
	}
	else
	{
		printf("The Device is not Working Properly.....\n");
		return 1;
	}
	GYRO_MODE = GYRO_MODE << 3;
	ACCEL_MODE = ACCEL_MODE << 3;
	HAL_I2C_Mem_Write(hi2c, MPU_ADDR, GYRO_CONFIG, 1, &GYRO_MODE, 1, HAL_MAX_DELAY); // Gyroscope Configuration...
	printf("The Gyroscope is in mode %d.....\n", GYRO_MODE);

	HAL_I2C_Mem_Write(hi2c, MPU_ADDR, ACCEL_CONFIG, 1, &ACCEL_MODE, 1, HAL_MAX_DELAY); // Accelerometer Configuration...
	printf("The Accelerometer is in mode %d.....\n", ACCEL_MODE);
}
void MPU_6050_POSITION(I2C_HandleTypeDef *hi2c, uint8_t raw_data[14], uint8_t GYRO_MODE)
{
	uint32_t initial = HAL_GetTick();
	float tilt; // I meant Roll
	HAL_I2C_Mem_Read(hi2c, MPU_ADDR, READINGS, 1, raw_data, 14, HAL_MAX_DELAY);
	// Extracting the Gyroscope and Accelerometer readings....
	int16_t accel_x = (int16_t)(raw_data[0] << 8 | raw_data[1]);
	int16_t accel_y = (int16_t)(raw_data[2] << 8 | raw_data[3]);
	int16_t accel_z = (int16_t)(raw_data[4] << 8 | raw_data[5]);

	int16_t gyro_x = (int16_t)(raw_data[8] << 8 | raw_data[9]);
	int16_t gyro_y = (int16_t)(raw_data[10] << 8 | raw_data[11]);
	int16_t gyro_z = (int16_t)(raw_data[12] << 8 | raw_data[13]);
	// Convert the gyro to degrees per second value
	switch (GYRO_MODE)
	{
	case 0:
		tilt = gyro_x / 131.0;
		break;
	case 1:
		tilt = gyro_x / 65.5;
		break;
	case 2:
		tilt = gyro_x / 32.8;
		break;
	case 3:
		tilt = gyro_x / 16.4;
		break;
	default:
		break;
	}
	float ACC_angle = atan2f(accel_y, accel_z) * (180 / M_PI);
	uint32_t final = HAL_GetTick();
	float dt = (float)(final - initial) / 1000;
	initial = HAL_GetTick();
	static float Position = 0.0f;
	Position = ALPHA * (Position + tilt * dt) + (1 - ALPHA) * ACC_angle;
	printf("Position is:%.2f\n", Position);
}
