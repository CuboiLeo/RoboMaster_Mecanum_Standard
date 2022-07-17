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

#define FRIC_SPEED_16	5300
#define FRIC_SPEED_28	7400
#define TRIGGER_DIRECTION -1

#define Shooting_Func_GroundInit	\
{																	\
		&Turn_Friction_Wheel_On,			\
				&Turn_Friction_Wheel_Off,	\
				&Trigger_Get_Data,				\
				&Trigger_Processing,			\
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
	
	uint8_t Fric_Wheel_On_Flag;
}Shooting_t;

typedef struct
{
	void (*Turn_Friction_Wheel_On)(void);
	void (*Turn_Friction_Wheel_Off)(void);
	void (*Trigger_Get_Data)(Shooting_t *Shooting);
	void (*Trigger_Processing)(Shooting_t *Shooting);
	void (*Shooting_Disabled)(void);
}Shooting_Func_t;

extern Shooting_t Shooting;
extern Shooting_Func_t Shooting_Func;

#endif
