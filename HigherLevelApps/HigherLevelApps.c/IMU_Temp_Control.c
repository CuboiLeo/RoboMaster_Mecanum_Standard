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
 
#include "IMU_Temp_Control.h"

void Board_A_IMU_Temp_Control_Init(void);
void Board_A_IMU_Temp_Control(void);

IMU_Temp_Control_Func_t IMU_Temp_Control_Func = IMU_Temp_Control_Func_GroundInit;
#undef IMU_Temp_Control_Func_GroundInit

//Start timer's pwm function
void Board_A_IMU_Temp_Control_Init(void)
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

void Board_A_IMU_Temp_Control(void)
{
	if(Board_A_IMU.Export_Data.Temperature < MPU6500_MAX_TEMP)
	{
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, MPU6500_TEMP_MAX_PWM - 1); //Maximum pwm before temperature is reached
	}
	else
	{
		Board_A_IMU.Temperature_Reached_Flag = 1;
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
	}
}
