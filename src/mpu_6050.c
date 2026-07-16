# MPU6050 STM32 HAL Driver

A lightweight, easy-to-use C library for interfacing the MPU6050 6-axis Accelerometer and Gyroscope with STM32 microcontrollers. It uses the STM32 Hardware Abstraction Layer (HAL) for I2C communication and features a built-in complementary filter for calculating stable roll angles.

## Features

* **I2C Communication:** Reads and writes to the MPU6050 using standard STM32 HAL I2C functions.
* **Configurable Ranges:** Support for adjusting the full-scale range of both the Gyroscope and Accelerometer.
* **Sensor Fusion:** Utilizes a complementary filter (default $\alpha = 0.98$) to combine gyroscope and accelerometer data into a clean, drift-free positional angle (Roll).

## Requirements

To use this library, your STM32 project must have the following configured:

1. **STM32 HAL Library:** Ensure `<main.h>` is included and the HAL drivers are linked.
2. **I2C Peripheral Enabled:** Configure an I2C bus (e.g., `I2C1`) in STM32CubeMX with standard mode (100kHz) or fast mode (400kHz).
3. **Floating-Point Unit (Optional but recommended):** Hardware FPU is recommended due to the use of `atan2f` and floating-point math in the complementary filter.
4. **`printf` Retargeting:** The library uses `printf` for debugging and output. You must redirect `printf` to a UART port (e.g., overriding `_write` in `syscalls.c`) to see the initialization status and angle outputs.

## Installation

1. Include `MPU_6050.h` in your `Core/Inc` directory.
2. Include the source code in your `Core/Src` directory.
3. Make sure `#include "MPU_6050.h"` is added to your `main.c` file.

## Usage Example

```c
#include "main.h"
#include "MPU_6050.h"

extern I2C_HandleTypeDef hi2c1; // Your configured I2C handle
uint8_t mpu_raw_data[14];

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_I2C1_Init();
    MX_USART2_UART_Init(); // Required if printf is retargeted here

    // Initialize MPU6050: Gyro Mode 0 (±250°/s), Accel Mode 0 (±2g)
    if (MPU6050_Init(&hi2c1, 0, 0) != 0) {
        // Handle initialization failure
        Error_Handler(); 
    }

    while (1) {
        // Read data and calculate Roll (Position)
        MPU_6050_POSITION(&hi2c1, mpu_raw_data, 0);
        
        // Loop delay dictates your sampling rate
        HAL_Delay(10); 
    }
}
