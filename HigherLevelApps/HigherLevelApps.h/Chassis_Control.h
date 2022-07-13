/**
 * @file Chassis_Control.h
 * @author Leo Liu
 * @brief header file for Chassis_Control
 * @version 1.0
 * @date 2022-07-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H

#include "PID.h"
#include "DR16_Remote.h"
#include "User_Defined_Math.h"
#include "Gimbal_Control.h"
#include "Robot_Control.h"

#define MOTOR_SPEED_MAX 3000 //6880
#define MOTOR_GEAR_RATIO 19
#define CHASSIS_WIDTH_HALF 0.15f
#define CHASSIS_LENGTH_HALF 0.18f

#define Chassis_Func_GroundInit		\
{																	\
		&Chassis_Speed_Get_Data,			\
				&Inverse_Kinematic_Calc,	\
				&Chassis_Processing,			\
}

typedef struct
{
	int Current_Mode;
	
	struct
	{
		float Vx;
		float Vy;
		float Wz;
	}Chassis_Coord;
	
	struct
	{
		float Vx;
		float Vy;
		float Wz;
	}Gimbal_Coord;
	
	float Wheel_Speed[4];
	
	uint8_t Chassis_Offline_Flag;
}Chassis_t;

typedef struct
{
	void (*Chassis_Speed_Get_Data)(Chassis_t *Chassis);
	void (*Inverse_Kinematic_Calc)(Chassis_t *Chassis);
	void (*Chassis_Processing)(Chassis_t *Chassis);
}Chassis_Func_t;

extern Chassis_t Chassis;
extern Chassis_Func_t Chassis_Func;

#endif
