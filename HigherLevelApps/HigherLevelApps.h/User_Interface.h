/**
 * @file User_Interface.c
 * @author Rongbo Hu
 * @brief Draw Referee UI
 * @version 1.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
#ifndef __USER_INTERFACE_H
#define __USER_INTERFACE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "CRC_Verification.h"
#include "cmsis_os.h"

/****************************Config Info*********************/
#define UART &huart6

/****************************SOF*********************/
#define UI_SOF 0xA5
/****************************CMD_ID********************/
#define UI_CMD_ID_Robo_Exchange 0x0301    
/****************************Content_ID********************/
#define UI_Data_ID_Del 0x0100 
#define UI_Data_ID_Draw1 0x0101
#define UI_Data_ID_Draw2 0x0102
#define UI_Data_ID_Draw5 0x0103
#define UI_Data_ID_Draw7 0x0104
#define UI_Data_ID_DrawChar 0x0110
/****************************RED_ID********************/
#define UI_Data_RobotID_RHero 1         
#define UI_Data_RobotID_REngineer 2
#define UI_Data_RobotID_RStandard1 3 //number 3
#define UI_Data_RobotID_RStandard2 4 //number 4
#define UI_Data_RobotID_RStandard3 5 //number 5
#define UI_Data_RobotID_RAerial 6
#define UI_Data_RobotID_RSentry 7
#define UI_Data_RobotID_RRadar 9
/****************************BLUE_ID********************/
#define UI_Data_RobotID_BHero 101
#define UI_Data_RobotID_BEngineer 102
#define UI_Data_RobotID_BStandard1 103 //number 3
#define UI_Data_RobotID_BStandard2 104 //number 4
#define UI_Data_RobotID_BStandard3 105 //number 5
#define UI_Data_RobotID_BAerial 106
#define UI_Data_RobotID_BSentry 107
#define UI_Data_RobotID_BRadar 109
/**************************RED_Pilot_ID************************/
#define UI_Data_CilentID_RHero 0x0101
#define UI_Data_CilentID_REngineer 0x0102
#define UI_Data_CilentID_RStandard1 0x0103 //number 3
#define UI_Data_CilentID_RStandard2 0x0104 //number 4
#define UI_Data_CilentID_RStandard3 0x0105 //number 5
#define UI_Data_CilentID_RAerial 0x0106
/***************************BLUE_Pilot_ID***********************/
#define UI_Data_CilentID_BHero 0x0165
#define UI_Data_CilentID_BEngineer 0x0166
#define UI_Data_CilentID_BStandard1 0x0167 //number 3
#define UI_Data_CilentID_BStandard2 0x0168 //number 4
#define UI_Data_CilentID_BStandard3 0x0169 //number 5
#define UI_Data_CilentID_BAerial 0x016A
/***************************Operation_Delete***************************/
#define UI_Data_Del_No_Operate 0
#define UI_Data_Del_Layer 1
#define UI_Data_Del_All 2
/***************************Operation_Graph********************/
#define UI_Graph_Add 1
#define UI_Graph_Change 2
#define UI_Graph_Del 3
/***************************Graph_Config_Type********************/
#define UI_Graph_Line 0         //ֱ
#define UI_Graph_Rectangle 1    //
#define UI_Graph_Circle 2       //
#define UI_Graph_Ellipse 3      //
#define UI_Graph_Arc 4          //
#define UI_Graph_Float 5        //
#define UI_Graph_Int 6          //
#define UI_Graph_Char 7         //
/***************************Graph_Config_Color********************/
#define UI_Color_Main 0         //
#define UI_Color_Yellow 1
#define UI_Color_Green 2
#define UI_Color_Orange 3
#define UI_Color_Purplish_red 4 //
#define UI_Color_Pink 5
#define UI_Color_Cyan 6         //
#define UI_Color_Black 7
#define UI_Color_White 8
/***************************Data_Size********************/
#define UI_Size_Frame_Header 5
#define UI_Size_CMD_ID 2
#define UI_Size_Data_Header 6
#define UI_Size_Data_Delete 2
#define UI_Size_Data_OneGraphics 15
#define UI_Size_Data_TwoGraphics 30
#define UI_Size_Data_FiveGraphics 75
#define UI_Size_Data_SevenGraphics 105
#define UI_Size_Data_CharacterGraphics 45
#define UI_Size_Frame_Tail 2   
#define UI_Size_Frame_Max 113    
#define UI_Size_Del 0x100 
#define UI_Size_Draw1 0x101
#define UI_Size_Draw2 0x102
#define UI_Size_Draw5 0x103
#define UI_Size_Draw7 0x104
#define UI_Size_DrawChar 0x110

