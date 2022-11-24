/**
 * @file Referee_System.h
 * @author Leo Liu
 * @brief header file for referee system
 * @version 1.0
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __REFEREE_SYSTEM_H
#define __REFEREE_SYSTEM_H

#include "CRC_Verification.h"
#include "dma.h"
#include "usart.h"
#include <stdint.h>
#include <stdbool.h>

#define Referee_System_Func_GroundInit	\
{																				\
		&Referee_UART_Receive_Interrupt,		\
				&Referee_System_Handler,				\
				&Referee_Get_Data,							\
}

#define REFEREE_BUFFER_LEN 												 389u					//Buffer length to receive all data
#define REFEREE_FRAME_HEADER_START 								 0xA5 				//Frame header
#define REFEREE_FRAME_HEADER_LEN 									 5 						//Frame header length
#define REFEREE_ID_LEN 														 2						//ID length
#define REFEREE_FRAME_TAIL_LEN 										 2 						//Frame tail length

#define REFEREE_OFFSET_SOF         								 0  					//SOF offset
#define REFEREE_OFFSET_DATA_LENGTH 								 1  					//Data length offset
#define REFEREE_OFFSET_SEQ         								 3  					//SEQ offset
#define REFEREE_OFFSET_CRC8        								 4  					//CRC offset

#define       REFEREE_GAME_STATUS	              	 0x0001 
#define       REFEREE_GAME_RESULT              		 0x0002 
#define       REFEREE_ROBOT_HP                     0x0003 
#define       REFEREE_DART_LAUNCH                  0x0004
#define       REFEREE_AI_CHALLENGEBUFF             0x0005
#define       REFEREE_EVENT_DATA                   0x0101
#define       REFEREE_SUPPLY_STATION               0x0102
//#define       REFEREE_REQUEST_RECHARGE             0x0103
#define       REFEREE_REFEREE_WARNING              0x0104
#define       REFEREE_DART_COUNTDOWN               0x0105
#define       REFEREE_ROBOT_STATE                  0x0201
#define       REFEREE_POWER_HEAT                   0x0202
#define       REFEREE_ROBOT_POSITION               0x0203
#define       REFEREE_ROBOT_BUFF                   0x0204
#define       REFEREE_AERIAL_ENERGY                0x0205
#define       REFEREE_INJURY_STATE                 0x0206
#define       REFEREE_SHOOTER_STATE                0x0207
#define       REFEREE_REMAINING_AMMO             	 0x0208
#define       REFEREE_ROBOT_RFID                   0x0209
#define       REFEREE_DART_CLIENT                  0x020A
#define       REFEREE_ROBOT_COMMUNICATE            0x0301
#define       REFEREE_USER_DEFINED                 0x0302


/*Calculation: REFEREE_FRAME_HEADER_LEN(5-byte) + REFEREE_ID_LEN(2-byte) + DATA(n-byte) + REFEREE_FRAME_TAIL_LEN(2-byte,CRC16)*/
#define       REFEREE_GAME_STATUS_LEN	             20
#define       REFEREE_GAME_RESULT_LEN              10 
#define       REFEREE_ROBOT_HP_LEN                 41 
#define       REFEREE_DART_LAUNCH_LEN              12
#define       REFEREE_AI_CHALLENGEBUFF_LEN         20
#define       REFEREE_EVENT_DATA_LEN               13
#define       REFEREE_SUPPLY_STATION_LEN           13
//#define       REFEREE_REQUEST_RECHARGE_LEN         11
#define       REFEREE_REFEREE_WARNING_LEN          11
#define       REFEREE_DART_COUNTDOWN_LEN           10	
#define       REFEREE_ROBOT_STATE_LEN              36
#define       REFEREE_POWER_HEAT_LEN               25
#define       REFEREE_ROBOT_POSITION_LEN           25
#define       REFEREE_ROBOT_BUFF_LEN               10
#define       REFEREE_AERIAL_ENERGY_LEN            10
#define       REFEREE_INJURY_STATE_LEN             10
#define       REFEREE_SHOOTER_STATE_LEN            16
#define       REFEREE_REMAINING_AMMO_LEN           15
#define       REFEREE_ROBOT_RFID_LEN               13
#define       REFEREE_DART_CLIENT_LEN              21
#define       REFEREE_ROBOT_COMMUNICATE_LEN        35
#define       REFEREE_USER_DEFINED_LEN             26


