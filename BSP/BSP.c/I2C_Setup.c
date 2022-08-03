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

void I2C_Read_8_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
void I2C_Write_8_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t	*data);
void I2C_Read_16_Bytes(uint16_t devAddr, uint16_t regAddr, uint8_t length, uint8_t *data);
void I2C_Write_16_Bytes(uint16_t devAddr, uint16_t regAddr, uint8_t length, uint8_t	*data);

//I2C read and write functions based on register bit length

void I2C_Read_8_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
	HAL_I2C_Mem_Read(&hi2c2, devAddr<<1, regAddr, I2C_MEMADD_SIZE_8BIT, data, length, I2C_TIMEOUT);
}

void I2C_Write_8_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t	*data)
{
	HAL_I2C_Mem_Read(&hi2c2, devAddr<<1, regAddr, I2C_MEMADD_SIZE_8BIT, data, length, I2C_TIMEOUT);
}

void I2C_Read_16_Bytes(uint16_t devAddr, uint16_t regAddr, uint8_t length, uint8_t *data)
{
	HAL_I2C_Mem_Read(&hi2c2, devAddr<<1, regAddr, I2C_MEMADD_SIZE_16BIT, data, length, I2C_TIMEOUT);
}

void I2C_Write_16_Bytes(uint16_t devAddr, uint16_t regAddr, uint8_t length, uint8_t *data)
{
	HAL_I2C_Mem_Write(&hi2c2, devAddr<<1, regAddr, I2C_MEMADD_SIZE_16BIT, data, length, I2C_TIMEOUT);
}
