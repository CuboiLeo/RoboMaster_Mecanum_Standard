/**
 * @file User_Defined_Math.h
 * @author Leo Liu
 * @brief Basic Math/Logic Operations
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __USER_DEFINED_MATH_H
#define __USER_DEFINED_MATH_H

#include "GM6020_Motor.h"
#include "Chassis_Control.h"
#include <stdio.h>

#define DEG_TO_RAD(DEG) (DEG * 0.0174532925199432957692369076848f)
#define RAD_TO_DEG(RAD)	(RAD * 57.295779513082320876798154814105f)

#define VAL_MIN(a, b) ((a) < (b) ? (b) : (a))
#define VAL_MAX(a, b) ((a) > (b) ? (b) : (a))
 
extern float VAL_LIMIT(float Value, float Upper_Limit, float Lower_Limit);
extern float Find_Gimbal_Min_Angle(float Angle);

#endif
