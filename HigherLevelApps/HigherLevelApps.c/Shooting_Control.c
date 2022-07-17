/**
 * @file Shooting_Control.c
 * @author Leo Liu
 * @brief control shooting
 * @version 1.0
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Shooting_Control.h"

Shooting_t Shooting;

void Turn_Friction_Wheel_On(void);
void Turn_Friction_Wheel_Off(void);
void Trigger_Get_Data(Shooting_t *Shooting);
void Trigger_Processing(Shooting_t *Shooting);
void Shooting_Disabled(void);

Shooting_Func_t Shooting_Func = Shooting_Func_GroundInit;
#undef Shooting_Func_GroundInit

void Turn_Friction_Wheel_On(void)
{
	float Fric_Wheel_Left = -FRIC_SPEED_16;
	float Fric_Wheel_Right = FRIC_SPEED_16;
	
	M3508_Fric_Wheel[0].Output_Current = PID_Func.Positional_PID(&Fric_Wheel_PID, Fric_Wheel_Left, M3508_Fric_Wheel[0].Actual_Speed);
	M3508_Fric_Wheel[1].Output_Current = PID_Func.Positional_PID(&Fric_Wheel_PID, Fric_Wheel_Right, M3508_Fric_Wheel[1].Actual_Speed);
	
	Shooting.Fric_Wheel_On_Flag = 1;
}

void Turn_Friction_Wheel_Off(void)
{
	for(int i = 0; i < 2; i++)
		M3508_Fric_Wheel[i].Output_Current = 0;
	
	Shooting.Fric_Wheel_On_Flag = 0;
}

void Trigger_Get_Data(Shooting_t *Shooting)
{
	if(State_Machine.Control_Source == Remote_Control)
	{
		if(DR16_Export_Data.Remote_Control.Dial_Wheel < 0)
		{
			Shooting->Type.Single_Fire_Flag = 1;
			Shooting->Type.Burst_Flag = 0;
		}
		
		else if(DR16_Export_Data.Remote_Control.Dial_Wheel > 0)
		{
			Shooting->Type.Single_Fire_Flag = 0;
			Shooting->Type.Burst_Flag = 1;
		}
		
		else
		{
			Shooting->Type.Single_Fire_Flag = 0;
			Shooting->Type.Burst_Flag = 0;
			Shooting->Type.Single_Fired_Flag = 0;
		}
	}
	
	else if(State_Machine.Control_Source == Computer)
	{
		if(DR16_Export_Data.Mouse.Left_Click == 1)
		{
			DR16_Export_Data.Mouse.Click_Counter++;
			
			if(DR16_Export_Data.Mouse.Click_Counter < 40)
			{
				Shooting->Type.Single_Fire_Flag = 1;
				Shooting->Type.Burst_Flag = 0;
			}
			
			else
			{
				Shooting->Type.Single_Fire_Flag = 0;
				Shooting->Type.Burst_Flag = 1;
			}
		}
		
		else
		{
			DR16_Export_Data.Mouse.Click_Counter = 0;
			Shooting->Type.Single_Fire_Flag = 0;
			Shooting->Type.Burst_Flag = 0;
		}
	}
}

void Trigger_Processing(Shooting_t *Shooting)
{
	if(Shooting->Fric_Wheel_On_Flag)
	{
		if(Shooting->Type.Single_Fire_Flag && Shooting->Type.Single_Fired_Flag != 1)
		{
			Shooting->Trigger.Target_Angle = M2006_Trigger.Total_Angle + TRIGGER_DIRECTION * M2006_ANGLE_1_BULLET;
			Shooting->Type.Single_Fired_Flag = 1;
		}
		
		if(Shooting->Type.Single_Fire_Flag && Shooting->Type.Single_Fired_Flag)
		{
			Shooting->Trigger.Target_Speed = PID_Func.Positional_PID(&Trigger_Angle_PID, Shooting->Trigger.Target_Angle, M2006_Trigger.Total_Angle);
			M2006_Trigger.Output_Current = PID_Func.Positional_PID(&Trigger_Speed_PID, Shooting->Trigger.Target_Speed, M2006_Trigger.Actual_Speed);
		}
		
		else if(Shooting->Type.Burst_Flag)
		{
			Shooting->Trigger.Target_Speed = TRIGGER_DIRECTION * 5000;
			M2006_Trigger.Output_Current = PID_Func.Positional_PID(&Trigger_Speed_PID, Shooting->Trigger.Target_Speed, M2006_Trigger.Actual_Speed);
		}
	}
}

void Shooting_Disabled(void)
{
	Shooting_Func.Turn_Friction_Wheel_Off();
	DR16_Export_Data.Mouse.Click_Counter = 0;
	Shooting.Type.Single_Fire_Flag = 0;
	Shooting.Type.Burst_Flag = 0;
	M2006_Trigger.Output_Current = 0;
}
