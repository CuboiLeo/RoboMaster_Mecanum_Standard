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

#define REFEREE_BUFFER_LEN 245	//Buffer length to receive all data
#define REFEREE_FRAME_HEADER 0xA5 //Frame header
#define REFEREE_FRAME_HEADER_LEN 5 //Frame header length
#define REFEREE_ID_LEN 2	//ID length
#define REFEREE_FRAME_TAIL_LEN 2 //Frame tail length

#define Referee_System_Func_GroundInit	\
{																				\
		&Referee_System_USART_Receive_DMA,	\
				&Referee_System_Handler,				\
				&Referee_Get_Data,							\
}

typedef struct
{
	enum
	{
		Game_State = 0x0001,
		Game_Result = 0x0002,
		Alive_Robot = 0x0003,
		
		Event = 0x0101,
		Warning = 0x0104,
		
		Robot_State = 0x0201,
		Power_n_Heat = 0x0202,
		Location = 0x0203,
		Robot_Buff = 0x0204,
		Damage = 0x0206,
		Shooter = 0x0207,
		Remaining_Ammo = 0x0208,
		RFID = 0x0209,
	}ID;
	
	enum
	{
		Game_State_Len = 20,
		Game_Result_Len = 10,
		Alive_Robot_Len = 41,
		
		Event_Len = 13,
		Warning_Len = 11,
		
		Robot_State_Len = 36,
		Power_n_Heat_Len = 25,
		Location_Len = 25,
		Robot_Buff_Len = 10,
		Damage_Len = 10,
		Shooter_Len = 16,
		Remaining_Ammo_Len = 15,
		RFID_Len = 13
	}Length;
	
	struct
	{
		uint8_t Type;
		uint8_t Progress;
		uint16_t Remaining_Time;
		uint64_t Sync_Time_Stamp;
	}Game_State;
	
	struct
	{
		uint8_t Winner;
	}Game_Result;
	
	struct
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
	
	struct
	{
		uint32_t Type;
	}Event;
	
	struct
	{
		uint8_t Card;
		uint8_t Foul_Robot_ID;
	}Warning;
	
	struct
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
	
	struct
	{
		uint16_t Chassis_Voltage;
		uint16_t Chassis_Current;
		float Chassis_Power;
		uint16_t Chassis_Power_Buffer;
		uint16_t Shooter_1_Heat;
		uint16_t Shooter_2_Heat;
		uint16_t Mobile_Shooter_Heat;
	}Power_n_Heat;
	
	struct
	{
		float x;
		float y;
		float z;
		float Yaw;
	}Location;
	
	struct
	{
		uint8_t Buff;
	}Robot_Buff;
	
	struct
	{
		uint8_t Armor_ID;
		uint8_t Type;
	}Damage;
	
	struct
	{
		uint8_t Ammo_Type;
		uint8_t Shooter_Type;
		uint8_t Frequency;
		float Speed;
	}Shooter;
	
	struct
	{
		uint16_t Type_17mm;
		uint16_t Type_42mm;
		uint16_t Type_Gold;
	}Remaining_Ammo;
	
	struct
	{
		uint32_t State;
	}RFID;
	
	uint16_t Info_Update_Frame;
	uint8_t Offline_Flag;
}Referee_System_t;

typedef struct
{
	void (*Referee_System_USART_Receive_DMA)(UART_HandleTypeDef *huartx);
	void (*Referee_System_Handler)(UART_HandleTypeDef *huartx);
	void (*Referee_Get_Data)(uint16_t Data_Length);
}Referee_System_Func_t;

extern Referee_System_t Referee_System;
extern Referee_System_Func_t Referee_System_Func;

#endif
