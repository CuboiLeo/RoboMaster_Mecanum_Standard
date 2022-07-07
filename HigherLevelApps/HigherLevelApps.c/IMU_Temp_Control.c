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
#include "Board_A_IMU.h"

void Board_A_IMU_Temp_Control(void);

void Board_A_IMU_Temp_Control(void)
{
	if(IMU_Export.temp < MPU6500_MAX_TEMP)
	{
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, MPU6500_TEMP_MAX_PWM - 1);
		IMU_Export.Temp_ReachFlag = 0;
	}
	else
	{
		IMU_Export.Temp_ReachFlag = 1;
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
	}
}
