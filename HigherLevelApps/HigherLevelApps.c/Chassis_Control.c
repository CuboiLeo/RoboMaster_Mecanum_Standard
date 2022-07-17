/**
 * @file Chassis_Control.c
 * @author Leo Liu
 * @brief control chassis
 * @version 1.0
 * @date 2022-07-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Chassis_Control.h"
Chassis_t Chassis;
 
void Chassis_Speed_Get_Data(Chassis_t *Chassis);
void Inverse_Kinematic_Calc(Chassis_t *Chassis);
void Chassis_Processing(Chassis_t *Chassis);

Chassis_Func_t Chassis_Func = Chassis_Func_GroundInit;
#undef Chassis_Func_GroundInit

void Chassis_Speed_Get_Data(Chassis_t *Chassis)
{
	if(State_Machine.Control_Source == Remote_Control)
	{
		Chassis->Gimbal_Coord.Vx = DR16_Export_Data.Remote_Control.Joystick_Left_Vx/300;
		Chassis->Gimbal_Coord.Vy = DR16_Export_Data.Remote_Control.Joystick_Left_Vy/300;
	}
	else if(State_Machine.Control_Source == Computer)
	{
		Chassis->Gimbal_Coord.Vx = (DR16_Export_Data.Keyboard.Press_W.Hold_Flag - DR16_Export_Data.Keyboard.Press_S.Hold_Flag) * 2.2f;
		Chassis->Gimbal_Coord.Vy = (DR16_Export_Data.Keyboard.Press_A.Hold_Flag - DR16_Export_Data.Keyboard.Press_D.Hold_Flag) * 2.2f;
	}
}

void Inverse_Kinematic_Calc(Chassis_t *Chassis)
{
	float speed_ratio = 1;
	float temp_speed_max = 0;

	Chassis->Wheel_Speed[0] = (Chassis->Chassis_Coord.Vx - Chassis->Chassis_Coord.Vy + Chassis->Chassis_Coord.Wz 
														* (CHASSIS_LENGTH_HALF + CHASSIS_WIDTH_HALF)) * MOTOR_GEAR_RATIO;	//Rear Right
	Chassis->Wheel_Speed[1] = (Chassis->Chassis_Coord.Vx + Chassis->Chassis_Coord.Vy + Chassis->Chassis_Coord.Wz 
														* (CHASSIS_LENGTH_HALF + CHASSIS_WIDTH_HALF)) * MOTOR_GEAR_RATIO;	//Front Right
	Chassis->Wheel_Speed[2] = (-Chassis->Chassis_Coord.Vx + Chassis->Chassis_Coord.Vy + Chassis->Chassis_Coord.Wz 
														* (CHASSIS_LENGTH_HALF + CHASSIS_WIDTH_HALF)) * MOTOR_GEAR_RATIO;	//Front Left
	Chassis->Wheel_Speed[3] = (-Chassis->Chassis_Coord.Vx - Chassis->Chassis_Coord.Vy + Chassis->Chassis_Coord.Wz 
														* (CHASSIS_LENGTH_HALF + CHASSIS_WIDTH_HALF)) * MOTOR_GEAR_RATIO;	//Rear Left
	
	for(int i = 0; i < 4; i++)
	{
		Chassis->Wheel_Speed[i] *= 100;
		if(fabs(Chassis->Wheel_Speed[i]) > temp_speed_max)
			temp_speed_max = fabs(Chassis->Wheel_Speed[i]);
		if(temp_speed_max > M3508_SPEED_MAX)
			speed_ratio = M3508_SPEED_MAX / temp_speed_max;
		
		Chassis->Wheel_Speed[i] *= speed_ratio;
		M3508_Chassis[i].Target_Speed = Chassis->Wheel_Speed[i];
		M3508_Chassis[i].Output_Current = PID_Func.Positional_PID(&Chassis_Speed_PID,M3508_Chassis[i].Target_Speed,M3508_Chassis[i].Actual_Speed);
	}
}

void Chassis_Processing(Chassis_t *Chassis)
{
	switch(Chassis->Current_Mode)
	{
		case(Follow_Gimbal):
		{
			float angle_diff = DEG_TO_RAD((GM6020_Yaw.Actual_Angle - YAW_MID_MECH_ANGLE) * GM6020_ANGLE_CONVERT);
			
			Chassis->Chassis_Coord.Vx = Chassis->Gimbal_Coord.Vx;
			Chassis->Chassis_Coord.Vy = Chassis->Gimbal_Coord.Vy;
			Chassis->Chassis_Coord.Wz = PID_Func.Positional_PID(&Chassis_Angle_PID,0,angle_diff);
			
			break;
		}	
		
		case(Not_Follow_Gimbal):
		{
			float angle_diff = DEG_TO_RAD((GM6020_Yaw.Actual_Angle - YAW_MID_MECH_ANGLE) * GM6020_ANGLE_CONVERT);
			
			Chassis->Chassis_Coord.Vx = Chassis->Gimbal_Coord.Vx * cos(angle_diff) - Chassis->Gimbal_Coord.Vy * sin(angle_diff);
			Chassis->Chassis_Coord.Vy = Chassis->Gimbal_Coord.Vx * sin(angle_diff) + Chassis->Gimbal_Coord.Vy * cos(angle_diff);
			Chassis->Chassis_Coord.Wz = 0;
			
			break;
		}
		
		case(Spin_Top):
		{
			float angle_diff = DEG_TO_RAD((GM6020_Yaw.Actual_Angle - YAW_MID_MECH_ANGLE) * GM6020_ANGLE_CONVERT);
			
			Chassis->Chassis_Coord.Vx = Chassis->Gimbal_Coord.Vx * cos(angle_diff) - Chassis->Gimbal_Coord.Vy * sin(angle_diff);
			Chassis->Chassis_Coord.Vy = Chassis->Gimbal_Coord.Vx * sin(angle_diff) + Chassis->Gimbal_Coord.Vy * cos(angle_diff);
			Chassis->Chassis_Coord.Wz = 5;
			
			break;
		}
		
		case(Disabled):
		{
			Chassis->Chassis_Coord.Vx = 0;
			Chassis->Chassis_Coord.Vy = 0;
			Chassis->Chassis_Coord.Wz = 0;
			
			PID_Func.Clear_PID_Data(&Chassis_Angle_PID);
			PID_Func.Clear_PID_Data(&Chassis_Speed_PID);
			break;
		}
	}
	Chassis_Func.Inverse_Kinematic_Calc(Chassis);
}
