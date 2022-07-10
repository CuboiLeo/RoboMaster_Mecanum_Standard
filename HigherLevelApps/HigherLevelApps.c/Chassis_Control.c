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
	Chassis->Speed.Vx = DR16_Export_Data.Remote_Control.Joystick_Left_Vx;
	Chassis->Speed.Vy = DR16_Export_Data.Remote_Control.Joystick_Left_Vy;
	Chassis->Speed.Wz = DR16_Export_Data.Remote_Control.Dial_Wheel;
}

void Inverse_Kinematic_Calc(Chassis_t *Chassis)
{
	float speed_ratio = 1;
	float temp_speed_max = 0;
	float angle_diff = 0;//DEG_TO_RAD((GM6020_Yaw.Total_Angle - 5924) * GM6020_ANGLE_CONVERT);
	float temp_Vx = Chassis->Speed.Vx;
	float temp_Vy = Chassis->Speed.Vy;
	
	Chassis->Speed.Vx = temp_Vx * cos(angle_diff) - temp_Vy * sin(angle_diff);
	Chassis->Speed.Vy = temp_Vy * cos(angle_diff) + temp_Vx * sin(angle_diff);
	
	Chassis->Temp_Speed[0] = Chassis->Speed.Vx - Chassis->Speed.Vy + Chassis->Speed.Wz;	//Rear Right
	Chassis->Temp_Speed[1] = Chassis->Speed.Vx + Chassis->Speed.Vy + Chassis->Speed.Wz;	//Front Right
	Chassis->Temp_Speed[2] = -Chassis->Speed.Vx + Chassis->Speed.Vy + Chassis->Speed.Wz;	//Front Left
	Chassis->Temp_Speed[3] = -Chassis->Speed.Vx - Chassis->Speed.Vy + Chassis->Speed.Wz;	//Rear Left
	
	for(int i = 0; i < 4; i++)
	{
		if(fabs(Chassis->Temp_Speed[i]) > temp_speed_max)
			temp_speed_max = fabs(Chassis->Temp_Speed[i]);
		Chassis->Temp_Speed[i]*=10;
	}
	
	if(temp_speed_max > M3508_SPEED_MAX)
		speed_ratio = M3508_SPEED_MAX / temp_speed_max;
	
	for(int i = 0; i < 4; i++)
		Chassis->Temp_Speed[i] = Chassis->Temp_Speed[i] * speed_ratio;
}

void Chassis_Processing(Chassis_t *Chassis)
{
	Chassis->Current_Mode = Follow_Gimbal;
	switch(Chassis->Current_Mode)
	{
		case(Follow_Gimbal):
		{
			Chassis_Func.Inverse_Kinematic_Calc(Chassis);
			break;
		}
		case(Not_Follow_Gimbal):
		{
			Chassis_Func.Inverse_Kinematic_Calc(Chassis);
			break;
		}
		case(Spin_Top):
		{
			Chassis_Func.Inverse_Kinematic_Calc(Chassis);
			break;
		}
		case(Disabled):
		{
			for(int i = 0; i < 4; i++)
			{
				M3508_Chassis[i].Output_Current = 0;
				PID_Func.Clear_PID_Data(&Chassis_PID);
			}
			break;
		}
	}
	
	for(int i = 0; i < 4; i++)
	{
		M3508_Chassis[i].Target_Speed = Chassis->Temp_Speed[i];
		M3508_Chassis[i].Output_Current = PID_Func.Positional_PID(&Chassis_PID,M3508_Chassis[i].Target_Speed,M3508_Chassis[i].Actual_Speed);
	}
}
