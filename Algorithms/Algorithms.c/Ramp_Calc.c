/**
 * @file Ramp_Calc.c
 * @author Leo Liu
 * @brief ramp up or down calculation
 * @version 1.0
 * @date 2022-07-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Ramp_Calc.h"

Ramp_Calc_t Ramp_Calc;

float Ramp_Up(Ramp_Calc_t *Ramp_Calc, float Max_Value);
float Ramp_Down(Ramp_Calc_t *Ramp_Calc, float Min_Value);
void Clear_Ramp(Ramp_Calc_t *Ramp_Calc);

Ramp_Calc_Func_t Ramp_Calc_Func = Ramp_Calc_Func_GroundInit;
#undef Ramp_Calc_Func_GroundInit

//Slowly ramp up the given value to the maximum
float Ramp_Up(Ramp_Calc_t *Ramp_Calc, float Max_Value)
{
	Ramp_Calc->Current_Value += RAMP_RATE;
	Ramp_Calc->Current_Value = VAL_MAX(Ramp_Calc->Current_Value, Max_Value);
	
	if(Ramp_Calc->Current_Value == Max_Value)
		Ramp_Calc->Ramp_Finished_Flag = 1;
	
	return Ramp_Calc->Current_Value;
}

//Slowly ramp down the given value to the minimum
float Ramp_Down(Ramp_Calc_t *Ramp_Calc, float Min_Value)
{
	Ramp_Calc->Current_Value -= RAMP_RATE;
	Ramp_Calc->Current_Value = VAL_MIN(Ramp_Calc->Current_Value, Min_Value);
	
	Ramp_Calc->Ramp_Finished_Flag = 0;
	
	return Ramp_Calc->Current_Value;
}

void Clear_Ramp(Ramp_Calc_t *Ramp_Calc)
{
	Ramp_Calc->Current_Value = 0;
	Ramp_Calc->Ramp_Finished_Flag = 0;
}
