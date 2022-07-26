/**
 * @file WT901_IMU.h
 * @author Leo Liu
 * @brief header file for WT901 IMu
 * @version 1.0
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __WT901_IMU_H
#define __WT901_IMU_H 

#include <stdint.h>
#include "i2c.h"
#include "User_Defined_Math.h"
#include "I2C_Setup.h"

#define DEV_ADDR_WT901	0x50
#define KEY_WT901				0x69

#define SAVE_WT901 			0x00
#define CALSW_WT901 		0x01
#define RSW_WT901 			0x02
#define RRATE_WT901			0x03
#define AXIS6_WT901			0x24

#define AX_WT901					0x34
#define AY_WT901					0x35
#define AZ_WT901					0x36
#define GX_WT901					0x37
#define GY_WT901					0x38
#define GZ_WT901					0x39
#define HX_WT901					0x3a
#define HY_WT901					0x3b
#define HZ_WT901					0x3c			
#define ROLL_WT901				0x3d
#define PITCH_WT901				0x3e
#define YAW_WT901					0x3f
#define TEMP_WT901				0x40

#define WT901_IMU_Func_GroundInit		\
		{																	\
				&WT901_IMU_Init,						\
						&WT901_IMU_Read_Data,		\
		}
typedef struct
{
		struct
		{
			int16_t Ax;
			int16_t Ay;
			int16_t Az;
			int16_t Gx;
			int16_t Gy;
			int16_t Gz;
			int16_t Hx;
			int16_t Hy;
			int16_t Hz;
			int16_t Yaw;
			int16_t Pitch;
			int16_t Roll;
			int16_t Temperature;
		}Raw_Data;
		
		struct
		{
			float Ax;
			float Ay;
			float Az;
			float Gx;
			float Gy;
			float Gz;
			float Hx;
			float Hy;
			float Hz;
			float Yaw;
			float Pitch;
			float Roll;
			float Temperature;
		}Calc_Data;
    
		struct
		{
			float Yaw;
			float Pitch;
			float Roll;
						
			float Prev_Yaw;
			float Total_Yaw;
			float Turn_Count;
			
			float Gyro_Yaw;
			float Gyro_Pitch;
			float Gyro_Roll;
			
			float Temperature;
		}Export_Data;
		
		uint8_t Check_Offline;
		uint8_t Offline_Flag;
} WT901_IMU_t;

typedef struct
{
	void (*WT901_IMU_Init)(void);
	void (*WT901_IMU_Read_Data)(WT901_IMU_t *WT901_IMU);
}WT901_IMU_Func_t;

extern WT901_IMU_t WT901_IMU;
extern WT901_IMU_Func_t WT901_IMU_Func;
extern FusionAhrs WT901_IMU_AHRS;

#endif
