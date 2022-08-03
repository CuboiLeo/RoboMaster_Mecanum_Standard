/**
 * @file M2006_Motor.h
 * @author Leo Liu
 * @brief header file for M2006
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __M2006_MOTOR_H
#define __M2006_MOTOR_H

#include "can.h"
#include "CAN_Setup.h"
#include "Motor_Init.h"
#include <stdint.h>
#include <stdio.h>

#define M2006_TRIGGER_ID 0x206 //Trigger motor ID
#define M2006_OUTPUT_MAX 10000.0f //M2006 motor maximum output current
#define M2006_GEAR_RATIO 36 //M2006 motor gear ratio
#define M2006_MECH_ANGLE_MAX 8192 //M2006 motor maximum mechanical angle
#define M2006_FEEDER_HOLE_NUM 8 //Number of holes in the feeder
#define M2006_ANGLE_1_BULLET 36864 //Mechanical angle needed to turn feeder for one bullet 
																	 //Calculated by (M2006_MECH_ANGLE_MAX * M2006_GEAR_RATIO / M2006_FEEDER_HOLE_NUM)

#define M2006_Func_GroundInit		  		\
		{														  		\
				&M2006_Trigger_Get_Data,  		\
						&M2006_Trigger_Send_Data, \
						&Check_M2006_Trigger, 		\
		}																	\
		
typedef struct
{
	void (*M2006_Trigger_Get_Data)(CAN_Export_Data_t RxMessage);
	void (*M2006_Trigger_Send_Data)(int16_t Trigger_Current);
	void (*Check_M2006_Trigger)(void);
}M2006_Func_t;

extern M2006_Func_t M2006_Func;
extern Motor_Init_t M2006_Trigger;

#endif
