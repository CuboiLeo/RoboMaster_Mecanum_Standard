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
#ifndef __IMU_TEMP_CONTROL_H
#define __IMU_TEMP_CONTROL_H
 
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tim.h"
#include "Board_A_IMU.h"

#define MPU6500_TEMP_MAX_PWM 5000 //Maximum pwm (determined by counter period set in CubeMX)
#define MPU6500_MAX_TEMP 50.0f //Desired temperature (50 is great don't change this)

#define IMU_Temp_Control_Func_GroundInit	\
{																					\
		&Board_A_IMU_Temp_Control_Init,				\
				&Board_A_IMU_Temp_Control,				\
}

typedef struct
{
	void (*Board_A_IMU_Temp_Control_Init)(void);
	void (*Board_A_IMU_Temp_Control)(void);
}IMU_Temp_Control_Func_t;

extern IMU_Temp_Control_Func_t IMU_Temp_Control_Func;

#endif
