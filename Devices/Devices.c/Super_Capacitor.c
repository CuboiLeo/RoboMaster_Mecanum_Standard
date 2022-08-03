/**
 * @file Super_Capacitor.c
 * @author Leo Liu
 * @brief super capacitor communication
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Super_Capacitor.h"
#include <stdio.h>

void Super_Capacitor_Get_Data(CAN_Export_Data_t RxMessage);
void Super_Capacitor_Send_Data(int16_t Target_Power);
void Check_Super_Capacitor(void);

Super_Capacitor_Func_t Super_Capacitor_Func = Super_Capacitor_Func_GroundInit;
#undef Super_Capacitor_Func_GroundInit

Super_Capacitor_t Super_Capacitor;

//Obtain super capacitor data from CAN
void Super_Capacitor_Get_Data(CAN_Export_Data_t RxMessage)
{
	Super_Capacitor.Input_Voltage = (int16_t)(RxMessage.CANx_Export_RxMessage[0] << 8 | RxMessage.CANx_Export_RxMessage[1]);
  Super_Capacitor.Capacitor_Voltage = (int16_t)(RxMessage.CANx_Export_RxMessage[2] << 8 | RxMessage.CANx_Export_RxMessage[3]);
  Super_Capacitor.Input_Current = (int16_t)(RxMessage.CANx_Export_RxMessage[4] << 8 | RxMessage.CANx_Export_RxMessage[5]);
	Super_Capacitor.Actual_Power = (int16_t)(RxMessage.CANx_Export_RxMessage[6] << 8 | RxMessage.CANx_Export_RxMessage[7]);
	
	Super_Capacitor.Info_Update_Frame++;
}

//Send super capacitor data through specified identifier
void Super_Capacitor_Send_Data(int16_t Target_Power)
{
	CAN_Func.CAN_0x210_Send_Data(&hcan1,Target_Power);
}

void Check_Super_Capacitor(void)
{
	if(Super_Capacitor.Info_Update_Frame < 1)
		Super_Capacitor.Offline_Flag = 1;
	else
		Super_Capacitor.Offline_Flag = 0;
	Super_Capacitor.Info_Update_Frame = 0;
}
