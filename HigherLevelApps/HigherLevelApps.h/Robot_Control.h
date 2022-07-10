/**
 * @file Robot_Control.h
 * @author Leo Liu
 * @brief header file for robot_control
 * @version 1.0
 * @date 2022-07-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Chassis_Control.h"

#define Robot_Control_Func_GroundInit	\
		{																	\
					&Robot_Control_Start,							\
							&Robot_Control_Send			\
		}
			
typedef struct
{
	void (*Robot_Control_Start)(void);
	void (*Robot_Control_Send)(void);
}Robot_Control_Func_t;

extern Robot_Control_Func_t Robot_Control_Func;