#define CMToPixel 56

typedef struct
{
	uint8_t graphic_name[3]; 

	uint32_t operate_type:3; 
	uint32_t graphic_type:3; 
	uint32_t layer:4; 
	uint32_t color:4; 
	uint32_t start_angle:9;
	uint32_t end_angle:9;
		
	uint32_t width:10; 
	uint32_t start_x:11; 
	uint32_t start_y:11;
		
	uint32_t radius:10; 
	uint32_t end_x:11; 
	uint32_t end_y:11;
} __attribute__((packed)) Graph_t;

typedef struct
{
  uint8_t dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header + UI_Size_Data_OneGraphics + UI_Size_Frame_Tail];
	uint16_t SendingSize; 
  uint8_t PacketNumber;
	Graph_t Graph[1];
} UI_Init_OneGraphics;

typedef struct
{
  uint8_t dataBuffer[UI_Size_Frame_Header + UI_Size_Data_Header + UI_Size_Data_TwoGraphics + UI_Size_Frame_Tail];
	uint16_t SendingSize; 
  uint8_t PacketNumber;
	Graph_t Graph[2];
} UI_Init_TwoGraphics;

typedef struct
{
  uint8_t dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header + UI_Size_Data_CharacterGraphics + UI_Size_Frame_Tail];
	uint16_t SendingSize; 
  uint8_t PacketNumber;
	Graph_t Graph[1];
	uint8_t Text[30];
	uint8_t TextNumber;
} UI_Init_CharacterGraphics;

typedef struct
{
	uint16_t Robot_ID;
	uint16_t Pilot_ID;
	uint8_t Initialized_Flag;
	uint8_t Init_Func_Inner_Counter;
	uint8_t Update_Counter;
	uint8_t Global_PacketNumber;
	
	struct
	{
		float Text_1_X; //Text 1-> Cap
		float Text_1_Y;
		float Text_2_X; //Text 2-> Auto
		float Text_2_Y;
		float Text_3_X; //Text 3-> State
		float Text_3_Y;
	}Text_Location;
}UI_t;

void UI_Initialization(void);
void UI_SuperCapText_Update(void);
void UI_StateText_Update(void);
void UI_AutoText_Update(void);
void UI_SuperCapNumber_Update(void);
void UI_TextSystem_Init(void);
void UI_GuidingSystem(void);
void UI_AimingSystem(void);
void UI_Add_Frame_And_Data_Header_OneGraphics(UI_Init_OneGraphics* UIt);
void UI_Add_OneGraphics(UI_Init_OneGraphics* UIt);
void UI_Add_Frame_Tail_OneGraphics(UI_Init_OneGraphics* UIt);
void UI_Add_Frame_And_Data_Header_Character(UI_Init_CharacterGraphics *UIt);
void UI_Add_Character(UI_Init_CharacterGraphics *UIt, uint16_t TextLength);
void UI_Add_Frame_Tail_Character(UI_Init_CharacterGraphics *UIt);
extern UI_Init_OneGraphics AimingSystem;
extern UI_Init_CharacterGraphics TextSystem;
extern UI_t UI;

#endif


/*
in cm
(0, 0) to (7.7, 9.3)
(0, 0) to (9, 4.5)
(8.5, 8.5) to (10.2, 11)
(24, 8.5) to (25.7, 11)
(0, 17) to (34.3, 20)
(-8.5, 6) to (0, 0)

*/


//Bug: It will shows 9
