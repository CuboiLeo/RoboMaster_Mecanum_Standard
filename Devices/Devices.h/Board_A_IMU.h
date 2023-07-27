/**
 * @file Board_A_IMU.h
 * @author Leo Liu
 * @brief communicate and obtain data from board A IMU_Calculated_Data
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __BOARD_A_IMU_H
#define __BOARD_A_IMU_H

#include "spi.h"
#include "IMU_Temp_Control.h"
#include "Fusion.h"
#include "MPU6500_Reg.h"
#include "IST8310_Reg.h"
#include "User_Defined_Math.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//#define BOARD_A_IMU_USE_MAGN 1 //Uncomment this to use magnetometer

#define Board_A_IMU_Func_GroundInit       \
    {                                 		\
        &Board_A_IMU_Init,                \
						&Board_A_IMU_Calibrate,				\
						&Board_A_IMU_Read_Data,				\
            &Board_A_IMU_Calc_Angle,      \
            &Board_A_IMU_Reset,           \
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
			int16_t Mx;
			int16_t My;
			int16_t Mz;
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
			float Mx;
			float My;
			float Mz;
		}Offset;
		
		struct
		{
			float Ax;
			float Ay;
			float Az;
			float Gx;
			float Gy;
			float Gz;
			float Mx;
			float My;
			float Mz;
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
		
		struct
		{
			float Now_Time;
			float Prev_Time;
			float Period;
		}Sample;
		
		uint8_t Temperature_Reached_Flag;
		uint8_t Calibrated_Flag;
		uint8_t Offline_Flag;
}Board_A_IMU_t;

typedef struct
{
	void (*Board_A_IMU_Init)(void);
	void (*Board_A_IMU_Calibrate)(Board_A_IMU_t *Board_A_IMU);
	void (*Board_A_IMU_Read_Data)(Board_A_IMU_t *Board_A_IMU);
  void (*Board_A_IMU_Calc_Angle)(Board_A_IMU_t *Board_A_IMU);
  void (*Board_A_IMU_Reset)(Board_A_IMU_t *IMU_Export);
}Board_A_IMU_Func_t;

extern Board_A_IMU_t Board_A_IMU;
extern Board_A_IMU_Func_t Board_A_IMU_Func;
extern FusionAhrs Board_A_IMU_AHRS;

#endif
