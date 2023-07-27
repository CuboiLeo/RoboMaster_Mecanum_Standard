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
Ramp_Calc_t Chassis_Ramp[4];
 
void Chassis_Speed_Get_Data(Chassis_t *Chassis);
void Inverse_Kinematic_Calc(Chassis_t *Chassis);
void Chassis_Processing(Chassis_t *Chassis);

Chassis_Func_t Chassis_Func = Chassis_Func_GroundInit;
#undef Chassis_Func_GroundInit

void Chassis_Speed_Get_Data(Chassis_t *Chassis)
{
	if(State_Machine.Control_Source == Remote_Control)
	{
		Chassis->Gimbal_Coord.Vx = DR16_Export_Data.Remote_Control.Joystick_Left_Vx / 660.0f;
		Chassis->Gimbal_Coord.Vy = DR16_Export_Data.Remote_Control.Joystick_Left_Vy / 660.0f;
	}
	else if(State_Machine.Control_Source == Computer)
	{
		Chassis->Gimbal_Coord.Vx = (DR16_Export_Data.Keyboard.Press_D.Hold_Flag - DR16_Export_Data.Keyboard.Press_A.Hold_Flag);
		Chassis->Gimbal_Coord.Vy = (DR16_Export_Data.Keyboard.Press_W.Hold_Flag - DR16_Export_Data.Keyboard.Press_S.Hold_Flag);
	}
}

void Inverse_Kinematic_Calc(Chassis_t *Chassis)
{
	float speed_ratio = 1;	//For uniformly reduce four wheel speeds if any of the speed goes beyond max speed
	float temp_speed_max = 0;	//Record down the max speed through for loop

	//These are the formulas for reverse kinematic calculation of the four wheels
	Chassis->Wheel_Speed[0] = (Chassis->Chassis_Coord.Vx - Chassis->Chassis_Coord.Vy + Chassis->Chassis_Coord.Wz 
														* (CHASSIS_LENGTH_HALF + CHASSIS_WIDTH_HALF)) * MOTOR_GEAR_RATIO;	//Rear Right
	Chassis->Wheel_Speed[1] = (Chassis->Chassis_Coord.Vx + Chassis->Chassis_Coord.Vy + Chassis->Chassis_Coord.Wz 
														* (CHASSIS_LENGTH_HALF + CHASSIS_WIDTH_HALF)) * MOTOR_GEAR_RATIO;	//Front Right
	Chassis->Wheel_Speed[2] = (-Chassis->Chassis_Coord.Vx + Chassis->Chassis_Coord.Vy + Chassis->Chassis_Coord.Wz 
														* (CHASSIS_LENGTH_HALF + CHASSIS_WIDTH_HALF)) * MOTOR_GEAR_RATIO;	//Front Left
	Chassis->Wheel_Speed[3] = (-Chassis->Chassis_Coord.Vx - Chassis->Chassis_Coord.Vy + Chassis->Chassis_Coord.Wz 
														* (CHASSIS_LENGTH_HALF + CHASSIS_WIDTH_HALF)) * MOTOR_GEAR_RATIO;	//Rear Left
	
	//First calculate the speed ratio for all the wheels
	for(int i = 0; i < 4; i++)
	{
		Chassis->Wheel_Speed[i] *= Chassis->Chassis_Coord.Speed_Coefficient;
		if(Super_Capacitor.Super_Cap_On)
			Chassis->Wheel_Speed[i] *= (1 + Super_Capacitor.Super_Cap_Accel_Rate);
		if(fabs(Chassis->Wheel_Speed[i]) > temp_speed_max)
			temp_speed_max = fabs(Chassis->Wheel_Speed[i]);
		if(temp_speed_max > M3508_SPEED_MAX)
			speed_ratio = M3508_SPEED_MAX / temp_speed_max;
	}
	
	//Proportionally reduce the speed
	for(int i = 0; i < 4; i++)
	{
		Chassis->Wheel_Speed[i] *= speed_ratio;
		M3508_Chassis[i].Target_Speed = Ramp_Calc_Func.Ramp(&Chassis_Ramp[i], CHASSIS_RAMP_RATE, Chassis->Wheel_Speed[i]);
		M3508_Chassis[i].Output_Current = PID_Func.Positional_PID(&Chassis_Speed_PID,M3508_Chassis[i].Target_Speed,M3508_Chassis[i].Actual_Speed);
		M3508_Chassis[i].Output_Current = VAL_LIMIT(M3508_Chassis[i].Output_Current, M3508_OUTPUT_MAX, (-M3508_OUTPUT_MAX));
	}
}

