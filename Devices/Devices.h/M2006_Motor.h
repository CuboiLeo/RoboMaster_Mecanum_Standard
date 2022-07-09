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
#include "can.h"
#include "CAN_Setup.h"
#include <stdint.h>
#include <stdio.h>

#define M2006_TRIGGER_ID 0x206
#define M2006_OUTPUT_MAX 10000
#define M2006_GEAR_RATIO 36
#define M2006_MECH_ANGLE_MAX 8192
#define M2006_FEEDER_HOLE_NUM 8
#define M2006_ANGLE_1_BULLET (M2006_MECH_ANGLE_MAX * M2006_GEAR_RATIO / M2006_FEEDER_HOLE_NUM)

#define M2006_Func_GroundInit		  \
		{														  \
				&M2006_Trigger_Get_Data,  \
						&Check_M2006_Trigger, \
		}															\
		
typedef struct
{
	void (*M2006_Trigger_Get_Data)(CAN_Export_Data_t RxMessage);
	void (*Check_M2006_Trigger)(void);
}M2006_Func_t;

extern M2006_Func_t M2006_Func;
