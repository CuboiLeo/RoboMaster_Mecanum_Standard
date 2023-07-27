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
		Gimbal->Current_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
		Gimbal->Target_Yaw -= DR16_Export_Data.Remote_Control.Joystick_Right_Vx / 1000.0f ;
		
		Gimbal->Current_Pitch = PITCH_DIRECTION * Board_A_IMU.Export_Data.Pitch;
		Gimbal->Target_Pitch += DR16_Export_Data.Remote_Control.Joystick_Right_Vy / 2200.0f;
		Gimbal->Target_Pitch = VAL_LIMIT(Gimbal->Target_Pitch,PITCH_UPPER_LIMIT,PITCH_LOWER_LIMIT);
	}
	
	else if(State_Machine.Control_Source == Computer)
	{
		Gimbal->Current_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
		Gimbal->Target_Yaw -= (float)DR16_Export_Data.Mouse.x * 0.01f;
	
		Gimbal->Current_Pitch = PITCH_DIRECTION * Board_A_IMU.Export_Data.Pitch;
		Gimbal->Target_Pitch -= (float)DR16_Export_Data.Mouse.y / 400.0f;
		Gimbal->Target_Pitch = VAL_LIMIT(Gimbal->Target_Pitch,PITCH_UPPER_LIMIT,PITCH_LOWER_LIMIT);
	}
}

void Gimbal_Processing(Gimbal_t *Gimbal)
{
	switch(Gimbal->Current_Mode)
	{
		case(Follow_Gimbal):
		{
			GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Angle_PID,Gimbal->Target_Yaw,Gimbal->Current_Yaw);
							
			Gimbal->Pitch_Angle_Output = PID_Func.Positional_PID(&Pitch_Angle_PID,Gimbal->Target_Pitch,-Gimbal->Current_Pitch);
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Speed_PID,Gimbal->Pitch_Angle_Output,-Board_A_IMU.Export_Data.Gyro_Pitch);
			
			break;
		}
		
		case(Not_Follow_Gimbal):
		{
			if(Shooting.Type.Auto_Aiming)
			{
				AutoAim_Yaw_Angle_PID.Kp = 500;
				AutoAim_Yaw_Angle_PID.Kd = 50000;
				
				AutoAim_Pitch_Angle_PID.Kp = 50.0f;
				AutoAim_Pitch_Angle_PID.Kd = 0;
				
				AutoAim_Pitch_Speed_PID.Kp = 9.1f;
				AutoAim_Pitch_Speed_PID.Ki = 0.09f;
				AutoAim_Pitch_Speed_PID.I_Out_Max = 9000.0f;
				
				if(Tx2_Data.Receiving.Auto_Aiming.Yaw != 0)
				{
					GM6020_Yaw.Output_Current = -PID_Func.Positional_PID(&AutoAim_Yaw_Angle_PID,Tx2_Data.Receiving.Auto_Aiming.Yaw,0);
				}
				else
				{
					Gimbal->Target_Yaw -= DR16_Export_Data.Remote_Control.Joystick_Right_Vx / 1000.0f ;
					GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Angle_PID,Gimbal->Target_Yaw,Gimbal->Current_Yaw);
				}
				
				if(Tx2_Data.Receiving.Auto_Aiming.Pitch != 0)	
				{	
					Gimbal->Pitch_Angle_Output = PID_Func.Positional_PID(&AutoAim_Pitch_Angle_PID,Tx2_Data.Receiving.Auto_Aiming.Pitch,0);	
					GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&AutoAim_Pitch_Speed_PID,Gimbal->Pitch_Angle_Output,-100.0f* Board_A_IMU.Export_Data.Gyro_Pitch); //GM6020_Pitch.Output_Current * 0.9f + 0.1f * (PID_Func.Positional_PID_Min_Error(&AutoAim_Pitch_Speed_PID,Gimbal->Pitch_Angle_Output,-Board_A_IMU.Export_Data.Gyro_Pitch, 0.0f));	
				}
				else
				{
					Gimbal->Target_Pitch += DR16_Export_Data.Remote_Control.Joystick_Right_Vy / 2200.0f;
					Gimbal->Target_Pitch = VAL_LIMIT(Gimbal->Target_Pitch,PITCH_UPPER_LIMIT,PITCH_LOWER_LIMIT);
					Gimbal->Pitch_Angle_Output = PID_Func.Positional_PID(&Pitch_Angle_PID,Gimbal->Target_Pitch,-Gimbal->Current_Pitch);
					GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&AutoAim_Pitch_Speed_PID,Gimbal->Pitch_Angle_Output,-Board_A_IMU.Export_Data.Gyro_Pitch);
				}
			}
			//Nothing special, just positional pid angle control
			else
			{
				GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Angle_PID,Gimbal->Target_Yaw,Gimbal->Current_Yaw);
				
				Gimbal->Pitch_Angle_Output = PID_Func.Positional_PID(&Pitch_Angle_PID,Gimbal->Target_Pitch,-Gimbal->Current_Pitch);
				GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Speed_PID,Gimbal->Pitch_Angle_Output,-Board_A_IMU.Export_Data.Gyro_Pitch);
			}
			break;
		}
		
		case(Spin_Top):
		{
			GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Angle_PID,Gimbal->Target_Yaw,Gimbal->Current_Yaw);
							
			Gimbal->Pitch_Angle_Output = PID_Func.Positional_PID(&Pitch_Angle_PID,Gimbal->Target_Pitch,-Gimbal->Current_Pitch);
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Speed_PID,Gimbal->Pitch_Angle_Output,-Board_A_IMU.Export_Data.Gyro_Pitch);
			break;
		}
		
		case(Disabled):
		{
			Gimbal->Target_Yaw = YAW_DIRECTION * Board_A_IMU.Export_Data.Total_Yaw;
			GM6020_Yaw.Output_Current = 0;
			GM6020_Pitch.Output_Current = 0;
			PID_Func.Clear_PID_Data(&Yaw_Angle_PID);
			PID_Func.Clear_PID_Data(&Pitch_Angle_PID);

			break;
		}
	}
}
