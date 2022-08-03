/**
 * @file MPU6050_IMU.c
 * @author Leo Liu
 * @brief external MPU6050 communication and calculation
 * @version 1.0
 * @date 2022-07-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "MPU6050_IMU.h"

MPU6050_IMU_t MPU6050_IMU;
FusionAhrs MPU6050_IMU_AHRS;

static uint8_t Buffer[14];

void MPU6050_IMU_Init(void);
void MPU6050_IMU_Calibrate(MPU6050_IMU_t *MPU6050_IMU);
void MPU6050_IMU_Read_Data(MPU6050_IMU_t *MPU6050_IMU);
void MPU6050_IMU_Calc_Angle(MPU6050_IMU_t *MPU6050_IMU);

MPU6050_IMU_Func_t MPU6050_IMU_Func = MPU6050_IMU_Func_GroundInit;
#undef MPU6050_IMU_Func_GroundInit

//Initialize MPU6050
void MPU6050_IMU_Init(void)
{
	uint8_t Temp_Data;
	uint8_t Check_Offline;
	
	I2C_Read_8_Bytes(DEV_ADDR, WHO_AM_I, 1, &Check_Offline);
	
	Temp_Data = 0x80;
	I2C_Write_8_Bytes(DEV_ADDR,	PWR_MGMT_1, 1, &Temp_Data);
	
	Temp_Data = 0x07;
	I2C_Write_8_Bytes(DEV_ADDR,	SMPLRT_DIV, 1, &Temp_Data);
	
	Temp_Data = 0x00;
	I2C_Write_8_Bytes(DEV_ADDR,	GYRO_CONFIG, 1, &Temp_Data);
	
	Temp_Data = 0x01;
	I2C_Write_8_Bytes(DEV_ADDR,	ACCEL_CONFIG, 1, &Temp_Data);
	
	Temp_Data = 0x00;
	I2C_Write_8_Bytes(DEV_ADDR,	PWR_MGMT_1, 1, &Temp_Data);
	
	if(Check_Offline == 0x68)
		MPU6050_IMU.Offline_Flag = 0;
	else
		MPU6050_IMU.Offline_Flag = 1;
}

//Calibrate IMU data to reduce zero deviation
void MPU6050_IMU_Calibrate(MPU6050_IMU_t *MPU6050_IMU)
{
	if(MPU6050_IMU->Calibrated_Flag == 0)
	{
		float Buffer[6];
		
		for(int i = 0; i < CALIBRATION_SAMPLE; i++)
		{
			MPU6050_IMU_Read_Data(MPU6050_IMU);
			Buffer[0] += MPU6050_IMU->Calc_Data.Ax;
			Buffer[1] += MPU6050_IMU->Calc_Data.Ay;
			Buffer[2] += MPU6050_IMU->Calc_Data.Az;
			Buffer[3] += MPU6050_IMU->Calc_Data.Gx;
			Buffer[4] += MPU6050_IMU->Calc_Data.Gy;
			Buffer[5] += MPU6050_IMU->Calc_Data.Gz;
			HAL_Delay(1);
		}

		MPU6050_IMU->Offset.Ax = Buffer[0] / CALIBRATION_SAMPLE;
		MPU6050_IMU->Offset.Ay = Buffer[1] / CALIBRATION_SAMPLE;
		MPU6050_IMU->Offset.Az = Buffer[2] / CALIBRATION_SAMPLE - 1.0f;
		MPU6050_IMU->Offset.Gx = Buffer[3] / CALIBRATION_SAMPLE;
		MPU6050_IMU->Offset.Gy = Buffer[4] / CALIBRATION_SAMPLE;
		MPU6050_IMU->Offset.Gz = Buffer[5] / CALIBRATION_SAMPLE;
		
		MPU6050_IMU->Calibrated_Flag = 1;
	}
	else
		;
}
		

void MPU6050_IMU_Read_Data(MPU6050_IMU_t *MPU6050_IMU)
{
	//Calculate sample period
	MPU6050_IMU->Sample.Now_Time = HAL_GetTick() / 1000.0f;
	MPU6050_IMU->Sample.Period = MPU6050_IMU->Sample.Now_Time - MPU6050_IMU->Sample.Prev_Time;
	MPU6050_IMU->Sample.Prev_Time = MPU6050_IMU->Sample.Now_Time;
	
	I2C_Read_8_Bytes(DEV_ADDR, ACCEL_XOUT_H, 14, Buffer);

	MPU6050_IMU->Raw_Data.Ax = ((int16_t)Buffer[0] << 8) | Buffer[1];
	MPU6050_IMU->Raw_Data.Ay = ((int16_t)Buffer[2] << 8) | Buffer[3];
	MPU6050_IMU->Raw_Data.Az = ((int16_t)Buffer[4] << 8) | Buffer[5];
	MPU6050_IMU->Raw_Data.Temperature = ((int16_t)Buffer[6]) << 8 | Buffer[7];
	MPU6050_IMU->Raw_Data.Gx = ((int16_t)Buffer[8] << 8) | Buffer[9];
	MPU6050_IMU->Raw_Data.Gy = ((int16_t)Buffer[10] << 8) | Buffer[11];
	MPU6050_IMU->Raw_Data.Gz = ((int16_t)Buffer[12] << 8) | Buffer[13];
	
	MPU6050_IMU->Calc_Data.Ax = MPU6050_IMU->Raw_Data.Ax / 16384.0f;
	MPU6050_IMU->Calc_Data.Ay = MPU6050_IMU->Raw_Data.Ay / 16384.0f;
	MPU6050_IMU->Calc_Data.Az = MPU6050_IMU->Raw_Data.Az / 16384.0f;
	MPU6050_IMU->Calc_Data.Gx = MPU6050_IMU->Raw_Data.Gx / 131.0f;
	MPU6050_IMU->Calc_Data.Gy = MPU6050_IMU->Raw_Data.Gy / 131.0f;
	MPU6050_IMU->Calc_Data.Gz = MPU6050_IMU->Raw_Data.Gz / 131.0f;
	MPU6050_IMU->Calc_Data.Temperature = MPU6050_IMU->Raw_Data.Temperature / 340.0f + 36.53f;
	
	if(MPU6050_IMU->Calibrated_Flag == 1)
	{
		MPU6050_IMU->Calc_Data.Ax -= MPU6050_IMU->Offset.Ax;
		MPU6050_IMU->Calc_Data.Ay -= MPU6050_IMU->Offset.Ay;
		MPU6050_IMU->Calc_Data.Az -= MPU6050_IMU->Offset.Az;
		MPU6050_IMU->Calc_Data.Gx -= MPU6050_IMU->Offset.Gx;
		MPU6050_IMU->Calc_Data.Gy -= MPU6050_IMU->Offset.Gy;
		MPU6050_IMU->Calc_Data.Gz -= MPU6050_IMU->Offset.Gz;
	}
}

void MPU6050_IMU_Calc_Angle(MPU6050_IMU_t *MPU6050_IMU)
{
	//Use fusion for AHRS estimation
	const FusionVector MPU6050_IMU_Accel = {MPU6050_IMU->Calc_Data.Ax, MPU6050_IMU->Calc_Data.Ay, MPU6050_IMU->Calc_Data.Az};
	const FusionVector MPU6050_IMU_Gyro = {MPU6050_IMU->Calc_Data.Gx, MPU6050_IMU->Calc_Data.Gy, MPU6050_IMU->Calc_Data.Gz}; 
	
	FusionAhrsUpdateNoMagnetometer(&MPU6050_IMU_AHRS, MPU6050_IMU_Gyro, MPU6050_IMU_Accel, MPU6050_IMU->Sample.Period);
	
	const FusionEuler MPU6050_IMU_Euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&MPU6050_IMU_AHRS));
	
	MPU6050_IMU->Export_Data.Prev_Yaw = MPU6050_IMU->Export_Data.Yaw;
	
	MPU6050_IMU->Export_Data.Yaw = MPU6050_IMU_Euler.angle.yaw;
	MPU6050_IMU->Export_Data.Pitch = MPU6050_IMU_Euler.angle.pitch;
	MPU6050_IMU->Export_Data.Roll = MPU6050_IMU_Euler.angle.roll;
	MPU6050_IMU->Export_Data.Gyro_Yaw = MPU6050_IMU->Calc_Data.Gx / 6.0f; // degree/s to RPM
	MPU6050_IMU->Export_Data.Gyro_Pitch = MPU6050_IMU->Calc_Data.Gy / 6.0f; // degree/s to RPM
	MPU6050_IMU->Export_Data.Gyro_Roll = MPU6050_IMU->Calc_Data.Gz / 6.0f; // degree/s to RPM
	MPU6050_IMU->Export_Data.Temperature = MPU6050_IMU->Calc_Data.Temperature;
	
	//Record number of turns
	if((MPU6050_IMU->Export_Data.Yaw - MPU6050_IMU->Export_Data.Prev_Yaw) < - 300)
		MPU6050_IMU->Export_Data.Turn_Count++;
	else if((MPU6050_IMU->Export_Data.Yaw - MPU6050_IMU->Export_Data.Prev_Yaw) > 300)
		MPU6050_IMU->Export_Data.Turn_Count--;
	
	MPU6050_IMU->Export_Data.Total_Yaw = MPU6050_IMU->Export_Data.Yaw + 360.0f * MPU6050_IMU->Export_Data.Turn_Count;
}
