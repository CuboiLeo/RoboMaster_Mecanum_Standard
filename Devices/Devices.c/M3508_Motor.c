/**
 * @file M3508_Motor.c
 * @author Leo Liu
 * @brief M3508 communication
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "M3508_Motor.h"
#include "Motor_Init.h"
#include <stdio.h>

Motor_Init_t M3508_Chassis[4];
Motor_Init_t M3508_Fric_Wheel[2];

void M3508_Chassis_Get_Data(CAN_Export_Data_t RxMessage);
void M3508_Fric_Wheel_Get_Data(CAN_Export_Data_t RxMessage);
void Check_M3508_Chassis(void);
void Check_M3508_Fric_Wheel(void);

M3508_Func_t M3508_Func = M3508_Func_GroundInit;
#undef M3508_Func_GroundInit

void M3508_Chassis_Get_Data(CAN_Export_Data_t RxMessage)
{
	uint8_t ID;
  ID = (uint8_t)(RxMessage.CAN_RxHeader.StdId - M3508_CHASSIS_START_ID);
		
  M3508_Chassis[ID].Actual_Angle = (int16_t)(RxMessage.CANx_Export_RxMessage[0] << 8 | RxMessage.CANx_Export_RxMessage[1]);
  M3508_Chassis[ID].Actual_Speed = (int16_t)(RxMessage.CANx_Export_RxMessage[2] << 8 | RxMessage.CANx_Export_RxMessage[3]);
  M3508_Chassis[ID].Actual_Current = (int16_t)(RxMessage.CANx_Export_RxMessage[4] << 8 | RxMessage.CANx_Export_RxMessage[5]);
  M3508_Chassis[ID].Temperature = RxMessage.CANx_Export_RxMessage[6];

  M3508_Chassis[ID].Info_Update_Frame++;
}

void M3508_Fric_Wheel_Get_Data(CAN_Export_Data_t RxMessage)
{
	switch(RxMessage.CAN_RxHeader.StdId)
	{
		case M3508_FRIC_WHEEL_LEFT_ID:
			M3508_Fric_Wheel[0].Actual_Angle = (int16_t)(RxMessage.CANx_Export_RxMessage[0] << 8 | RxMessage.CANx_Export_RxMessage[1]);
			M3508_Fric_Wheel[0].Actual_Speed = (int16_t)(RxMessage.CANx_Export_RxMessage[2] << 8 | RxMessage.CANx_Export_RxMessage[3]);
			M3508_Fric_Wheel[0].Actual_Current = (int16_t)(RxMessage.CANx_Export_RxMessage[4] << 8 | RxMessage.CANx_Export_RxMessage[5]);
			M3508_Fric_Wheel[0].Temperature = RxMessage.CANx_Export_RxMessage[6];
		
			M3508_Fric_Wheel[0].Info_Update_Frame++;
			break;
		case M3508_FRIC_WHEEL_RIGHT_ID:
			M3508_Fric_Wheel[1].Actual_Angle = (int16_t)(RxMessage.CANx_Export_RxMessage[0] << 8 | RxMessage.CANx_Export_RxMessage[1]);
			M3508_Fric_Wheel[1].Actual_Speed = (int16_t)(RxMessage.CANx_Export_RxMessage[2] << 8 | RxMessage.CANx_Export_RxMessage[3]);
			M3508_Fric_Wheel[1].Actual_Current = (int16_t)(RxMessage.CANx_Export_RxMessage[4] << 8 | RxMessage.CANx_Export_RxMessage[5]);
			M3508_Fric_Wheel[1].Temperature = RxMessage.CANx_Export_RxMessage[6];
		
			M3508_Fric_Wheel[1].Info_Update_Frame++;
			break;
	}
}

void Check_M3508_Chassis(void)
{
	for(int i = 0; i < 4; i++)
	{
		if(M3508_Chassis[i].Info_Update_Frame < 1)
			M3508_Chassis[i].Offline_Flag = 1;
		else
			M3508_Chassis[i].Offline_Flag = 0;
		M3508_Chassis[i].Info_Update_Frame = 0;
	}
}

void Check_M3508_Fric_Wheel(void)
{
	for(int i = 0; i < 2; i++)
	{
		if(M3508_Fric_Wheel[i].Info_Update_Frame < 1)
			M3508_Fric_Wheel[i].Offline_Flag = 1;
		else
			M3508_Fric_Wheel[i].Offline_Flag = 0;
		M3508_Fric_Wheel[i].Info_Update_Frame = 0;
	}
}
