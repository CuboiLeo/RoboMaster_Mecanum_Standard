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
	Tx2_Data.Auto_Aiming.Auto_Aiming_Yaw = Tx2_Data.Rx_Buffer[0];
	Tx2_Data.Auto_Aiming.Auto_Aiming_Pitch = Tx2_Data.Rx_Buffer[1];
}

void Jetson_Tx2_Send_Data(void)
{
	Tx2_Data.Sending.Super_Capacitor_Charge = Tx2_Data.Sending.Super_Capacitor_Charge + 1;
	Tx2_Data.Sending.Team_Color = 1;
	Tx2_Data.Sending.Is_In_AutoAim = Tx2_Data.Sending.Is_In_AutoAim + 2;
	Tx2_Data.Tx_Buffer[0] = Tx2_Data.Sending.Super_Capacitor_Charge;
	Tx2_Data.Tx_Buffer[1] = (Tx2_Data.Sending.Team_Color << 1) | Tx2_Data.Sending.Is_In_AutoAim;
}
