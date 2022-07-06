/**
 * @file IMU_Temp_Control.c
 * @author Leo Liu
 * @brief control the temperature of the IMU_Calculated_Data
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tim.h"

#define MPU6500_TEMP_MAX_PWM 5000
#define MPU6500_MAX_TEMP 40.0f

extern void Board_A_IMU_Temp_Control(void);
