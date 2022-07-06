/**
 * @file Filters.h
 * @author Leo Liu
 * @brief filters algorithms
 * @version 1.0
 * @date 2022-07-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define gz_LpfAttFactor 1
 
void Filter_IIRLPF(float *in,float *out, float LpfAttFactor);
