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
Referee_Robot_State_t Referee_Robot_State;

HAL_StatusTypeDef Referee_UART_Receive_DMA_No_Interrupt(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef Referee_UART_Receive_Interrupt(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void Referee_System_Handler(UART_HandleTypeDef *huart);
void Referee_Get_Data(uint16_t Data_Length);
void Referee_Set_Robot_State(void);

Referee_System_Func_t Referee_System_Func = Referee_System_Func_GroundInit;
#undef Referee_System_Func_GroundInit

void Referee_Set_Robot_State(void)
{
	Referee_Robot_State.Game_Type = Referee_System.Game_Status.Type; 
	Referee_Robot_State.ID = Referee_System.Robot_State.ID; 
	Referee_Robot_State.Level = Referee_System.Robot_State.Level;
	
	Referee_Robot_State.Cooling_Rate = Referee_System.Robot_State.Shooter_1_Cooling_Rate_17mm;
	Referee_Robot_State.Heat_Max = Referee_System.Robot_State.Shooter_1_Heat_Max_17mm; 	
	Referee_Robot_State.Bullet_Speed_Max = Referee_System.Robot_State.Shooter_1_Speed_Limit_17mm;
	Referee_Robot_State.Chassis_Power_Max = Referee_System.Robot_State.Chassis_Power_Max;
	
	Referee_Robot_State.Chassis_Power = Referee_System.Power_n_Heat.Chassis_Power; 
	Referee_Robot_State.Power_Buffer = Referee_System.Power_n_Heat.Chassis_Power_Buffer; 
	Referee_Robot_State.Shooter_Heat = Referee_System.Power_n_Heat.Shooter_1_Heat; 
	Referee_Robot_State.Shooting_Frequency = Referee_System.Shooter.Frequency; 
	Referee_Robot_State.Shooting_Speed = Referee_System.Shooter.Speed; 
	
	switch(Referee_Robot_State.Bullet_Speed_Max)
	{
		case 15:
			Shooting.Fric_Wheel.Target_Speed = FRIC_SPEED_15;
			break;
		case 18:
			Shooting.Fric_Wheel.Target_Speed = FRIC_SPEED_18;
			break;
		case 30:
			Shooting.Fric_Wheel.Target_Speed = FRIC_SPEED_30;
			break;
		default:
			Shooting.Fric_Wheel.Target_Speed = FRIC_SPEED_15;
			break;
	}
	
	switch(Referee_Robot_State.Chassis_Power_Max)
	{
		case 45:
			Chassis.Chassis_Coord.Speed_Coefficient = CHASSIS_NORMAL_SPEED_COEFF_45;
			Chassis.Chassis_Coord.Spin_Rate = CHASSIS_SPINTOP_RATE_POWER_45;
			break;
		case 50:
			Chassis.Chassis_Coord.Speed_Coefficient = CHASSIS_NORMAL_SPEED_COEFF_50;
			Chassis.Chassis_Coord.Spin_Rate = CHASSIS_SPINTOP_RATE_POWER_50;
			break;
		case 55:
			Chassis.Chassis_Coord.Speed_Coefficient = CHASSIS_NORMAL_SPEED_COEFF_55;
			Chassis.Chassis_Coord.Spin_Rate = CHASSIS_SPINTOP_RATE_POWER_55;
			break;
		case 60:
			Chassis.Chassis_Coord.Speed_Coefficient = CHASSIS_NORMAL_SPEED_COEFF_60;
			Chassis.Chassis_Coord.Spin_Rate = CHASSIS_SPINTOP_RATE_POWER_60;
			break;
		case 80:
			Chassis.Chassis_Coord.Speed_Coefficient = CHASSIS_NORMAL_SPEED_COEFF_80;
			Chassis.Chassis_Coord.Spin_Rate = CHASSIS_SPINTOP_RATE_POWER_80;
			break;
		case 100:
			Chassis.Chassis_Coord.Speed_Coefficient = CHASSIS_NORMAL_SPEED_COEFF_100;
			Chassis.Chassis_Coord.Spin_Rate = CHASSIS_SPINTOP_RATE_POWER_100;
			break;
		case 120:
			Chassis.Chassis_Coord.Speed_Coefficient = CHASSIS_NORMAL_SPEED_COEFF_120;
			Chassis.Chassis_Coord.Spin_Rate = CHASSIS_SPINTOP_RATE_POWER_120;
			break;
		default:
			Chassis.Chassis_Coord.Speed_Coefficient = CHASSIS_NORMAL_SPEED_COEFF_45;
			Chassis.Chassis_Coord.Spin_Rate = CHASSIS_SPINTOP_RATE_POWER_45;
			break;
	}
}

HAL_StatusTypeDef Referee_UART_Receive_DMA_No_Interrupt(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	if (huart->RxState == HAL_UART_STATE_READY)
  {
    if ((pData == NULL) || (Size == 0))
    {
      return HAL_ERROR;
    }

		huart->pRxBuffPtr = pData;
		huart->RxXferSize = Size;
		huart->ErrorCode = HAL_UART_ERROR_NONE;
		
		/* Enable the DMA Stream */
		HAL_DMA_Start(huart->hdmarx,(uint32_t)&huart->Instance->DR,(uint32_t)pData, Size);
		/* Enable the DMA transfer for the receiver request by setting the DMAR bit in the UART CR3 register */
		SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
		
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

HAL_StatusTypeDef Referee_UART_Receive_Interrupt(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	__HAL_UART_CLEAR_IDLEFLAG(huart);
	__HAL_UART_ENABLE(huart);
	__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);
	Referee_UART_Receive_DMA_No_Interrupt(huart,pData,Size);
	
	return HAL_OK;
}


void Referee_System_Handler(UART_HandleTypeDef *huart)
{
	__HAL_UART_CLEAR_IDLEFLAG(huart);
  __HAL_DMA_DISABLE(huart->hdmarx);

  Referee_System.DMA_Counter = __HAL_DMA_GET_COUNTER(huart->hdmarx);
  Referee_System_Func.Referee_Get_Data(REFEREE_BUFFER_LEN - Referee_System.DMA_Counter);

  __HAL_DMA_SET_COUNTER(huart->hdmarx, REFEREE_BUFFER_LEN);
  __HAL_DMA_ENABLE(huart->hdmarx);
}

//Get referee system data based on ID
void Referee_Get_Data(uint16_t Data_Length)
{
	for(int n = 0; n < Data_Length;)
	{
		if(Referee_System.Buffer[n] == REFEREE_FRAME_HEADER_START)
		{
			switch(Referee_System.Buffer[n + 5] | Referee_System.Buffer[n + 6] << 8)
			{
				case REFEREE_GAME_STATUS:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_GAME_STATUS_LEN))
                {
                    memcpy(&Referee_System.Game_Status, &Referee_System.Buffer[n + 7], sizeof(uint8_t[11]));
                    n += REFEREE_GAME_STATUS_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_GAME_RESULT:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_GAME_RESULT_LEN))
                {
                    memcpy(&Referee_System.Game_Result, &Referee_System.Buffer[n + 7], sizeof(uint8_t[1]));
                    n += REFEREE_GAME_RESULT_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_ROBOT_HP:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_ROBOT_HP_LEN))
                {
                    memcpy(&Referee_System.Alive_Robot, &Referee_System.Buffer[n + 7], sizeof(uint8_t[32]));
                    n += REFEREE_ROBOT_HP_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_EVENT_DATA:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_EVENT_DATA_LEN))
                {
                    memcpy(&Referee_System.Event, &Referee_System.Buffer[n + 7], sizeof(uint8_t[4]));
                    n += REFEREE_EVENT_DATA_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_REFEREE_WARNING: 
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_REFEREE_WARNING_LEN))
                {
                    memcpy(&Referee_System.Warning, &Referee_System.Buffer[n + 7], sizeof(uint8_t[2]));
                    n += REFEREE_REFEREE_WARNING_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_ROBOT_STATE:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_ROBOT_STATE_LEN))
                {
                    memcpy(&Referee_System.Robot_State, &Referee_System.Buffer[n + 7], sizeof(uint8_t[27]));
                    n += REFEREE_ROBOT_STATE_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_POWER_HEAT:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_POWER_HEAT_LEN))
                {
                    memcpy(&Referee_System.Power_n_Heat, &Referee_System.Buffer[n + 7], sizeof(uint8_t[16]));
                    n += REFEREE_POWER_HEAT_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_ROBOT_POSITION:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_ROBOT_POSITION_LEN))
                {
                    memcpy(&Referee_System.Location, &Referee_System.Buffer[n + 7], sizeof(uint8_t[16]));
                    n += REFEREE_ROBOT_POSITION_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_ROBOT_BUFF:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_ROBOT_BUFF_LEN))
                {
                    memcpy(&Referee_System.Robot_Buff, &Referee_System.Buffer[n + 7], sizeof(uint8_t[1]));
                    n += REFEREE_ROBOT_BUFF_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_INJURY_STATE:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_INJURY_STATE_LEN))
                {
                    memcpy(&Referee_System.Damage, &Referee_System.Buffer[n + 7], sizeof(uint8_t[1]));
                    n += REFEREE_INJURY_STATE_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_SHOOTER_STATE:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_SHOOTER_STATE_LEN))
                {
                    memcpy(&Referee_System.Shooter, &Referee_System.Buffer[n + 7], sizeof(uint8_t[7]));
                    n += REFEREE_SHOOTER_STATE_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_REMAINING_AMMO:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_REMAINING_AMMO_LEN))
                {
                    memcpy(&Referee_System.Remaining_Ammo, &Referee_System.Buffer[n + 7], sizeof(uint8_t[6]));
                    n += REFEREE_REMAINING_AMMO_LEN;
                }
                else
                    n++;
                break;
            case REFEREE_ROBOT_RFID:
                if (CRC_Verif_Func.Verify_CRC16_Check_Sum(Referee_System.Buffer + n, REFEREE_ROBOT_RFID_LEN))
                {
                    memcpy(&Referee_System.RFID, &Referee_System.Buffer[n + 7], sizeof(uint8_t[4]));
                    n += REFEREE_ROBOT_RFID_LEN;
                }
                else
                    n++;
                break;
            default:
                n++;
                break;
          }
			}
      else
				n++;
		}
}
