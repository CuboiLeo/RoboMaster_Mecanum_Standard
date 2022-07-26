/**
 * @file WT901_IMU.c
 * @author Leo Liu
 * @brief external WT901 communication and calculation
 * @version 1.0
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <math.h>
#include "WT901_IMU.h"

WT901_IMU_t WT901_IMU;

static uint8_t Buffer[26];

void WT901_IMU_Init(void);
void WT901_IMU_Read_Data(WT901_IMU_t *WT901_IMU);

WT901_IMU_Func_t WT901_IMU_Func = WT901_IMU_Func_GroundInit;
#undef WT901_IMU_Func_GroundInit

void WT901_IMU_Init(void)
{
	/*uint16_t Temp_Data;
	
	Temp_Data = 0xB588;
	I2C_Write_8_Bytes(DEV_ADDR_WT901,	KEY_WT901, 2, (uint8_t*)(&Temp_Data));
	HAL_Delay(5);
	
	Temp_Data = 0x0001;
	I2C_Write_8_Bytes(DEV_ADDR_WT901,	CALSW_WT901, 2, (uint8_t*)(&Temp_Data));
	HAL_Delay(5);
	
	Temp_Data = 0x0000;
	I2C_Write_8_Bytes(DEV_ADDR_WT901,	AXIS6_WT901, 2, (uint8_t*)(&Temp_Data));
	HAL_Delay(5);
	
	Temp_Data = 0x0000;
	I2C_Write_8_Bytes(DEV_ADDR_WT901,	CALSW_WT901, 2, (uint8_t*)(&Temp_Data));
	HAL_Delay(5);
	
	Temp_Data = 0x0000;
	I2C_Write_8_Bytes(DEV_ADDR_WT901,	SAVE_WT901, 2, (uint8_t*)(&Temp_Data));
	HAL_Delay(5);*/
}

void WT901_IMU_Read_Data(WT901_IMU_t *WT901_IMU)
{
	I2C_Read_8_Bytes(DEV_ADDR_WT901, AX_WT901, 26, Buffer);

	WT901_IMU->Raw_Data.Ax = ((int16_t)Buffer[0] << 8) | Buffer[1];
	WT901_IMU->Raw_Data.Ay = ((int16_t)Buffer[2] << 8) | Buffer[3];
	WT901_IMU->Raw_Data.Az = ((int16_t)Buffer[4] << 8) | Buffer[5];
	WT901_IMU->Raw_Data.Gx = ((int16_t)Buffer[6] << 8) | Buffer[7];
	WT901_IMU->Raw_Data.Gy = ((int16_t)Buffer[8] << 8) | Buffer[9];
	WT901_IMU->Raw_Data.Gz = ((int16_t)Buffer[10] << 8) | Buffer[11];
	WT901_IMU->Raw_Data.Hx = ((int16_t)Buffer[12] << 8) | Buffer[13];
	WT901_IMU->Raw_Data.Hy = ((int16_t)Buffer[14] << 8) | Buffer[15];
	WT901_IMU->Raw_Data.Hz = ((int16_t)Buffer[16] << 8) | Buffer[17];
	WT901_IMU->Raw_Data.Roll = ((int16_t)Buffer[18] << 8) | Buffer[19];
	WT901_IMU->Raw_Data.Pitch = ((int16_t)Buffer[20] << 8) | Buffer[21];
	WT901_IMU->Raw_Data.Yaw = ((int16_t)Buffer[22] << 8) | Buffer[23];
	WT901_IMU->Raw_Data.Temperature = ((int16_t)Buffer[24]) << 8 | Buffer[25];
	
	WT901_IMU->Calc_Data.Ax = WT901_IMU->Raw_Data.Ax / 32768.0f * 16.0f;
	WT901_IMU->Calc_Data.Ay = WT901_IMU->Raw_Data.Ay / 32768.0f * 16.0f;
	WT901_IMU->Calc_Data.Az = WT901_IMU->Raw_Data.Az / 32768.0f * 16.0f;
	WT901_IMU->Calc_Data.Gx = WT901_IMU->Raw_Data.Gx / 32768.0f * 2000.0f;
	WT901_IMU->Calc_Data.Gy = WT901_IMU->Raw_Data.Gy / 32768.0f * 2000.0f;
	WT901_IMU->Calc_Data.Gz = WT901_IMU->Raw_Data.Gz / 32768.0f * 2000.0f;
	WT901_IMU->Calc_Data.Hx = WT901_IMU->Raw_Data.Hx;
	WT901_IMU->Calc_Data.Hy = WT901_IMU->Raw_Data.Hy;
	WT901_IMU->Calc_Data.Hz = WT901_IMU->Raw_Data.Hz;
	WT901_IMU->Calc_Data.Roll = WT901_IMU->Raw_Data.Roll / 32768.0f * 180.0f;
	WT901_IMU->Calc_Data.Pitch = WT901_IMU->Raw_Data.Pitch / 32768.0f * 180.0f;
	WT901_IMU->Calc_Data.Yaw = WT901_IMU->Raw_Data.Yaw / 32768.0f * 180.0f;
	WT901_IMU->Calc_Data.Temperature = WT901_IMU->Raw_Data.Temperature / 100.0f;
}

void ShortToChar(short sData,unsigned char cData[])
{
	cData[0]=sData&0xff;
	cData[1]=sData>>8;
}
short CharToShort(unsigned char cData[])
{
	return ((short)cData[1]<<8)|cData[0];
}
