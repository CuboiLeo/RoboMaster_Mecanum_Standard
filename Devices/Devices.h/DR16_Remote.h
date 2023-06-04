/**
 * @file DR16_Remote.h
 * @author Leo Liu
 * @brief communicate and obtain data from DR16
 * @version 1.0
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __DR16_REMOTE_H
#define __DR16_REMOTE_H

#include "dma.h"
#include "usart.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DR16_BUFFER_NUMBER 22 //Total buffer number
#define DR16_BUFFER_TRUE_NUMBER 18 //Useful data
#define DR16_BUFFER_LAST_NUMBER 4 //Verification bits

#define DR16_JOYSTICK_MAX 660 //Joystick maximum value

#define TOTAL_KEY_NUMBER 16 //Total number of keys that are provided

//Switch states and corresponding value
#define SWITCH_UP 	((uint16_t)1)
#define SWITCH_MID 	((uint16_t)3)
#define SWITCH_DOWN ((uint16_t)2)

//Bit shift for keys
#define KEY_W ((uint16_t)0x01<<0)
#define KEY_S ((uint16_t)0x01<<1)
#define KEY_A ((uint16_t)0x01<<2)
#define KEY_D ((uint16_t)0x01<<3)
#define KEY_SHIFT ((uint16_t)0x01<<4)
#define KEY_CTRL ((uint16_t)0x01<<5)   
#define KEY_Q ((uint16_t)0x01<<6)      
#define KEY_E ((uint16_t)0x01<<7)      
#define KEY_R ((uint16_t)0x01<<8)    
#define KEY_F ((uint16_t)0x01<<9)
#define KEY_G ((uint16_t)0x01<<10)     
#define KEY_Z ((uint16_t)0x01<<11)    
#define KEY_X ((uint16_t)0x01<<12)
#define KEY_C ((uint16_t)0x01<<13)
#define KEY_V ((uint16_t)0x01<<14)
#define KEY_B ((uint16_t)0x01<<15)  

#define DR16_Func_GroundInit         \
    {                                \
        &DR16_Handler,               \
            &DR16_USART_Receive_DMA, \
            &DR16_Data_Process,   	 \
            &Keyboard_Data_Process,  \
						&Key_State_Detect,       \
						&Key_State_Clear,				 \
						&Check_DR16,						 \
    }
		
typedef struct
{
	uint8_t Hold_Flag;
	uint8_t Switch_Flag;
	uint8_t State_Count;
	uint8_t Current_State;
	uint8_t Prev_State;
}Key_State;
	
typedef struct
{	
	uint8_t DR16_Buffer[DR16_BUFFER_NUMBER];
	
	struct
	{
		int16_t x;
		int16_t y;
		bool Left_Click;
		bool Right_Click;
		int16_t Click_Counter;
	}Mouse;

	struct
	{
		int16_t Joystick_Left_Vx;
		int16_t Joystick_Left_Vy;
		int16_t Joystick_Right_Vx;
		int16_t Joystick_Right_Vy;
		uint8_t Left_Switch;
		uint8_t Right_Switch;
		int16_t Dial_Wheel;
	}Remote_Control;
	
	struct
	{
		uint16_t Buffer;
		Key_State Press_W;
		Key_State Press_S;
		Key_State Press_A;
		Key_State Press_D;
		Key_State Press_Shift;
		Key_State Press_Ctrl;
		Key_State Press_Q;
		Key_State Press_E;
		Key_State Press_R;
		Key_State Press_F;
		Key_State Press_G;
		Key_State Press_Z;
		Key_State Press_X;
		Key_State Press_C;
		Key_State Press_V;
		Key_State Press_B;
	}Keyboard;
	
	uint16_t Info_Update_Frame;
	uint8_t Offline_Flag;
}DR16_Export_Data_t;

typedef struct
{
    void (*DR16_Handler)(UART_HandleTypeDef *huart);
    void (*DR16_USART_Receive_DMA)(UART_HandleTypeDef *huartx);
    void (*DR16_Data_Process)(uint8_t *pData);
    void (*Keyboard_Data_Process)(void);
    void (*Key_State_Detect)(Key_State *Key);
		void (*Key_State_Clear)(Key_State *Key);
		void (*Check_DR16)(void);
}DR16_Func_t;

extern DR16_Export_Data_t DR16_Export_Data;
extern DR16_Func_t DR16_Func;

#endif
