/**
 * @file Gimbal_Control.c
 * @author Leo Liu
 * @brief control gimbal
 * @version 1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
#include "Gimbal_Control.h"

void Gimbal_Init(void);
void Gimbal_Control_Get_Data(Gimbal_t *Gimbal);
void Gimbal_Processing(Gimbal_t *Gimbal);

Gimbal_Func_t Gimbal_Func = Gimbal_Func_GroundInit;
#undef Gimbal_Func_GroundInit

Gimbal_t Gimbal;

void Gimbal_Init(void)
{
	//Set the origin for yaw and pitch
	GM6020_Yaw.Target_Angle = YAW_MID_MECH_ANGLE;
	GM6020_Pitch.Target_Angle = PITCH_MID_MECH_ANGLE;
}

void Gimbal_Control_Get_Data(Gimbal_t *Gimbal)
{
	//The multiplying/dividing constant are tested value and can be changed
	if(State_Machine.Control_Source == Remote_Control)
	{
		GM6020_Yaw.Target_Angle -= DR16_Export_Data.Remote_Control.Joystick_Right_Vx / 150.0f;
		GM6020_Pitch.Target_Angle += DR16_Export_Data.Remote_Control.Joystick_Right_Vy / 300.0f;
		GM6020_Pitch.Target_Angle = VAL_LIMIT(GM6020_Pitch.Target_Angle,PITCH_UPPER_LIMIT,PITCH_LOWER_LIMIT);
	}
	
	else if(State_Machine.Control_Source == Computer)
	{
		GM6020_Yaw.Target_Angle += (float)DR16_Export_Data.Mouse.x * 3.0f;
		GM6020_Pitch.Target_Angle -= (float)DR16_Export_Data.Mouse.y * 0.10f;
		GM6020_Pitch.Target_Angle = VAL_LIMIT(GM6020_Pitch.Target_Angle,PITCH_UPPER_LIMIT,PITCH_LOWER_LIMIT);
	}
}

void Gimbal_Processing(Gimbal_t *Gimbal)
{
	switch(Gimbal->Current_Mode)
	{
		case(Follow_Gimbal):
		{
			//Reset the target angle so gimbal doesn't spin like crazy
			if(Gimbal->Prev_Mode != Follow_Gimbal)
				GM6020_Yaw.Target_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
			
			//If remote control is controlling then gimbal keeps moving accordingly
			if(DR16_Export_Data.Remote_Control.Joystick_Right_Vx != 0)
			{
				Gimbal->Temp.Yaw_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
				Gimbal->Temp.Yaw_Speed = PID_Func.Positional_PID(&Yaw_Angle_PID,GM6020_Yaw.Target_Angle,Gimbal->Temp.Yaw_Angle);
				GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Speed_PID,Gimbal->Temp.Yaw_Speed,-GM6020_Yaw.Actual_Speed);
			}
			
			//If remote control is not controlling then gimbal goes in reverse direction to meet up with chassis
			else
			{
				Gimbal->Temp.Yaw_Speed = PID_Func.Positional_PID(&Yaw_Angle_PID,YAW_MID_MECH_ANGLE,Find_Gimbal_Min_Angle(GM6020_Yaw.Actual_Angle));
				GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Speed_PID,Gimbal->Temp.Yaw_Speed,-GM6020_Yaw.Actual_Speed);
				GM6020_Yaw.Target_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
			}
			
			Gimbal->Temp.Pitch_Angle = PITCH_DIRECTION * (GM6020_Pitch.Actual_Angle);
			Gimbal->Temp.Pitch_Speed = PID_Func.Positional_PID(&Pitch_Angle_PID,GM6020_Pitch.Target_Angle,Gimbal->Temp.Pitch_Angle);
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Speed_PID,Gimbal->Temp.Pitch_Speed,GM6020_Pitch.Actual_Speed);
			
			Gimbal->Prev_Mode = Follow_Gimbal;
			
			break;
		}
		
		case(Not_Follow_Gimbal):
		{
			//Reset the target angle so gimbal doesn't spin like crazy
			if(Gimbal->Prev_Mode != Not_Follow_Gimbal)
				GM6020_Yaw.Target_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
			
			//Nothing special, just positional pid angle control
			Gimbal->Temp.Yaw_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
			Gimbal->Temp.Yaw_Speed = PID_Func.Positional_PID(&Yaw_Angle_PID,GM6020_Yaw.Target_Angle,Gimbal->Temp.Yaw_Angle);
			GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Speed_PID,Gimbal->Temp.Yaw_Speed,-GM6020_Yaw.Actual_Speed);
			
			Gimbal->Temp.Pitch_Angle = PITCH_DIRECTION * (GM6020_Pitch.Actual_Angle);
			Gimbal->Temp.Pitch_Speed = PID_Func.Positional_PID(&Pitch_Angle_PID,GM6020_Pitch.Target_Angle,Gimbal->Temp.Pitch_Angle);
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Speed_PID,Gimbal->Temp.Pitch_Speed,GM6020_Pitch.Actual_Speed);
			
			Gimbal->Prev_Mode = Not_Follow_Gimbal;
			
			break;
		}
		
		case(Spin_Top):
		{
			//Reset the target angle so gimbal doesn't spin like crazy
			if(Gimbal->Prev_Mode != Spin_Top)
				GM6020_Yaw.Target_Angle = (int32_t)(YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw / GM6020_ANGLE_CONVERT + YAW_MID_MECH_ANGLE);
			
			//Gimbal is held in position through IMU data instead of yaw encoder
			Gimbal->Temp.Yaw_Angle = (int32_t)(YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw / GM6020_ANGLE_CONVERT + YAW_MID_MECH_ANGLE);
			Gimbal->Temp.Yaw_Speed = PID_Func.Positional_PID(&Yaw_Angle_PID,GM6020_Yaw.Target_Angle,Gimbal->Temp.Yaw_Angle);
			GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Speed_PID,Gimbal->Temp.Yaw_Speed,-GM6020_Yaw.Actual_Speed);
			
			Gimbal->Temp.Pitch_Angle = PITCH_DIRECTION * (GM6020_Pitch.Actual_Angle);
			Gimbal->Temp.Pitch_Speed = PID_Func.Positional_PID(&Pitch_Angle_PID,GM6020_Pitch.Target_Angle,Gimbal->Temp.Pitch_Angle);
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Speed_PID,Gimbal->Temp.Pitch_Speed,GM6020_Pitch.Actual_Speed);
			
			Gimbal->Prev_Mode = Spin_Top;
			
			break;
		}
		
		case(Disabled):
		{
			GM6020_Yaw.Target_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
			GM6020_Yaw.Output_Current = 0;
			GM6020_Pitch.Output_Current = 0;
			PID_Func.Clear_PID_Data(&Yaw_Angle_PID);
			PID_Func.Clear_PID_Data(&Yaw_Speed_PID);
			PID_Func.Clear_PID_Data(&Pitch_Angle_PID);
			PID_Func.Clear_PID_Data(&Pitch_Speed_PID);
			
			Gimbal->Prev_Mode = Disabled;
			
			break;
		}
	}
}
