/**
 * @file Motor_Init.h
 * @author Leo Liu
 * @brief general initialization for motors
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __MOTOR_INIT_H
#define __MOTOR_INIT_H
#include <stdint.h>
#include <stdio.h>

typedef struct
{
	 int16_t Actual_Angle;
	 int16_t Prev_Angle;
	 int16_t Actual_Speed;
	 int16_t Actual_Current;
	 int8_t Temperature;
	 
	 int32_t Target_Angle;
	 int16_t Target_Speed;
	 int32_t Total_Angle;
	 int16_t Turn_Count;
	 int16_t Output_Current;
	 
	 uint16_t Info_Update_Frame;
	 uint8_t Offline_Flag;
}Motor_Init_t;

#endif
