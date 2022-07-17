/**
 * @file I2C_Setup.c
 * @author Leo Liu
 * @brief 
 * @version 1.0
 * @date 2022/07/14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
#include "I2C_Setup.h"

void I2C_Read_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
void I2C_Write_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t	*data);

void I2C_Read_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
	HAL_I2C_Mem_Read(&hi2c2, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, data, length, I2C_TIMEOUT);
}

void I2C_Write_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
	HAL_I2C_Mem_Write(&hi2c2, devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, data, length, I2C_TIMEOUT);
}
