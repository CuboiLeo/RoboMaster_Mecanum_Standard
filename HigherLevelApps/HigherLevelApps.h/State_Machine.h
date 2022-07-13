/**
 * @file State_Machine.h
 * @author Leo Liu
 * @brief header file for state machine
 * @version 1.0
 * @date 2022-07-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include "DR16_Remote.h"
#include "M3508_Motor.h"
#include "GM6020_Motor.h"
#include "M2006_Motor.h"
#include "Robot_Control.h"

#define State_Machine_Func_GroundInit \
{																			\
		&Remote_Control_Update,						\
}

typedef struct
{
	enum
	{
		Computer,
		Remote_Control,
	}Control_Source;
	
}State_Machine_t;

typedef struct
{
	void (*Remote_Control_Update)(void);
}State_Machine_Func_t;

extern State_Machine_t State_Machine;
extern State_Machine_Func_t State_Machine_Func;

#endif
