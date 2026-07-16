/*
 * MPU_6050.h
 *
 *  Created on: Jul 15, 2026
 *      Author: Winner-Emele
 */

#ifndef INC_MPU_6050_H_
#define INC_MPU_6050_H_
#include <main.h>
#include <math.h>
#include <stdio.h>
#define MPU_ADDR (0x68 << 1)
#define WHO_AM_I (0x75)
#define WAKE (0x6B)
#define ALPHA (0.98f)
#define READINGS (0x3B)
#define GYRO_CONFIG (0x1B)
#define ACCEL_CONFIG (0x1C)
void MPU6050_Init(I2C_HandleTypeDef *hi2c, uint8_t GYRO_MODE, uint8_t ACCEL_MODE);        // Initializing...
void MPU_6050_POSITION(I2C_HandleTypeDef *hi2c, uint8_t raw_data[14], uint8_t GYRO_MODE); // using Complementary Filters
#endif                                                                                    /* INC_MPU_6050_H_ */