void Chassis_Processing(Chassis_t *Chassis)
{
	Gimbal.Angle_Difference = DEG_TO_RAD(Find_Gimbal_Min_Angle(GM6020_Yaw.Actual_Angle - YAW_MID_MECH_ANGLE) * GM6020_ANGLE_CONVERT);
	
	//Calculate different variables based on current mode
	switch(Chassis->Current_Mode)
	{
		case(Follow_Gimbal):
		{
			//Chassis always approaches the yaw motor mid mechanical angle
			//And because chassis and gimbal coordinates are alligned, they are the same
			Chassis->Chassis_Coord.Vx = Chassis->Gimbal_Coord.Vx;
			Chassis->Chassis_Coord.Vy = Chassis->Gimbal_Coord.Vy;
			Chassis->Chassis_Coord.Wz = PID_Func.Positional_PID(&Chassis_Angle_PID,0,Gimbal.Angle_Difference);
			if(fabs(Chassis->Chassis_Coord.Wz) < 1.5f)
				Chassis->Chassis_Coord.Wz = 0;
			
			break;
		}	
		
		case(Not_Follow_Gimbal):
		{
			//The gimbal coordinate is converted to chassis coordinate through trigonometry	
			Chassis->Chassis_Coord.Vx = Chassis->Gimbal_Coord.Vx * cos(Gimbal.Angle_Difference) - Chassis->Gimbal_Coord.Vy * sin(Gimbal.Angle_Difference);
			Chassis->Chassis_Coord.Vy = Chassis->Gimbal_Coord.Vx * sin(Gimbal.Angle_Difference) + Chassis->Gimbal_Coord.Vy * cos(Gimbal.Angle_Difference);
			Chassis->Chassis_Coord.Wz = 0;
			
			break;
		}
		
		case(Spin_Top):
		{
			//The gimbal coordinate is converted to chassis coordinate through trigonometry
			Chassis->Chassis_Coord.Vx = Chassis->Gimbal_Coord.Vx * cos(Gimbal.Angle_Difference) - Chassis->Gimbal_Coord.Vy * sin(Gimbal.Angle_Difference);
			Chassis->Chassis_Coord.Vy = Chassis->Gimbal_Coord.Vx * sin(Gimbal.Angle_Difference) + Chassis->Gimbal_Coord.Vy * cos(Gimbal.Angle_Difference);
			Chassis->Chassis_Coord.Wz = Chassis->Chassis_Coord.Spin_Rate; //This is where you control how fast the spintop spins
			
			break;
		}
		
		case(Disabled):
		{
			//Disabled so everything is zero
			Chassis->Chassis_Coord.Vx = 0;
			Chassis->Chassis_Coord.Vy = 0;
			Chassis->Chassis_Coord.Wz = 0;
			
			for(int i = 0; i < 4; i++)
				Ramp_Calc_Func.Clear_Ramp(&Chassis_Ramp[i]);
			
			PID_Func.Clear_PID_Data(&Chassis_Angle_PID);
			PID_Func.Clear_PID_Data(&Chassis_Speed_PID);
			break;
		}
	}
	
	Chassis_Func.Inverse_Kinematic_Calc(Chassis);
}
