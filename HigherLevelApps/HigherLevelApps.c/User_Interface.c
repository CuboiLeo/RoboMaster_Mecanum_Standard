/**
 * @file User_Interface.c
 * @author Rongbo Hu
 * @brief Draw Referee UI
 * @version 1.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
 
#include "User_Interface.h"

UI_Init_OneGraphics AimingSystem;
UI_Init_CharacterGraphics TextSystem;
UI_t UI;

uint8_t TextInit[30] = {0};
//User Interface Application////////////////////////////////////////////////////////////////////////

void UI_Initialization(void)
{
	UI.Text_Location.Text_1_X = 16.5; //Text 1-> Cap
	UI.Text_Location.Text_1_Y = 4;
	UI.Text_Location.Text_2_X = 25;  //Text 2-> Auto
	UI.Text_Location.Text_2_Y = 15;
	UI.Text_Location.Text_3_X = 25; //Text 3-> State
	UI.Text_Location.Text_3_Y = 14;
}

void UI_SuperCapText_Update()
{
	//memcpy(TextSystem.Text, Text, 30);
	TextSystem.Graph->operate_type = UI_Graph_Change;
	TextSystem.Graph->graphic_name[0] = 'C';
	TextSystem.Graph->graphic_name[1] = 'a';
	TextSystem.Graph->graphic_name[2] = 'p';
	TextSystem.Graph->end_angle = TextSystem.TextNumber;
	//memcpy(TextSystem.Text, "Cap", 3);
	TextSystem.Graph->start_x = UI.Text_Location.Text_1_X * CMToPixel;
	TextSystem.Graph->start_y = UI.Text_Location.Text_1_Y * CMToPixel;
	TextSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_Character(&TextSystem);
	UI_Add_Character(&TextSystem, 30);
  UI_Add_Frame_Tail_Character(&TextSystem);
	osDelay(200);
	HAL_UART_Transmit(UART, TextSystem.dataBuffer, sizeof(TextSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	memcpy(TextSystem.Text, TextInit, 30);
}

void UI_StateText_Update()
{
	TextSystem.Graph->operate_type = UI_Graph_Change;
	TextSystem.Graph->graphic_name[0] = 'S';
	TextSystem.Graph->graphic_name[1] = 't';
	TextSystem.Graph->graphic_name[2] = 'a';
	TextSystem.Graph->end_angle = TextSystem.TextNumber;
	//memcpy(TextSystem.Text, "Cap", 3);
	TextSystem.Graph->start_x = UI.Text_Location.Text_3_X * CMToPixel;
	TextSystem.Graph->start_y = UI.Text_Location.Text_3_Y * CMToPixel;
	TextSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_Character(&TextSystem);
	UI_Add_Character(&TextSystem, 30);
  UI_Add_Frame_Tail_Character(&TextSystem);
	osDelay(200);
	HAL_UART_Transmit(UART, TextSystem.dataBuffer, sizeof(TextSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	memcpy(TextSystem.Text, TextInit, 30);
}

void UI_AutoText_Update()
{
	//memcpy(TextSystem.Text, Text, 30);
	TextSystem.Graph->operate_type = UI_Graph_Change;
	TextSystem.Graph->graphic_name[0] = 'A';
	TextSystem.Graph->graphic_name[1] = 'u';
	TextSystem.Graph->graphic_name[2] = 't';
	TextSystem.Graph->end_angle = TextSystem.TextNumber;
	//memcpy(TextSystem.Text, "Cap", 3);
	TextSystem.Graph->start_x = UI.Text_Location.Text_2_X * CMToPixel;
	TextSystem.Graph->start_y = UI.Text_Location.Text_2_Y * CMToPixel;
	TextSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_Character(&TextSystem);
	UI_Add_Character(&TextSystem, 30);
  UI_Add_Frame_Tail_Character(&TextSystem);
	osDelay(200);
	HAL_UART_Transmit(UART, TextSystem.dataBuffer, sizeof(TextSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	memcpy(TextSystem.Text, TextInit, 30);
}

void UI_TextSystem_Init()
{
	TextSystem.Graph->graphic_name[0] = '0';
	TextSystem.Graph->graphic_name[1] = '0';
	TextSystem.Graph->graphic_name[2] = '0';
	TextSystem.Graph->operate_type = UI_Graph_Add;
	TextSystem.Graph->graphic_type = UI_Graph_Char;
	TextSystem.Graph->layer = 8;
	TextSystem.Graph->color = UI_Color_Orange;
	TextSystem.Graph->start_angle = 20;    //Size of Char
	TextSystem.Graph->end_angle = 0;	    //Length of Char
	TextSystem.Graph->width = 3;
	TextSystem.Graph->start_x = 0;
	TextSystem.Graph->start_y = 0;
	TextSystem.Graph->radius = 0;
	TextSystem.Graph->end_x = 0;
	TextSystem.Graph->end_y = 0;
	
	TextSystem.SendingSize = UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header + UI_Size_Data_CharacterGraphics + UI_Size_Frame_Tail;
	
	memcpy(TextSystem.Text, TextInit, 30);
	TextSystem.Graph->graphic_name[0] = 'C';
	TextSystem.Graph->graphic_name[1] = 'a';
	TextSystem.Graph->graphic_name[2] = 'p';
	TextSystem.Graph->end_angle = 3;
	memcpy(TextSystem.Text, "Cap", 3);
	TextSystem.Graph->start_x = UI.Text_Location.Text_1_X * CMToPixel;
	TextSystem.Graph->start_y = UI.Text_Location.Text_1_Y * CMToPixel;
	TextSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_Character(&TextSystem);
	UI_Add_Character(&TextSystem, 30);
  UI_Add_Frame_Tail_Character(&TextSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, TextSystem.dataBuffer, sizeof(TextSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	memcpy(TextSystem.Text, TextInit, 30);
	TextSystem.Graph->graphic_name[0] = 'A';
	TextSystem.Graph->graphic_name[1] = 'u';
	TextSystem.Graph->graphic_name[2] = 't';
	TextSystem.Graph->end_angle = 4;	    //Length of Char
	memcpy(TextSystem.Text, "Auto", 4);
	TextSystem.Graph->start_x = UI.Text_Location.Text_2_X * CMToPixel;
	TextSystem.Graph->start_y = UI.Text_Location.Text_2_Y * CMToPixel;
	TextSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_Character(&TextSystem);
	UI_Add_Character(&TextSystem, 30);
  UI_Add_Frame_Tail_Character(&TextSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, TextSystem.dataBuffer, sizeof(TextSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	memcpy(TextSystem.Text, TextInit, 30);
	TextSystem.Graph->graphic_name[0] = 'S';
	TextSystem.Graph->graphic_name[1] = 't';
	TextSystem.Graph->graphic_name[2] = 'a';
	TextSystem.Graph->end_angle = 5;	    //Length of Char
	memcpy(TextSystem.Text, "State", 5);
	TextSystem.Graph->start_x = UI.Text_Location.Text_3_X * CMToPixel;
	TextSystem.Graph->start_y = UI.Text_Location.Text_3_Y * CMToPixel;
	TextSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_Character(&TextSystem);
	UI_Add_Character(&TextSystem, 30);
  UI_Add_Frame_Tail_Character(&TextSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, TextSystem.dataBuffer, sizeof(TextSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
}

void UI_GuidingSystem()
{
	//Two Line System
	float LineOneStartX = 11.3; //7
	float LineOneStartY = 0;
	float LineOneEndX = 13.9;  //14
	float LineOneEndY = 4.72;  //2
	float X_offSet = 0;
	
	AimingSystem.Graph->graphic_name[0] = '0';
	AimingSystem.Graph->graphic_name[1] = '0';
	AimingSystem.Graph->graphic_name[2] = '0';
	AimingSystem.Graph->operate_type = UI_Graph_Add;
	AimingSystem.Graph->graphic_type = UI_Graph_Line;
	AimingSystem.Graph->layer = 7;
	AimingSystem.Graph->color = UI_Color_Purplish_red;
	AimingSystem.Graph->start_angle = 0;
	AimingSystem.Graph->end_angle = 0;
	AimingSystem.Graph->width = 5;
	AimingSystem.Graph->start_x = 0;
	AimingSystem.Graph->start_y = 0;
	AimingSystem.Graph->radius = 0;
	AimingSystem.Graph->end_x = 0;
	AimingSystem.Graph->end_y = 0;
	
	AimingSystem.SendingSize = UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header + UI_Size_Data_OneGraphics + UI_Size_Frame_Tail;
	
	AimingSystem.Graph->graphic_name[0] = 'G';
	AimingSystem.Graph->graphic_name[1] = 'u';
	AimingSystem.Graph->graphic_name[2] = '1';
	AimingSystem.Graph->start_x = (uint32_t) (LineOneStartX * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (LineOneStartY * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) (LineOneEndX * CMToPixel - X_offSet);
	AimingSystem.Graph->end_y = (uint32_t) (LineOneEndY * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 5000);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'G';
	AimingSystem.Graph->graphic_name[1] = 'u';
	AimingSystem.Graph->graphic_name[2] = '2';
	AimingSystem.Graph->start_x = (uint32_t) ((34.3f - LineOneStartX) * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (LineOneStartY * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) ((34.3f - LineOneEndX) * CMToPixel - X_offSet); 
	AimingSystem.Graph->end_y = (uint32_t) (LineOneEndY * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 5000);
	UI.Global_PacketNumber++;
}

void UI_AimingSystem()
{
	//Three square shape config
	float ShapeOneBottom = 6.85;
	float ShapeOneTop = 8.57;
	float ShapeOneDistance = (17.15-0.9);
	float ShapeTwoBottom = 6.62;
	float ShapeTwoTop = 6.93;
	float ShapeTwoDistance = (17.15-0.15);
	float ShapeThreeBottom = 7;
	float ShapeThreeTop = 7.6;
	float ShapeThreeDistance = (17.15-0.225);
	float X_offSet = 0;
	
	AimingSystem.Graph->graphic_name[0] = '0';
	AimingSystem.Graph->graphic_name[1] = '0';
	AimingSystem.Graph->graphic_name[2] = '0';
	AimingSystem.Graph->operate_type = UI_Graph_Add;
	AimingSystem.Graph->graphic_type = UI_Graph_Line;
	AimingSystem.Graph->layer = 9;
	AimingSystem.Graph->color = UI_Color_White;
	AimingSystem.Graph->start_angle = 0;
	AimingSystem.Graph->end_angle = 0;
	AimingSystem.Graph->width = 3;
	AimingSystem.Graph->start_x = 0;
	AimingSystem.Graph->start_y = 0;
	AimingSystem.Graph->radius = 0;
	AimingSystem.Graph->end_x = 0;
	AimingSystem.Graph->end_y = 0;
	
	AimingSystem.SendingSize = UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header + UI_Size_Data_OneGraphics + UI_Size_Frame_Tail;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '1';
	AimingSystem.Graph->start_x = (uint32_t) (ShapeOneDistance * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (ShapeOneBottom * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) (ShapeOneDistance * CMToPixel - X_offSet);
	AimingSystem.Graph->end_y = (uint32_t) (ShapeOneTop * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '2';
	AimingSystem.Graph->start_x = (uint32_t) ((34.3f - ShapeOneDistance) * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (ShapeOneBottom * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) ((34.3f - ShapeOneDistance) * CMToPixel - X_offSet);
	AimingSystem.Graph->end_y = (uint32_t) (ShapeOneTop * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '3';
	AimingSystem.Graph->start_x = (uint32_t) (ShapeOneDistance * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (ShapeOneTop * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) (ShapeTwoDistance * CMToPixel - X_offSet);
	AimingSystem.Graph->end_y = (uint32_t) (ShapeOneTop * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '4';
	AimingSystem.Graph->start_x = (uint32_t) ((34.3f - ShapeOneDistance) * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (ShapeOneTop * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) ((34.3f - ShapeTwoDistance) * CMToPixel - X_offSet);
	AimingSystem.Graph->end_y = (uint32_t) (ShapeOneTop * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '5';
	AimingSystem.Graph->start_x = (uint32_t) (ShapeTwoDistance * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (ShapeTwoBottom * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) (ShapeTwoDistance * CMToPixel - X_offSet);
	AimingSystem.Graph->end_y = (uint32_t) (ShapeTwoTop * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '6';
	AimingSystem.Graph->start_x = (uint32_t) ((34.3f - ShapeTwoDistance) * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (ShapeTwoBottom * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) ((34.3f - ShapeTwoDistance) * CMToPixel - X_offSet);
	AimingSystem.Graph->end_y = (uint32_t) (ShapeTwoTop * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;

	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '7';
	AimingSystem.Graph->start_x = (uint32_t) (ShapeTwoDistance * CMToPixel - X_offSet);
	AimingSystem.Graph->start_y = (uint32_t) (ShapeTwoTop * CMToPixel);
	AimingSystem.Graph->end_x = (uint32_t) (ShapeThreeDistance * CMToPixel - X_offSet);
	AimingSystem.Graph->end_y = (uint32_t) (ShapeThreeBottom * CMToPixel);
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '8';
	AimingSystem.Graph->start_x = (34.3f - ShapeTwoDistance) * CMToPixel - X_offSet;
	AimingSystem.Graph->start_y = ShapeTwoTop * CMToPixel;
	AimingSystem.Graph->end_x = (34.3f - ShapeThreeDistance) * CMToPixel - X_offSet;
	AimingSystem.Graph->end_y = ShapeThreeBottom * CMToPixel;
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '9';
	AimingSystem.Graph->start_x = ShapeThreeDistance * CMToPixel - X_offSet;
	AimingSystem.Graph->start_y = ShapeThreeBottom * CMToPixel;
	AimingSystem.Graph->end_x = ShapeThreeDistance * CMToPixel - X_offSet;
	AimingSystem.Graph->end_y = ShapeThreeTop * CMToPixel;
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
	
	AimingSystem.Graph->graphic_name[0] = 'A';
	AimingSystem.Graph->graphic_name[1] = 'i';
	AimingSystem.Graph->graphic_name[2] = '0';
	AimingSystem.Graph->start_x = (34.3f - ShapeThreeDistance) * CMToPixel - X_offSet;
	AimingSystem.Graph->start_y = ShapeThreeBottom * CMToPixel;
	AimingSystem.Graph->end_x = (34.3f - ShapeThreeDistance) * CMToPixel - X_offSet;
	AimingSystem.Graph->end_y = ShapeThreeTop * CMToPixel;
	AimingSystem.PacketNumber = UI.Global_PacketNumber;
	UI_Add_Frame_And_Data_Header_OneGraphics(&AimingSystem);
	UI_Add_OneGraphics(&AimingSystem);
  UI_Add_Frame_Tail_OneGraphics(&AimingSystem);
	osDelay(150);
	HAL_UART_Transmit(UART, AimingSystem.dataBuffer, sizeof(AimingSystem.dataBuffer), 500);
	UI.Global_PacketNumber++;
}

void UI_Add_Frame_And_Data_Header_OneGraphics(UI_Init_OneGraphics* UIt)
{	
  UIt->dataBuffer[0] = UI_SOF;
	UIt->dataBuffer[1] = (UI_Size_Data_Header + UI_Size_Data_OneGraphics) & 0xFF;
	UIt->dataBuffer[2] = (UI_Size_Data_Header + UI_Size_Data_OneGraphics) >> 8;
  UIt->dataBuffer[3] = UIt->PacketNumber;
  CRC_Verif_Func.Append_CRC8_Check_Sum(UIt->dataBuffer, UI_Size_Frame_Header);
	
	UIt->dataBuffer[UI_Size_Frame_Header] = UI_CMD_ID_Robo_Exchange & 0xFF;
	UIt->dataBuffer[UI_Size_Frame_Header+1] = UI_CMD_ID_Robo_Exchange >> 8;
	
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 0] = UI_Data_ID_Draw1 & 0xFF;
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 1] = UI_Data_ID_Draw1 >> 8;
	
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 2] = UI.Robot_ID & 0xFF;
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 3] = UI.Robot_ID >> 8;
	
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 4] = UI.Pilot_ID & 0xFF;
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 5] = UI.Pilot_ID >> 8;
}

void UI_Add_OneGraphics(UI_Init_OneGraphics* UIt)
{
	uint8_t* byteArray = (uint8_t*)&(UIt->Graph);
  memcpy(&UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header], byteArray, UI_Size_Data_OneGraphics);
}

void UI_Add_Frame_Tail_OneGraphics(UI_Init_OneGraphics* UIt)
{
	CRC_Verif_Func.Append_CRC16_Check_Sum(UIt->dataBuffer, UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header + UI_Size_Data_OneGraphics + UI_Size_Frame_Tail);
}
void UI_Add_Frame_And_Data_Header_Character(UI_Init_CharacterGraphics *UIt)
{
	UIt->dataBuffer[0] = UI_SOF;
	UIt->dataBuffer[1] = (UI_Size_Data_Header + UI_Size_Data_CharacterGraphics) & 0xFF;
	UIt->dataBuffer[2] = (UI_Size_Data_Header + UI_Size_Data_CharacterGraphics) >> 8;
  UIt->dataBuffer[3] = UIt->PacketNumber;
  CRC_Verif_Func.Append_CRC8_Check_Sum(UIt->dataBuffer, UI_Size_Frame_Header);
	
	UIt->dataBuffer[UI_Size_Frame_Header] = UI_CMD_ID_Robo_Exchange & 0xFF;
	UIt->dataBuffer[UI_Size_Frame_Header+1] = UI_CMD_ID_Robo_Exchange >> 8;
	
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 0] = UI_Data_ID_DrawChar & 0xFF;
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 1] = UI_Data_ID_DrawChar >> 8;
	
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 2] = UI.Robot_ID & 0xFF;
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 3] = UI.Robot_ID >> 8;
	
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 4] = UI.Pilot_ID & 0xFF;
	UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + 5] = UI.Pilot_ID >> 8;
}

void UI_Add_Character(UI_Init_CharacterGraphics *UIt, uint16_t TextLength)
{
	uint8_t* byteArray = (uint8_t*)&(UIt->Graph);
	//uint8_t* TextbyteArray = (uint8_t*)&(UIt->Text);
  memcpy(&UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header], byteArray, 15);
	memcpy(&UIt->dataBuffer[UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header + 15], UIt->Text, TextLength);
}

void UI_Add_Frame_Tail_Character(UI_Init_CharacterGraphics *UIt)
{
	CRC_Verif_Func.Append_CRC16_Check_Sum(UIt->dataBuffer, UI_Size_Frame_Header + UI_Size_CMD_ID + UI_Size_Data_Header + UI_Size_Data_CharacterGraphics + UI_Size_Frame_Tail);
}
