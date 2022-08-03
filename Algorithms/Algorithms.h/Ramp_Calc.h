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

#define RAMP_RATE 0.5f //This determines how fast the ramping is

#define Ramp_Calc_Func_GroundInit	\
		{															\
				&Ramp_Up,									\
						&Ramp_Down,						\
						&Clear_Ramp,					\
		}

typedef struct
{
	float Current_Value;
	
	uint8_t Ramp_Finished_Flag;
}Ramp_Calc_t;

typedef struct
{
	float (*Ramp_Up)(Ramp_Calc_t *Ramp_Calc, float Max_Value);
	float (*Ramp_Down)(Ramp_Calc_t *Ramp_Calc, float Min_Value);
	void (*Clear_Ramp)(Ramp_Calc_t *Ramp_Calc);
}Ramp_Calc_Func_t;

extern Ramp_Calc_Func_t Ramp_Calc_Func;

#endif
