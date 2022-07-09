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
 
#include "spi.h"
#include "IMU_Temp_Control.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma anon_unions
#define IMU_OFFSET_NUMBER 6
#define BOARD_DOWN 1
//#define IST8310 1
#define Kp 2.0f
#define Ki 0.01f

#define Board_A_IMU_Func_GroundInit       \
    {                                 		\
        &Board_A_IMU_Init,                \
            &Board_A_IMU_Get_Data,        \
            &Board_A_IMU_AHRS_Update,     \
            &Board_A_IMU_Attitude_Update, \
            &Board_A_IMU_Reset,           \
            &Board_A_IMU_Check,           \
						&Board_A_IMU_Temp_Control			\
    }

typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t mx;
    int16_t my;
    int16_t mz;

    int16_t temp;

    union
    {
        struct
        {
            int16_t gz;
            int16_t gy;
            int16_t gx;
        };
        int16_t Original_gX[3];
    } Original_Gyro; //原始速度

    struct
    {
        int16_t ax_offset;
        int16_t ay_offset;
        int16_t az_offset;

        int16_t gx_offset;
        int16_t gy_offset;
        int16_t gz_offset;
    } Offset; //零漂值

    HAL_StatusTypeDef Info_Update_Flag;
} IMU_Original_Data_t;

typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t mx;
    int16_t my;
    int16_t mz;

    float temp;

    union
    {
        struct
        {
            float wz;
            float wy;
            float wx;
        };
        float Real_wX[3];
    } Real_Gyro;

    union
    {
        struct
        {
            float gz;
            float gy;
            float gx;
        };
        float Original_gX[3];
    } Gyro;

    float vx;
    float vy;
    float vz;

    union
    {
        struct
        {
            float yaw;
            float pit;
            float rol;
        };
        float Real_Angle[3];
    } Axis;

    union
    {
        struct
        {
            float lastyaw;
            float lastpit;
            float lastrol;
        };
        float Last_Angle[3];
    } Last_Axis;
    int16_t turnCount[3];
} IMU_Calculated_Data_t;

typedef struct
{
    float Current_Angle[3];
    float SpeedLPF[3];
    float Total_Angle[3];
    float temp;
    uint8_t Temp_ReachFlag;     
    uint8_t IMU_Init_Condition; 
    uint8_t Info_Update_Flag; 
    uint8_t Offline_Flag;
} IMU_Export_t;

typedef struct
{
	void (*Board_A_IMU_Init)(void);
  void (*Board_A_IMU_Get_Data)(void);
  void (*Board_A_IMU_AHRS_Update)(void);
  void (*Board_A_IMU_Attitude_Update)(void);
  void (*Board_A_IMU_Reset)(IMU_Export_t *IMU_Export);
  void (*Board_A_IMU_Check)(void);
	void (*Board_A_IMU_Temp_Control)(void);
}Board_A_IMU_Func_t;

extern IMU_Original_Data_t IMU_Original_Data;
extern IMU_Calculated_Data_t IMU_Calculated_Data;
extern IMU_Export_t IMU_Export;
extern Board_A_IMU_Func_t Board_A_IMU_Func;
