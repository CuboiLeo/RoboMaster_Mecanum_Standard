/**
 * @file M3508_Motor.h
 * @author Leo Liu
 * @brief header file for M3508
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "can.h"
#include "CAN_Setup.h"
#include <stdint.h>
#include <stdio.h>

#define M3508_CHASSIS_START_ID 0x201
#define M3508_CHASSIS_END_ID	0x204
#define M3508_FRIC_WHEEL_LEFT_ID 0x201
#define M3508_FRIC_WHEEL_RIGHT_ID 0x202
#define M3508_OUTPUT_MAX 16384

#define M3508_Func_GroundInit					  \
		{																	  \
				&M3508_Chassis_Get_Data,			  \
						&M3508_Fric_Wheel_Get_Data, \
						&Check_M3508_Chassis,			  \
						&Check_M3508_Fric_Wheel,		\
		}

typedef struct
{
	void (*M3508_Chassis_Get_Data)(CAN_Export_Data_t RxMessage);
	void (*M3508_Fric_Wheel_Get_Data)(CAN_Export_Data_t RxMessage);
	void (*Check_M3508_Chassis)(void);
	void (*Check_M3508_Fric_Wheel)(void);
}M3508_Func_t;

extern M3508_Func_t M3508_Func;
