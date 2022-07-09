/**
 * @file CAN_Setup.h
 * @author Leo Liu
 * @brief 
 * @version 1.0
 * @date 2022/07/08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __CAN_SETUP_H
#define __CAN_SETUP_H

#include "can.h"
#include "cmsis_os.h"
#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CAN1_Type 1
#define CAN2_Type 2

#define CAN_Func_GroundInit             \
    {                                   \
        &CAN_IT_Init,                   \
            &CAN_RxMessage_Export_Data, \
						&CAN_0x1FF_Send_Data,				\
						&CAN_0x200_Send_Data,				\
						&CAN_0x2FF_Send_Data,				\
						&CAN_0x210_Send_Data,				\
			      &Check_CAN,                 \
    }

#define CAN_Data_GroundInit \
    {                       \
        {0}, {0},           \
    }

typedef struct
{
    struct
    {
        CAN_FilterTypeDef CAN_Filter;
    } CAN_FilterTypedef;

    struct
    {
        CAN_RxHeaderTypeDef CANx_RxHeader;
        uint8_t CAN_RxMessage[8];
    } CAN_RxTypedef;

} CAN_Data_t;

typedef struct
{
    void (*CAN_IT_Init)(CAN_HandleTypeDef *hcanx, uint8_t Can_type);
    void (*CAN_RxMessage_Export_Data)(CAN_HandleTypeDef *hcanx, osMessageQId CANx_Handle, uint8_t Can_type);
		void (*CAN_0x1FF_Send_Data)(CAN_HandleTypeDef *CAN_Num, int16_t Output_1, int16_t Output_2, int16_t Output_3, int16_t Output_4);
		void (*CAN_0x200_Send_Data)(CAN_HandleTypeDef *CAN_Num, int16_t Output_1, int16_t Output_2, int16_t Output_3, int16_t Output_4);
		void (*CAN_0x2FF_Send_Data)(CAN_HandleTypeDef *CAN_Num, int16_t Output_1, int16_t Output_2, int16_t Output_3, int16_t Output_4);
		void (*CAN_0x210_Send_Data)(CAN_HandleTypeDef *CAN_Num, int16_t Target_Power);
    void (*Check_CAN)(void);
} CAN_Func_t;

typedef struct
{
    uint8_t Info_Update_Flag;   
    uint16_t Info_Update_Frame; 
    uint8_t Offline_Flag;      
} CAN_Devices_t;


typedef struct
{
    CAN_RxHeaderTypeDef CAN_RxHeader;
    uint8_t CANx_Export_RxMessage[8];
} CAN_Export_Data_t;


typedef struct
{
    CAN_HandleTypeDef *Canx;
    CAN_TxHeaderTypeDef CAN_TxHeader;
    uint8_t CANx_Send_RxMessage[8];
} CAN_Send_Data_t;

extern CAN_Devices_t Monitor_CAN1, Monitor_CAN2;
extern CAN_Func_t CAN_Func;
extern osMessageQId CAN1_ReceiveHandle;
extern osMessageQId CAN2_ReceiveHandle;
extern osMessageQId CAN_SendHandle;

#endif
