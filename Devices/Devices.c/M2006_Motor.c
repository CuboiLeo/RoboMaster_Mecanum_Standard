/**
 * @file M2006_Motor.c
 * @author Leo Liu
 * @brief M2006 communication
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "M2006_Motor.h"
#include <stdio.h>

Motor_Init_t M2006_Trigger;

void M2006_Trigger_Get_Data(CAN_Export_Data_t RxMessage);
void M2006_Trigger_Send_Data(int16_t Trigger_Current);
void Check_M2006_Trigger(void);

M2006_Func_t M2006_Func = M2006_Func_GroundInit;
#undef M2006_Func_GroundInit

//Obatin trigger motor data from CAN
void M2006_Trigger_Get_Data(CAN_Export_Data_t RxMessage)
{
	M2006_Trigger.Prev_Angle = M2006_Trigger.Actual_Angle;
	M2006_Trigger.Actual_Angle = (int16_t)(RxMessage.CANx_Export_RxMessage[0] << 8 | RxMessage.CANx_Export_RxMessage[1]);
  M2006_Trigger.Actual_Speed = (int16_t)(RxMessage.CANx_Export_RxMessage[2] << 8 | RxMessage.CANx_Export_RxMessage[3]);
	if((M2006_Trigger.Actual_Angle - M2006_Trigger.Prev_Angle) < -6500 )
		M2006_Trigger.Turn_Count++;
	else if((M2006_Trigger.Actual_Angle - M2006_Trigger.Prev_Angle) > 6500)
		M2006_Trigger.Turn_Count--;
	M2006_Trigger.Total_Angle = M2006_Trigger.Actual_Angle + (M2006_MECH_ANGLE_MAX * M2006_Trigger.Turn_Count);
	M2006_Trigger.Info_Update_Frame++;
}

//Send trigger data through specified identifier
void M2006_Trigger_Send_Data(int16_t Trigger_Current)
{
	CAN_Func.CAN_0x1FF_Send_Data(&hcan2,0,Trigger_Current,0,0);
}

void Check_M2006_Trigger(void)
{
	if(M2006_Trigger.Info_Update_Frame < 1)
		M2006_Trigger.Offline_Flag = 1;
	else
		M2006_Trigger.Offline_Flag = 0;
	M2006_Trigger.Info_Update_Frame = 0;
}
