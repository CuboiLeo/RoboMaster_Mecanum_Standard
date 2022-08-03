/**
 * @file Referee_System.c
 * @author Leo Liu
 * @brief reading data from the referee system
 * @version 1.0
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Referee_System.h"

Referee_System_t Referee_System;

static int USART_Receive_DMA_NO_IT(UART_HandleTypeDef *huartx, uint8_t *pData, uint32_t Size);
void Referee_System_USART_Receive_DMA(UART_HandleTypeDef *huartx);
void Referee_System_Handler(UART_HandleTypeDef *huartx);
void Referee_Get_Data(uint16_t Data_Length);

Referee_System_Func_t Referee_System_Func = Referee_System_Func_GroundInit;
#undef Referee_System_Func_GroundInit

uint8_t Referee_System_Buffer[REFEREE_BUFFER_LEN];
uint16_t DMA_Counter;

//Enable USART DMA transfer
static int USART_Receive_DMA_NO_IT(UART_HandleTypeDef *huartx, uint8_t *pData, uint32_t Size)
{
	if(huartx->RxState == HAL_UART_STATE_READY)
	{
		if((pData == NULL) || (Size == 0))
			return HAL_ERROR;
		
		huartx->pRxBuffPtr = pData;
		huartx->RxXferSize = Size;
		huartx->ErrorCode = HAL_UART_ERROR_NONE;
		
		HAL_DMA_Start(huartx->hdmarx, (uint32_t)&huartx->Instance->DR, (uint32_t)pData, Size);
		
		SET_BIT(huartx->Instance->CR3, USART_CR3_DMAR);
	}
	else
		return HAL_BUSY;
	
	return HAL_OK;
}

void Referee_System_USART_Receive_DMA(UART_HandleTypeDef *huartx)
{
	__HAL_UART_CLEAR_IDLEFLAG(huartx);
	__HAL_UART_ENABLE(huartx);
	__HAL_UART_ENABLE_IT(huartx, UART_IT_IDLE);
	USART_Receive_DMA_NO_IT(huartx, Referee_System_Buffer, REFEREE_BUFFER_LEN);
}

void Referee_System_Handler(UART_HandleTypeDef *huartx)
{
	__HAL_UART_CLEAR_IDLEFLAG(huartx);
  __HAL_DMA_DISABLE(huartx->hdmarx);

	DMA_Counter = __HAL_DMA_GET_COUNTER(huartx->hdmarx);
	Referee_Get_Data(REFEREE_BUFFER_LEN - DMA_Counter);

	__HAL_DMA_SET_COUNTER(huartx->hdmarx, REFEREE_BUFFER_LEN);
	__HAL_DMA_ENABLE(huartx->hdmarx);

	Referee_System.Info_Update_Frame++;
}

//Get referee system data based on ID
void Referee_Get_Data(uint16_t Data_Length)
{
	for(int i = 0; i < Data_Length;)
	{
		if(Referee_System_Buffer[i] == REFEREE_FRAME_HEADER)
		{
			switch(Referee_System_Buffer[i + 5] | Referee_System_Buffer[i + 6] << 8)
			{
				case Game_State:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Game_State_Len))
					{
						memcpy(&Referee_System.Game_State, &Referee_System_Buffer[i + 7], 11);
						i += Game_State_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Game_Result:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Game_Result_Len))
					{
						memcpy(&Referee_System.Game_Result, &Referee_System_Buffer[i + 7], 1);
						i += Game_Result_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Alive_Robot:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Alive_Robot_Len))
					{
						memcpy(&Referee_System.Alive_Robot, &Referee_System_Buffer[i + 7], 32);
						i += Alive_Robot_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Event:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Event_Len))
					{
						memcpy(&Referee_System.Event, &Referee_System_Buffer[i + 7], 4);
						i += Event_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Warning:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Warning_Len))
					{
						memcpy(&Referee_System.Warning, &Referee_System_Buffer[i + 7], 4);
						i += Warning_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Robot_State:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Robot_State_Len))
					{
						memcpy(&Referee_System.Robot_State, &Referee_System_Buffer[i + 7], 27);
						i += Robot_State_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Power_n_Heat:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Power_n_Heat_Len))
					{
						memcpy(&Referee_System.Power_n_Heat, &Referee_System_Buffer[i + 7], 16);
						i += Power_n_Heat_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Location:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Location_Len))
					{
						memcpy(&Referee_System.Location, &Referee_System_Buffer[i + 7], 16);
						i += Location_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Robot_Buff:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Robot_Buff_Len))
					{
						memcpy(&Referee_System.Robot_Buff, &Referee_System_Buffer[i + 7], 1);
						i += Robot_Buff_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Damage:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Damage_Len))
					{
						memcpy(&Referee_System.Damage, &Referee_System_Buffer[i + 7], 1);
						i += Damage_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case Shooter:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Shooter_Len))
					{
						memcpy(&Referee_System.Shooter, &Referee_System_Buffer[i + 7], 7);
						i += Shooter_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;

				case Remaining_Ammo:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, Remaining_Ammo_Len))
					{
						memcpy(&Referee_System.Remaining_Ammo, &Referee_System_Buffer[i + 7], 6);
						i += Remaining_Ammo_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				case RFID:
					if(CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System_Buffer + i, RFID_Len))
					{
						memcpy(&Referee_System.RFID, &Referee_System_Buffer[i + 7], 4);
						i += RFID_Len;
						Referee_System.Offline_Flag = 0;
					}
					else
						i++;
					break;
					
				default:
					i++;
					break;
			}
		}
		else
			i++;
	}
	Referee_System.Info_Update_Frame++;
}
