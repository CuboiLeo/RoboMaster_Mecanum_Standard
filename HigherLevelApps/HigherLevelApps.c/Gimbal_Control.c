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
	Gimbal.Target_Yaw = Board_A_IMU.Export_Data.Total_Yaw;
	Gimbal.Target_Pitch = PITCH_MID_MECH_ANGLE;
}

void Gimbal_Control_Get_Data(Gimbal_t *Gimbal)
{
	//The multiplying/dividing constant are tested value and can be changed
	if(State_Machine.Control_Source == Remote_Control)
	{
		Gimbal->Target_Yaw -= DR16_Export_Data.Remote_Control.Joystick_Right_Vx / 1000.0f ;
		Gimbal->Target_Pitch += DR16_Export_Data.Remote_Control.Joystick_Right_Vy / 220.0f;
		Gimbal->Target_Pitch = VAL_LIMIT(Gimbal->Target_Pitch,PITCH_UPPER_LIMIT,PITCH_LOWER_LIMIT);
	}
	
	else if(State_Machine.Control_Source == Computer)
	{
		Gimbal->Target_Yaw -= (float)DR16_Export_Data.Mouse.x / 60.0f;
		Gimbal->Target_Pitch -= (float)DR16_Export_Data.Mouse.y / 5.0f;
		Gimbal->Target_Pitch = VAL_LIMIT(Gimbal->Target_Pitch,PITCH_UPPER_LIMIT,PITCH_LOWER_LIMIT);
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
			{
				Gimbal->Target_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
			}
			Gimbal->Current_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
			Gimbal->Yaw_Error = Gimbal->Target_Yaw - Gimbal->Current_Yaw;
			GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Angle_PID,Gimbal->Target_Yaw,Gimbal->Current_Yaw);
							
			Gimbal->Current_Pitch = PITCH_DIRECTION * GM6020_Pitch.Actual_Angle;
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Angle_PID,Gimbal->Target_Pitch,Gimbal->Current_Pitch);

			Gimbal->Prev_Mode = Follow_Gimbal;
			
			break;
		}
		
		case(Not_Follow_Gimbal):
		{
			//Reset the target angle so gimbal doesn't spin like crazy
			if(Gimbal->Prev_Mode != Not_Follow_Gimbal)
				Gimbal->Target_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
			if(Shooting.Type.Auto_Aiming)
			{
				Gimbal->Target_Yaw = Tx2_Data.Receiving.Auto_Aiming.Yaw; 
				GM6020_Yaw.Output_Current = -PID_Func.Positional_PID(&AutoAim_Yaw_PID,Gimbal->Target_Yaw,0);
				
				Gimbal->Target_Pitch = Tx2_Data.Receiving.Auto_Aiming.Pitch; 
				Gimbal->Pitch_Angle_Output = PID_Func.Positional_PID(&AutoAim_Pitch_Angle_PID,Gimbal->Target_Pitch,0);
				
				if((GM6020_Pitch.Actual_Angle > PITCH_LOWER_LIMIT && GM6020_Pitch.Actual_Angle < PITCH_UPPER_LIMIT) || \
					 (GM6020_Pitch.Actual_Angle < PITCH_LOWER_LIMIT && Gimbal->Target_Pitch > 0) || \
					 (GM6020_Pitch.Actual_Angle > PITCH_UPPER_LIMIT && Gimbal->Target_Pitch < 0))
					GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&AutoAim_Pitch_Speed_PID,Gimbal->Pitch_Angle_Output,GM6020_Pitch.Actual_Speed);
				else
					GM6020_Pitch.Output_Current = 0;
			}
			//Nothing special, just positional pid angle control
			else
			{
				Gimbal->Current_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
				GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Angle_PID,Gimbal->Target_Yaw,Gimbal->Current_Yaw);
				
				Gimbal->Current_Pitch = PITCH_DIRECTION * GM6020_Pitch.Actual_Angle;
				GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Angle_PID,Gimbal->Target_Pitch,Gimbal->Current_Pitch);
			}
			
			Gimbal->Prev_Mode = Not_Follow_Gimbal;
			
			break;
		}
		
		case(Spin_Top):
		{
			//Reset the target angle so gimbal doesn't spin like crazy
			if(Gimbal->Prev_Mode != Spin_Top)
				Gimbal->Target_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
			
			//Gimbal is held in position through IMU data instead of yaw encoder
			Gimbal->Current_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
			GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Angle_PID,Gimbal->Target_Yaw,Gimbal->Current_Yaw);
			
			Gimbal->Current_Pitch = PITCH_DIRECTION * (GM6020_Pitch.Actual_Angle);
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Angle_PID,Gimbal->Target_Pitch,Gimbal->Current_Pitch);

			Gimbal->Prev_Mode = Spin_Top;
			
			break;
		}
		
		case(Disabled):
		{
			Gimbal->Target_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
			GM6020_Yaw.Output_Current = 0;
			GM6020_Pitch.Output_Current = 0;
			PID_Func.Clear_PID_Data(&Yaw_Angle_PID);
			PID_Func.Clear_PID_Data(&Pitch_Angle_PID);
			
			Gimbal->Prev_Mode = Disabled;
			
			break;
		}
	}
}
