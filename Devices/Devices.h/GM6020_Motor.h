/**
 * @file GM6020_Motor.h
 * @author Leo Liu
 * @brief header file for GM6020
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __GM6020_MOTOR_H
#define __GM6020_MOTOR_H

#include "can.h"
#include "CAN_Setup.h"
#include "Motor_Init.h"
#include <stdint.h>
#include <stdio.h>

#define GM6020_YAW_ID 0x20B //Yaw motor ID
#define GM6020_PITCH_ID 0x20A //Pitch Motor ID
#define GM6020_OUTPUT_MAX 30000.0f //GM6020 motor maximum output current
#define GM6020_MECH_ANGLE_MAX 8192.0f //GM6020 maximum mechanical angle
#define GM6020_ANGLE_CONVERT 0.0439453125f //(360/GM6020_MECH_ANGLE_MAX)

#define GM6020_Func_GroundInit        \
    {                                 \
        &GM6020_Yaw_Get_Data,         \
            &GM6020_Pitch_Get_Data,   \
						&GM6020_Gimbal_Send_Data, \
            &Check_GM6020_Yaw,        \
            &Check_GM6020_Pitch,      \
    }

typedef struct
{
	void (*GM6020_Yaw_Get_Data)(CAN_Export_Data_t RxMessage);
  void (*GM6020_Pitch_Get_Data)(CAN_Export_Data_t RxMessage);
	void (*GM6020_Gimbal_Send_Data)(int16_t Pitch_Output, int16_t Yaw_Output);
  void (*Check_GM6020_Yaw)(void);
  void (*Check_GM6020_Pitch)(void);
}GM6020_Func_t;

extern GM6020_Func_t GM6020_Func;
extern Motor_Init_t GM6020_Yaw;
extern Motor_Init_t GM6020_Pitch;

#endif
