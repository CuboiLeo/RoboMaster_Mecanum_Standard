/**
 * @file Jetson_Tx2.h
 * @author Leo Liu
 * @brief communicate and obtain data from Jetson
 * @version 1.0
 * @date 2022-10-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __JETSON_TX2_H
#define __JETSON_TX2_H

#include "dma.h"
#include "usart.h"
#include "Board_A_IMU.h"
#include "User_Defined_Math.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define Tx2_Func_GroundInit				\
	{																\
			&Jetson_Tx2_Initialization,	\
					&Jetson_Tx2_Get_Data,		\
					&Jetson_Tx2_Send_Data,	\
	}

typedef struct
{
	uint8_t Rx_Buffer[12];
	uint8_t Tx_Buffer[40];
	
	struct
	{
		uint8_t Team_Color; //0 for red, 1 for blue
		float Pitch_Angle;
		float Pitch_Angular_Rate;
		float Yaw_Angular_Rate;
	}Sending;
	
	struct
	{
		uint8_t Frame_ID;
		uint8_t Frame_Type; //0 for autoaiming, 1 for navigation, 2 for heart beat
		
		struct
		{
			float X_Vel;	// 0.1 m/s
			float Y_Vel; // 0.1 m/s
			int8_t Yaw_Angular_Rate;  // degree/s
			uint8_t State; // 0 for stationary, 1 for moving, 2 for spinning
		}Navigation;
			
		struct
		{
			float Yaw;
			float Pitch;
		}Auto_Aiming;
			
		struct
		{
			uint8_t a;
			uint8_t b;
			uint8_t c;
			uint8_t d;
		}Heart_Beat;
		
		union
		{
			uint32_t data[2];
			uint8_t Data[8];
		}Raw_Data;
	}Receiving;
	
}Tx2_Data_t;

typedef struct
{
	void (*Jetson_Tx2_Initialization)(void);
	void (*Jetson_Tx2_Get_Data)(void);
	void (*Jetson_Tx2_Send_Data)(void);
}Tx2_Func_t;


extern Tx2_Data_t Tx2_Data;
extern Tx2_Func_t Tx2_Func;

#endif
