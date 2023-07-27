/**
 * @file Ramp_Calc.h
 * @author Leo Liu
 * @brief header file for ramp calc
 * @version 1.0
 * @date 2022-07-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __RAMP_CALC_H
#define __RAMP_CALC_H

#include "User_Defined_Math.h"

#define Ramp_Calc_Func_GroundInit	\
		{															\
				&Ramp,										\
						&Clear_Ramp,					\
		}

typedef struct
{
	float Current_Value;
	uint8_t Ramp_Finished_Flag;
}Ramp_Calc_t;

typedef struct
{
	float (*Ramp)(Ramp_Calc_t *Ramp_Calc, float Ramp_Rate, float Target_Value);
	void (*Clear_Ramp)(Ramp_Calc_t *Ramp_Calc);
}Ramp_Calc_Func_t;

extern Ramp_Calc_Func_t Ramp_Calc_Func;

#endif
