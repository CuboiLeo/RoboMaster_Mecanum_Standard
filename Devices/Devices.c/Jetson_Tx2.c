/**
 * @file Jetson_Tx2.c
 * @author Leo Liu
 * @brief communicate and obtain data from Jetson
 * @version 1.0
 * @date 2022-10-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Jetson_Tx2.h"

void Jetson_Tx2_Initialization(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void Jetson_Tx2_Get_Data(void);
void Jetson_Tx2_Send_Data(void);

Tx2_Func_t Tx2_Func = Tx2_Func_GroundInit;
Tx2_Data_t Tx2_Data = {0};

#undef Tx2_Func_GroundInit

void Jetson_Tx2_Initialization(void)
{
	HAL_UART_Receive_DMA(&huart7, Tx2_Data.Rx_Buffer, sizeof(Tx2_Data.Rx_Buffer));
	HAL_UART_Transmit_DMA(&huart7, Tx2_Data.Tx_Buffer, sizeof(Tx2_Data.Tx_Buffer));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_DMA(&huart7, Tx2_Data.Rx_Buffer, sizeof(Tx2_Data.Rx_Buffer));
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Transmit_DMA(&huart7, Tx2_Data.Tx_Buffer, sizeof(Tx2_Data.Tx_Buffer));
}

void Jetson_Tx2_Get_Data(void)
{
	if(Tx2_Data.Rx_Buffer[0] == 0xAA)
	{
		Tx2_Data.Receiving.Frame_ID = Tx2_Data.Rx_Buffer[0];
		Tx2_Data.Receiving.Frame_Type = Tx2_Data.Rx_Buffer[1];
		switch(Tx2_Data.Receiving.Frame_Type)
		{
			case 0:
				memcpy(&Tx2_Data.Receiving.Raw_Data.Data[0],&Tx2_Data.Rx_Buffer[4],8*sizeof(uint8_t));
				if(fabs(*(float*)&Tx2_Data.Receiving.Raw_Data.data[0]) > 0.01f)
					Tx2_Data.Receiving.Auto_Aiming.Yaw = *(float*)&Tx2_Data.Receiving.Raw_Data.data[0];
				if(fabs(*(float*)&Tx2_Data.Receiving.Raw_Data.data[1]) > 0.01f)
					Tx2_Data.Receiving.Auto_Aiming.Pitch = *(float*)&Tx2_Data.Receiving.Raw_Data.data[1];
				break;
			
			case 1:
				Tx2_Data.Receiving.Navigation.X_Vel = 0.1f * (int8_t)Tx2_Data.Rx_Buffer[2];
				Tx2_Data.Receiving.Navigation.Y_Vel = 0.1f * (int8_t)Tx2_Data.Rx_Buffer[3];
				Tx2_Data.Receiving.Navigation.Yaw_Angular_Rate = (int8_t)Tx2_Data.Rx_Buffer[4];
				Tx2_Data.Receiving.Navigation.State = Tx2_Data.Rx_Buffer[5];
				break;
			
			case 2:
				Tx2_Data.Receiving.Heart_Beat.a = Tx2_Data.Rx_Buffer[2];
				Tx2_Data.Receiving.Heart_Beat.b = Tx2_Data.Rx_Buffer[3];
				Tx2_Data.Receiving.Heart_Beat.c = Tx2_Data.Rx_Buffer[4];
				Tx2_Data.Receiving.Heart_Beat.d = Tx2_Data.Rx_Buffer[5];
				break;
			
			default:
				break;
		}
	}
	
}

void Jetson_Tx2_Send_Data(void)
{
	Tx2_Data.Sending.Pitch_Angle = VAL_LIMIT(128 + Board_A_IMU.Export_Data.Pitch,255,0);
	Tx2_Data.Sending.Pitch_Angular_Rate = VAL_LIMIT(128 + Board_A_IMU.Export_Data.Gyro_Pitch,255,0);
	Tx2_Data.Sending.Yaw_Angular_Rate = VAL_LIMIT(128 + Board_A_IMU.Export_Data.Gyro_Yaw,255,0);
	Tx2_Data.Tx_Buffer[0] = 0xAA;
	Tx2_Data.Tx_Buffer[1] = (uint8_t)Tx2_Data.Sending.Pitch_Angle;
	Tx2_Data.Tx_Buffer[2] = (uint8_t)Tx2_Data.Sending.Pitch_Angular_Rate;
	Tx2_Data.Tx_Buffer[3] = (uint8_t)Tx2_Data.Sending.Yaw_Angular_Rate;
	Tx2_Data.Tx_Buffer[4] = Tx2_Data.Sending.Team_Color;
}
