/**
 * @file Filters.c
 * @author Leo Liu
 * @brief filters algorithms
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Filters.h"

void Filter_IIRLPF(float *in,float *out, float LpfAttFactor)
{
	*out = *out + LpfAttFactor*(*in - *out); 
}
