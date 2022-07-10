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
#include "PID.h"
#include "DR16_Remote.h"
#include "User_Defined_Math.h"

#define MOTOR_SPEED_MAX 6880
#define MOTOR_GEAR_RATIO 14

#define Chassis_Func_GroundInit		\
{																	\
		&Chassis_Speed_Get_Data,			\
				&Inverse_Kinematic_Calc,	\
				&Chassis_Processing,			\
}

typedef struct
{
	enum
	{
		Follow_Gimbal,
		Not_Follow_Gimbal,
		Spin_Top,
		Disabled,
	}Mode;
	
	struct
	{
		int16_t Vx;
		int16_t Vy;
		int16_t Wz;
	}Speed;
	
	int Current_Mode;
	float Temp_Speed[4];
}Chassis_t;

typedef struct
{
	void (*Chassis_Speed_Get_Data)(Chassis_t *Chassis);
	void (*Inverse_Kinematic_Calc)(Chassis_t *Chassis);
	void (*Chassis_Processing)(Chassis_t *Chassis);
}Chassis_Func_t;

extern Chassis_t Chassis;
extern Chassis_Func_t Chassis_Func;
