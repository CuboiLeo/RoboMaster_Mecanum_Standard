/**
 * @file I2C_Setup.h
 * @author Leo Liu
 * @brief 
 * @version 1.0
 * @date 2022/07/14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __I2C_SETUP_H
#define __I2C_SETUP_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "i2c.h"
#include "MPU6050_IMU.h"

void I2C_Read_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
void I2C_Write_Bytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);

#endif
