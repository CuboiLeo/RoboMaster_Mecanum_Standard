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
#include <stdio.h>

Motor_Init_t M3508_Chassis[4];
Motor_Init_t M3508_Fric_Wheel[2];

void M3508_Chassis_Get_Data(CAN_Export_Data_t RxMessage);
void M3508_Chassis_Send_Data(int16_t Motor_1_Current,int16_t Motor_2_Current,int16_t Motor_3_Current,int16_t Motor_4_Current);
void M3508_Fric_Wheel_Get_Data(CAN_Export_Data_t RxMessage);
void M3508_Fric_Wheel_Send_Data(int16_t Fric_Wheel_Left_Current,int16_t Fric_Wheel_Right_Current);
void Check_M3508_Chassis(void);
void Check_M3508_Fric_Wheel(void);


M3508_Func_t M3508_Func = M3508_Func_GroundInit;
#undef M3508_Func_GroundInit

//Obtain chassis data from CAN
void M3508_Chassis_Get_Data(CAN_Export_Data_t RxMessage)
{
	uint8_t ID;
  ID = (uint8_t)(RxMessage.CAN_RxHeader.StdId - M3508_CHASSIS_START_ID);
	
	M3508_Chassis[ID].Prev_Angle = M3508_Chassis[ID].Actual_Angle;
  M3508_Chassis[ID].Actual_Angle = (int16_t)(RxMessage.CANx_Export_RxMessage[0] << 8 | RxMessage.CANx_Export_RxMessage[1]);
  M3508_Chassis[ID].Actual_Speed = (int16_t)(RxMessage.CANx_Export_RxMessage[2] << 8 | RxMessage.CANx_Export_RxMessage[3]);
  M3508_Chassis[ID].Actual_Current = (int16_t)(RxMessage.CANx_Export_RxMessage[4] << 8 | RxMessage.CANx_Export_RxMessage[5]);
  M3508_Chassis[ID].Temperature = RxMessage.CANx_Export_RxMessage[6];
	if((M3508_Chassis[ID].Actual_Angle - M3508_Chassis[ID].Prev_Angle) < -6500 )
		M3508_Chassis[ID].Turn_Count++;
	else if((M3508_Chassis[ID].Actual_Angle - M3508_Chassis[ID].Prev_Angle) > 6500)
		M3508_Chassis[ID].Turn_Count--;
	M3508_Chassis[ID].Total_Angle = M3508_Chassis[ID].Actual_Angle + (M3508_MECH_ANGLE_MAX * M3508_Chassis[ID].Turn_Count);
  M3508_Chassis[ID].Info_Update_Frame++;
}

//Send chassis data through specified identifier
void M3508_Chassis_Send_Data(int16_t Motor_1_Current,int16_t Motor_2_Current,int16_t Motor_3_Current,int16_t Motor_4_Current)
{
	CAN_Func.CAN_0x200_Send_Data(&hcan1,Motor_1_Current,Motor_2_Current,Motor_3_Current,Motor_4_Current);
}

//Obtain friction wheel data from CAN
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

//Send friction wheel data through specified identifier
void M3508_Fric_Wheel_Send_Data(int16_t Fric_Wheel_Left_Current,int16_t Fric_Wheel_Right_Current)
{
	CAN_Func.CAN_0x200_Send_Data(&hcan2,Fric_Wheel_Left_Current,Fric_Wheel_Right_Current,0,0);
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
