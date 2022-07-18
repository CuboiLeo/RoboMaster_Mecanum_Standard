/**
 * @file Shooting_Control.h
 * @author Leo Liu
 * @brief header file for shooting control
 * @version 1.0
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __SHOOTING_CONTROL_H
#define __SHOOTING_CONTROL_H

#include "M3508_Motor.h"
#include "M2006_Motor.h"
#include "State_Machine.h"
#include "DR16_Remote.h"
#include "Ramp_Calc.h"

#define FRIC_SPEED_16	5300
#define FRIC_SPEED_28	7400
#define TRIGGER_DIRECTION -1
#define FRIC_LEFT_DIRECTION -1
#define FRIC_RIGHT_DIRECTION 1

#define Shooting_Func_GroundInit	\
{																	\
		&Shooting_Init,								\
				&Trigger_Get_Data,				\
				&Shooting_Processing,			\
				&Shooting_Disabled,				\
}

typedef struct
{
	struct
	{
		uint8_t Single_Fire_Flag;
		uint8_t Single_Fired_Flag;
		uint8_t Burst_Flag;
	}Type;
	
	struct
	{
		float Target_Angle;
		float Target_Speed;
	}Trigger;
	
	struct
	{
		uint8_t Turned_On;
	}Fric_Wheel;
	
	uint8_t Fric_Wheel_Ready_Flag;
}Shooting_t;

typedef struct
{
	void (*Shooting_Init)(void);
	void (*Trigger_Get_Data)(Shooting_t *Shooting);
	void (*Shooting_Processing)(Shooting_t *Shooting);
	void (*Shooting_Disabled)(void);
}Shooting_Func_t;

extern Shooting_t Shooting;
extern Shooting_Func_t Shooting_Func;

#endif
