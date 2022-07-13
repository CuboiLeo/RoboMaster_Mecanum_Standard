/**
 * @file State_Machine.c
 * @author Leo Liu
 * @brief control the state of the robot
 * @version 1.0
 * @date 2022-07-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "State_Machine.h"
State_Machine_t State_Machine;

void Remote_Control_Update(void);

State_Machine_Func_t State_Machine_Func = State_Machine_Func_GroundInit;
#undef State_Machine_Func_GroundInit

void Remote_Control_Update(void)
{
	if(DR16_Export_Data.Remote_Control.Left_Switch == SWITCH_DOWN && DR16_Export_Data.Remote_Control.Right_Switch == SWITCH_DOWN)
	{
		Robot_Control_Func.Robot_Control_Disabled();
	}
	
	else if(DR16_Export_Data.Remote_Control.Right_Switch == SWITCH_MID)
	{
		State_Machine.Control_Source = Remote_Control;
		switch(DR16_Export_Data.Remote_Control.Left_Switch)
		{
			case(SWITCH_DOWN):
			{
				Chassis.Current_Mode = Follow_Gimbal;
				Gimbal.Current_Mode = Follow_Gimbal;
				break;
			}
			case(SWITCH_MID):
			{
				Chassis.Current_Mode = Not_Follow_Gimbal;
				Gimbal.Current_Mode = Not_Follow_Gimbal;
				break;
			}
			case(SWITCH_UP):
			{
				Chassis.Current_Mode = Spin_Top;
				Gimbal.Current_Mode = Spin_Top;
				break;
			}
		}
	}
		
	else if(DR16_Export_Data.Remote_Control.Right_Switch == SWITCH_UP)
	{
		State_Machine.Control_Source = Computer;
	}
}
