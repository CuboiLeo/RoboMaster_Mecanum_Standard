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
#include "Super_Capacitor.h"
#include "Ramp_Calc.h"

#define MOTOR_SPEED_MAX 3000 //6880(theoretical max speed) //This is where to change the maximum speed
#define MOTOR_GEAR_RATIO 19 //Gear ratio of m3508 planetary gear
#define CHASSIS_WIDTH_HALF 0.15f //Measured value of the chassis's half width
#define CHASSIS_LENGTH_HALF 0.18f //Measured value of the chassis's half length
#define CHASSIS_RAMP_RATE 200.0f

#define CHASSIS_NORMAL_SPEED_COEFF_45 60 //Normal speed coeff
#define CHASSIS_NORMAL_SPEED_COEFF_50 130 //Normal speed coeff
#define CHASSIS_NORMAL_SPEED_COEFF_55 140 //Normal speed coeff
#define CHASSIS_NORMAL_SPEED_COEFF_60 150 //Normal speed coeff
#define CHASSIS_NORMAL_SPEED_COEFF_80 180 //Normal speed coeff
#define CHASSIS_NORMAL_SPEED_COEFF_100 210 //Normal speed coeff
#define CHASSIS_NORMAL_SPEED_COEFF_120 240 //Normal speed coeff

#define CHASSIS_SPINTOP_RATE_POWER_45 3.0f //Chassis spintop rate
#define CHASSIS_SPINTOP_RATE_POWER_50 1.7f //Chassis spintop rate
#define CHASSIS_SPINTOP_RATE_POWER_55 1.9f //Chassis spintop rate
#define CHASSIS_SPINTOP_RATE_POWER_60 2.1f //Chassis spintop rate
#define CHASSIS_SPINTOP_RATE_POWER_80 2.6f //Chassis spintop rate
#define CHASSIS_SPINTOP_RATE_POWER_100 3.1f //Chassis spintop rate
#define CHASSIS_SPINTOP_RATE_POWER_120 3.6f //Chassis spintop rate

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
		float Spin_Rate;
		float Speed_Coefficient;
	}Chassis_Coord; //Chassis coordinate
	
	struct
	{
		float Vx;
		float Vy;
		float Wz;
	}Gimbal_Coord; //Gimbal coordinate
	
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