typedef __packed struct
{
	uint16_t DMA_Counter;
	uint8_t Buffer[REFEREE_BUFFER_LEN];
	
	struct __attribute__ ((__packed__))
	{
		uint8_t Type;
		uint8_t Progress;
		uint16_t Remaining_Time;
		uint64_t Sync_Time_Stamp;
	}Game_Status;
	
	struct __attribute__ ((__packed__))
	{
		uint8_t Winner;
	}Game_Result;
	
	struct __attribute__ ((__packed__))
	{
		uint16_t Red_1_HP;   
		uint16_t Red_2_HP;  
		uint16_t Red_3_HP;  
		uint16_t Red_4_HP;  
		uint16_t Red_5_HP;  
		uint16_t Red_6_HP;  
		uint16_t Red_7_HP;  
		uint16_t Red_Outpost_HP; 
		uint16_t Red_Base_HP; 
		
		uint16_t Blue_1_HP;   
		uint16_t Blue_2_HP;   
		uint16_t Blue_3_HP;   
		uint16_t Blue_4_HP;   
		uint16_t Blue_5_HP;   
		uint16_t Blue_6_HP;   
		uint16_t Blue_7_HP; 
		uint16_t Blue_Outpost_HP; 
		uint16_t Blue_Base_HP;   
	}Alive_Robot;
	
	struct __attribute__ ((__packed__))
	{
		uint32_t Type;
	}Event;
	
	struct __attribute__ ((__packed__))
	{
		uint8_t Card;
		uint8_t Foul_Robot_ID;
	}Warning;
	
	struct __attribute__ ((__packed__))
	{
		uint8_t ID;
		uint8_t Level;
		uint16_t Remaining_HP;
		uint16_t HP_Max;
		
		uint16_t Shooter_1_Cooling_Rate_17mm;
		uint16_t Shooter_1_Heat_Max_17mm;
		uint16_t Shooter_1_Speed_Limit_17mm;
		
		uint16_t Shooter_2_Cooling_Rate_17mm;
		uint16_t Shooter_2_Heat_Max_17mm;
		uint16_t Shooter_2_Speed_Limit_17mm;
		
		uint16_t Shooter_Cooling_Rate_42mm;
		uint16_t Shooter_Heat_Max_42mm;
		uint16_t Shooter_Speed_Limit_42mm;
		
		uint16_t Chassis_Power_Max;	
	}Robot_State;
	
	struct __attribute__ ((__packed__))
	{
		uint16_t Chassis_Voltage;
		uint16_t Chassis_Current;
		float Chassis_Power;
		uint16_t Chassis_Power_Buffer;
		uint16_t Shooter_1_Heat;
		uint16_t Shooter_2_Heat;
		uint16_t Mobile_Shooter_Heat;
	}Power_n_Heat;
	
	struct __attribute__ ((__packed__))
	{
		float x;
		float y;
		float z;
		float Yaw;
	}Location;
	
	struct __attribute__ ((__packed__))
	{
		uint8_t Buff;
	}Robot_Buff;
	
	struct __attribute__ ((__packed__))
	{
		uint8_t Armor_ID;
		uint8_t Type;
	}Damage;
	
	struct __attribute__ ((__packed__))
	{
		uint8_t Ammo_Type;
		uint8_t Shooter_Type;
		uint8_t Frequency;
		float Speed;
	}Shooter;
	
	struct __attribute__ ((__packed__))
	{
		uint16_t Type_17mm;
		uint16_t Type_42mm;
		uint16_t Type_Gold;
	}Remaining_Ammo;
	
	struct __attribute__ ((__packed__))
	{
		uint32_t State;
	}RFID;
		
	uint16_t Info_Update_Frame;
	uint8_t Offline_Flag;
}Referee_System_t;

typedef struct
{
	HAL_StatusTypeDef (*Referee_UART_Receive_Interrupt)(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
	void (*Referee_System_Handler)(UART_HandleTypeDef *huart);
	void (*Referee_Get_Data)(uint16_t Data_Length);
}Referee_System_Func_t;

extern Referee_System_t Referee_System;
extern Referee_System_Func_t Referee_System_Func;

#endif
